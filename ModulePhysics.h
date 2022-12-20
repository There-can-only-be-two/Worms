#pragma once
#include "Module.h"
#include "Globals.h"

class Atmosphere
{
public:
	float density; // Density of air
	float windx; // Wind x
	float windy; // Wind y
};

class Ground : public SDL_Rect
{
public:
	float x, y, w, h; // Geometry (x,y,w,h)
	SDL_Rect pixels(); // Convert geometry to pixels to draw w/ SDL
};

// Class: Water
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

	bool debug;
};