/*
 * This file is part of RUfl
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license
 * Copyright 2005 James Bursa <james@semichrome.net>
 */

#include "rufl_internal.h"


/**
 * Look up a character in the substitution table.
 *
 * \param  c  character to look up
 * \return  font number containing the character, or NOT_AVAILABLE
 */

unsigned int rufl_substitution_lookup(unsigned int c)
{
	unsigned int block = c >> 8;

	if (256 < block)
		return NOT_AVAILABLE;

	if (rufl_substitution_table->index[block] == BLOCK_NONE_AVAILABLE)
		return NOT_AVAILABLE;
	else {
		return rufl_substitution_table->block
				[rufl_substitution_table->index[block]]
				[c & 255];
	}
}
