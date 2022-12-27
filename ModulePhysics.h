#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

// The physics engine implements at least four of the following forces: 
// impulsive (F*t), 
// gravity (g=9,8 or 10) (Fg = m*g), 
// lift (L=1/2*p*Cl*S),
// drag (D=1/2*p*Cd*S), 
// buoyancy (EN CATALÀ: flotabilitat) (, 
// elastic(springs) (f=kx).

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum bodytype {
	PLAYER,
	SHOT
};

// Class: Atmosphere -> From the example
class PhysBody
{
public:
	double px = 0.0f, py = 0.0f;
	double vx = 0.0f, vy = 0.0f;
	double ax = 0.0f, ay = 0.0f;

	bool isAlive;
	bodytype type;
};

class Circle : public PhysBody {
	int radius;
};

class Rect : public PhysBody {
	int w, h;
};

class Atmosphere
{
public:
	double density; // Density of air
	double windx; // Wind x
	double windy; // Wind y
};

// Class: Ground -> From the example
class Ground : public SDL_Rect
{
public:
	double x, y, w, h; // Geometry (x,y,w,h)
	SDL_Rect pixels() {} // Convert geometry to pixels to draw w/ SDL
};

// Class: Water, public Ground -> From the example
class Water : public Ground
{
public:
	double density; // Density of fluid
	double vx; // Velocity x
	double vy; // Velocity y
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

	SDL_Rect CreateGround(float gx, float gy, float gw, float gh);

private:
	Atmosphere atm;
	Ground ground;
	Water water;
	bool debug;
};

// Compute modulus of a vector
float modulus(float vx, float vy);

// Compute Aerodynamic Drag force
void compute_aerodynamic_drag(float& fx, float& fy, const Rect& ball, const Atmosphere& atmosphere);

// Compute Hydrodynamic Drag force
void compute_hydrodynamic_drag(float& fx, float& fy, const Rect& ball, const Water& water);

// Compute Hydrodynamic Buoyancy force
void compute_hydrodynamic_buoyancy(float& fx, float& fy, const Rect& ball, const Water& water);

// Integration scheme: Velocity Verlet
void integrator_velocity_verlet(Rect& ball, float dt);

// Detect collision with ground
bool is_colliding_with_ground(const Rect& ball, const Ground& ground);

// Detect collision with water
bool is_colliding_with_water(const Rect& ball, const Water& water);

// Detect collision between circle and rectange
bool check_collision_circle_rectangle(float cx, float cy, float cr, float rx, float ry, float rw, float rh);