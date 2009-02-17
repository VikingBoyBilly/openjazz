
/*
 *
 * scene.cpp
 *
 * Created as scene.c on the 23rd of August 2005
 * Renamed scene.cpp on the 3rd of February 2009
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
 * Deals with the loading, displaying and freeing of the cutscenes.
 *
 */


#include "OpenJazz.h"


Scene::Scene (char * fn) {

	File *f;
	char *string;
	int type;

	try {

		f = new File(fn, false);

	} catch (int e) {

		throw FAILURE;

	}

	// Skip to files
	f->seek(25, true);
	f->seek(f->loadChar(), true);

// At this point, next bytes should be 0x50 0x01 0x00 0x00 0x00
// Then, (0x3f 0x02)
//    OR (Nothing)
// Then, (0x2a
//        Then the length of the music file name
//        Then the bytes of the music file name)
//    OR (0x3f, then another byte)
//    OR (0x4c, not followed by any font stuff)
//    OR (0xa6, then 20 bytes?)
//    OR (Nothing)
// Then 0x58 0x01 0x00
// Then the length of a font file name
// Then a font file name
// Then 0x58 0x02 0x00
// Then the length of a font file name
// Then a font file name

	f->seek(5, false);
	type = f->loadChar();

	while (type == 0x3f) {

		f->seek(1, false);
		type = f->loadChar();

	}

	if (type != 0x4C) {

		if (type == 0x2A) {

			// Music file name
			string = f->loadString();
			playMusic(string);
			delete[] string;

		} else if (type == 0x63) {

			f->seek(1, false);

		} else if (type == 0xA6) {

			f->seek(20, false);

		} else f->seek(-1, false); // type should be 58

		while (f->loadChar() == 0x58) {

			// Font names (file names minus extensions)
			f->seek(2, false);
			string = f->loadString();

			// Do something with this

			delete[] string;

		}

	}

	f->seek(-1, false);

	while (f->loadChar() == 0x3f) {

		f->seek(1, false);

	}

	f->seek(-1, false);

// Then 0x4c 0x00 0x00 0x00 0x00 0x01 0x00
// Then, (0x46
//        Then a small number, e.g. 0x01, 0x02
//        Then 0x00 0x4a
//        Then (0x02 0x5d)
//          OR (0x01 0xdb)
//    OR (0x57 0x14...)

//	printf("Initial search reached %d\n", f->tell());

	// Skip to the palette
	f->seek(23, true);
	type = f->loadChar();

	f->seek(19, true);

	f->skipRLE();

	f->seek((type * 4) - 11, false);

	// Load the palette

//	printf("Palette at %d\n", f->tell());

	f->loadPalette(scenePalette);

	usePalette(scenePalette);

	f->seek(4, false);

//	printf("Pixels at %d\n", f->tell());

	sceneBGs[0] = f->loadSurface(320, 200);

	delete f;

	return;

}


Scene::~Scene () {

	// To do

	SDL_FreeSurface(sceneBGs[0]); // Temporary

	return;

}



int Scene::run () {

	SDL_Rect dst;

	while (1) {

		if (loop() == QUIT) return QUIT;

		if ((controls[C_ENTER].state == SDL_PRESSED) ||
		    (controls[C_ESCAPE].state == SDL_PRESSED)  ) {

			releaseControl(C_ENTER);
			releaseControl(C_ESCAPE);

			return SUCCESS;

		}

		// Temporary stuff
		SDL_FillRect(screen, NULL, BLACK);
		dst.x = (screenW - 320) >> 1;
		dst.y = (screenH - 200) >> 1;
		SDL_BlitSurface(sceneBGs[0], NULL, screen, &dst);
		SDL_Delay(20);

	}

	return SUCCESS;

}


