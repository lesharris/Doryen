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

#ifndef _TCOD_PERLIN_H
#define _TCOD_PERLIN_H

#include <cstdint>
#include <vector>
#include <array>

namespace Doryen
{

	enum class TypeNoise : unsigned char
	{
		Perlin,
		Simplex,
		Wavelet,
	};

	namespace Algorithms
	{

		template<int Dimension>
		class Perlin
		{

		private:

			std::array<std::uint8_t, 512> p{};

			static float Fade(float t);

			static float Lerp(float t, float a, float b);

			static float Grad(std::uint8_t hash, float x, float y, float z);

		public:

			const static std::uint8_t MAX_OCTAVES = 128;

			/* fractal stuff */
			float lacunarity;

			std::array<float, MAX_OCTAVES> exponent;

			/* noise type */
			TypeNoise noise_type = TypeNoise::Simplex;

			// Construct

			Perlin();

			Perlin(std::int32_t _dimensions, float _lacunarity);

			// Methods

			float noise(const std::array<float, Dimension>& input);

			float fractalNoise(const std::array<float, Dimension>& input, int octaves);

			float turbulenceNoise(const std::array<float, Dimension>& input, int octaves);

			// Getters

			TypeNoise getNoiseType() const;

			// Setters

			void setNoiseType(TypeNoise _noiseType);
		};


	}
}

#endif
