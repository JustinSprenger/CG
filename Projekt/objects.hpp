#ifndef OBJECTS_HPP
#define OBJECTS_HPP
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
void drawWireCube(); // Wuerfel mit Kantenlaenge 2 im Drahtmodell
void drawCube();     // Bunter Wuerfel mit Kantenlaenge 2
void drawSphere(GLuint slices, GLuint stacks); // Kugel mit radius 1 bzw. Durchmesser 2

#endif
