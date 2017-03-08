#include "SphereENV.h"
#include <concrt.h>


SphereENV::SphereENV()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);
	glBindVertexArray(VAO);

	glm::mat4 modelview = Window::V * toWorld;
	GLfloat pi = glm::pi<float>();

	float fstacks = (float)stacks;
	float fslices = (float)slices;
	for (int i = 0; i < slices; i++) {
		for (int j = 0; j < stacks; j++) {
			// Top left
			sphereVertices.push_back(glm::vec3(
				radius * -cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
				radius * -cos(pi * (j + 1.0f) / fslices),
				radius * sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices)));
			sphereNormals.push_back(glm::normalize(glm::vec3(
				-cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
				-cos(pi * (j + 1.0f) / fslices),
				sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices))));
			// Top right
			sphereVertices.push_back(glm::vec3(
				radius * -cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices),
				radius * -cos(pi * (j + 1.0) / fslices),
				radius * sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices)));
			sphereNormals.push_back(glm::normalize(glm::vec3(
				-cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices),
				-cos(pi * (j + 1.0) / fslices),
				sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices))));
			// Bottom right
			sphereVertices.push_back(glm::vec3(
				radius * -cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
				radius * -cos(pi * j / fslices),
				radius * sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices)));
			sphereNormals.push_back(glm::normalize(glm::vec3(
				-cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
				-cos(pi * j / fslices),
				sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices))));

			// Need to repeat 2 of the vertices since we can only draw triangles. Eliminates the confusion
			// of array indices.
			// Top left
			sphereVertices.push_back(glm::vec3(
				radius * -cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
				radius * -cos(pi * (j + 1.0f) / fslices),
				radius * sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices)));
			sphereNormals.push_back(glm::normalize(glm::vec3(
				-cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
				-cos(pi * (j + 1.0f) / fslices),
				sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices))));
			// Bottom right
			sphereVertices.push_back(glm::vec3(
				radius * -cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
				radius * -cos(pi * j / fslices),
				radius * sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices)));
			sphereNormals.push_back(glm::normalize(glm::vec3(
				-cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
				-cos(pi * j / fslices),
				sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices))));
			// Bottom left
			sphereVertices.push_back(glm::vec3(
				radius * -cos(2.0f * pi * i / fstacks) * sin(pi * j / fslices),
				radius * -cos(pi * j / fslices),
				radius * sin(2.0f * pi * i / fstacks) * sin(pi * j / fslices)));
			sphereNormals.push_back(glm::normalize(glm::vec3(
				-cos(2.0f * pi * i / fstacks) * sin(pi * j / fslices),
				-cos(pi * j / fslices),
				sin(2.0f * pi * i / fstacks) * sin(pi * j / fslices))));

		}
	}


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(glm::vec3), sphereVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.



					 /* NBO */

	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sphereNormals.size() * sizeof(glm::vec3), sphereNormals.data(), GL_STATIC_DRAW);

	// Enable the usage of layout location 2 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(2,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

					 // We've sent the vertex data over to OpenGL, but there's still something missing.
					 // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.

					 /*END*/


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}


SphereENV::~SphereENV()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &NBO);
}

// This function draws a solid SphereENV
void SphereENV::solidSphere(GLuint shaderProgram, glm::vec3 camera_pos, GLuint skyboxTexture) {
	// No need to generate the sphere again if the previous rendering already
	// used the same number of stacks and slices
	glm::mat4 modelview = Window::V * toWorld;
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");
	glUniform3f(glGetUniformLocation(shaderProgram, "cameraPos"), camera_pos.x, camera_pos.y, camera_pos.z);

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, sphereVertices.size());
	glBindVertexArray(0);
}

void SphereENV::translate(float x, float y) {
	this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)) * this->toWorld;
	position_x += x;
	position_y += y;
}

void SphereENV::draw(GLuint shaderProgram) {

	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");

	// Materials
	glUniform3f(glGetUniformLocation(shaderProgram, "material2.ambient"), 0.7f, 0.1f, 0.1f);

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, points.size());
	glBindVertexArray(0);
}

void SphereENV::render() {
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");

	// Materials
	glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 0.1f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 0.5f, 0.4f, 0.5f);
	glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 0.5f, 0.5f, 0.4f);
	glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 30);

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, points.size());
	glBindVertexArray(0);
}

void SphereENV::update() {
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), 1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}