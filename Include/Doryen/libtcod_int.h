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

#ifndef _TCODLIB_INT_H
#define _TCODLIB_INT_H

#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>

#include <Doryen/fov.h>
#include <Doryen/sys.h>
#include <Doryen/console_types.h>
#include <Doryen/mersenne_types.h>

/* tcodlib internal stuff */
#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char uint8;
typedef unsigned int uint32;

typedef struct
{
	int a;
	int r;
	int g;
	int b;
} TCOD_color_t;

/* a cell in the console */
typedef struct
{
	int c;        /* character ascii code */
	int cf;        /* character number in font */
	TCOD_color_t fore;    /* foreground color */
	TCOD_color_t back;    /* background color */
	uint8 dirt;    /* cell modified since last flush ? */
} char_t;

/* Console non public data */
typedef struct {
	char_t *buf; /* current console */
	char_t *oldbuf; /* console for last frame */
	/* console width and height (in characters,not pixels) */
	int w,h;
	/* default background operator for print & print_rect functions */
	TCOD_bkgnd_flag_t bkgnd_flag;
	/* default alignment for print & print_rect functions */
	TCOD_alignment_t alignment;
	/* foreground (text), background and key colors */
	TCOD_color_t fore,back,key;
	uint8 fade;
	bool haskey; /* a key color has been defined */
} TCOD_console_data_t;

/* fov internal stuff */
typedef struct {
	bool transparent:1;
	bool walkable:1;
	bool fov:1;
} cell_t;
typedef struct {
	int width;
	int height;
	int nbcells;
	cell_t *cells;
} map_t;

/* pseudorandom number generator toolkit */
typedef struct {
	/* algorithm identifier */
	TCOD_random_algo_t algo;
	/* distribution */
	TCOD_distribution_t distribution;
	/* Mersenne Twister stuff */
	uint32 mt[624];
	int cur_mt;
	/* Complementary-Multiply-With-Carry stuff */
	/* shared with Generalised Feedback Shift Register */
	uint32 Q[4096], c;
    int cur;
} mersenne_data_t;

#if defined(__ANDROID__) && !defined(NDEBUG)
#include <android/log.h>
#ifdef printf
#undef printf
#endif
#ifdef vprintf
#undef vprintf
#endif
#define printf(args...) __android_log_print(ANDROID_LOG_INFO, "libtcod", ## args)
#define vprintf(args...) __android_log_vprint(ANDROID_LOG_INFO, "libtcod", ## args)

#ifdef assert
#undef assert
#endif
#define assert(cond) if(!(cond)) __android_log_assert(#cond, "libtcod", "assertion failed: %s", #cond)
#endif

#ifdef NDEBUG
#define TCOD_IF(x) if (x)
#define TCOD_IFNOT(x) if (!(x))
#define TCOD_ASSERT(x)
#define TCOD_LOG(x)
#else
#define TCOD_IF(x) assert(x);
#define TCOD_IFNOT(x) assert(x); if (0)
#define TCOD_ASSERT(x) assert(x)
#define TCOD_LOG(x) printf x
#endif

#ifndef NO_OPENGL

#endif

void TCOD_fatal_nopar(const char *msg);

/* TCOD_list nonpublic methods */
void TCOD_list_set_size(TCOD_list_t l, int size);

/* color values */
#define TCOD_BLACK 0,0,0
#define TCOD_WHITE 255,255,255

#ifdef __cplusplus
}
#endif
#endif

