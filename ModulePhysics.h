#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

// The physics engine implements at least four of the following forces: 
// impulsive (mrua), 
// gravity (g=9,8 or 10), 
// lift (Fy=F-N=F-m*g*sin angle),
// drag (Fx=F-Ff=F*cos angle-m*g*sin angle*coef_dragging), 
// buoyancy (EN CATALÀ: flotabilitat), 
// elastic(springs) (f=kx).

// Class: Atmosphere -> From the example
class PhysBody {
	int x, y;

};

class Circle : public PhysBody {
	int radius;
};

class Rectangle :public PhysBody {
	int w, h;
};

class Atmosphere
{
public:
	float density; // Density of air
	float windx; // Wind x
	float windy; // Wind y
};

// Class: Ground -> From the example
class Ground : public SDL_Rect
{
public:
	float x, y, w, h; // Geometry (x,y,w,h)
	SDL_Rect pixels(); // Convert geometry to pixels to draw w/ SDL
};

// Class: Water, public Ground -> From the example
class Water : public Ground
{
public:
	float density; // Density of fluid
	float vx; // Velocity x
	float vy; // Velocity y
};

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

private:
	Atmosphere atm;
	Ground ground;
	Water water;
	bool debug;
};