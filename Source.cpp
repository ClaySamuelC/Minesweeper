#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Minesweeper : public olc::PixelGameEngine
{
public:
	Minesweeper()
	{
		sAppName = "Minesweeper";
	}

public:
	bool OnUserCreate() override
	{
		
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
	

		return true;
	}
};


int main()
{
	Minesweeper demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}
