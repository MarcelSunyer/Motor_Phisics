#include "Globals.h"
#include "Application.h"
#include "ModuleHole.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include <time.h>


// Constructor
ModuleHole::ModuleHole(Application* app, bool start_enabled) : Module(app, start_enabled)
{}
// Destructor
ModuleHole::~ModuleHole()
{}

bool ModuleHole::Start()
{
	juans = App->textures->Load("Assets/Palo_golf.png");
	LOG("Loading holes");
	//Posicion_1 || Hacer una variable x y y para cada espacio de la array y que estos sean randoms.

	
	juan[0].x = 100, juan[0].y = 100;

	juan[1].x = 200, juan[1].y = 200;
	juan[2].x = 300, juan[2].y = 300;
	
	
	return true;
}

update_status ModuleHole::Update()
{
	if (tocado == true)
	{
		
	}
	Randomizer(random);
	App->renderer->Blit(juans, juan[random].x, juan[random].y);
	App->renderer->DrawCircle(juan[random].x, juan[random].y, 10, 255, 255, 255);
	
	return UPDATE_CONTINUE;
}

bool ModuleHole::CleanUp()
{
	LOG("Unloading player");
	return true;
}

void ModuleHole::Randomizer(int x)
{
	time_t times = 3000;
	srand(time(&times));
	random = rand() % 3 + 0;
}

