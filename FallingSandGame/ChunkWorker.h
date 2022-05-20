#pragma once
#include "World.h"

class ChunkWorker
{
public:
	ChunkWorker(World& _world, Chunk* _chunk) : world(_world), chunk(_chunk) {}
	~ChunkWorker() = default;

	void UpdateChunk();
	virtual void UpdateCell(int x, int y, Cell& cell) = 0;

	//Helpers
	bool InBounds(int x, int y);
	bool IsEmpty(int x, int y);

	bool HasProperties(int x, int y, Cell::Properties props);

	void KeepAlive(int x, int y);

	//Cell Movement
	Cell& GetCell(int x, int y);
	void SetCell(int x, int y, const Cell& cell);
	void MoveCell(int x, int y, int xto, int yto);

protected:
	World& world;
	Chunk* chunk = nullptr;
};
