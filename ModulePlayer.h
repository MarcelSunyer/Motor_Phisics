#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"

enum class Controls
{
	POSITION,
	VELOCITY,
	FORCE,
	MOMENTUM,	//Es lo mismo que cantidad de movimiento p = m * v
	ACCELERATION
};

enum class FPS
{
	HALF_FPS,
	STANDAR,
	DOUBLE_FPS

};

enum class Integrador
{
	VERLET,
	FWD_EULER,
	BWD_EULER

};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	//Debug info controles (Solo visual)
	SDL_Texture* texto_controles;
	SDL_Rect rect_texto_controles = { 0, 50, 400 , 50 };

	//God Mode para volar
<<<<<<< Updated upstream
	bool god_mode;
	SDL_Texture* god_mode_texture;
=======
	bool god_mode = false;
>>>>>>> Stashed changes

	//Variable para cambiar los controles
	Controls control_system = Controls::VELOCITY;

	// Variable para controlar los FPS (delta time)
	FPS fps_limit = FPS::STANDAR;

	//Debug info FPS (Solo visual)
	SDL_Texture* num_fps;
	SDL_Rect rect_num_fps = { 0, 30, 100, 30 };

	// Variable para controlar el integrador seleccionado
	Integrador selected_integrator = Integrador::VERLET;

	//Debug info integrador (Solo visual)
	SDL_Texture* integrator_name;
	SDL_Rect rect_integrator_name = { 0, 60, 200, 30 };

};