#include "Obsticle.h"

float boundx;
float boundy;
float positionx;
float positiony;
float high;


Obsticle::Obsticle()
{
	
}

Obsticle::Obsticle(float x, float y, float posx, float posy)
{
	boundx = x;
	boundy = y;
	positionx = posx;
	positiony = posy;
	high = -1.5;
}


Obsticle::~Obsticle()
{
}

float Obsticle::getBoundX()
{
	return boundx;
}

float Obsticle::getBoundY()
{
	return boundy;
}

float Obsticle::getPosX()
{
	return positionx;
}

float Obsticle::getPosY()
{
	return positiony;
}

float Obsticle::getHigh()
{
	return high;
}
