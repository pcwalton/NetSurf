/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2004 Richard Wilson <not_ginger_matt@users.sourceforge.net>
 */

#include <stdbool.h>
#include <stdio.h>
#include "nstheme/desktop/gui.h"
#include "nstheme/desktop/nstheme.h"
#include "nstheme/utils/utils.h"

bool application_quit = false;

static void application_init(void);
static void application_exit(void);


/**
 * NSTheme main().
 */
int main(int argc, char** argv) {
	application_init();
	while (!application_quit) gui_poll();
	application_exit();
	return EXIT_SUCCESS;
}


/**
 * Initialise application.
 */
void application_init(void) {
	stdout = stderr;
	gui_init();
}


/**
 * Finalise application.
 */
void application_exit(void) {
	gui_exit();
}
