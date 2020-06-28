// Joan Andrés (@Andres6936) Github.

#include "Offscreen.hpp"

using namespace Doryen;

Functor::Offscreen::Offscreen(std::string _name, std::reference_wrapper<Console> _console) : ISample(_name, _console)
{

}

void Functor::Offscreen::render(KeyCode key, const Mouse& mouse)
{
	static Console secondary(sample.get().getWidth() / 2, sample.get().getHeight() / 2);
	static Console screenshot(sample.get().getWidth(), sample.get().getHeight());

	static bool init = false; // draw the secondary screen only the first time
	static int counter = 0;
	static int x = 0, y = 0; // secondary screen position
	static int xdir = 1, ydir = 1; // movement direction

	if (!init)
	{
		init = true;
		secondary.printFrame(0, 0, sample.get().getWidth() / 2, sample.get().getHeight() / 2, false,
				Doryen::BackgroundFlag::SET,
				"Offscreen console");

		secondary.printRectEx(sample.get().getWidth() / 4, 2, sample.get().getWidth() / 2 - 2,
				sample.get().getHeight() / 2,
				TCOD_BKGND_NONE, TCOD_CENTER,
				"You can render to an offscreen console and blit in on another one, simulating alpha transparency.");
	}

	counter++;
	if (counter % 20 == 0)
	{
		// move the secondary screen every 2 seconds
		x += xdir;
		y += ydir;

		if (x == sample.get().getWidth() / 2 + 5)
		{
			xdir = -1;
		}
		else if (x == -5)
		{
			xdir = 1;
		}

		if (y == sample.get().getHeight() / 2 + 5)
		{
			ydir = -1;
		}
		else if (y == -5)
		{
			ydir = 1;
		}
	}

	// restore the initial screen
	screenshot.blit({ 0, 0 }, sample.get(), { 0, 0 });

	// For avoid that program terminate
	if (not(x < 0) and not(y < 0))
	{
		// blit the overlapping screen
		secondary.blit({ 0, 0 }, sample.get(), { x, y }, 1.0f, 0.75f);
	}
}
