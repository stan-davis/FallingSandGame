#include "World.h"

World::World(int _width, int _height) : width(_width), height(_height)
{
	for (int x = 0; x <= width; x++)
		for (int y = 0; y <= height; y++)
			CreateChunk({ x, y });
}

World::~World()
{
	for (Chunk* chunk : chunks)
		delete chunk;
}

void World::DisplayDebugInfo(olc::PixelGameEngine* pge)
{
	for (auto& chunk : chunks)
	{
		pge->DrawRect(chunk->GetPosition(), { CHUNK_WIDTH, CHUNK_HEIGHT }, olc::RED);
		
		if(chunk->RectMax().x > 0)
			pge->DrawRect(chunk->GetPosition() + chunk->RectMin(), chunk->RectMax() - chunk->RectMin(), olc::GREEN);
	}
}

Chunk* World::CreateChunk(std::pair<int, int> position)
{
	auto [x, y] = position;

	if (x <= -width || y <= -height || x > width || y > height)
		return nullptr; //Chunk not in world bounds, failed

	Chunk* chunk = new Chunk(CHUNK_WIDTH, CHUNK_HEIGHT, x, y);

	chunk_lookup.insert({ position, chunk });
	chunks.push_back(chunk);

	return chunk;
}

void World::RemoveEmptyChunks()
{
	for (int i = 0; i < chunks.size(); i++)
	{
		Chunk* chunk = chunks.at(i);

		if (chunk->GetFilledCellCount() == 0)
		{
			chunk_lookup.erase({ chunk->GetPosition().x / CHUNK_WIDTH, chunk->GetPosition().y / CHUNK_HEIGHT });
			chunks[i] = chunks.back();
			chunks.pop_back();
			i--;
			
			delete chunk;
		}
	}
}

Cell& World::GetCell(int x, int y)
{
	return GetChunk(x, y)->GetCell(x, y);
}

void World::SetCell(int x, int y, const Cell& cell)
{
	if (Chunk* chunk = GetChunk(x, y))
		chunk->SetCell(x, y, cell);
}

void World::MoveCell(int x, int y, int xto, int yto)
{
	if (Chunk* src = GetChunk(x, y))
		if (Chunk* dst = GetChunk(xto, yto)) 
			dst->MoveCell(src, x, y, xto, yto);
}

void World::KeepAlive(int x, int y)
{
	if (Chunk* chunk = GetChunk(x, y))
		chunk->KeepAlive(x, y);
}

bool World::InBounds(int x, int y)
{
	if (Chunk* chunk = GetChunk(x, y))
		return chunk->InBounds(x, y);

	return false;
}

bool World::IsEmpty(int x, int y)
{
	return InBounds(x, y) && GetChunk(x, y)->IsEmpty(x, y);
}

Chunk* World::GetChunk(int x, int y)
{
	std::pair<int, int> position = { floor(float(x) / CHUNK_WIDTH), floor(float(y) / CHUNK_HEIGHT) };

	auto itr = chunk_lookup.find(position);
	auto end = chunk_lookup.end();
	
	Chunk* chunk = itr != end ? itr->second : nullptr;

	return chunk ? chunk : CreateChunk(position);
}
