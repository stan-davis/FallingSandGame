#include "ChunkWorker.h"

void ChunkWorker::UpdateChunk()
{
	for (int x = chunk->RectMin().x; x < chunk->RectMax().x; x++)
		for (int y = chunk->RectMin().y; y < chunk->RectMax().y; y++)
		{
			Cell& cell = chunk->GetCell(y * chunk->GetWidth() + x);

			if (cell.properties & Cell::Properties::NONE) 
				continue;

			int px = x + chunk->GetPosition().x;
			int py = y + chunk->GetPosition().y;

			UpdateCell(px, py, cell);
		}
}

bool ChunkWorker::InBounds(int x, int y)
{
	return chunk->InBounds(x, y) || world.InBounds(x, y);
}

bool ChunkWorker::IsEmpty(int x, int y)
{
	if (chunk->InBounds(x, y))
		return chunk->IsEmpty(x, y);

	return world.IsEmpty(x, y);
}

bool ChunkWorker::HasProperties(int x, int y, Cell::Properties props)
{
	return InBounds(x, y) && GetCell(x, y).properties & props;
}

void ChunkWorker::KeepAlive(int x, int y)
{
	if (chunk->InBounds(x, y))
		return chunk->KeepAlive(x, y);

	return world.KeepAlive(x, y);
}

Cell& ChunkWorker::GetCell(int x, int y)
{
	if (chunk->InBounds(x, y))
		return chunk->GetCell(x, y);

	return world.GetCell(x, y);
}

void ChunkWorker::SetCell(int x, int y, const Cell& cell)
{
	if (chunk->InBounds(x, y))
		return chunk->SetCell(x, y, cell);

	return world.SetCell(x, y, cell);
}

void ChunkWorker::MoveCell(int x, int y, int xto, int yto)
{
	int px = 0;
	int py = 0;

	if (x == chunk->GetPosition().x)							px = -1;
	if (x == chunk->GetPosition().x + chunk->GetWidth() - 1)	px = 1;
	if (y == chunk->GetPosition().y)							py = -1;
	if (y == chunk->GetPosition().y + chunk->GetHeight() - 1)	py = 1;

	if (px != 0) world.KeepAlive(x + px, y);
	if (py != 0) world.KeepAlive(x, y + py);
	if (px != 0 && py != 0) world.KeepAlive(x + px, y + py);

	if (chunk->InBounds(x, y) && chunk->InBounds(xto, yto))
		chunk->MoveCell(chunk, x, y, xto, yto);

	return world.MoveCell(x, y, xto, yto);
}
