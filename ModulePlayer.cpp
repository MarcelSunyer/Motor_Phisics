#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "PhysBody.h"

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

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (App->physics->balls.front().on_floor == true)
	{
		App->physics->balls.front().physics_enabled = false;
	}


	// Cambiar la posición del saque
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



	//Reset de la posición de la bola
	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		App->physics->balls.front().x = 2.0f;
		App->physics->balls.front().y = (App->physics->ground.y + App->physics->ground.h) + 2.0f;
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

	return UPDATE_CONTINUE;
}



