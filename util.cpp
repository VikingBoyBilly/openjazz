
/*
 *
 * util.cpp
 *
 * Created as util.c on the 22nd of July 2008 from parts of main.c
 * Renamed util.cpp on the 3rd of February 2009
 *
 * Part of the OpenJazz project
 *
 *
 * Copyright (c) 2005-2009 Alister Thomson
 *
 * OpenJazz is distributed under the terms of
 * the GNU General Public License, version 2.0
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/*
 * Contains core utility functions.
 *
 */


#include "OpenJazz.h"
#include <string.h>


int fileExists (char * fileName) {

	File *f;

	printf("Check: ");

	try {

		f = new File(fileName, false);

	} catch (int e) {

		return 0;

	}

	delete f;

	return 1;

}


SDL_Surface * createSurface (unsigned char * pixels, int width, int height) {

	SDL_Surface *ret;
	int y;

	// Create the surface
	ret = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 8, 0, 0, 0, 0);

	// Set the surface's palette
	SDL_SetPalette(ret, SDL_LOGPAL, logicalPalette, 0, 256);

	// Upload pixel data to the surface
	if (SDL_MUSTLOCK(ret)) SDL_LockSurface(ret);

	for (y = 0; y < height; y++)
		memcpy(((unsigned char *)(ret->pixels)) + (ret->pitch * y),
			pixels + (width * y), width);

	if (SDL_MUSTLOCK(ret)) SDL_UnlockSurface(ret);

	// Free redundant pixel data
	delete[] pixels;

	return ret;

}


SDL_Surface * createBlankSurface () {

	SDL_Surface *ret;
	unsigned char *pixels;

	pixels = new unsigned char[1];
	*pixels = 0;
	ret = createSurface(pixels, 1, 1);
	SDL_SetColorKey(ret, SDL_SRCCOLORKEY, 0);

	return ret;

}



