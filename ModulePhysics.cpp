#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include <cmath>
#include "ModuleHole.h"

// TODO 1: Include Box 2 header and library

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
		debug = true;

}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	// Create ground
	ground = Ground();
	ground.x = 0.0f; // [m]
	ground.y = 0.0f; // [m]
	ground.w = 30.0f; // [m]
	ground.h = 5.0f; // [m]
	
	//-------------------------------------------------------------------------
	//Plataformas				 
	
	plataforma_1 = Ground();
	plataforma_1.x = 3.0f; // [m]
	plataforma_1.y = 15.0f; // [m]
	plataforma_1.w = 15.0f; // [m]
	plataforma_1.h = 2.0f; // [m]
	
	plataforma_1_pared_d = Ground();
	plataforma_1_pared_d.x = plataforma_1.x + plataforma_1.w + 0.005f;
	plataforma_1_pared_d.y = plataforma_1.y;
	plataforma_1_pared_d.w = 0.2f;
	plataforma_1_pared_d.h = plataforma_1.h;

	plataforma_1_pared_i = Ground();
	plataforma_1_pared_i.x = plataforma_1.x - 0.01f;
	plataforma_1_pared_i.y = plataforma_1.y;
	plataforma_1_pared_i.w = 0.2f;
	plataforma_1_pared_i.h = plataforma_1.h;
	

	//-------------------------------------------------------------------------
	
	plataforma_2 = Ground();
	plataforma_2.x = 35.0f; // [m]
	plataforma_2.y = 22.0f; // [m]
	plataforma_2.w = 10.0f; // [m]
	plataforma_2.h = 2.0f; // [m]

	plataforma_2_pared_d = Ground();
	plataforma_2_pared_d.x = plataforma_2.x + plataforma_2.w + 0.005f;
	plataforma_2_pared_d.y = plataforma_2.y;
	plataforma_2_pared_d.w = 0.2f;
	plataforma_2_pared_d.h = plataforma_2.h;

	plataforma_2_pared_i = Ground();
	plataforma_2_pared_i.x = plataforma_2.x - 0.01f;
	plataforma_2_pared_i.y = plataforma_2.y;
	plataforma_2_pared_i.w = 0.2f;
	plataforma_2_pared_i.h = plataforma_2.h;
	
	//-------------------------------------------------------------------------
	
	plataforma_3 = Ground();
	plataforma_3.x = 5.0f; // [m]
	plataforma_3.y = 28.0f; // [m]
	plataforma_3.w = 5.0f; // [m]
	plataforma_3.h = 2.0f; // [m]

	plataforma_3_pared_d = Ground();
	plataforma_3_pared_d.x = plataforma_3.x + plataforma_3.w + 0.005f;
	plataforma_3_pared_d.y = plataforma_3.y;
	plataforma_3_pared_d.w = 0.2f;
	plataforma_3_pared_d.h = plataforma_3.h;

	plataforma_3_pared_i = Ground();
	plataforma_3_pared_i.x = plataforma_3.x - 0.01f;
	plataforma_3_pared_i.y = plataforma_3.y;
	plataforma_3_pared_i.w = 0.2f;
	plataforma_3_pared_i.h = plataforma_3.h;

	//-------------------------------------------------------------------------
	//Paredes
	pared_d = Ground();
	pared_d.x = 0.0f; // [m]
	pared_d.y = 0.0f; // [m]
	pared_d.w = 0.2f; // [m]
	pared_d	.h = 40.0f; // [m]

	//-------------------------------------------------------------------------

	pared_i = Ground();
	pared_i.x = 51.0f; // [m]
	pared_i.y = 0.0f; // [m]
	pared_i.w = 2.0f; // [m]
	pared_i.h = 50.0f; // [m]

	// Create Water
	water = Water();
	water.x = ground.x + ground.w; // Start where ground ends [m]
	water.y = 0.0f; // [m]
	water.w = 21.0f; // [m]
	water.h = 5.0f; // [m]
	water.density = 50.0f; // [kg/m^3]
	water.vx = -1.0f; // [m/s]
	water.vy = 0.0f; // [m/s]

	// Create atmosphere
	atmosphere = Atmosphere();
	atmosphere.windx = 10.0f; // [m/s]
	atmosphere.windy = 5.0f; // [m/s]
	atmosphere.density = 1.0f; // [kg/m^3]

	// Create a ball
	PhysBall ball = PhysBall();

	// Set static properties of the ball
	ball.mass = 10.0f; // [kg]
	ball.surface = 1.0f; // [m^2]
	ball.radius = 1.0f; // [m]
	ball.cd = 0.4f; // [-]
	ball.cl = 1.2f; // [-]
	ball.b = 10.0f; // [...]
	ball.coef_friction = 0.9f; // [-]
	ball.coef_restitution = 0.8f; // [-]

	// Set initial position and velocity of the ball
	ball.x = 2.0f;
	ball.y = (ground.y + ground.h) + 1.1;
	ball.vx = 0;
	ball.vy = 0;
	ball.physics_enabled = false;

	if (ball.x >= 5.0)
	{
		App->hole->tocado = true;
	}
	// Add ball to the collection
	balls.push_back(ball);
	
	return true;
}

update_status ModulePhysics::PreUpdate()
{

	dt = 1 / target_fps;

	// Process all balls in the scenario
	for (auto& ball : balls)
	{
		// Skip ball if physics not enabled
		if (!ball.physics_enabled)
		{
			continue;
		}

		

		// Step #1: Compute forces
		// ----------------------------------------------------------------------------------------

		// Gravity force
		float fgx = ball.mass * 0.0f;
		float fgy = ball.mass * -10.0f; // Let's assume gravity is constant and downwards
		ball.fx += fgx; ball.fy += fgy; // Add this force to ball's total force

		// Aerodynamic Drag force (only when not in water)
		if (!is_colliding_with_water(ball, water))
		{
			float fdx = 0.0f; float fdy = 0.0f;
			compute_aerodynamic_drag(fdx, fdy, ball, atmosphere);
			ball.fx += fdx; ball.fy += fdy; // Add this force to ball's total force
		}

		// Hydrodynamic forces (only when in water)
		if (is_colliding_with_water(ball, water))
		{
			// Hydrodynamic Drag force
			float fhdx = 0.0f; float fhdy = 0.0f;
			compute_hydrodynamic_drag(fhdx, fhdy, ball, water);
			ball.fx += fhdx; ball.fy += fhdy; // Add this force to ball's total force

			// Hydrodynamic Buoyancy force
			float fhbx = 0.0f; float fhby = 0.0f;
			compute_hydrodynamic_buoyancy(fhbx, fhby, ball, water);
			ball.fx += fhbx; ball.fy += fhby; // Add this force to ball's total force
		}

		// Impulsive forces (only if there is any)
		if (true)
		{

		}

		// Other forces
		// ...

		// Step #2: 2nd Newton's Law
		// ----------------------------------------------------------------------------------------
		
		// SUM_Forces = mass * accel --> accel = SUM_Forces / mass
		ball.ax = ball.fx / ball.mass;
		ball.ay = ball.fy / ball.mass;

		// Step #3: Integrate --> from accel to new velocity & new position
		// ----------------------------------------------------------------------------------------

		// We will use the 2nd order "Velocity Verlet" method for integration.
		
		switch (App->player->selected_integrator)
		{
		case Integrador::VERLET:
			integrator_velocity_verlet(ball, dt);
			break;
		case Integrador::FWD_EULER:
			integrator_velocity_euler_forward(ball, dt);
			break;
		case Integrador::BWD_EULER:
			integrator_velocity_euler_backward(ball, dt);
			break;
		}

		// Step #4: solve collisions
		// ----------------------------------------------------------------------------------------

		// Calculate if it is off floor
		if (ball.vy > -0.5 && ball.vy < 0.5)
		{

		}
		else
		{
			ball.on_floor = false;
		}



		// Solve collision between ball and ground
		if (is_colliding_with_ground(ball, ground))
		{
			//Decección de que está en el suelo
			if (ball.vy > -0.9 && ball.vy < 0.9)
			{
			
				switch (App->player->selected_collisioner)
				{
					case Collisioner::M1:
						ball.y = ball.y + 1.1;
					break;

					case Collisioner::M2:
						ball.y = ball.y + 1 - ball.radius;
					break;

					case Collisioner::M3:

					break;
				}

				ball.on_floor = true;
			}
			
			float old_y = ball.y;

			switch (App->player->selected_collisioner)
			{
			case Collisioner::M1:
				// TP ball to ground surface
				ball.y = ground.y + ground.h + ball.radius;
				break;
			case Collisioner::M2:
				//TP ball to the relative position "inside" the ground but in the other way
				ball.y = ball.y + 2 *( (ground.y + ground.h) - old_y) + 2*ball.radius;
				break;
			case Collisioner::M3:
				//Without any collision handling

				break;
			}

			// Elastic bounce with ground
			ball.vy = - ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;
		}
		
		//Plataformas
		
		if (is_colliding_with_ground(ball, plataforma_1))
		{
			//Decección de que está en el suelo
			if (ball.vy > -0.9 && ball.vy < 0.9)
			{

				switch (App->player->selected_collisioner)
				{
				case Collisioner::M1:
					ball.y = ball.y + 1.1;
					break;

				case Collisioner::M2:
					ball.y = ball.y + 1 - ball.radius;
					break;

				case Collisioner::M3:

					break;
				}

				ball.on_floor = true;
			}

			float old_y = ball.y;

			switch (App->player->selected_collisioner)
			{
			case Collisioner::M1:
				// TP ball to ground surface
				ball.y = plataforma_1.y + plataforma_1.h + ball.radius;
				break;
			case Collisioner::M2:
				//TP ball to the relative position "inside" the ground but in the other way
				ball.y = ball.y + 2 * ((plataforma_1.y + plataforma_1.h) - old_y) + 2 * ball.radius;
				break;
			case Collisioner::M3:
				//Without any collision handling

				break;
			}

			// Elastic bounce with ground
			ball.vy = -ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;
		}
		
		//-----------------------------------------------------------------------

		if (is_colliding_with_ground(ball, plataforma_2))
		{
			//Decección de que está en el suelo
			if (ball.vy > -0.9 && ball.vy < 0.9)
			{

				switch (App->player->selected_collisioner)
				{
				case Collisioner::M1:
					ball.y = ball.y + 1.1;
					break;

				case Collisioner::M2:
					ball.y = ball.y + 1 - ball.radius;
					break;

				case Collisioner::M3:

					break;
				}

				ball.on_floor = true;
			}

			float old_y = ball.y;

			switch (App->player->selected_collisioner)
			{
			case Collisioner::M1:
				// TP ball to ground surface
				ball.y = plataforma_2.y + plataforma_2.h + ball.radius;
				break;
			case Collisioner::M2:
				//TP ball to the relative position "inside" the ground but in the other way
				ball.y = ball.y + 2 * ((plataforma_2.y + plataforma_2.h) - old_y) + 2 * ball.radius;
				break;
			case Collisioner::M3:
				//Without any collision handling

				break;
			}

			// Elastic bounce with ground
			ball.vy = -ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;
		}

		//-----------------------------------------------------------------------
		
		if (is_colliding_with_ground(ball, plataforma_3))
		{
			//Decección de que está en el suelo
			if (ball.vy > -0.9 && ball.vy < 0.9)
			{

				switch (App->player->selected_collisioner)
				{
				case Collisioner::M1:
					ball.y = ball.y + 1.1;
					break;

				case Collisioner::M2:
					ball.y = ball.y + 1 - ball.radius;
					break;

				case Collisioner::M3:

					break;
				}

				ball.on_floor = true;
			}

			float old_y = ball.y;

			switch (App->player->selected_collisioner)
			{
			case Collisioner::M1:
				// TP ball to ground surface
				ball.y = plataforma_3.y + plataforma_3.h + ball.radius;
				break;
			case Collisioner::M2:
				//TP ball to the relative position "inside" the ground but in the other way
				ball.y = ball.y + 2 * ((plataforma_3.y + plataforma_3.h) - old_y) + 2 * ball.radius;
				break;
			case Collisioner::M3:
				//Without any collision handling

				break;
			}

			// Elastic bounce with ground
			ball.vy = -ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;
		}
		
		//Paredes
		//Pared de la izquierda
		if (is_colliding_with_ground(ball, pared_d))
		{
			float old_x = ball.x;

			switch (App->player->selected_collisioner)
			{
			case Collisioner::M1:
				// TP ball to ground surface
				ball.x = (pared_d.x + pared_d.w) + ball.radius;
				break;
			case Collisioner::M2:
				//TP ball to the relative position "inside" the ground but in the other way
				ball.x = ball.x - 0.7 * ((pared_d.x + pared_d.w) - old_x);
				break;
			case Collisioner::M3:
				//Without any collision handling

				break;
			}

			// Elastic bounce with wall
			ball.vx = -ball.vx;

			// FUYM non-elasticity
			ball.vy *= ball.coef_friction;
			ball.vx *= ball.coef_restitution;
		}
		
		//------------------------------------------------------------------
		//Pared de la derecha
		if (is_colliding_with_ground(ball, pared_i))
		{
			float old_x = ball.x;

			switch (App->player->selected_collisioner)
			{
			case Collisioner::M1:
				// TP ball to ground surface
				ball.x = pared_i.x - ball.radius;
				break;
			case Collisioner::M2:
				//TP ball to the relative position "inside" the ground but in the other way
				ball.x = ball.x - 0.7 * (pared_i.x - old_x);
				break;
			case Collisioner::M3:
				//Without any collision handling

				break;
			}

			// Elastic bounce with wall
			ball.vx = -ball.vx;

			// FUYM non-elasticity
			ball.vy *= ball.coef_friction;
			ball.vx *= ball.coef_restitution;
		}

		//PLATAFORMAS 2
//Paredes Izquierda plataformas

		if (is_colliding_with_ground(ball, plataforma_1_pared_i))
		{
			float old_x = ball.x;

			switch (App->player->selected_collisioner)
			{
			case Collisioner::M1:
				// TP ball to ground surface
				ball.x = (plataforma_1_pared_i.x + plataforma_1_pared_i.w) + ball.radius;
				break;
			case Collisioner::M2:
				//TP ball to the relative position "inside" the ground but in the other way
				ball.x = ball.x - 0.7 * ((plataforma_1_pared_i.x + plataforma_1_pared_i.w) - old_x);
				break;
			case Collisioner::M3:
				//Without any collision handling

				break;
			}

			// Elastic bounce with wall
			ball.vx = -ball.vx;

			// FUYM non-elasticity
			ball.vy *= ball.coef_friction;
			ball.vx *= ball.coef_restitution;
		}

		if (is_colliding_with_ground(ball, plataforma_2_pared_i))
		{
			float old_x = ball.x;

			switch (App->player->selected_collisioner)
			{
			case Collisioner::M1:
				// TP ball to ground surface
				ball.x = (plataforma_1_pared_i.x + plataforma_1_pared_i.w) + ball.radius;
				break;
			case Collisioner::M2:
				//TP ball to the relative position "inside" the ground but in the other way
				ball.x = ball.x - 0.7 * ((plataforma_1_pared_i.x + plataforma_1_pared_i.w) - old_x);
				break;
			case Collisioner::M3:
				//Without any collision handling

				break;
			}

			// Elastic bounce with wall
			ball.vx = -ball.vx;

			// FUYM non-elasticity
			ball.vy *= ball.coef_friction;
			ball.vx *= ball.coef_restitution;
		}

		if (is_colliding_with_ground(ball, plataforma_3_pared_i))
		{
			float old_x = ball.x;

			switch (App->player->selected_collisioner)
			{
			case Collisioner::M1:
				// TP ball to ground surface
				ball.x = (plataforma_3_pared_i.x + plataforma_3_pared_i.w) + ball.radius;
				break;
			case Collisioner::M2:
				//TP ball to the relative position "inside" the ground but in the other way
				ball.x = ball.x - 0.7 * ((plataforma_3_pared_i.x + plataforma_3_pared_i.w) - old_x);
				break;
			case Collisioner::M3:
				//Without any collision handling

				break;
			}

			// Elastic bounce with wall
			ball.vx = -ball.vx;

			// FUYM non-elasticity
			ball.vy *= ball.coef_friction;
			ball.vx *= ball.coef_restitution;
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------
		//Paredes Derecha plataformas

		if (is_colliding_with_ground(ball, plataforma_1_pared_d))
		{
			float old_x = ball.x;

			switch (App->player->selected_collisioner)
			{
			case Collisioner::M1:
				// TP ball to ground surface
				ball.x = plataforma_1_pared_d.x - ball.radius;
				break;
			case Collisioner::M2:
				//TP ball to the relative position "inside" the ground but in the other way
				ball.x = ball.x - 0.7 * (plataforma_1_pared_d.x - old_x);
				break;
			case Collisioner::M3:
				//Without any collision handling

				break;
			}

			// Elastic bounce with wall
			ball.vx = -ball.vx;

			// FUYM non-elasticity
			ball.vy *= ball.coef_friction;
			ball.vx *= ball.coef_restitution;
		}

		if (is_colliding_with_ground(ball, plataforma_2_pared_d))
		{
			float old_x = ball.x;

			switch (App->player->selected_collisioner)
			{
			case Collisioner::M1:
				// TP ball to ground surface
				ball.x = plataforma_2_pared_d.x - ball.radius;
				break;
			case Collisioner::M2:
				//TP ball to the relative position "inside" the ground but in the other way
				ball.x = ball.x - 0.7 * (plataforma_2_pared_d.x - old_x);
				break;
			case Collisioner::M3:
				//Without any collision handling

				break;
			}

			// Elastic bounce with wall
			ball.vx = -ball.vx;

			// FUYM non-elasticity
			ball.vy *= ball.coef_friction;
			ball.vx *= ball.coef_restitution;
		}

		if (is_colliding_with_ground(ball, plataforma_3_pared_d))
		{
			float old_x = ball.x;

			switch (App->player->selected_collisioner)
			{
			case Collisioner::M1:
				// TP ball to ground surface
				ball.x = plataforma_3_pared_d.x - ball.radius;
				break;
			case Collisioner::M2:
				//TP ball to the relative position "inside" the ground but in the other way
				ball.x = ball.x - 0.7 * (plataforma_3_pared_d.x - old_x);
				break;
			case Collisioner::M3:
				//Without any collision handling

				break;
			}

			// Elastic bounce with wall
			ball.vx = -ball.vx;

			// FUYM non-elasticity
			ball.vy *= ball.coef_friction;
			ball.vx *= ball.coef_restitution;
		}

		// Step #0: Clear old values
		// ----------------------------------------------------------------------------------------

		// Reset total acceleration and total accumulated force of the ball
		ball.fx = ball.fy = 0.0f;
		ball.ax = ball.ay = 0.0f;


		//DEBUG CONTROLS
		//Aplicar fuerza de viento hacia la derecha
		if (App->input->GetKey(SDL_SCANCODE_1)==KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			atmosphere.windx += 5;
		}
		//Aplicar fuerza de viento hacia la izquierda
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			atmosphere.windx -= 5;
		}
		//Aplicar fuerza de viento hacia arriba
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
		{
			atmosphere.windy += 5;
		}
		//Aplicar fuerza de viento hacia abajo
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			atmosphere.windy -= 5;
		}
		//Aumenta densidad de l'atmosfera
		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			atmosphere.density += 5;
		}
		//Disminuye densidad de l'atmosfera
		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			atmosphere.density -= 5;
		}
		//Aumenta la massa y el radio de la pelota
		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			ball.mass += 10;
			ball.radius += 0.5;
		}
		//Disminuye la massa y el radio de la pelota
		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			ball.mass -= 5;
			ball.radius -= 0.5;
		}
		//Aumenta la resistencia aerodinamica
		if (App->input->GetKey(SDL_SCANCODE_4) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			ball.cd += 1;
			
		}
		//Disminuye la resistencia aerodinamica
		if (App->input->GetKey(SDL_SCANCODE_4) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			ball.cd -= 1;

		}
		//Aumenta el lift aerodinamico
		if (App->input->GetKey(SDL_SCANCODE_5) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			ball.cl += 1;

		}
		//Disminuye el lift aerodinamico
		if (App->input->GetKey(SDL_SCANCODE_5) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			ball.cl -= 1;

		}
		//Aumenta el coeficiente de friccion
		if (App->input->GetKey(SDL_SCANCODE_6) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			ball.coef_friction += 2;

		}
		//Disminuye el coeficiente de friccionCoeficient de restitució
		if (App->input->GetKey(SDL_SCANCODE_6) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			ball.coef_friction -= 2;

		}
		//Aumenta el coeficient de restitució
		if (App->input->GetKey(SDL_SCANCODE_7) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			ball.coef_restitution += 2;

		}
		//Disminuye el coeficient de restitució
		if (App->input->GetKey(SDL_SCANCODE_7) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			ball.coef_restitution -= 2;

		}
		//Aumenta la densidad del agua
		if (App->input->GetKey(SDL_SCANCODE_8) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			water.density += 5;

		}
		//Disminuye la densidad del agua
		if (App->input->GetKey(SDL_SCANCODE_8) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			water.density -= 5;

		}
	}
	
	// Continue game
	return UPDATE_CONTINUE;
}

update_status ModulePhysics::PostUpdate()
{
	//Cambiar los FPS
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		switch (App->player->fps_limit)
		{
		case FPS::HALF_FPS:
			App->player->fps_limit = FPS::STANDAR_FPS;
			App->player->rect_num_fps = { 0, 30, 100, 30 };
			target_fps = 60;
			LOG("FPS->STANDAR_FPS")
				break;
		case FPS::STANDAR_FPS:
			App->player->fps_limit = FPS::DOUBLE_FPS;
			App->player->rect_num_fps = { 0, 60, 100, 30 };
			target_fps = 120;
			LOG("FPS->VARIABLE_DT")
				break;
		case FPS::DOUBLE_FPS:
			App->player->fps_limit = FPS::HALF_FPS;
			App->player->rect_num_fps = { 0, 0, 100, 30 };
			target_fps = 30;
			LOG("FPS->HALF_FPS")
				break;
		}
	}


	if (App->scene_intro->start == false ) {

		// Colors
		int color_r, color_g, color_b;

		// Draw ground
		color_r = 0; color_g = 255; color_b = 0;
		App->renderer->DrawQuad(ground.pixels(), color_r, color_g, color_b);

		App->renderer->DrawQuad(plataforma_1.pixels(), 255, color_g, color_b);
		App->renderer->DrawQuad(plataforma_1_pared_d.pixels(), 255, 255, 255);
		App->renderer->DrawQuad(plataforma_1_pared_i.pixels(), 255, 255, 255);

		App->renderer->DrawQuad(plataforma_2.pixels(), 255, 0, 255);
		App->renderer->DrawQuad(plataforma_2_pared_d.pixels(), 255, 255, 255);
		App->renderer->DrawQuad(plataforma_2_pared_i.pixels(), 255, 255, 255);

		App->renderer->DrawQuad(plataforma_3.pixels(), color_r, color_g, 255);
		App->renderer->DrawQuad(plataforma_3_pared_d.pixels(), 255, 255, 255);
		App->renderer->DrawQuad(plataforma_3_pared_i.pixels(), 255, 255, 255);

		App->renderer->DrawQuad(pared_d.pixels(), 255, 255, 255);
		App->renderer->DrawQuad(pared_i.pixels(), 255, 255, 255);
		// Draw water
		color_r = 0; color_g = 0; color_b = 255;
		App->renderer->DrawQuad(water.pixels(), color_r, color_g, color_b);

		// Draw all balls in the scenario
		for (auto& ball : balls)
		{
			// Convert from physical magnitudes to geometrical pixels
			int pos_x = METERS_TO_PIXELS(ball.x);
			int pos_y = SCREEN_HEIGHT - METERS_TO_PIXELS(ball.y);
			int size_r = METERS_TO_PIXELS(ball.radius);

			// Select color
			if (ball.physics_enabled)
			{
				color_r = 255; color_g = 255; color_b = 255;
			}
			else
			{
				color_r = 255; color_g = 0; color_b = 0;
			}

			// Draw ball
			App->renderer->DrawCircle(pos_x, pos_y, size_r, color_r, color_g, color_b);

			if (check_collision_circle_rectangle(pos_x, pos_y, ball.radius, App->hole->juan[App->hole->random].x, App->hole->juan[App->hole->random].y, App->hole->juan[App->hole->random].w, App->hole->juan[App->hole->random].h) == true)
			{
				ball.x = 2.0f;
				ball.y = (ground.y + ground.h) + 2.0f;
				ball.vx = 0;
				ball.vy = 0;
				ball.fx = 0;
				ball.fy = 0;
				ball.physics_enabled = false;

			}
		
			double radianes = App->physics->balls.front().angle * M_PI / 180;

			// Calcula las coordenadas del vector unitario
			double x = std::sin(radianes);
			double y = std::cos(radianes);

			if (!App->physics->balls.front().physics_enabled)
			{
				//LINEA QUE INDICA LA DIRECCION HACIA LA QUE SE TIRARA LA PELOTA
				App->renderer->DrawLine(pos_x, pos_y, pos_x + METERS_TO_PIXELS(x) * (App->physics->balls.front().potencia / 8), pos_y - METERS_TO_PIXELS(y) * (App->physics->balls.front().potencia / 8), 0, 255, 0);
			}

		}
	}
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	return true;
}

// Compute modulus of a vector
float modulus(float vx, float vy)
{
	return std::sqrt(vx * vx + vy * vy);
}

// Compute Aerodynamic Drag force
void compute_aerodynamic_drag(float &fx, float& fy, const PhysBall &ball, const Atmosphere &atmosphere)
{
	float rel_vel[2] = { ball.vx - atmosphere.windx, ball.vy - atmosphere.windy }; // Relative velocity
	float speed = modulus(rel_vel[0], rel_vel[1]); // Modulus of the relative velocity
	float rel_vel_unitary[2] = { rel_vel[0] / speed, rel_vel[1] / speed }; // Unitary vector of relative velocity
	float fdrag_modulus = 0.5f * atmosphere.density * speed * speed * ball.surface * ball.cd; // Drag force (modulus)
	fx = -rel_vel_unitary[0] * fdrag_modulus; // Drag is antiparallel to relative velocity
	fy = -rel_vel_unitary[1] * fdrag_modulus; // Drag is antiparallel to relative velocity
}

// Compute Hydrodynamic Drag force
void compute_hydrodynamic_drag(float& fx, float& fy, const PhysBall& ball, const Water& water)
{
	float rel_vel[2] = { ball.vx - water.vx, ball.vy - water.vy }; // Relative velocity
	float speed = modulus(rel_vel[0], rel_vel[1]); // Modulus of the relative velocity
	float rel_vel_unitary[2] = { rel_vel[0] / speed, rel_vel[1] / speed }; // Unitary vector of relative velocity
	float fdrag_modulus = ball.b * speed; // Drag force (modulus)
	fx = -rel_vel_unitary[0] * fdrag_modulus; // Drag is antiparallel to relative velocity
	fy = -rel_vel_unitary[1] * fdrag_modulus; // Drag is antiparallel to relative velocity
}

// Compute Hydrodynamic Buoyancy force
void compute_hydrodynamic_buoyancy(float& fx, float& fy, const PhysBall& ball, const Water& water)
{
	// Compute submerged area (assume ball is a rectangle, for simplicity)
	float water_top_level = water.y + water.h; // Water top level y
	float h = 2.0f * ball.radius; // Ball "hitbox" height
	float surf = h * (water_top_level - ball.y); // Submerged surface
	if ((ball.y + ball.radius) < water_top_level) surf = h * h; // If ball completely submerged, use just all ball area
	surf *= 0.4; // FUYM to adjust values (should compute the area of circle segment correctly instead; I'm too lazy for that)

	// Compute Buoyancy force
	double fbuoyancy_modulus = water.density * 10.0 * surf; // Buoyancy force (modulus)
	fx = 0.0; // Buoyancy is parallel to pressure gradient
	fy = fbuoyancy_modulus; // Buoyancy is parallel to pressure gradient
}


// Apply Impulsive force to a ball - (0º movimiento hacia arriba, 90º movimiento a la derecha)
void PhysBall::ApplyImpulse(float angulo, float potencia) {
	
	//Calculamos un vector unitario y lo multiplicamos por la potencia
	//Convertir angulo radianes
	double radianes = angulo * M_PI / 180;

	// Calcula las coordenadas del vector unitario
	double x = std::sin(radianes);
	double y = std::cos(radianes);
	
	// Actualiza la velocidad del cuerpo en función de la fuerza impulsiva y la masa
	this->vx += x * potencia / this->mass;
	this->vy += y * potencia / this->mass;

}

// Integration scheme: Velocity Verlet
void integrator_velocity_verlet(PhysBall& ball, float dt)
{
	ball.x += ball.vx * dt + 0.5f * ball.ax * dt * dt;
	ball.y += ball.vy * dt + 0.5f * ball.ay * dt * dt;
	ball.vx += ball.ax * dt;
	ball.vy += ball.ay * dt;
}

// Integration scheme: Velocity Euler forward
void integrator_velocity_euler_forward(PhysBall& ball, float dt)
{
	ball.vx = ball.vx + ball.ax * dt;
	ball.vy = ball.vy + ball.ay * dt;
	ball.x = ball.x + ball.vx * dt;
	ball.y = ball.y + ball.vy * dt;
	
}

// Integration scheme: Velocity Euler backward
void integrator_velocity_euler_backward(PhysBall& ball, float dt)
{
	ball.x = ball.x + ball.vx * dt;
	ball.y = ball.y + ball.vy * dt;
	ball.vx = ball.vx + ball.ax * dt;
	ball.vy = ball.vy + ball.ay * dt;
}

// Detect collision with ground
bool is_colliding_with_ground(const PhysBall& ball, const Ground& ground)
{
	float rect_x = (ground.x + ground.w / 2.0f); // Center of rectangle
	float rect_y = (ground.y + ground.h / 2.0f); // Center of rectangle
	return check_collision_circle_rectangle(ball.x, ball.y, ball.radius, rect_x, rect_y, ground.w, ground.h);
}

// Detect collision with water
bool is_colliding_with_water(const PhysBall& ball, const Water& water)
{
	float rect_x = (water.x + water.w / 2.0f); // Center of rectangle
	float rect_y = (water.y + water.h / 2.0f); // Center of rectangle
	return check_collision_circle_rectangle(ball.x, ball.y, ball.radius, rect_x, rect_y, water.w, water.h);
}

// Detect collision between circle and rectange
bool check_collision_circle_rectangle(float cx, float cy, float cr, float rx, float ry, float rw, float rh)
{
	// Algorithm taken from https://stackoverflow.com/a/402010

	// Distance from center of circle to center of rectangle
	float dist_x = std::abs(cx - rx);
	float dist_y = std::abs(cy - ry);

	// If circle is further than half-rectangle, not intersecting
	if (dist_x > (rw / 2.0f + cr)) { return false; }
	if (dist_y > (rh / 2.0f + cr)) { return false; }

	// If circle is closer than half-rectangle, is intersecting
	if (dist_x <= (rw / 2.0f)) { return true; }
	if (dist_y <= (rh / 2.0f)) { return true; }

	// If all of above fails, check corners
	float a = dist_x - rw / 2.0f;
	float b = dist_y - rh / 2.0f;
	float cornerDistance_sq = a * a + b * b;
	return (cornerDistance_sq <= (cr * cr));
}

// Convert from meters to pixels (for SDL drawing)
SDL_Rect Ground::pixels()
{
	SDL_Rect pos_px{};
	pos_px.x = METERS_TO_PIXELS(x);
	pos_px.y = SCREEN_HEIGHT - METERS_TO_PIXELS(y);
	pos_px.w = METERS_TO_PIXELS(w);
	pos_px.h = METERS_TO_PIXELS(-h); // Can I do this? LOL
	return pos_px;
}



