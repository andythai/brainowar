#include "main.h"

GLFWwindow* window;

void error_callback(int error, const char* description)
{
	// Print error
	fputs(description, stderr);
}

void setup_callbacks()
{
	// Set the error callback
	glfwSetErrorCallback(error_callback);
	// Set the key callback
	glfwSetKeyCallback(window, Window::key_callback);
	// Set the mouse button callback
	glfwSetMouseButtonCallback(window, Window::mouse_callback);
	// Set the mouse scrolling callback
	glfwSetScrollCallback(window, Window::scroll_callback);
	// Sets the cursor callback
	glfwSetCursorPosCallback(window, Window::cursor_callback);
	// Set the window resize callback
	glfwSetFramebufferSizeCallback(window, Window::resize_callback);
}

void setup_glew()
{
	// Initialize GLEW. Not needed on OSX systems.
#ifndef __APPLE__
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		glfwTerminate();
	}
	fprintf(stdout, "Current GLEW version: %s\n", glewGetString(GLEW_VERSION));
#endif
}

void setup_opengl_settings()
{
#ifndef __APPLE__
	// Setup GLEW. Don't do this on OSX systems.
	setup_glew();
#endif
	// Enable depth buffering
	glEnable(GL_DEPTH_TEST);
	// Related to shaders and z value comparisons for the depth buffer
	glDepthFunc(GL_LEQUAL);
	// Set polygon drawing mode to fill front and back of each polygon
	// You can also use the paramter of GL_LINE instead of GL_FILL to see wireframes
	glFrontFace(GL_CW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Enable culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// Set clear color
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void print_versions()
{
	// Get info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

	//If the shading language symbol is defined
#ifdef GL_SHADING_LANGUAGE_VERSION
	std::printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
}

int main(void)
{
	// Create the GLFW window
	window = Window::create_window(640, 480);
	// Print OpenGL and GLSL versions
	print_versions();
	// Setup callbacks
	setup_callbacks();
	// Setup OpenGL settings, including lighting, materials, etc.
	setup_opengl_settings();
	// Initialize objects/pointers for rendering
	Window::initialize_objects();

	// ThinkGear variables
	char *comPortName = NULL;
	int   dllVersion = 0;
	int   connectionId = 0;
	int	  connectionId2 = 0;
	int   packetsRead = 0;
	int   errCode = 0;

	double secondsToRun = 0;
	time_t startTime = 0;
	time_t currTime = 0;
	char  *currTimeStr = NULL;
	int set_filter_flag = 0;
	int count = 0;

	/* Print driver version number */
	dllVersion = TG_GetVersion();
	printf("Stream SDK for PC version: %d\n", dllVersion);

	/* Get a connection ID handle to ThinkGear */
	connectionId = TG_GetNewConnectionId();
	if (connectionId < 0) {
		fprintf(stderr, "ERROR: TG_GetNewConnectionId() returned %d for PLAYER 1.\n",
			connectionId);
		fprintf(stderr, "Press ENTER to continue...\n", connectionId);
		std::cin.ignore();
		//exit(EXIT_FAILURE);
	}

	/* Get a connection ID handle to ThinkGear for player 2 */
	connectionId2 = TG_GetNewConnectionId();
	if (connectionId2 < 0) {
		fprintf(stderr, "ERROR: TG_GetNewConnectionId() returned %d for PLAYER 2.\n",
			connectionId);
		fprintf(stderr, "Press ENTER to continue...\n", connectionId2);
		std::cin.ignore();
		//exit(EXIT_FAILURE);
	}


	/* Attempt to connect the connection ID handle to serial port "COM5" */
	/* NOTE: On Windows, COM10 and higher must be preceded by \\.\, as in
	*       "\\\\.\\COM12" (must escape backslashes in strings).  COM9
	*       and lower do not require the \\.\, but are allowed to include
	*       them.  On Mac OS X, COM ports are named like
	*       "/dev/tty.MindSet-DevB-1".
	*/
	comPortName = "\\\\.\\COM4";
	errCode = TG_Connect(connectionId,
		comPortName,
		TG_BAUD_57600,
		TG_STREAM_PACKETS);
	if (errCode < 0) {
		fprintf(stderr, "ERROR: TG_Connect() returned %d for PLAYER 1.\n", errCode);
		fprintf(stderr, "Press ENTER to continue...\n", errCode);
		std::cin.ignore();
		//exit(EXIT_FAILURE);
	}

	/* PLAYER 2 */
	/**
	comPortName = "\\\\.\\COM4";
	errCode = TG_Connect(connectionId2,
		comPortName,
		TG_BAUD_57600,
		TG_STREAM_PACKETS);
	if (errCode < 0) {
		fprintf(stderr, "ERROR: TG_Connect() returned %d for PLAYER 2.\n", errCode);
		fprintf(stderr, "Press ENTER to continue...\n", errCode);
		std::cin.ignore();
		//exit(EXIT_FAILURE);
	}
	**/

	// Get names
	std::string p1_name;
	std::string p2_name;
	std::cout << "\nEnter name for PLAYER 1: ";
	std::getline(std::cin, p1_name);
	/**
	std::cout << "Enter name for PLAYER 2: ";
	std::getline(std::cin, p2_name);
	**/
	p2_name = "BOT";

	// Set timers
	secondsToRun = 3;
	int sec_before_start = 10;
	time_t start_delay = time(NULL);
	std::cout << "\nSTARTING GAME IN 10 SECONDS...\n" << std::endl;
	bool game_over = false;

	// Loop while GLFW window should stay open
	while (!glfwWindowShouldClose(window))
	{

		// Wait 10 sec before game starts
		if (difftime(time(NULL), start_delay) > sec_before_start && game_over == false) {
			startTime = time(NULL);
			while (difftime(time(NULL), startTime) < secondsToRun) {

				/* Read all currently available Packets, one at a time... */
				do {
					// Main render display callback. Rendering of objects is done here.
					Window::display_callback(window);


					/* Read a single Packet from the connection */
					packetsRead = TG_ReadPackets(connectionId, 1);

					/* If TG_ReadPackets() was able to read a Packet of data... */
					if (packetsRead == 1) {

						/* If the Packet containted a new raw wave value... */
						if (TG_GetValueStatus(connectionId, TG_DATA_ATTENTION) != 0) {

							/* Get the current time as a string */
							currTime = time(NULL);
							currTimeStr = ctime(&currTime);

							/* Get and print out the new raw value */
							int player1_attention = (int)TG_GetValue(connectionId, TG_DATA_ATTENTION);
							int player1_meditation = (int)TG_GetValue(connectionId, TG_DATA_MEDITATION);
							//int player2_attention = (int)TG_GetValue(connectionId2, TG_DATA_ATTENTION);
							//int player2_meditation = (int)TG_GetValue(connectionId2, TG_DATA_MEDITATION);
							int player2_attention = 0 + (rand() % (int)(100 - 0 + 1));
							int player2_meditation = 0 + (rand() % (int)(100 - 0 + 1));


							// Print player 1 measures
							fprintf(stdout, "%s: P1 (%s)\t ATT: %d\n", currTimeStr, p1_name, player1_attention);
							fprintf(stdout, "%s: P1 (%s)\t MED: %d\n", currTimeStr, p1_name, player1_meditation);

							// Print player 2 measures
							fprintf(stdout, "%s: P2 (%s)\t ATT: %d\n", currTimeStr, p2_name, player2_attention);
							fprintf(stdout, "%s: P2 (%s)\t MED: %d\n\n", currTimeStr, p2_name, player2_meditation);
							fflush(stdout);

							// Idle callback. Updating objects, etc. can be done here.
							game_over = Window::idle_callback(player1_attention, player2_attention, 
								player1_meditation, player2_meditation, p1_name, p2_name);

							// Ball reaches past boundaries
							if (game_over) {
								std::cout << "GAME OVER, BALL REACHED PAST BOUNDARIES." << std::endl;
								std::cout << "\nPress ENTER to reset!" << std::endl;
								std::cout << "After pressing ENTER, game will restart immediately." << std::endl;
								std::cin.ignore();
								Window::reset_ball();
							}

						} /* end "If Packet contained a raw wave value..." */

					} /* end "If TG_ReadPackets() was able to read a Packet..." */

				} while (packetsRead > 0); /* Keep looping until all Packets read */
			}
		} /* end "Keep reading ThinkGear Packets for 10 seconds..." */
		else {
			Window::display_callback(window);
		}
		/**
		// Main render display callback. Rendering of objects is done here.
		Window::display_callback(window);
		// Idle callback. Updating objects, etc. can be done here.
		Window::idle_callback();
		**/
	}

	Window::clean_up();
	// Destroy the window
	glfwDestroyWindow(window);
	// Terminate GLFW
	glfwTerminate();

	exit(EXIT_SUCCESS);
}