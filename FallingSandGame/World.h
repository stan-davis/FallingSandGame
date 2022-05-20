#pragma once
#include "Chunk.h"
#include "Utility.h"

class World
{
public:
	World(int _width, int _height);
	~World();

	void DisplayDebugInfo(olc::PixelGameEngine* pge);

	//Helpers
	bool InBounds(int x, int y);
	bool IsEmpty(int x, int y);

	//Chunk Getters
	std::vector<Chunk*> GetChunks() { return chunks; }
	Chunk* GetChunk(int x, int y);

	//Cell Movement
	Cell& GetCell(int x, int y);
	void SetCell(int x, int y, const Cell& cell);
	void MoveCell(int x, int y, int xto, int yto);

	void KeepAlive(int x, int y);
	void RemoveEmptyChunks();
	
private:
	Chunk* CreateChunk(std::pair<int, int> position);

	//Variables
	int width;
	int height;

	const int CHUNK_WIDTH = 100;
	const int CHUNK_HEIGHT = 100;

	std::vector<Chunk*> chunks;
	std::unordered_map<std::pair<int, int>, Chunk*, pair_hash> chunk_lookup;
};
