#include "window.h"

const char* window_title = "Brain-o-War";

// Initializations
Skybox * skybox;
SphereENV * env;
GLuint skyboxTexture;

// On/off vars
int env_location = 0;
int id_selected = -1;

GLint skybox_shaderProgram;
GLint object_shaderProgram;
GLint env_shaderProgram;

// On some systems you need to change this to the absolute path
#define SKYBOX_VERTEX_SHADER_PATH "../skybox.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../skybox.frag"
#define OBJECT_VERTEX_SHADER_PATH "../object.vert"
#define OBJECT_FRAGMENT_SHADER_PATH "../object.frag"
#define ENV_VERTEX_SHADER_PATH "../env.vert"
#define ENV_FRAGMENT_SHADER_PATH "../env.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 10.0f);		// e  | Position of camera, z = 20.0f default, reset 1.0f
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at, z = 0.0f default
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

// Callback variables
bool click = true;
float cursor_x = 0;
float cursor_y = 0;
float cursor_x_old = 0;
float cursor_y_old = 0;
glm::vec3 old_location;
bool lmb = false;
bool rmb = false;
bool button_down = false;

void Window::initialize_objects()
{
	// Initialize camera
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);

	// And skybox
	skybox = new Skybox(200);
	skyboxTexture = skybox->loadCubemap();

	env = new SphereENV();
	env->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// Load the shader program. Make sure you have the correct filepath up top
	skybox_shaderProgram = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
	object_shaderProgram = LoadShaders(OBJECT_VERTEX_SHADER_PATH, OBJECT_FRAGMENT_SHADER_PATH);
	env_shaderProgram = LoadShaders(ENV_VERTEX_SHADER_PATH, ENV_FRAGMENT_SHADER_PATH);
	
	env->shaderProgram = env_shaderProgram;
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(skybox);
	delete(env);

	glDeleteProgram(skybox_shaderProgram);
	glDeleteProgram(object_shaderProgram);
	glDeleteProgram(env_shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

glm::vec3 Window::trackball(float x, float y)    // Use separate x and y values for the mouse location
{
	glm::vec3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	float d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
	v.x = (2.0f*x - Window::width) / Window::width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v.y = (Window::height - 2.0f*y) / Window::height;   // this does the equivalent to the above for the mouse Y position
	v.z = 0.0;   // initially the mouse z position is set to zero, but this will change below
	d = (float)v.length();    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d<1.0) ? d : 1.0f;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	v.z = (float)sqrtf(1.001 - d*d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	v = normalize(v); // Still need to normalize, since we only capped d, not v.
	return v;  // return the mouse location on the surface of the trackball
}




/************************************
*		START NEW CALLBACKS			*
*									*
*************************************/



void Window::cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}


/************************************
*		END NEW CALLBACKS			*
*									*
*************************************/


void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

bool Window::idle_callback(int player1_att, int player2_att, int player1_med, int player2_med)
{
	float x_bounds_p1 = -9.0f;
	float x_bounds_p2 = 9.0f;
	float y_bounds_p1 = -7.0f;
	float y_bounds_p2 = 7.0f;
	float x_translation = (player1_att - player2_att) / 100.0f;
	float y_translation = (player1_med - player2_med) / 250.0f;
	env->translate(x_translation, y_translation);

	if (env->position_x > x_bounds_p2 || env->position_y > y_bounds_p2) {
		std::cout << "\nPLAYER 1 WINS!" << std::endl;
		return true;
	}
	else if (env->position_x < x_bounds_p1 || env->position_y < y_bounds_p1) {
		std::cout << "\nPLAYER 2 WINS!" << std::endl;
		return true;
	}
	else {
		return false;
	}
}

void Window::reset_ball() {
	env->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	env->position_x = 0;
	env->position_y = 0;
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID, skybox
	glUseProgram(skybox_shaderProgram);
	skybox->draw(skybox_shaderProgram);



	// Use object shader
	glDisable(GL_CULL_FACE);

	glUseProgram(env_shaderProgram);
	env->solidSphere(skybox_shaderProgram, cam_pos, skyboxTexture);

	// Send light info
	glUseProgram(object_shaderProgram);
	skybox->sendLight(object_shaderProgram);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}