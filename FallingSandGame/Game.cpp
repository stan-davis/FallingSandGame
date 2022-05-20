#include "Game.h"
#include "PrimaryChunkWorker.h"

Game::Game()
{
	sAppName = APP_NAME;
}

Game::~Game()
{
	delete world;
}

bool Game::OnUserCreate()
{
	srand(clock());

	world = new World(1,1);

	//DEFINE ALL TYPES
	ROCK =
	{
		Cell::Type::ROCK,
		Cell::Properties::NONE | Cell::Properties::DISSOLVEABLE,
		olc::GREY
	};

	WOOD =
	{
		Cell::Type::WOOD,
		Cell::Properties::NONE | Cell::Properties::FLAMMABLE,
		olc::DARK_RED
	};

	SAND =
	{
		Cell::Type::SAND,
		Cell::Properties::MOVE,
		olc::YELLOW
	};

	WATER =
	{
		Cell::Type::WATER,
		Cell::Properties::LIQUID,
		olc::BLUE,
		2,
		2
	};

	ACID =
	{
		Cell::Type::ACID,
		Cell::Properties::LIQUID,
		olc::GREEN
	};

	FIRE =
	{
		Cell::Type::FIRE,
		Cell::Properties::MOVE,
		olc::RED,
	};

	type_names.insert({ Cell::Type::EMPTY, "EMPTY" });
	type_names.insert({ Cell::Type::ROCK, "ROCK" });
	type_names.insert({ Cell::Type::SAND, "SAND" });
	type_names.insert({ Cell::Type::WATER, "WATER" });
	type_names.insert({ Cell::Type::ACID, "ACID" });
	type_names.insert({ Cell::Type::FIRE, "FIRE" });
	type_names.insert({ Cell::Type::WOOD, "WOOD" });

	return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{
	Clear(olc::BLACK);

	GetUserInput();

	world->RemoveEmptyChunks();

	for (Chunk* chunk : world->GetChunks())
		PrimaryChunkWorker(*world, chunk).UpdateChunk();

	for (Chunk* chunk : world->GetChunks())
		chunk->Commit();

	for (Chunk* chunk : world->GetChunks())
		chunk->UpdateRect();

	for (Chunk* chunk : world->GetChunks())
		chunk->Draw(this);

	DebugInterface();
	DrawString({ 0,10 }, "Selected: " + type_names[selected_type.type]);
	DrawString({ 0,20 }, "Brush Size: " + std::to_string(brush_size));
	DrawRect(
		{ GetMouseX() - brush_size, GetMouseY() - brush_size },
		{ brush_size * 2, brush_size * 2 },
		olc::MAGENTA);

	mouse_prev = { (float)GetMouseX(), (float)GetMouseY() };

	return true;
}

void Game::GetUserInput()
{
	if (GetKey(olc::F2).bPressed)
		debug_enabled = !debug_enabled;

	if (GetKey(olc::K1).bPressed)
		selected_type = Cell();
	if (GetKey(olc::K2).bPressed)
		selected_type = ROCK;
	if (GetKey(olc::K3).bPressed)
		selected_type = SAND;
	if (GetKey(olc::K4).bPressed)
		selected_type = WATER;
	if (GetKey(olc::K5).bPressed)
		selected_type = ACID;
	if (GetKey(olc::K6).bPressed)
		selected_type = FIRE;
	if (GetKey(olc::K7).bPressed)
		selected_type = WOOD;

	if (GetMouse(0).bHeld)
	{
		olc::vf2d mouse = { (float)GetMouseX(), (float)GetMouseY() };
		TraverseMatrix(mouse, mouse_prev);
	}

	if (GetMouseWheel() < 0)
	{
		if (brush_size > 1)
			brush_size--;
	}

	if (GetMouseWheel() > 0)
	{
		if (brush_size < MAX_BRUSH_SIZE)
			brush_size++;
	}
}

bool Game::TraverseMatrix(olc::vf2d a, olc::vf2d b)
{
	if (a == b)
		DrawBrush(a);

	int xdiff = a.x - b.x;
	int ydiff = a.y - b.y;

	bool isXDiffLarger = abs(xdiff) > abs(ydiff);

	int xMod = xdiff < 0 ? 1 : -1;
	int yMod = ydiff < 0 ? 1 : -1;

	int upperBound = std::max(abs(xdiff), abs(ydiff));
	int min = std::min(abs(xdiff), abs(ydiff));
	float slope = (min == 0 || upperBound == 0) ? 0 : (float(min + 1) / (upperBound + 1));

	int smallerCount;

	for (int i = 1; i <= upperBound; i++)
	{
		smallerCount = (int)floor(i * slope);

		int yIncrease, xIncrease;

		if (isXDiffLarger) 
		{
			xIncrease = i;
			yIncrease = smallerCount;
		}
		else 
		{
			yIncrease = i;
			xIncrease = smallerCount;
		}

		int currentY = a.y + (yIncrease * yMod);
		int currentX = a.x + (xIncrease * xMod);

		if (world->InBounds(currentX, currentY))
			DrawBrush({ currentX, currentY });
	}

	return false;
}

void Game::DrawBrush(olc::vi2d pos)
{
	for(int x = -brush_size; x <= brush_size; x++)
		for (int y = -brush_size; y <= brush_size; y++)
		{
			if(selected_type.type == Cell::Type::FIRE)
				selected_type.life_time = static_cast<int16_t>(rand() % 80 + 50);

			if (world->InBounds(pos.x + x, pos.y + y))
				world->GetChunk(pos.x + x, pos.y + y)->SetCell(pos.x + x, pos.y + y, selected_type);
		}
}

void Game::DebugInterface()
{
	if (!debug_enabled)
		return;

	world->DisplayDebugInfo(this);
	DrawString({ 0,0 }, "Debug Enabled");
}
