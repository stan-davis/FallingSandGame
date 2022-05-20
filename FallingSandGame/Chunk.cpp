#include "Chunk.h"
#include "Utility.h"

Chunk::Chunk(int _width, int _height, int x, int y) : width(_width), height(_height), cx(x * width), cy(y * height)
{
	cells = new Cell[width * height];
}

Chunk::~Chunk()
{
	delete[] cells;
}

void Chunk::Draw(olc::PixelGameEngine* pge)
{
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
		{
			if (cells[y * width + x].type == Cell::Type::EMPTY)
				continue;

			pge->Draw(cx + x, cy + y, cells[y * width + x].color);
		}
}

bool Chunk::InBounds(int x, int y)
{
	return x >= cx && x < cx + width
		&& y >= cy && y < cy + height;
}

bool Chunk::IsEmpty(int x, int y)
{
	return IsEmpty(GetIndex(x, y));
}

bool Chunk::IsEmpty(int index)
{
	return GetCell(index).type == Cell::Type::EMPTY;
}

void Chunk::SetCell(int index, const Cell& cell)
{
	Cell& dst = cells[index];

	KeepAlive(index);

	if (dst.type == Cell::Type::EMPTY && cell.type != Cell::Type::EMPTY)
		filled_cell_count++;
	else if (dst.type != Cell::Type::EMPTY && cell.type == Cell::Type::EMPTY)
		filled_cell_count--;

	dst = cell;
}

void Chunk::MoveCell(Chunk* src, int x, int y, int xto, int yto)
{
	changes.emplace_back(src, src->GetIndex(x, y), GetIndex(xto, yto));
}

void Chunk::Commit()
{
	changes.emplace_back(nullptr, -1, -1);

	for (int i = 0; i < changes.size() - 1; i++)
	{
		if (std::get<2>(changes[i + 1]) != std::get<2>(changes[i]))
		{
			auto [chunk, src, dst] = changes[i];

			Cell origin_dst = GetCell(dst); //Save the destination cell

			if (GetCell(dst).properties & Cell::Properties::DISSOLVEABLE)
			{
				SetCell(dst, chunk->GetCell(src));
				chunk->SetCell(src, Cell());
			}
			else if (GetCell(dst).properties & Cell::Properties::FLAMMABLE)
			{
				chunk->GetCell(src).life_time++;

				SetCell(dst, chunk->GetCell(src));
				chunk->SetCell(src, chunk->GetCell(src));
			}
			else
			{
				SetCell(dst, chunk->GetCell(src));
				chunk->SetCell(src, origin_dst);
			}
		}
	}

	changes.clear();
}

void Chunk::KeepAlive(int index)
{
	int x = index % width;
	int y = index / width;

	min_w.x = std::clamp(std::min(x - 2, min_w.x), 0, width);
	min_w.y = std::clamp(std::min(y - 2, min_w.y), 0, height);
	max_w.x = std::clamp(std::max(x + 2, max_w.x), 0, width);
	max_w.y = std::clamp(std::max(y + 2, max_w.y), 0, height);
}

void Chunk::UpdateRect()
{
	min = min_w;
	max = max_w;

	min_w.x = width;
	min_w.y = height;

	max_w.x = -1;
	max_w.y = -1;
}
