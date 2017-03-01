#version 330 core
// This is a sample fragment shader.
// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 Normal;
in vec3 Position;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

// Uniform
uniform samplerCube skybox;
uniform vec3 cameraPos;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
	vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));

	

	color = texture(skybox, R);
	color = color * vec4(0.35f, 0.55f, 0.75f, 0.9f); // 0.1 0.3 0.4 1.0
	//color = vec4(0.7, 0.7, 0.2, 1);
}
