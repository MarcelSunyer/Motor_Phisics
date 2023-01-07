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
	bool god_mode;

	//Variable para cambiar los controles
	Controls control_system = Controls::VELOCITY;
};