#pragma once
#include "Cell.h"

class Chunk
{
public:
	Chunk(int _width, int _height, int x, int y);
	~Chunk();

	void Draw(olc::PixelGameEngine* pge);

	//Helpers
	int GetIndex(int x, int y) { return (y - cy) * width + (x - cx); }
	bool InBounds(int x, int y);

	bool IsEmpty(int x, int y);
	bool IsEmpty(int index);

	int GetWidth() { return width; }
	int GetHeight() { return height; }

	//Chunk Get
	olc::vi2d GetPosition() { return { cx, cy }; }
	unsigned int GetFilledCellCount() { return filled_cell_count; }

	//Cell Setget
	Cell& GetCell(int index) { return cells[index]; }
	Cell& GetCell(int x, int y) { return GetCell(GetIndex(x, y)); }

	void SetCell(int index, const Cell& cell);
	void SetCell(int x, int y, const Cell& cell) { SetCell(GetIndex(x, y), cell); }

	//Movement
	void MoveCell(Chunk* src, int x, int y, int xto, int yto);
	void Commit();

	//Dirty Rect
	void KeepAlive(int index);
	void KeepAlive(int x, int y) { KeepAlive(GetIndex(x, y)); }

	void UpdateRect();
	olc::vi2d RectMin() { return min; }
	olc::vi2d RectMax() { return max; }

private:
	//Init
	int width;
	int height;
	int cx;
	int cy;

	//Dirty Rect
	olc::vi2d min;
	olc::vi2d max;

	//Working rect
	olc::vi2d min_w;
	olc::vi2d max_w;

	//Cells
	Cell* cells = nullptr;
	std::vector<std::tuple<Chunk*, int, int>> changes;
	int filled_cell_count = 0;
};

