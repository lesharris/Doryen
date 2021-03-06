// Joan Andrés (@Andres6936) Github.

#include "Bsp.hpp"
#include "../BspListener.hpp"

using namespace Doryen;

Functor::BSP::BSP(Console& _console) : ISample("Bsp toolkit", _console)
{

}


void Functor::BSP::render(KeyCode key, const Mouse& mouse)
{
	static Algorithms::BinarySpacePartition* bsp = NULL;
	static bool generate = true;
	static bool refresh = false;
	static Map map{ SAMPLE_SCREEN_WIDTH, SAMPLE_SCREEN_HEIGHT };
	static BspListener listener;

	if (generate || refresh)
	{
		// dungeon generation
		if (!bsp)
		{
			// create the bsp
			bsp = new Algorithms::BinarySpacePartition(
					0, 0, SAMPLE_SCREEN_WIDTH, SAMPLE_SCREEN_HEIGHT);
		}
		else
		{
			// restore the nodes size
			bsp->resize(0, 0, SAMPLE_SCREEN_WIDTH, SAMPLE_SCREEN_HEIGHT);
		}

		if (generate)
		{
			// build a new random bsp tree
			bsp->removeSons();
			bsp->splitRecursive(bspDepth, minRoomSize + (roomWalls ? 1 : 0),
					minRoomSize + (roomWalls ? 1 : 0), 1.5f, 1.5f);
		}

		map.clear();
		// create the dungeon from the bsp
		bsp->traverseInvertedLevelOrder(&listener, map);
		generate = false;
		refresh = false;
	}
	sample.clear();
	sample.setForegroundColor(Doryen::Palette::GRAY_WARN_1);
	sample.write(1, 1,
			format("ENTER : rebuild bsp\nSPACE : rebuild dungeon\n+-: bsp depth {}\n*/: room size {}\n1 : random room size {}",
					bspDepth, minRoomSize,
					randomRoom ? "ON" : "OFF"));

	if (randomRoom)
	{
		sample.write(1, 6, format("2 : room walls {}",
				roomWalls ? "ON" : "OFF"));
	}
	// render the level
	for (int y = 0; y < SAMPLE_SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < SAMPLE_SCREEN_WIDTH; x++)
		{
			bool wall = map.isWalkable(x, y);
			sample.setCellBackgroundColor(x, y, wall ? Palette::PRIMARY_VIVID : Palette::PRIMARY_DARK,
					Doryen::BlendModes::SET);
		}
	}
	if (key == KeyCode::ENTER || key == KeyCode::PRINT_SCREEN)
	{
		generate = true;
	}
	else if (key == KeyCode::SPACE)
	{
		refresh = true;
	}
	else if (key == KeyCode::KP_ADD)
	{
		bspDepth++;
		generate = true;
	}
	else if (key == KeyCode::KP_SUB && bspDepth > 1)
	{
		bspDepth--;
		generate = true;
	}
	else if (key == KeyCode::KP_MUL)
	{
		minRoomSize++;
		generate = true;
	}
	else if (key == KeyCode::KP_DIV && minRoomSize > 2)
	{
		minRoomSize--;
		generate = true;
	}
	else if (key == KeyCode::K_1 or key == KeyCode::KP_1)
	{
		randomRoom = !randomRoom;
		if (!randomRoom)
		{ roomWalls = true; }
		refresh = true;
	}
	else if (key == KeyCode::K_2 or key == KeyCode::KP_2)
	{
		roomWalls = !roomWalls;
		refresh = true;
	}
}
