#pragma once
#include "olcPixelGameEngine.h"
#include "World.h"

class Game : public olc::PixelGameEngine
{
public:
	Game();
	~Game();

private:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	
	bool TraverseMatrix(olc::vf2d a, olc::vf2d b);
	void DrawBrush(olc::vi2d pos);
	void GetUserInput();
	void DebugInterface();
	
	const std::string APP_NAME = "Falling Sand Game - Stanley Davis (2022)";
	bool debug_enabled = false;

	//Drawing
	olc::vf2d mouse_prev;

	const int MAX_BRUSH_SIZE = 5;
	int brush_size = 1;

	//Simulation
	World* world;
	Cell selected_type;

	//Cell Types
	Cell ROCK;
	Cell WOOD;
	Cell SAND;
	Cell WATER;
	Cell ACID;
	Cell FIRE;

	std::map<uint16_t, std::string> type_names;
};

