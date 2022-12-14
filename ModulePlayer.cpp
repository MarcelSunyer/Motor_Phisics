#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "PhysBody.h"

#include "math.h"
#include <cmath>

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	control_system = Controls::VELOCITY;

	texto_controles = App->textures->Load("Assets/Controls.png");

	num_fps = App->textures->Load("Assets/dt.png");

	integrator_name = App->textures->Load("Assets/Integrators.png");

	god_mode_texture = App->textures->Load("Assets/GodMode.png");

	collisioner_name = App->textures->Load("Assets/Collisions.png");

	return true;
}



// Update: draw background
update_status ModulePlayer::Update()
{
	//Cambiar integrador
	//Cambiar los FPS
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		switch (selected_integrator)
		{
		case Integrador::VERLET:
			selected_integrator = Integrador::FWD_EULER;
			rect_integrator_name = { 0, 30, 200, 30 };
			LOG("FWD_EULER");
			break;
		case Integrador::FWD_EULER:
			selected_integrator = Integrador::BWD_EULER;
			rect_integrator_name = { 0, 0, 200, 30 };
			LOG("BWD_EULER");
			break;
		case Integrador::BWD_EULER:
			selected_integrator = Integrador::VERLET;
			rect_integrator_name = { 0, 60, 200, 30 };
			LOG("VERLET");
			break;
		}
	}

	//Cambiar el collisioner
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		switch (selected_collisioner)
		{
		case Collisioner::M1:
			selected_collisioner = Collisioner::M2;
			rect_collisioner_name = { 0, 60, 150, 30 };
			LOG("M2");
			break;
		case Collisioner::M2:
			selected_collisioner = Collisioner::M3;
			rect_collisioner_name = { 0, 90, 150, 30 };
			LOG("M3");
			break;
		case Collisioner::M3:
			selected_collisioner = Collisioner::M1;			
			rect_collisioner_name = { 0, 30, 150, 30 };
			LOG("M1");
			break;
		}
	}

	
	if (App->physics->balls.front().on_floor == true)
	{
		App->physics->balls.front().physics_enabled = false;
	}

	// Cambiar la posici?n del saque
	if (App->physics->balls.front().physics_enabled == false)
	{
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			App->physics->balls.front().x = App->physics->balls.front().x - 0.1;

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			App->physics->balls.front().x = App->physics->balls.front().x + 0.1;
	}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		App->physics->balls.front().physics_enabled = !App->physics->balls.front().physics_enabled;

	//On/Off GodMode
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		App->physics->balls.front().physics_enabled = !App->physics->balls.front().physics_enabled;
		god_mode = !god_mode;
	}



	//Reset de la posici?n de la bola
	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		App->physics->balls.front().x = 2.0f;
		App->physics->balls.front().y = (App->physics->ground.y + App->physics->ground.h) + App->physics->balls.front().radius;
		App->physics->balls.front().vx = 0;
		App->physics->balls.front().vy = 0;
		App->physics->balls.front().fx = 0;
		App->physics->balls.front().fy = 0;
		App->physics->balls.front().physics_enabled = false;

	}

	//Para cabiar de esquema de controles
	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		switch (control_system)
		{
		case Controls::POSITION:
			control_system = Controls::VELOCITY;
			rect_texto_controles = { 0, 50, 400 , 50 };
			LOG("VELOCITY")
				break;
		case Controls::VELOCITY:
			control_system = Controls::FORCE;
			rect_texto_controles = { 0, 100, 400 , 50 };
			LOG("FORCE")
				break;
		case Controls::FORCE:
			control_system = Controls::MOMENTUM;
			rect_texto_controles = { 0, 150, 400 , 50 };
			LOG("MOMENTUM");
			break;
		case Controls::MOMENTUM:
			control_system = Controls::ACCELERATION;
			rect_texto_controles = { 0, 250, 400 , 50 };
			LOG("ACCELERATION")
				break;
		case Controls::ACCELERATION:
			control_system = Controls::POSITION;
			rect_texto_controles = { 0, 200, 400 , 50 };
			LOG("POSITION")
				break;
		}
	}

	if (god_mode)
	{
		switch (control_system)
		{
		case Controls::POSITION:

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				App->physics->balls.front().y = App->physics->balls.front().y + 0.5;

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				App->physics->balls.front().y = App->physics->balls.front().y - 0.5;

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				App->physics->balls.front().x = App->physics->balls.front().x - 0.5;

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				App->physics->balls.front().x = App->physics->balls.front().x + 0.5;

			break;
		case Controls::VELOCITY:

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				App->physics->balls.front().vy = 10;

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				App->physics->balls.front().vy = -10;

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				App->physics->balls.front().vx = -10;

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				App->physics->balls.front().vx = 10;

			break;
		case Controls::FORCE:

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				App->physics->balls.front().fy = 200;

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				App->physics->balls.front().fy = -200;

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				App->physics->balls.front().fx = -200;

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				App->physics->balls.front().fx = 200;

			break;
		case Controls::MOMENTUM:

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				App->physics->balls.front().vy = 50 / App->physics->balls.front().mass;

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				App->physics->balls.front().vy = -50 / App->physics->balls.front().mass;

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				App->physics->balls.front().vx = -50 / App->physics->balls.front().mass;

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				App->physics->balls.front().vx = 50 / App->physics->balls.front().mass;

			break;
		case Controls::ACCELERATION:

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				App->physics->balls.front().fy = 400 / App->physics->balls.front().mass;

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				App->physics->balls.front().fy = -400 / App->physics->balls.front().mass;

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				App->physics->balls.front().fx = -400 / App->physics->balls.front().mass;

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				App->physics->balls.front().fx = 400 / App->physics->balls.front().mass;

			break;
		}
	}



	//APUNTAR

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		//Cambiar el angulo (-)
		if (App->physics->balls.front().angle != -90)
		{
			App->physics->balls.front().angle = App->physics->balls.front().angle - 15;
			LOG("Angulo: %d", this->App->physics->balls.front().angle)
		}
		else
		{
			LOG("Angulo: %d", this->App->physics->balls.front().angle)
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		//Cambiar el angulo (+)
		if (App->physics->balls.front().angle != 90)
		{
			App->physics->balls.front().angle = App->physics->balls.front().angle + 15;
			LOG("Angulo: %d", this->App->physics->balls.front().angle)
		}
		else
		{
			LOG("Angulo: %d", this->App->physics->balls.front().angle)
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		//Cambiar potencia (+)
		if (App->physics->balls.front().potencia != 750)
		{
			App->physics->balls.front().potencia = App->physics->balls.front().potencia + 10;
			LOG("Potencia: %d", this->App->physics->balls.front().potencia)
		}
		else
		{
			LOG("Potencia: %d", this->App->physics->balls.front().potencia)
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		//Cambiar potencia (-)
		if (App->physics->balls.front().potencia != 0)
		{
			App->physics->balls.front().potencia = App->physics->balls.front().potencia - 10;
			LOG("Potencia: %d", this->App->physics->balls.front().potencia)
		}
		else
		{
			LOG("Potencia: %d", this->App->physics->balls.front().potencia)
		}
	}

	//Disparar
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !App->physics->balls.front().physics_enabled)
	{
		App->physics->balls.front().ApplyImpulse(App->physics->balls.front().angle, App->physics->balls.front().potencia);
		App->physics->balls.front().physics_enabled = true;
	}

	if (!App->physics->balls.front().physics_enabled)
	{
		//LINEA QUE INDICA LA DIRECCION HACIA LA QUE SE TIRARA LA PELOTA
		/*int posx = App->physics->balls.front().x * METERS_PER_PIXELS;
		App->renderer->DrawLine(posx, App->physics->balls.front().y, App->physics->balls.front().x + 16, App->physics->balls.front().y + 16, 0, 255, 0);*/
	}

	return UPDATE_CONTINUE;
}



// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(texto_controles);
	App->textures->Unload(num_fps);
	App->textures->Unload(integrator_name);
	App->textures->Unload(god_mode_texture);
	App->textures->Unload(collisioner_name);

	return true;
}

