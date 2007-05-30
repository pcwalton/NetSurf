/*
 * This file is part of NetSurf, http://netsurf-browser.org/
 * Licensed under the GNU General Public License,
 *		  http://www.opensource.org/licenses/gpl-license
 * Copyright 2006 Richard Wilson <info@tinct.net>
 */

/** \file
 * Knockout rendering (interface).
 */

#include "desktop/plotters.h"


bool knockout_plot_start(struct plotter_table *plotter);
bool knockout_plot_end(void);


extern const struct plotter_table knockout_plotters;
