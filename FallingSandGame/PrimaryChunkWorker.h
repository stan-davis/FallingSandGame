#pragma once
#include "ChunkWorker.h"

class PrimaryChunkWorker : public ChunkWorker
{
public:
	PrimaryChunkWorker(World& _world, Chunk* _chunk) : ChunkWorker(_world, _chunk) {}
	void UpdateCell(int x, int y, Cell& cell) override;

private:
	void UpdateWater(int x, int y, Cell& cell);
	void UpdateSand(int x, int y, Cell& cell);
	void UpdateAcid(int x, int y, Cell& cell);
	void UpdateFire(int x, int y, Cell& cell);
};
