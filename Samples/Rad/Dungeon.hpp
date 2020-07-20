/*
* Umbra
* Copyright (c) 2009, 2010 Mingos, Jice
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The names of Mingos or Jice may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY MINGOS & JICE ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL MINGOS OR JICE BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <Doryen/libtcod.hpp>

using namespace Doryen;

class Dungeon : public Algorithms::BinarySpacePartitionCallback
{

private :

	// Constants Static

	const static std::uint8_t BSP_DEPTH = 8;
	const static std::uint8_t MIN_ROOM_SIZE = 4;

	const static bool RANDOM_ROOM = true;
	const static bool ROOM_WALLS = false;

	const static std::uint8_t MAP_WIDTH = 39;
	const static std::uint8_t MAP_HEIGHT = 50;

	// Variables

	Map map{ MAP_WIDTH, MAP_HEIGHT };

	// Methods Private

	void vline(int x, int y1, int y2);

	void vline_up(int x, int y);

	void vline_down(int x, int y);

	void hline(int x1, int y, int x2);

	void hline_left(int x, int y);

	void hline_right(int x, int y);

public :

	void generateDungeon();

	// libtcod bsp callback stuff
	bool visitNode(Algorithms::BinarySpacePartition* node, Map& userData);

	Map& getReferenceToMap();
};
