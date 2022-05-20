#pragma once
#include "olcPixelGameEngine.h"

struct Cell
{
	enum Type
	{
		EMPTY	= 0,
		ROCK	= 1,
		SAND	= 2,
		WATER	= 3,
		ACID	= 4,
		FIRE	= 5,
		WOOD	= 6
	};

	enum Properties
	{
		NONE			= 0b00000000,
		MOVE			= 0b00000010,
		LIQUID			= 0b00000100,
		FLAMMABLE		= 0b00001000,
		DISSOLVEABLE	= 0b00010000
	};

	uint16_t type = Type::EMPTY;
	uint8_t properties = Properties::NONE;
	olc::Pixel color = olc::BLACK;

	uint8_t spread_rate = 1;
	uint8_t fall_rate = 1;
	int16_t life_time = 0;
}; //11 bytes