#version 330 core
// This is a sample fragment shader.
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 TexCoords;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

// Uniform
uniform samplerCube skybox;
uniform mat4 view;
uniform Material material2;


void main()
{
	color = texture(skybox, TexCoords);
	//color = vec4(0.7, 0.7, 0.2, 1);
}
