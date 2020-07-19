/*
* libtcod 1.5.1
* Copyright (c) 2008,2009,2010,2012 Jice & Mingos
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Jice or Mingos may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY JICE AND MINGOS ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL JICE OR MINGOS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <math.h>
#include "Doryen/Color.hpp"

using namespace Doryen;

// Getters

short Color::getRed() const
{
	return r;
}

short Color::getGreen() const
{
	return g;
}

short Color::getBlue() const
{
	return b;
}

short Color::getAlpha() const
{
	return a;
}

// Setters

void Color::setRed(const short _r)
{
	r = _r;
}

void Color::setGreen(const short _g)
{
	g = _g;
}

void Color::setBlue(const short _b)
{
	b = _b;
}

void Color::setAlpha(const short _a)
{
	a = _a;
}

Color::Color(short r, short g, short b) noexcept
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 255;
}

Color::Color() noexcept
{
	r = 0;
	g = 0;
	b = 0;
	a = 255;
}

Color::Color(short r, short g, short b, short a) noexcept
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

bool Color::equals(const Color& c) const noexcept
{
	return r == c.r && g == c.g && b == c.b && a == c.a;
}

// Private function
short clamp(short a, short b, short x)
{
	if (x < a)
	{
		return a;
	}
	else
	{
		if (x > b)
		{
			return b;
		}
		else
		{
			return x;
		}
	}
}

void Color::trasformColor(const Color& another, Doryen::BackgroundFlag flag)
{
	short nr = 0;
	short ng = 0;
	short nb = 0;

	switch (flag)
	{

		// Noping
	case BackgroundFlag::NONE:
	case BackgroundFlag::DEFAULT:
		break;

	case BackgroundFlag::SET:

		// Copy information color
		this->r = another.r;
		this->g = another.g;
		this->b = another.b;
		break;

	case BackgroundFlag::MULTIPLY:

		this->multiply(another);
		break;

	case BackgroundFlag::LIGHTEN:

		this->r = std::max(this->r, another.r);
		this->g = std::max(this->g, another.g);
		this->b = std::max(this->b, another.b);
		break;

	case BackgroundFlag::DARKEN:

		this->r = std::min(this->r, another.r);
		this->g = std::min(this->g, another.g);
		this->b = std::min(this->b, another.b);
		break;

	case BackgroundFlag::SCREEN:

		this->r = (short)(255 - (255 - this->r) * (255 - another.r) / 255);
		this->g = (short)(255 - (255 - this->g) * (255 - another.g) / 255);
		this->b = (short)(255 - (255 - this->b) * (255 - another.b) / 255);
		break;

	case BackgroundFlag::COLOR_DODGE:

		if (this->r != 255)
		{
			nr = (short)((255 * another.r) / (255 - this->r));
		}
		else
		{
			nr = 255;
		}

		if (this->g != 255)
		{
			ng = (short)((255 * another.g) / (255 - this->g));
		}
		else
		{
			ng = 255;
		}

		if (this->b != 255)
		{
			nb = (short)((255 * another.b) / (255 - this->b));
		}
		else
		{
			nb = 255;
		}

		this->r = clamp(0, 255, nr);
		this->g = clamp(0, 255, ng);
		this->b = clamp(0, 255, nb);
		break;

	case BackgroundFlag::COLOR_BURN:

		if (another.r > 0)
		{
			nr = (short)(255 - (255 * (255 - this->r)) / another.r);
		}
		else
		{
			nr = 255;
		}

		if (another.g > 0)
		{
			ng = (short)(255 - (255 * (255 - this->g)) / another.g);
		}
		else
		{
			ng = 255;
		}

		if (another.b > 0)
		{
			nb = (short)(255 - (255 * (255 - this->b)) / another.b);
		}
		else
		{
			nb = 255;
		}

		this->r = clamp(0, 255, nr);
		this->g = clamp(0, 255, ng);
		this->b = clamp(0, 255, nb);
		break;

	case BackgroundFlag::ADD:

		this->add(another);
		break;

	case BackgroundFlag::ADDA:

		// TODO: Implemented Alpha Sum
		this->add(another);
		break;

	case BackgroundFlag::BURN:

		nr = (short)(this->r + another.r - 255);
		ng = (short)(this->g + another.g - 255);
		nb = (short)(this->b + another.b - 255);

		this->r = clamp(0, 255, nr);
		this->g = clamp(0, 255, ng);
		this->b = clamp(0, 255, nb);
		break;

	case BackgroundFlag::OVERLAY:

		if (another.r <= 128)
		{
			nr = (short)(2 * (another.r) * this->r / 255);
		}
		else
		{
			nr = (short)(255 - 2 * (255 - another.r) * (255 - this->r) / 255);
		}

		if (another.g <= 128)
		{
			ng = (short)(2 * (another.g) * this->g / 255);
		}
		else
		{
			ng = (short)(255 - 2 * (255 - another.g) * (255 - this->g) / 255);
		}

		if (another.b <= 128)
		{
			nb = (short)(2 * (another.b) * this->b / 255);
		}
		else
		{
			nb = (short)(255 - 2 * (255 - another.b) * (255 - this->b) / 255);
		}

		this->r = clamp(0, 255, nr);
		this->g = clamp(0, 255, ng);
		this->b = clamp(0, 255, nb);
		break;

	case BackgroundFlag::ALPH:

		// TODO: Implemented Alpha
		break;
	}
}

void Color::multiply(const Color& other)
{
	this->r = (short)(this->r * other.r / 255);
	this->g = (short)(this->g * other.g / 255);
	this->b = (short)(this->b * other.b / 255);
}

void Color::add(const Color& other)
{
	this->r = (short)(this->r + other.r);
	this->g = (short)(this->g + other.g);
	this->b = (short)(this->b + other.b);

	this->r = std::min((short)255, r);
	this->g = std::min((short)255, g);
	this->b = std::min((short)255, b);
}

Color::Color(const Color& _rhs) noexcept
{
	this->r = _rhs.r;
	this->g = _rhs.g;
	this->b = _rhs.b;
	this->a = _rhs.a;
}

void Color::operator=(const Color& _rhs)
{
	this->r = _rhs.r;
	this->g = _rhs.g;
	this->b = _rhs.b;
	this->a = _rhs.a;
}
