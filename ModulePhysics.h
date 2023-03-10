#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "iostream"

// The physics engine implements at least four of the following forces: 
// impulsive (F*t), 
// gravity (g=9,8 or 10) (Fg = m*g), 
// lift (L=1/2*p*Cl*S),
// drag (D=1/2*p*Cd*S), 
// buoyancy (EN CATAL?: flotabilitat) (, 
// elastic(springs) (f=kx).

#define M_PI 3.14159265358979323846
#define PIXELS_PER_METER 32.0f // if touched change METER_PER_PIXEL too
#define METERS_PER_PIXEL 0.03125f // this is 1 / PIXELS_PER_METER !
#define RAD_PER_DEG ((float)180/M_PI)
#define DEG_PER_RAD ((float)M_PI/180)

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXELS_TO_METERS(p)  ((float) METERS_PER_PIXEL * p)
#define RAD_TO_DEG(r) ((float) RAD_PER_DEG * r)
#define DEG_TO_RAD(r) ((float) DEG_PER_RAD * r)
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

#define GRAVITY -10
#define DELTATIME 0.016666666666666 

#define AIR_DENSITY 1.293f

enum Bodytype
{
	RECTANGLE,
	CIRCLE,	
};

enum Label
{
	PLAYER_1,
	PLAYER_2,
	GRENADE,
	MISSILE,
	DEAD
};

// Class: Atmosphere -> From the example
class PhysBody
{
public:

	double px = 0.0f, py = 0.0f;
	double vx = 0.0f, vy = 0.0f;
	double ax = 0.0f, ay = 0.0f;

	int shootAngle;
	int shootForce;
	bool isAlive;
	bool isStable;
	int isJumping;
	bool isGrounded;
	bool isHit;
	Bodytype type;
	Label label;

	//THIS IS FOR TESTING DO NOT DELETE
	double radius = 1.0f;

	// Force (total) applied to the ball
	float fx = 0.0f;
	float fy = 0.0f;

	// Drag
	float drag_fx = 0.0f;
	float drag_fy = 0.0f;
	float v_rel_x = 0.0f;
	float v_rel_y = 0.0f;
	float drag_surface = 0.3f;

	// Mass
	float mass;

	// Aerodynamics stuff
	float surface; // Effective wet surface
	float cl; // Aerodynamic Lift coefficient
	float cd; // Aerodynamic Drag coefficient
	float b; // Hydrodynamic Drag coefficient

	// Coefficients of friction & restitution (for bounces)
	float coef_friction;
	float coef_restitution;

};

class Circle : public PhysBody {
public:
	float radius = 1.0f;
	uint life = 100;

};

class Rect : public PhysBody {
public:
	int w, h;
};

struct Explosion
{
	double steps;
	double stepIterator;
	double radius;
	double x;
	double y;
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

class Enemy : public Ground
{
public:
	int life;
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

	Ground* CreateGround(float gx, float gy, float gw, float gh);
	Water* CreateWater(float wx, float wy, float ww, float wh);
	Atmosphere* CreateAtmosphere();
	Enemy* CreateEnemy(float ex, float ey, float ew, float eh);

	void CheckIfHit(PhysBody& body, Explosion& ex);

	void Drag(PhysBody* phbody);

	p2List<Circle*> listBodies;
	double pointList[32][2];
	double vectorX;
	double vectorY;
	double angle;

	// Compute modulus of a vector
	float modulus(float vx, float vy);

	// Compute Aerodynamic Drag force
	void compute_aerodynamic_drag(float& fx, float& fy, const Circle& ball, const Atmosphere& atmosphere);

	// Compute Hydrodynamic Drag force
	void compute_hydrodynamic_drag(float& fx, float& fy, const Circle& ball, const Water& water);

	// Compute Hydrodynamic Buoyancy force
	void compute_hydrodynamic_buoyancy(float& fx, float& fy, const Circle& ball, const Water& water);

	// Integration scheme: Velocity Verlet
	void integrator_velocity_verlet(Circle& ball, float dt);

	// Detect collision with ground
	bool is_colliding_with_ground(const Circle& ball, const Ground& ground);

	// Detect collision with water
	bool is_colliding_with_water(const Circle& ball, const Water& water);

	// Detect collision with enemy
	bool is_colliding_with_enemy(const Circle& ball, const Enemy& enemy);

	bool is_colliding_with_player(const Circle& ball, const Circle& player);

	// Detect collision between circle and rectange
	bool check_collision_circle_rectangle(float cx, float cy, float cr, float rx, float ry, float rw, float rh);

	bool check_collision_circles(float cx1, float cy1, float cr1, float cx2, float cy2, float cr2);

	// Detect Direction
	void collision_direction(Circle& pBody, const Ground& ground);
	void detect_direction_ground(Circle& pBody, const Ground& ground);
	void detect_direction_enemy(Circle& pBody, const Enemy& enemy);

	void SetGravity(float g);
	float GetGravity();
private:

	Atmosphere atm;
	Ground ground;
	Water water;
	Enemy enemy;
	bool debug;
	float dt;
	float gravity;
};
