#include "PrimaryChunkWorker.h"

void PrimaryChunkWorker::UpdateCell(int x, int y, Cell& cell)
{
	switch (cell.type)
	{
		case Cell::Type::WATER:
			UpdateWater(x, y, cell);
			break;
		case Cell::Type::SAND:
			UpdateSand(x, y, cell);
			break;
		case Cell::Type::ACID:
			UpdateAcid(x, y, cell);
			break;
		case Cell::Type::FIRE:
			UpdateFire(x, y, cell);
			break;
	}
}

void PrimaryChunkWorker::UpdateWater(int x, int y, Cell& cell)
{
	int dir = 0;

	while (dir == 0)
		dir = (rand() % 3) - 1;

	for (int i = cell.fall_rate; i > 0; --i)
	{
		for (int j = cell.spread_rate; j > 0; --j)
		{
			if (IsEmpty(x, y + i))
			{
				MoveCell(x, y, x, y + i);
				return;
			}
			else if (IsEmpty(x + (j * dir), y + i))
			{
				MoveCell(x, y, x + (j * dir), y + i);
				return;
			}
			else if (IsEmpty(x + (j * -dir), y + i))
			{
				MoveCell(x, y, x + (j * -dir), y + i);
				return;
			}
			else if (IsEmpty(x + (j * dir), y))
			{
				MoveCell(x, y, x + (j * dir), y);
				return;
			}
		}
	}
}

void PrimaryChunkWorker::UpdateSand(int x, int y, Cell& cell)
{
	int dir = 0;

	while (dir == 0)
		dir = (rand() % 3) - 1;

	for (int i = cell.fall_rate; i > 0; --i)
	{
		for (int j = cell.spread_rate; j > 0; --j)
		{
			if (IsEmpty(x, y + i) || HasProperties(x, y + i, Cell::Properties::LIQUID))
			{
				MoveCell(x, y, x, y + i);
				return;
			}
			else if (IsEmpty(x + (j * dir), y + i) || HasProperties(x + (j * dir), y + i, Cell::Properties::LIQUID))
			{
				MoveCell(x, y, x + (j * dir), y + i);
				return;
			}
			else if (IsEmpty(x + (j * -dir), y + i) || HasProperties(x + (j * dir), y + i, Cell::Properties::LIQUID))
			{
				MoveCell(x, y, x + (j * -dir), y + i);
				return;
			}
		}
	}
}

void PrimaryChunkWorker::UpdateAcid(int x, int y, Cell& cell)
{
	int dir = 0;

	while (dir == 0)
		dir = (rand() % 3) - 1;

	bool chance = (rand() % 3) - 1 > 0;
	
	for (int i = cell.fall_rate; i > 0; --i)
	{
		for (int j = cell.spread_rate; j > 0; --j)
		{
			if (IsEmpty(x, y + i))
			{
				MoveCell(x, y, x, y + i);
				return;
			}
			else if (HasProperties(x, y + i, Cell::Properties::DISSOLVEABLE))
			{
				if (chance)
					MoveCell(x, y, x, y + i);
				return;
			}
			else if (IsEmpty(x + (j * dir), y + i))
			{
				MoveCell(x, y, x + (j * dir), y + i);
				return;
			}
			else if (HasProperties(x + (j * dir), y + i, Cell::Properties::DISSOLVEABLE))
			{
				if (chance)
					MoveCell(x, y, x + (j * dir), y + i);
				return;
			}
			else if (IsEmpty(x + (j * -dir), y + i))
			{
				MoveCell(x, y, x + (j * -dir), y + i);
				return;
			}
			else if (HasProperties(x + (j * -dir), y + i, Cell::Properties::DISSOLVEABLE))
			{
				if (chance)
					MoveCell(x, y, x + (j * -dir), y + i);
				return;
			}
			else if (IsEmpty(x + (j * dir), y))
			{
				MoveCell(x, y, x + (j * dir), y);
				return;
			}
			else if (HasProperties(x + (j * dir), y, Cell::Properties::DISSOLVEABLE))
			{
				if (chance)
					MoveCell(x, y, x + (j * dir), y);
				return;
			}
		}
	}
}

void PrimaryChunkWorker::UpdateFire(int x, int y, Cell& cell)
{
	if (cell.life_time <= 0)
		SetCell(x, y, Cell());

	cell.life_time--;

	int xdir = (rand() % 3) - 1;
	int ydir = (rand() % 3) - 1;

	for (int j = cell.spread_rate; j > 0; --j)
	{
		if (HasProperties(x + (j * xdir), y + ydir, Cell::Properties::FLAMMABLE))
		{
			MoveCell(x, y, x + (j * xdir), y + ydir);
			return;
		}
		else if (HasProperties(x + (j * -xdir), y + ydir, Cell::Properties::FLAMMABLE))
		{
			MoveCell(x, y, x + (j * -xdir), y + ydir);
			return;
		}
		else if (HasProperties(x + (j * xdir), y + -ydir, Cell::Properties::FLAMMABLE))
		{
			MoveCell(x, y, x + (j * xdir), y + -ydir);
			return;
		}
		else if (HasProperties(x + (j * -xdir), y + -ydir, Cell::Properties::FLAMMABLE))
		{
			MoveCell(x, y, x + (j * -xdir), y + -ydir);
			return;
		}
		else if (IsEmpty(x + (j * xdir), y - 1))
		{
			MoveCell(x, y, x + (j * xdir), y - 1);
			return;
		}
		else
			KeepAlive(x, y);
	}
}
