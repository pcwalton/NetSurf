/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2004 Richard Wilson <not_ginger_matt@users.sourceforge.net>
 */

/** \file
 * RISC OS specific options.
 */

#ifndef _NETSURF_RISCOS_OPTIONS_H_
#define _NETSURF_RISCOS_OPTIONS_H_

#include "nstheme/desktop/options.h"

extern char *option_language;

#define EXTRA_OPTION_DEFINE \
char *option_language = 0;\

#define EXTRA_OPTION_TABLE \
{ "language",               OPTION_STRING,  &option_language }
#endif
