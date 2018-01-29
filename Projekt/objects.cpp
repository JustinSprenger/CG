// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////    DrahtWuerfel-Objekt
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GLuint VertexArrayIDWireCube = 0;

static void createWireCube()
{
	// Vertexarrays kapseln ab OpenGL3 Eckpunkte, Texturen und Normalen
	glGenVertexArrays(1, &VertexArrayIDWireCube);
	glBindVertexArray(VertexArrayIDWireCube);

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f, -1.0f,-1.0f, 1.0f, 
		-1.0f, 1.0f,-1.0f, -1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,  1.0f,-1.0f, 1.0f,  
		 1.0f, 1.0f,-1.0f,  1.0f, 1.0f, 1.0f, 
		-1.0f,-1.0f,-1.0f, -1.0f, 1.0f,-1.0f, 
		-1.0f,-1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,  1.0f, 1.0f,-1.0f, 
		 1.0f,-1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  
		-1.0f,-1.0f,-1.0f,  1.0f,-1.0f,-1.0f,  
		-1.0f,-1.0f, 1.0f,  1.0f,-1.0f, 1.0f, 
		-1.0f, 1.0f,-1.0f,  1.0f, 1.0f,-1.0f, 
		-1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f
	};

	// Vertexbuffer-Daten z.B. auf Grafikkarte kopieren
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// Erklären wie die Vertex-Daten zu benutzen sind
	glEnableVertexAttribArray(0); // Kein Disable ausführen !
	glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
	);

	glBindVertexArray(0);
}

void drawWireCube()
{
	if (!VertexArrayIDWireCube)
	{
		createWireCube();
	}

	glBindVertexArray(VertexArrayIDWireCube);
	glDrawArrays(GL_LINES, 0, 24); // 12 Linien haben 24 Punkte
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////    Wuerfel-Objekt Bunt
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GLuint VertexArrayIDSolidCube = 0;

GLuint VertexArrayID;

std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;

GLuint vertexbuffer;
GLuint normalbuffer; // Hier alles analog für Normalen in location == 2
GLuint uvbuffer; // Hier alles analog für Texturkoordinaten in location == 1 (2 floats u und v!)

static void createCube()
{
	GLuint vertexbuffer;
	//GLuint colorbuffer;
	
	glGenVertexArrays(1, &VertexArrayIDSolidCube);
	glBindVertexArray(VertexArrayIDSolidCube);

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f, -1.0f,-1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f, -1.0f,-1.0f,-1.0f, -1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f, -1.0f,-1.0f,-1.0f,  1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,  1.0f,-1.0f,-1.0f, -1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f, -1.0f,-1.0f, 1.0f, -1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f, -1.0f,-1.0f, 1.0f,  1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,  1.0f,-1.0f,-1.0f,  1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,  1.0f, 1.0f, 1.0f,  1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,-1.0f, -1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f, -1.0f, 1.0f,-1.0f, -1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f,  1.0f,-1.0f, 1.0f
	};

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// One color for each vertex. They were generated randomly.
	/**static const GLfloat g_color_buffer_data[] = { 
		0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f
	};**/

	//glGenBuffers(1, &colorbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0); // Kein Disable ausführen !
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : colors
	//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glEnableVertexAttribArray(1); // Kein Disable ausführen !
	glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
	);
	
	glBindVertexArray(0);
}

void drawCube()
{
	if (!VertexArrayIDSolidCube)
	{
		createCube();
	}

	// Draw the triangles !
	glBindVertexArray(VertexArrayIDSolidCube);
	glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////    Kugel-Objekt
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GLuint VertexArrayIDSphere = 0;
GLuint lats;
GLuint longs;


// Dieser Code  basiert auf http://ozark.hendrix.edu/~burch/cs/490/sched/feb8/
static void createSphere()
{
	glGenVertexArrays(1, &VertexArrayIDSphere);
	glBindVertexArray(VertexArrayIDSphere);

	static GLuint sphereVertexBuffer = 0;
	static GLfloat* sphereVertexBufferData = NULL;
	static GLfloat* sphereNormalBufferData = NULL;

	sphereVertexBufferData = new GLfloat [6 * (lats + 1) * (longs + 1)];
	sphereNormalBufferData = new GLfloat [6 * (lats + 1) * (longs + 1)];
	int index = 0;

    for (int i = 0; i <= lats; i++) 
	{
          GLfloat lat0 = (GLfloat) M_PI * ((GLfloat) -0.5 + (GLfloat) (i - 1) / (GLfloat) lats);
          GLfloat z0  = sin(lat0);
          GLfloat zr0 =  cos(lat0);
    
          GLfloat lat1 = (GLfloat) M_PI * ((GLfloat) -0.5 + (GLfloat) i / (GLfloat) lats);
          GLfloat z1 = sin(lat1);
          GLfloat zr1 = cos(lat1);
    
		  for (int j = 0; j <= longs; j++)
		  {
              GLfloat lng = (GLfloat) 2 * (GLfloat) M_PI * (GLfloat) (j - 1) / (GLfloat) longs;
              GLfloat x = cos(lng);
              GLfloat y = sin(lng);

			  sphereNormalBufferData[index] = x * zr0;
			  sphereVertexBufferData[index++] = x * zr0;
			  sphereNormalBufferData[index] = y * zr0;
			  sphereVertexBufferData[index++] = y * zr0;
			  sphereNormalBufferData[index] = z0;
			  sphereVertexBufferData[index++] = z0;
			  sphereNormalBufferData[index] = x * zr1;
			  sphereVertexBufferData[index++] = x * zr1;
			  sphereNormalBufferData[index] = y * zr1;
			  sphereVertexBufferData[index++] = y * zr1;
			  sphereNormalBufferData[index] = z1;
			  sphereVertexBufferData[index++] = z1;
          }
     }

	GLuint vertexbuffer;
	GLuint normalbuffer;
	
	
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * (lats + 1) * (longs + 1), sphereVertexBufferData, GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * (lats + 1) * (longs + 1), sphereNormalBufferData, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0); // Kein Disable ausführen !
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : normals
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glEnableVertexAttribArray(2); // Kein Disable ausführen !
	glVertexAttribPointer(
			2,                                // attribute. No particular reason for 2, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
	);

	glBindVertexArray(0);
}

// Nur die Angabe bei der ersten Kugel ist relevant
void drawSphere(GLuint slats, GLuint slongs)
{
	if (!VertexArrayIDSphere)
	{
		lats = slats;
		longs = slongs;
		createSphere();
	}

	glBindVertexArray(VertexArrayIDSphere);
	// Draw the triangles !
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (lats + 1) * (longs + 1)); 
}