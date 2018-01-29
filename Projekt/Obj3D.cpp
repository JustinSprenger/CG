#include "Obj3D.h"
#include "objloader.hpp"



Obj3D::Obj3D(const char* fn)
{
	bool res = loadOBJ(fn, vertices, uvs, normals);

	// Jedes Objekt eigenem VAO zuordnen, damit mehrere Objekte moeglich sind
	// VAOs sind Container fuer mehrere Buffer, die zusammen gesetzt werden sollen.
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Ein ArrayBuffer speichert Daten zu Eckpunkten (hier xyz bzw. Position)
	glGenBuffers(1, &vertexbuffer); // Kennung erhalten
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // Daten zur Kennung definieren
												 // Buffer zugreifbar f�r die Shader machen
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	// Erst nach glEnableVertexAttribArray kann DrawArrays auf die Daten zugreifen...
	glEnableVertexAttribArray(0); // siehe layout im vertex shader: location = 0 
	glVertexAttribPointer(0,  // location = 0 
		3,  // Datenformat vec3: 3 floats fuer xyz 
		GL_FLOAT,
		GL_FALSE, // Fixedpoint data normalisieren ?
		0, // Eckpunkte direkt hintereinander gespeichert
		(void*)0); // abweichender Datenanfang ? 

				   //6.2//
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2); // siehe layout im vertex shader 
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//7.2//
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // siehe layout im vertex shader 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void Obj3D::display()
{
	glBindVertexArray(VertexArrayID);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}


Obj3D::~Obj3D()
{
	// Daten werden geloescht
	//5.5//
	glDeleteBuffers(1, &vertexbuffer);

	//6.5//
	glDeleteBuffers(1, &normalbuffer);

	//7.4//
	glDeleteBuffers(1, &uvbuffer);
}
