#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

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

	std::unique_ptr<wchar_t[]> field;

public:
	void shuffleField()
	{
		for (int i = vFieldSize.x * vFieldSize.y - 1; i >= 0; i--)
		{
			int r = rand() % (i + 1);
			wchar_t tmp = field[i];
			field[i] = field[r];
			field[r] = tmp;
		}
	}

	bool OnUserCreate() override
	{
		field = std::make_unique<wchar_t[]>(vFieldSize.x * vFieldSize.y);
		nMineTotal = 10;
		
		// populate field
		for (int i = 0; i < vFieldSize.x * vFieldSize.y; i++)
		{
			field[i] = (i < nMineTotal) ? L'#' : L'.';
		}

		shuffleField();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);

		// Input:
		if (GetKey(olc::SPACE).bPressed)
			shuffleField();

		// Draw field
		for (int y = 0; y < vFieldSize.y; y++)
		{
			for (int x = 0; x < vFieldSize.x; x++)
			{
				olc::Pixel color;
				switch (field[y * vFieldSize.x + x])
				{
				case L'#':
					color = olc::RED;
					break;
				default:
					color = olc::GREY;
				}
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
