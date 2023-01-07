#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"

ModuleRender::ModuleRender(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	renderer = NULL;
	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT;
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if(VSYNC == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);
	
	if(renderer == NULL)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	control_system = Controls::VELOCITY;

	texto_controles = App->textures->Load("Assets/Controls.png");

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleRender::Update()
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
		if (App->physics->balls.front().angle != -90 )
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

// PostUpdate present buffer to screen
update_status ModuleRender::PostUpdate()
{
	//Blit del titulo
	SDL_Rect rect = { 0, 0, 500, 50 };
	Blit(texto_controles, 0, 0, &rect);

	//Blit del Control method
	Blit(texto_controles, 0, 50, &rect_texto_controles);
	
	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if(renderer != NULL)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed, double angle, int pivot_x, int pivot_y )
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = (int) (camera.x * speed) + x * SCREEN_SIZE;
	rect.y = (int) (camera.y * speed) + y * SCREEN_SIZE;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * SCREEN_SIZE);
		rec.y = (int)(camera.y + rect.y * SCREEN_SIZE);
		rec.w *= SCREEN_SIZE;
		rec.h *= SCREEN_SIZE;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);
	
	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * SCREEN_SIZE, camera.y + y1 * SCREEN_SIZE, camera.x + x2 * SCREEN_SIZE, camera.y + y2 * SCREEN_SIZE);
	else
		result = SDL_RenderDrawLine(renderer, x1 * SCREEN_SIZE, y1 * SCREEN_SIZE, x2 * SCREEN_SIZE, y2 * SCREEN_SIZE);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float) M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int) (x + radius * cos( i * factor));
		points[i].y = (int) (y + radius * sin( i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}