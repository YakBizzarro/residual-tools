/* Residual - A 3D game interpreter
 *
 * Residual is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * $URL: https://residual.svn.sourceforge.net/svnroot/residual/residual/trunk/tools/patchex/patchex.cpp $
 * $Id: patchex.cpp 1475 2009-06-18 14:12:27Z aquadran $
 *
 */

/* Patch extractor
 * (C) 2008 Andrea Corna
 * 
 * This source code is adopted and striped for Residual project.
 *
 * res_system functions are taken from system.c written by Stuart Caie
 * from libmspack (http://www.cabextract.org.uk/libmspack/).
 *
 * Patch Extractor is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License (LGPL) version 2.1
 *
 * For further details, see the file COPYING.LIB distributed with libmspack
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>

#include "tools/patchex/mspack.h"

// Some useful type and function
typedef unsigned char byte;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;

namespace Patchex {

// Command line actions
enum act { UNKNOWN_ACTION, CABINET_ACTION, LOCALISED_ACTION};

// Languages codes
#define LANG_ALL1 "@@"
#define LANG_ALL2 "Common"

const char *kLanguages_ext[] = { "English", "French", "German", "Italian", "Portuguese", "Spanish", NULL};
const char *kLanguages_code1[] = { "US", "FR", "GE", "IT", "PT", "SP",  NULL };
const char *kLanguages_code2[] = { "Eng", "Fra", "Deu", "Ita", "Brz", "Esp",  NULL };

// Extraction constans
#define RAND_A			(0x343FD)
#define RAND_B			(0x269EC3)
#define CODE_TABLE_SIZE		(0x100)
#define CONTAINER_MAGIC		"1CNT"
#define CABINET_MAGIC			"MSCF"


}

using namespace Patchex;
	
int main(int argc, char *argv[]) {
	unsigned int lang;
	enum act action;

	action = UNKNOWN_ACTION;

	// Argument checks and usage display
	if (argc != 3) {
		printf("Usage: patchex PATCH_EXECUTABLE LANGUAGE\n");
		printf("Extract update files of game update from PATCH_EXECUTABLE (e.g. gfupd101.exe) in a specified LANGUAGE.\n");
		printf("Please be sure that the update contains this language.\n");
		printf("Available languages:\n");
		for (int i = 0; kLanguages_code1[i]; i++)
			printf("- %s\n", kLanguages_ext[i]);
		printf("Alternately original archive could be extracted as original.cab with CABINET keyword insted of language.\n");
		exit(1);
	}

	// Actions check
	// Cabinet check
	if (strncasecmp("CABINET", argv[2], strlen(argv[2])) == 0) {
		printf("Cabinet extraction selected\n");
		action = CABINET_ACTION;
	}

	// Language check
	for(int i = 0; kLanguages_code1[i]; i++)
		if (strncasecmp(kLanguages_ext[i], argv[2], strlen(argv[2])) == 0) {
			printf("%s selected.\n", kLanguages_ext[i]);
			lang = i;
			action = LOCALISED_ACTION;
			break;
		}

	// Unknown action
	if (action == UNKNOWN_ACTION) {
		printf("Unknown language!\n");
		exit(1);
	}

	// Extraction !
	CabFile cabd(argv[1]);
	cabd.setLanguage(lang);
	if (action == CABINET_ACTION)
		cabd.extractCabinet();
	else if (action == LOCALISED_ACTION)
		cabd.extractFiles();
	cabd.close();
	
	return 0;
}
