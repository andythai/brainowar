#version 330 core
// This is a sample fragment shader.

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 FragPos;
in vec3 Normal;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;


// Forward declaration
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

// Uniform
uniform DirLight dirLight;
uniform Material material;
uniform mat4 view;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
	vec3 norm = normalize(Normal);


	vec3 result = material.ambient;
	float viewx = view[3][0];
	float viewy = view[3][1];
	float viewz = view[3][2];
	vec3 viewPos = {viewx, viewy, viewz};
	vec3 viewDir = normalize(viewPos - FragPos);
		
	// Directional
	result = CalcDirLight(dirLight, norm, viewDir) * result;

	// Output
	color = vec4(result, 1.0f);
}


// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Combine results
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}