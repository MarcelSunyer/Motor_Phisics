#include "Globals.h"
#include "Application.h"
#include "ModuleHole.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"


// Constructor
ModuleHole::ModuleHole(Application* app, bool start_enabled) : Module(app, start_enabled)
{}
// Destructor
ModuleHole::~ModuleHole()
{}

bool ModuleHole::Start()
{
	juan = App->textures->Load("Assets/Palo_golf.png");
	LOG("Loading holes");
	//Posicion_1 || Hacer una variable x y y para cada espacio de la array y que estos sean randoms.

	p2Point<int> juan;
	/*posiciones[0] = p2Point{}*/
	
	
	return true;
}

update_status ModuleHole::Update()
{
	
	App->renderer->Blit(juan,290,320);
	App->renderer->DrawCircle(posiciones[0], 17, 255, 255, 255);
	
	return UPDATE_CONTINUE;
}

bool ModuleHole::CleanUp()
{
	LOG("Unloading player");
	return true;
}
