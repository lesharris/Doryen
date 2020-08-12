// Joan Andrés (@Andres6936) Github.

#include "Lines.hpp"
#include "../LineListener.hpp"

using namespace Doryen;

Functor::Lines::Lines(Console& _console) : ISample("Line drawing", _console)
{

}

void Functor::Lines::render(KeyCode key, const Mouse& mouse)
{
	static Console bk(sample.getWidth(), sample.getHeight()); // colored background
	static bool init = false;

	static const char* flagNames[] = {
			"TCOD_BKGND_NONE",
			"TCOD_BKGND_SET",
			"TCOD_BKGND_MULTIPLY",
			"TCOD_BKGND_LIGHTEN",
			"TCOD_BKGND_DARKEN",
			"TCOD_BKGND_SCREEN",
			"TCOD_BKGND_COLOR_DODGE",
			"TCOD_BKGND_COLOR_BURN",
			"TCOD_BKGND_ADD",
			"TCOD_BKGND_ADDALPHA",
			"TCOD_BKGND_BURN",
			"TCOD_BKGND_OVERLAY",
			"TCOD_BKGND_ALPHA",
			"TCOD_BKGND_DEFAULT"
	};

	if (key == KeyCode::ENTER or key == KeyCode::KP_ENTER)
	{
		// switch to the next blending mode
		backFlag = switchBackgroundFlag(backFlag);
	}

	// TODO: Here, I break code, Fix. See Git

	if (!init)
	{
		// initialize the colored background
		for (int x = 0; x < sample.getWidth(); x++)
		{
			for (int y = 0; y < sample.getHeight(); y++)
			{
				Doryen::Color col;
				col.r = (uint8)(x * 255 / (sample.getWidth() - 1));
				col.g = (uint8)((x + y) * 255 / (sample.getWidth() - 1 + sample.getHeight() - 1));
				col.b = (uint8)(y * 255 / (sample.getHeight() - 1));
				bk.setCharBackground(x, y, col, Doryen::BackgroundFlag::SET);
			}
		}
		init = true;
	}

	// blit the background
	bk.blit({ 0, 0 }, sample, { 0, 0 });
	// render the gradient
	int recty = (int)((sample.getHeight() - 2) *
					  ((1.0f + cosf(sample.getElapsedSeconds())) / 2.0f));
	for (int x = 0; x < sample.getWidth(); x++)
	{
		Doryen::Color col;
		col.r = (uint8)(x * 255 / sample.getWidth());
		col.g = (uint8)(x * 255 / sample.getWidth());
		col.b = (uint8)(x * 255 / sample.getWidth());

		sample.setCharBackground(x, recty, col, backFlag);
		sample.setCharBackground(x, recty + 1, col, backFlag);
		sample.setCharBackground(x, recty + 2, col, backFlag);
	}
	// calculate the segment ends
	float angle = sample.getElapsedSeconds() * 2.0f;
	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);
	int xo = (int)(sample.getWidth() / 2 * (1 + cosAngle));
	int yo = (int)(sample.getHeight() / 2 + sinAngle * sample.getWidth() / 2);
	int xd = (int)(sample.getWidth() / 2 * (1 - cosAngle));
	int yd = (int)(sample.getHeight() / 2 - sinAngle * sample.getWidth() / 2);

	// render the line
	RenderLine listener;

	Doryen::Algorithms::Line objLine = Doryen::Algorithms::Line();

	objLine.line(xo, yo, xd, yd, listener);

	// print the current flag
	sample.write(2, 2, format("{} (ENTER to change)", flagNames[(int)backFlag]));
}


Doryen::BackgroundFlag Functor::Lines::switchBackgroundFlag(Doryen::BackgroundFlag flag)
{
	switch (flag)
	{
	case BackgroundFlag::NONE:
		return BackgroundFlag::SET;

	case BackgroundFlag::SET:
		return BackgroundFlag::MULTIPLY;

	case BackgroundFlag::MULTIPLY:
		return BackgroundFlag::LIGHTEN;

	case BackgroundFlag::LIGHTEN:
		return BackgroundFlag::DARKEN;

	case BackgroundFlag::DARKEN:
		return BackgroundFlag::SCREEN;

	case BackgroundFlag::SCREEN:
		return BackgroundFlag::COLOR_DODGE;

	case BackgroundFlag::COLOR_DODGE:
		return BackgroundFlag::COLOR_BURN;

	case BackgroundFlag::COLOR_BURN:
		return BackgroundFlag::ADD;

	case BackgroundFlag::ADD:
		return BackgroundFlag::ADDA;

	case BackgroundFlag::ADDA:
		return BackgroundFlag::BURN;

	case BackgroundFlag::BURN:
		return BackgroundFlag::OVERLAY;

	case BackgroundFlag::OVERLAY:
		return BackgroundFlag::ALPH;

	case BackgroundFlag::ALPH:
		return BackgroundFlag::DEFAULT;

	case BackgroundFlag::DEFAULT:
		return BackgroundFlag::NONE;
	}
}
