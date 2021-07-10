#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

struct tile
{
	int neighbors;
	bool revealed;
	bool flagged;
	bool mined;
};

void revealTile(tile& t)
{
	t.flagged = false;
	t.revealed = true;
}

void flagTile(tile& t)
{
	if (!t.revealed)
		t.flagged = !t.flagged;
}

class Minesweeper : public olc::PixelGameEngine
{
public:
	Minesweeper()
	{
		sAppName = "Minesweeper";
	}

	olc::vi2d vFieldSize = { 10, 8 };
	olc::vi2d vTileSize = { 10, 10 };

	int nMineTotal;

	std::unique_ptr<tile[]> field;

public:
	int getNeighborMineCount(int i)
	{
		if (field[i].mined)
			return 0;

		int n = 0;

		int xStart = (i % vFieldSize.x == 0) ? 0 : -1;
		int xEnd = (i % vFieldSize.x == vFieldSize.x - 1) ? 0 : 1;
		int yStart = (i < vFieldSize.x) ? 0 : -1;
		int yEnd = (i >= vFieldSize.x * (vFieldSize.y - 1)) ? 0 : 1;
		for (int x = xStart; x <= xEnd; x++)
		{
			for (int y = yStart; y <= yEnd; y++)
			{
				int l = i + (y * vFieldSize.x) + x;
				if (l != i && field[l].mined)
					n++;
			}
		}

		return n;
	}

	void shuffleField()
	{
		for (int i = vFieldSize.x * vFieldSize.y - 1; i >= 0; i--)
		{
			int r = rand() % (i + 1);
			tile tmp = field[i];
			field[i] = field[r];
			field[r] = tmp;
		}
	}

	void setNeighborCounts()
	{
		for (int i = 0; i < vFieldSize.x * vFieldSize.y; i++)
		{
			field[i].neighbors = getNeighborMineCount(i);
		}
	}

	void resetField()
	{
		shuffleField();
		setNeighborCounts();
	}

	void recursiveReveal(int i)
	{
		revealTile(field[i]);

		if (field[i].neighbors == 0 && !field[i].mined)
		{
			int xStart = (i % vFieldSize.x == 0) ? 0 : -1;
			int xEnd = (i % vFieldSize.x == vFieldSize.x - 1) ? 0 : 1;
			int yStart = (i < vFieldSize.x) ? 0 : -1;
			int yEnd = (i >= vFieldSize.x * (vFieldSize.y - 1)) ? 0 : 1;
			for (int x = xStart; x <= xEnd; x++)
			{
				for (int y = yStart; y <= yEnd; y++)
				{
					int l = i + (y * vFieldSize.x) + x;
					if (l != i && !field[l].revealed && !field[l].mined)
						recursiveReveal(l);
				}
			}
		}
	}

	bool OnUserCreate() override
	{
		field = std::make_unique<tile[]>(vFieldSize.x * vFieldSize.y);
		nMineTotal = 10;
		
		// populate field
		for (int i = 0; i < vFieldSize.x * vFieldSize.y; i++)
		{
			field[i] = { 0, false, false, (i < nMineTotal) ? true : false };
		}

		resetField();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		auto GetTile = [&](int x, int y) {
			return field[y * vFieldSize.x + x];
		};

		auto SetTile = [&](int x, int y, tile t)
		{
			field[y * vFieldSize.x + x] = t;
		};

		Clear(olc::BLACK);

		// Input
		if (GetKey(olc::SPACE).bPressed)
			resetField();

		// Mouse input
		olc::vi2d vMouse = { GetMouseX(), GetMouseY() };
		olc::vi2d vTileSelected = vMouse / vTileSize;
		
		if (GetMouse(0).bPressed)
			recursiveReveal(vTileSelected.y * vFieldSize.x + vTileSelected.x);
		if (GetMouse(1).bPressed)
			flagTile(field[vTileSelected.y * vFieldSize.x + vTileSelected.x]);

		// Draw field
		for (int y = 0; y < vFieldSize.y; y++)
		{
			for (int x = 0; x < vFieldSize.x; x++)
			{
				olc::Pixel color;
				tile curr = field[y * vFieldSize.x + x];

				if		(curr.flagged)
					color = olc::GREEN;
				else if (curr.revealed)
					color = (curr.mined) ? olc::RED : olc::WHITE;
				else
					color = olc::GREY;

				FillRect(x * vTileSize.x, y * vTileSize.y, vTileSize.x - 1, vTileSize.y - 1, color);

				if (curr.revealed && curr.neighbors > 0)
					DrawString(olc::vi2d(x * vTileSize.x + 1, y * vTileSize.y + 1), std::to_string(curr.neighbors), olc::GREEN);
			}
		}

		return true;
	}
};


int main()
{
	Minesweeper demo;
	if (demo.Construct(demo.vFieldSize.x * demo.vTileSize.x, demo.vFieldSize.y * demo.vTileSize.y, 4, 4)) 
		demo.Start();

	return 0;
}
