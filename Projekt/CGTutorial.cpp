// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <list>


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

#include "Obj3D.h"

// Befindet sich bei den OpenGL-Tutorials unter "common"
#include "shader.hpp"

// Wuerfel und Kugel
#include "objects.hpp"
#include "objloader.hpp"
#include "texture.hpp"

class Point{
public:
	Point();
	float x;
	float y;
};

class Border{
public: 
	Border();
	Point bl;
	Point br;
	Point tl;
	Point tr;
};

class Obsticle {
public:
	Obsticle();	
	float boundX;
	float boundY;
	float posX;
	float posY;
	float height;
	Border border;
};

Border createBorder(float bl_x, float bl_y, float br_x, float br_y, float tl_x, float tl_y, float tr_x, float tr_y)
{
	Point bl = Point(); bl.x = bl_x; bl.y = bl_y;
	Point br = Point(); br.x = br_x; br.y = br_y;
	Point tl = Point(); tl.x = tl_x; tl.y = tl_y;
	Point tr = Point(); tr.x = tr_x; tr.y = tr_y;
	Border border = Border();
	border.bl = bl; border.br = br; border.tl = tl; border.tr = tr;
	return border;
}

Obsticle createObsticle(float boundX, float boundY, float posX, float posY, Border border)
{
	Obsticle o = Obsticle();
	o.boundX = boundX;
	o.boundY = boundY;
	o.posX = posX;
	o.posY = posY;
	o.height = -1.5f;
	o.border = border;
 	return o;
}


void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
float drehung = 0.0f;
float xangel = 0.0f;
float yangel = 0.0f;
float zangel = 0.0f;
 
float neigungWS = 0.0f;
float neigungAD = 0.0f;
float MAX_ROTATE = 30;
float MIN_ROTATE = -30;
float STEP = 2.0f;
float movex = 0.0f;
float movey = 0.0f;
float ACCELERATE = 0.03f;
float acceleratex = 0.0f;
float acceleratey = 0.0f;
float acceleratez = 0.0f;
float MAXSPEED = 0.1f;
float MINSPEED = -MAXSPEED;
float SLOWING = 0.04f;
float posx = 0.0f;
float posy = 0.0f;
float posz = 0.0f;

int level = 1;
int counter = 0;
bool count = false;

float oldtime = 0;
float newtime = 0;
float diftime = 0;

GLuint Texture;
GLuint TextureBall;
GLuint TextureWorld;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//std::cout << " KEY : " << key << " SCANCODE : " << scancode << " ACTION : " << action << " MODS : " << mods;
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_X:
		xangel = xangel + 1.0f;
		break;
	case GLFW_KEY_Y:
		yangel = yangel + 1.0f;
		break;
	case GLFW_KEY_Z:
		zangel = zangel + 1.0f;
		break;
	case GLFW_KEY_W:
		if (neigungWS < MAX_ROTATE)
		{
			neigungWS = neigungWS + STEP;
		}
		xangel = neigungWS;
		break;
	case GLFW_KEY_A:
		if(neigungAD > MIN_ROTATE)
		{
			neigungAD = neigungAD - STEP;
		}
		yangel = neigungAD;
		break;
	case GLFW_KEY_S:
		if (neigungWS > MIN_ROTATE)
		{
			neigungWS = neigungWS - STEP;
		}
		xangel = neigungWS;
		break;
	case GLFW_KEY_D:
		if (neigungAD < MAX_ROTATE)
		{
			neigungAD = neigungAD + STEP;
		}
		yangel = neigungAD;
		break;
	case GLFW_KEY_R:
		posx = 0.0;
		posy = 0.0;
		posz = 0.0;
		acceleratex = 0.0;
		acceleratey = 0.0;
		acceleratez = 0.0;
		neigungAD = 0.0f;
		neigungWS = 0.0f;
		xangel = 0.0f;
		yangel = 0.0f;
		zangel = 0.0f;
		break;
	default:
		break;
	}
}

// Diese Drei Matrizen global (Singleton-Muster), damit sie jederzeit modifiziert und
// an die Grafikkarte geschickt werden koennen
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
GLuint programID;
std::vector<Obsticle> obsticlesL1;
std::vector<Obsticle> obsticlesL2;
std::vector<Obsticle> obsticlesL3;
std::vector<Obsticle> obsticles;

void sendMVP()
{
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; 
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform, konstant fuer alle Eckpunkte
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &Projection[0][0]);
}

void zeichneKS(/*double x,double y,double z*/)
{
	/*glm::mat4 Save = Model;//hilfsvariable
	Model = glm::scale(Model, glm::vec3(x,y,z));//berechnen
	sendMVP();//an die grafikkarte schicken
	drawCube();//Zeichnen
	Model = Save;//Model zurrücksetzen*/

	glm::mat4 Save = Model;//hilfsvariable
	Model = glm::scale(Model, glm::vec3(3, 0.01, 0.01));//berechnen
	sendMVP();//an die grafikkarte schicken
	drawCube();//Zeichnen
	Model = Save;//Model zurrücksetzen

	Model = glm::scale(Model, glm::vec3(0.01, 3, 0.01));//berechnen
	sendMVP();//an die grafikkarte schicken
	drawCube();//Zeichnen
	Model = Save;//Model zurrücksetzen

	Model = glm::scale(Model, glm::vec3(0.01, 0.01, 3));//berechnen
	sendMVP();//an die grafikkarte schicken
	drawCube();//Zeichnen
	Model = Save;//Model zurrücksetzen
}

void drawWorld()
{
	
}

void drawLight(int x, int y, int z)
{
	glm::vec4 lightPos = Model*glm::vec4(x, y, z, 1);
	glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);
}

void drawFloor(Obj3D* w)
{
	glm::mat4 Save = Model;//hilfsvariable
	Model = glm::scale(Model, glm::vec3(3.0, 3.0, 0.1));//berechnen
	sendMVP();//an die grafikkarte schicken
	//drawCube();//Zeichnen
	w->display();
	Model = Save;//Model zurrücksetzen
}

void drawWalls(Obj3D* w)
{
	glm::mat4 Save = Model;//hilfsvariable
	Model = glm::scale(Model, glm::vec3(3, 0.1, 0.2));//berechnen
	Model = glm::translate(Model, glm::vec3(0.0, 29.0, -1.5));
	sendMVP();//an die grafikkarte schicken
	//drawCube();//Zeichnen
	w->display();
	Model = glm::translate(Model, glm::vec3(0.0, -58.0, 0.0));
	sendMVP();//an die grafikkarte schicken
	//drawCube();//Zeichnen
	w->display();
	Model = Save;//Model zurrücksetzen

	Save = Model;//hilfsvariable
	Model = glm::scale(Model, glm::vec3(0.1, 2.8, 0.2));//berechnen
	Model = glm::translate(Model, glm::vec3(29.0, 0.0, -1.5));
	sendMVP();//an die grafikkarte schicken
	//drawCube();//Zeichnen
	w->display();
	Model = glm::translate(Model, glm::vec3(-58.0, 0.0, 0.0));
	sendMVP();//an die grafikkarte schicken
	//drawCube();//Zeichnen
	w->display();
	Model = Save;//Model zurrücksetzen
}

float slow(float speed)
{
	float slows = SLOWING * diftime;

	if (speed > 0)
	{
		if (speed >= slows) { speed = speed - slows; }
		else { speed = 0; }

	}
	if (speed < 0)
	{
		if (speed <= slows) { speed = speed + slows; }
		else { speed = 0; }
	}
	return speed;
}

void position()
{
	acceleratex = acceleratex + (neigungWS * ACCELERATE * diftime);
	acceleratex = slow(acceleratex);
	
	if (acceleratex > MAXSPEED) 
	{
		acceleratex = MAXSPEED;
	}
	if (acceleratex < MINSPEED)
	{
		acceleratex = MINSPEED;
	}
	acceleratey = acceleratey + (neigungAD * ACCELERATE * diftime);
	acceleratey = slow(acceleratey);

	if (acceleratey > MAXSPEED)
	{
		acceleratey = MAXSPEED;
	}
	if (acceleratey < MINSPEED)
	{
		acceleratey = MINSPEED;
	}
	posx = posx + acceleratex;
	posy = posy + acceleratey;
	posz = posz + acceleratez;
}

void drawBall()
{
	position();
	glm::mat4 Save = Model;//hilfsvariable
	Model = glm::scale(Model, glm::vec3(0.25, 0.25, 0.25));//berechnen
	Model = glm::translate(Model, glm::vec3(-posy, posx, -1.5 + posz));
	sendMVP();//an die grafikkarte schicken
	drawSphere(100,100);//Zeichnen
	//s->display();
	Model = Save;//Model zurrücksetzen
}

void borderCollision()
{
	
	//Collision mit den Äußeren Kanten
	if (posx >= 10) { acceleratex = -acceleratex; posx = 10; }
	if (posx <= -10) { acceleratex = -acceleratex; posx = -10;}
	if (posy >= 10) { acceleratey = -acceleratey; posy = 10; }
	if (posy <= -10) { acceleratey = -acceleratey; posy = -10; }

	//Collision mit dem Loch
	if (posx < -8.7 && posx > -9.5 && posy < 9.5 && posy > 8.7) {
		acceleratez = 0.1; 
		count = true;
	}

	//kleiner eigener Timer
	if (count == true) {
		
		if (counter == 100) {
			posx = 0.0;
			posy = 0.0;
			posz = 0.0;
			acceleratex = 0.0;
			acceleratey = 0.0;
			acceleratez = 0.0;
			level = level + 1; 
			count = false;
			counter = 0;
		}
		counter = counter + 1;
	}
	if (level >= 4) {
		level = 1;
	}
}

bool hasCollided(float ballx, float bally, Border border)
{
	//Überprüfung, ob der ball eines der Objekte auf dem Brett berührt hat
	if ((ballx >= border.bl.x && ballx <= border.br.x) && (bally >= border.bl.y && bally <= border.tl.y))
	{
		//std::cout << " has collided " << std::endl;
		return true;
	}
	return false;
}

void obsticleCollision()
{
	std::cout << "x : " << posy << " y : "<<posx<< std::endl;
	// Überprüft welches Objekt eine Collision hat (mit abstand zu diesen objekten)
	
		for (std::vector<Obsticle>::iterator it = obsticles.begin(); it != obsticles.end(); ++it)
		{
			Border border = it->border;
			//std::cout << "Border : bl{" << border.bl.x << "/" << border.bl.y << "} br{" << border.br.x << "/" << border.br.y << "} tl{" << border.tl.x << "/" << border.tl.y << "} tr{" << border.tr.x << "/" << border.tr.y << "}" << std::endl;
			if (hasCollided(posy, posx, border))
			{
				float x1, x2, x3, x4;
				x1 = border.tl.y - posx;
				x2 = border.tr.x - posy;
				x3 = posx - border.bl.y;
				x4 = posy - border.tl.x;
				
				//float buff = 1.0f;
				
				//Ecke
				//if (x1 <= buff && x2 <= buff || x2 <= buff && x3 <= buff || x3 <= buff && x4 <= buff || x4 <= buff && x1 <= buff)
				
					//Kante
					if (x1 <= x2 && x1 <= x3 && x1 <= x4) { acceleratex = -acceleratex; posx = border.tl.y; }
					if (x2 <= x1 && x2 <= x3 && x2 <= x4) { acceleratey = -acceleratey; posy = border.tr.x; }
					if (x3 <= x1 && x3 <= x2 && x3 <= x4) { acceleratex = -acceleratex; posx = border.bl.y; }
					if (x4 <= x1 && x4 <= x2 && x4 <= x3) { acceleratey = -acceleratey; posy = border.tl.x; }

			}
		}
	
}

void drawObsticle(Obj3D* o)
{

	glm::mat4 Save = Model;
		for (std::vector<Obsticle>::iterator it = obsticles.begin(); it != obsticles.end(); ++it)
		{
			Model = glm::scale(Model, glm::vec3((it)->boundX, (it)->boundY, 0.2));//berechnen
			Model = glm::translate(Model, glm::vec3((it)->posX, (it)->posY, (it)->height));
			//std::cout << ". OBJ[" << (it)->boundX<<"/"<< (it)->boundY<<"] ["<< (it)->posX<<"/"<< (it)->posY<<"/"<< (it)->height<<"]"<< std::endl;
			sendMVP();
			o->display();
			Model = Save;
		}
}

void drawWhole()
{
	glm::mat4 Save = Model;
	Model = glm::scale(Model, glm::vec3(0.3, 0.3, 0.01));//berechnen
	Model = glm::translate(Model, glm::vec3(-7.5, -8.0, -10.5));
	sendMVP();//an die grafikkarte schicken
	drawSphere(100, 100);//Zeichnen	
	Model = Save;//Model zurrücksetzen
}

void drawScene(Obj3D* w)
{
	/*
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureWorld);

	drawWorld();
*/
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);

	drawFloor(w);
	drawWalls(w);
	drawObsticle(w);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureBall);

	borderCollision();
	obsticleCollision();
	
	drawWhole();

	drawBall();
}

int main(void)
{
	
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	// Fehler werden auf stderr ausgegeben, s. o.
	glfwSetErrorCallback(error_callback);

	// Open a window and create its OpenGL context
	// glfwWindowHint vorher aufrufen, um erforderliche Resourcen festzulegen
	GLFWwindow* window = glfwCreateWindow(1024, // Breite
										  768,  // Hoehe
										  "Ball-Maze", // Ueberschrift
										  NULL,  // windowed mode
										  NULL); // shared windoe

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Make the window's context current (wird nicht automatisch gemacht)
    glfwMakeContextCurrent(window);

	// Initialize GLEW
	// GLEW ermöglicht Zugriff auf OpenGL-API > 1.1
	glewExperimental = true; // Needed for core profile

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Auf Keyboard-Events reagieren
	glfwSetKeyCallback(window, key_callback);

	// Dark blue background

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");

	// Shader auch benutzen !
	glUseProgram(programID);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, Texture);

	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 0);

	Texture = loadBMP_custom("wood_011.bmp");
	TextureBall = loadBMP_custom("mandrill.bmp");
	TextureWorld = loadBMP_custom("blue-sky.bmp");

	Obj3D world("cube2.obj");
	//Obj3D sphere("sphere5.obj");

	level = 1;

	//obsticles.resize(4);
	//erst x untere kante dann y untere kante, dann seite x,y dann oben x,y dann seite x,y

	//Level 1
	obsticlesL1.push_back(createObsticle(2.0f, 0.1f,  0.0f,  19.0f, createBorder(-9.0f, 6.2f, 9.0f, 6.2f, -9.0f, 9.0f, 9.0f, 9.0f)));//Oben funktioniert
	obsticlesL1.push_back(createObsticle(2.5f, 0.1f, -0.2f, -19.0f, createBorder(-9.0f, -9.0f, 11.0f, -9.0f, -9.0f, -6.2f, 11.0f, -6.2f)));//Unten funktioniert
	obsticlesL1.push_back(createObsticle(0.1f, 1.8f, 19.0f,   0.0f, createBorder(-9.0f, -7.5f, -6.2f, -7.5f, -9.0f, 7.5f, -6.2f, 7.5f)));//Links funktioniert
	obsticlesL1.push_back(createObsticle(0.1f, 1.4f, -19.0f,  0.4f, createBorder(6.2f, -4.2f, 9.0f, -4.2f, 6.2f, 7.5f, 9.0f, 7.5f)));//Rechts funktioniert
	obsticlesL1.push_back(createObsticle(1.4f, 0.1f,  -0.3f, -7.4f, createBorder(-4.5f, -4.2f, 7.0f, -4.2f, -4.5f, -1.6f, 7.0f, -1.6f)));//MittelBalken funktioniert


	//Level2
	obsticlesL2.push_back(createObsticle(2.0f, 0.1f, 0.0f, 19.0f, createBorder(-9.0f, 6.2f, 9.0f, 6.2f, -9.0f, 9.0f, 9.0f, 9.0f)));//Oben funktioniert
	obsticlesL2.push_back(createObsticle(2.5f, 0.1f, -0.2f, -19.0f, createBorder(-9.0f, -9.0f, 11.0f, -9.0f, -9.0f, -6.2f, 11.0f, -6.2f)));//Unten funktioniert
	obsticlesL2.push_back(createObsticle(0.1f, 1.05f, 19.0f, 0.2f, createBorder(-9.0f, -4.0f, -6.2f, -4.0f, -9.0f, 5.5f, -6.2f, 5.5f)));//Links funktioniert
	obsticlesL2.push_back(createObsticle(0.1f, 1.4f, -19.0f, 0.4f, createBorder(6.2f, -4.2f, 9.0f, -4.2f, 6.2f, 7.5f, 9.0f, 7.5f)));//Rechts funktioniert
	obsticlesL2.push_back(createObsticle(1.4f, 0.1f, -0.3f, 4.0f, createBorder(-4.8f, 0.2f, 7.5f, 0.2f, -4.8f, 3.0f, 7.5f, 3.0f)));//MittelBalkenMitte funktioniert
	obsticlesL2.push_back(createObsticle(1.4f, 0.1f, 0.29f, 11.7f, createBorder(-8.0f, 3.2f, 5.0f, 3.2f, -8.0f, 6.1f, 5.0f, 6.1f)));//MittelBalkenOben funktioniert
	obsticlesL2.push_back(createObsticle(2.4f, 0.1f, 0.17f, -9.4f, createBorder(-11.0f, -5.2f, 9.0f, -5.2f, -11.0f, -2.3f, 9.0f, -2.3f)));//MittelBalkenUnten funktioniert


	//Level3
	obsticlesL3.push_back(createObsticle(2.0f, 0.1f, 0.0f, 19.0f, createBorder(-9.0f, 6.2f, 9.0f, 6.2f, -9.0f, 9.0f, 9.0f, 9.0f)));//Oben funktioniert
	obsticlesL3.push_back(createObsticle(2.5f, 0.1f, -0.2f, -19.0f, createBorder(-9.0f, -9.0f, 11.0f, -9.0f, -9.0f, -6.2f, 11.0f, -6.2f)));//Unten funktioniert
	obsticlesL3.push_back(createObsticle(0.1f, 1.5f, 19.0f, -0.2f, createBorder(-9.0f, -8.2f, -6.2f, -8.2f, -9.0f, 5.8f, -6.2f, 5.8f)));//Links funktioniert
	obsticlesL3.push_back(createObsticle(0.1f, 0.9f, 10.8f, 0.445f, createBorder(-5.7f, -3.0f, -2.9f, -3.0f, -5.7f, 6.2f, -2.9f, 6.2f)));//LinksInnen funktioniert
	obsticlesL3.push_back(createObsticle(0.1f, 0.355f, -19.0f, 4.1f, createBorder(6.2f, 3.4f, 9.0f, 3.4f, 6.2f, 7.5f, 9.0f, 7.5f)));//Rechts funktioniert
	obsticlesL3.push_back(createObsticle(0.1f, 0.75f, -19.0f, -1.4f, createBorder(6.2f, -8.2f, 9.0f, -8.2f, 6.2f, -0.2f, 9.0f, -0.2f)));//RechtsUnten funktioniert nicht
	obsticlesL3.push_back(createObsticle(1.4f, 0.1f, -0.3f, 4.0f, createBorder(-4.8f, 0.2f, 8.3f, 0.2f, -4.8f, 3.0f, 8.3f, 3.0f)));//MittelBalkenMitte funktioniert nicht
	obsticlesL3.push_back(createObsticle(1.4f, 0.1f, -0.3f, 12.0f, createBorder(-4.8f, 3.4f, 7.5f, 3.4f, -4.8f, 6.2f, 7.5f, 6.2f)));//MittelBalkenOben funktioniert nicht
	obsticlesL3.push_back(createObsticle(1.4f, 0.1f, -0.3f, -4.0f, createBorder(-4.8f, -3.0f, 8.3f, -3.0f, -4.8f, -0.2f, 8.3f, -0.2f)));//MittelBalkenUnten funktioniert nicht

	//float bl_x, float bl_y, float br_x, float br_y, float tl_x, float tl_y, float tr_x, float tr_y

	/*
	obsticles.push_back(createObsticle(2.0f, 0.1f, 0.0f, 19.0f, createBorder(-9.0f, 6.2f, 9.0f, 6.2f, -9.0f, 9.0f, 9.0f, 9.0f)));//Oben
	obsticles.push_back(createObsticle(2.0f, 0.1f, 0.0f, -19.0f, createBorder(-9.0f, -6.2f, 9.0f, -6.2f, -9.0f, -9.0f, 9.0f, -9.0f)));//Unten
	obsticles.push_back(createObsticle(0.1f, 1.8f, 19.0f, 0.0f, createBorder(-9.0f, -6.2f, 9.0f, -6.2f, -9.0f, -9.0f, 9.0f, -9.0f)));//Links
	obsticles.push_back(createObsticle(0.1f, 1.4f, -19.0f, 0.4f, createBorder(-9.0f, 6.2f, 9.0f, 6.2f, -9.0f, 9.0f, 9.0f, 9.0f)));//Rechts
	*/

	// Eventloop
	while (!glfwWindowShouldClose(window))
	{
		switch (level)
		{
		case 1:
			obsticles = obsticlesL1;
			break;
		case 2:
			obsticles = obsticlesL2;
			break;
		case 3:
			obsticles = obsticlesL3;
			break;
		default:
			break;
		}
		
		newtime = glfwGetTime();
		diftime = newtime - oldtime;
		oldtime = newtime;
		std::cout << " FrameTime: " << diftime;

		glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		
		// Camera matrix
		View = glm::lookAt(glm::vec3(0,0,-5), // Camera is at (0,0,-5), in World Space
						   glm::vec3(0,0,0),  // and looks at the origin
						   glm::vec3(0,1,0)); // Head is up (set to 0,-1,0 to look upside-down)
		
		// Model matrix : an identity matrix (model will be at the origin)
		Model = glm::mat4(1.0f);
		Model = glm::rotate(Model, xangel, glm::vec3(1.0f, 0.0f, 0.0f));
		Model = glm::rotate(Model, yangel, glm::vec3(0.0f, 1.0f, 0.0f));
		Model = glm::rotate(Model, zangel, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 Save = Model;
		Model = glm::translate(Model, glm::vec3(1.5, 0.0, 0.0));
		Model = glm::scale(Model, glm::vec3(1.0 / 1000.0, 1.0 / 1000.0, 1.0 / 1000.0));

		sendMVP();

		Model = Save;
		Model = glm::scale(Model, glm::vec3(0.5, 0.5, 0.5));
		sendMVP();


		//draw

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWorld);

		Save = Model;//hilfsvariable
		Model = glm::scale(Model, glm::vec3(10.0, 10.0, 10.0));//berechnen
		sendMVP();//an die grafikkarte schicken
		world.display();
		Model = Save;

		drawLight(0,0,-15);
		drawScene(&world);
		
		Model = Save;
		zeichneKS();
		sendMVP();

		glfwSwapBuffers(window);
        glfwPollEvents();
	} 

	glDeleteProgram(programID);
	//glDeleteTextures(1, &Texture);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}
//OBJ3D teapot("teapot");

Border::Border()
{
}

Point::Point()
{
}
