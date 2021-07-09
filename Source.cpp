#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

struct tile
{
	uint8_t tile;
	bool revealed;
	bool flagged;
	bool mined;
};

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

	bool OnUserCreate() override
	{
		field = std::make_unique<tile[]>(vFieldSize.x * vFieldSize.y);
		nMineTotal = 10;
		
		// populate field
		for (int i = 0; i < vFieldSize.x * vFieldSize.y; i++)
		{
			field[i] = { 0, false, false, (i < nMineTotal) ? true : false };
		}

		shuffleField();

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


		// Input:
		if (GetKey(olc::SPACE).bPressed)
			shuffleField();

		// Mouse input
		olc::vi2d vMouse = { GetMouseX(), GetMouseY() };
		olc::vi2d vTileSelected = vMouse / vTileSize;
		
		if (GetMouse(0).bPressed)
		{
			field[vTileSelected.y * vFieldSize.x + vTileSelected.x].flagged = 
				!field[vTileSelected.y * vFieldSize.x + vTileSelected.x].flagged;
		}

		// Draw field
		for (int y = 0; y < vFieldSize.y; y++)
		{
			for (int x = 0; x < vFieldSize.x; x++)
			{
				olc::Pixel color;
				tile curr = field[y * vFieldSize.x + x];

				if		(curr.flagged)	color = olc::GREEN;
				else if (curr.mined)	color = olc::RED;
				else if (curr.revealed) color = olc::WHITE;
				else					color = olc::GREY;

				FillRect(x * vTileSize.x, y * vTileSize.y, vTileSize.x - 1, vTileSize.y - 1, color);
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
