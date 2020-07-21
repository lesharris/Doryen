/*
* Photon reactor
* Copyright (c) 2011 Jice
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The names of Jice may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY JICE ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL JICE BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "PhotonShader.hpp"

PhotonShader::PhotonShader(Map& map) noexcept: Shader(map)
{
	int size = map.getWidth() * map.getHeight();
	int maxDiameter = 2 * maxRadius + 1;

	// initialize data
	ff.resize(size * maxDiameter * maxDiameter);
	std::fill(ff.begin(), ff.end(), 0.0f);

	ffSum = new float[size];
	data = new CellData[size];

	memset(data, 0, sizeof(CellData) * size);
	// compute form factors
	for (int y = 0; y < map.getHeight(); y++)
	{
		for (int x = 0; x < map.getWidth(); x++)
		{
			computeFormFactor(x, y);
		}
	}
}

int PhotonShader::addLight(int x, int y, int radius, const Doryen::Color& col)
{
	if (radius > maxRadius) maxRadius = radius;
	return Shader::addLight(x, y, radius, col);
}

// compute form factor of cell x,y relative to all its surrounding cells
void PhotonShader::computeFormFactor(int x, int y)
{
	int ominx = x - maxRadius;
	int ominy = y - maxRadius;
	int omaxx = x + maxRadius;
	int omaxy = y + maxRadius;
	int minx = MAX(ominx, 0);
	int miny = MAX(ominy, 0);
	int maxx = MIN(omaxx, map.getWidth() - 1);
	int maxy = MIN(omaxy, map.getHeight() - 1);
	int maxDiameter = 2 * maxRadius + 1;

	auto cellFormFactor = ff.begin() + (x + y * map.getWidth()) * maxDiameter * maxDiameter;

	map.computeFov(x, y, maxRadius);
	int squareRad = (maxRadius * maxRadius);
	//float invRad=1.0/squareRad;
	double curFfSum = 0;
	float offset = 1.0f / (1.0f + (float)(maxRadius * maxRadius) / 20);
	float factor = 1.0f / (1.0f - offset);
	for (int cx = minx, cdx = minx - ominx; cx <= maxx; cx++, cdx++)
	{
		for (int cy = miny, cdy = miny - ominy; cy <= maxy; cy++, cdy++)
		{
			if (map.isInFov(cx, cy))
			{
				int dist = (maxRadius - cdx) * (maxRadius - cdx) + (maxRadius - cdy) * (maxRadius - cdy);
				if (dist <= squareRad)
				{
					//float value = (1.0f-dist*invRad) ;
					//float value =1.0f/(1.0f+(float)(dist)/20);
					float value = (1.0f / (1.0f + (float)(dist) / 20) - offset) * factor;
					curFfSum += value;
					cellFormFactor[cdx + cdy * maxDiameter] = value;
				}
			}
		}
	}
	// scale so that the sum of all form factors for cell x,y is 1.0
	ffSum[x + y * map.getWidth()] = curFfSum;
	if (curFfSum > 1E-8)
	{
		curFfSum = 1.0 / curFfSum;
		for (int cx = minx, cdx = minx - ominx; cx <= maxx; cx++, cdx++)
		{
			for (int cy = miny, cdy = miny - ominy; cy <= maxy; cy++, cdy++)
			{
				cellFormFactor[cdx + cdy * maxDiameter] *= curFfSum;
			}
		}
	}
}

void PhotonShader::computeLightContribution(int lx, int ly, int lradius, const FColor &lcol, float reflectivity)
{
	const std::int32_t O_MIN_X = lx - lradius;
	const std::int32_t O_MIN_Y = ly - lradius;
	const std::int32_t O_MAX_X = lx + lradius;
	const std::int32_t O_MAX_Y = ly + lradius;

	const std::int32_t MIN_X = std::max(O_MIN_X, 0);
	const std::int32_t MIN_Y = std::max(O_MIN_Y, 0);

	const std::int32_t MAX_X = std::min(O_MAX_X, map.getWidth() - 1);
	const std::int32_t MAX_Y = std::min(O_MAX_Y, map.getHeight() - 1);

	int maxDiameter = 2 * maxRadius + 1;

	auto cellFormFactor = ff.begin() + (lx + ly * map.getWidth()) * maxDiameter * maxDiameter;

	// compute the light's contribution
#define MIN_FACTOR (1.0f/255.0f)
	int width = map.getWidth();

	for (int y = MIN_Y, cdy = MIN_Y - O_MIN_Y; y <= MAX_Y; y++, cdy++)
	{
		CellData* cellData = &data[MIN_X + y * width];

		auto cellFormRow = std::next(cellFormFactor, MIN_X - O_MIN_X + cdy * maxDiameter);

		for (int x = MIN_X; x <= MAX_X; x++, cellData++, cellFormRow++)
		{
			float cellff = *cellFormRow;
			if (cellff > MIN_FACTOR)
			{
				cellData->incoming.r += lcol.r * cellff;
				cellData->incoming.g += lcol.g * cellff;
				cellData->incoming.b += lcol.b * cellff;
			}
		}
	}
}

void PhotonShader::propagate()
{
	CellData* cellData = data;

	int size = map.getWidth() * map.getHeight();
	lightsCoord.clear();
	for (int c = 0; c < size; c++, cellData++)
	{
		cellData->emission.r = cellData->incoming.r * reflectivity;
		cellData->emission.g = cellData->incoming.g * reflectivity;
		cellData->emission.b = cellData->incoming.b * reflectivity;
		cellData->outgoing.r += cellData->incoming.r * selfIllumination;
		cellData->outgoing.g += cellData->incoming.g * selfIllumination;
		cellData->outgoing.b += cellData->incoming.b * selfIllumination;
		cellData->incoming.r = 0;
		cellData->incoming.g = 0;
		cellData->incoming.b = 0;
		if (cellData->emission.r > 0 || cellData->emission.g > 0 || cellData->emission.b > 0)
			lightsCoord.push(Coord(c % map.getWidth(), c / map.getWidth()));
	}
}

void PhotonShader::compute()
{
	// turn off all lights
	int size = map.getWidth() * map.getHeight();
	memset(data, 0, sizeof(CellData) * size);

	// first pass. lights only
	for (Light& l : lights)
	{
		int off = l.x + l.y * map.getWidth();
		CellData* cellData = &data[off];
		float sum = ffSum[off];
		cellData->emission.r = l.col.r * sum;
		cellData->emission.g = l.col.r * sum;
		cellData->emission.b = l.col.r * sum;
		computeLightContribution(l.x, l.y, l.radius, cellData->emission, 0.5f);
	}

	// other passes. all lit cells act as lights
	int pass=1;
	while ( pass < nbPass )
	{
		propagate();
		CellData* cellData = data;
		//for (int y=0; y < map.getHeight(); y++ ) {
		//	for (int x=0; x < map.getWidth(); x++, cellData++ ) {
		for (Coord* c = lightsCoord.begin(); c != lightsCoord.end(); c++)
		{
			cellData = &data[c->x + c->y * map.getWidth()];
			computeLightContribution(c->x, c->y, maxRadius, cellData->emission, reflectivity);
		}
		pass++;
	}
	
	CellData *cellData=data;

	propagate();

	for (int c=0; c < size; c++,cellData++) {
		lightmap[c].r = (uint8)MIN(255,cellData->outgoing.r);
		lightmap[c].g = (uint8)MIN(255,cellData->outgoing.g);
		lightmap[c].b = (uint8)MIN(255,cellData->outgoing.b);
	}	
}
