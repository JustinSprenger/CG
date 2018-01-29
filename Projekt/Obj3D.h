#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#define _USE_MATH_DEFINES
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class Obj3D
{
	// Memberdaten
	GLuint VertexArrayID;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	GLuint vertexbuffer;
	GLuint normalbuffer; // Hier alles analog für Normalen in location == 2
	GLuint uvbuffer; // Hier alles analog für Texturkoordinaten in location == 1 (2 floats u und v!)

	GLuint VertexArrayIDSphere = 0;
	GLuint lats;
	GLuint longs;

public:
	Obj3D(const char* fn); // Konstruktor
	void display();
	~Obj3D(); // Destruktor
};

