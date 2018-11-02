/*
 * BlockArray.h
 *
 *  Created on: Nov 2, 2018
 *      Author: jludwig
 */

#pragma once

#include <stdint.h>

class BlockArray {
public:
	BlockArray();
	virtual ~BlockArray();

	int read_from_block(uint32_t block_number, unsigned bytes_to_read, char *buffer);
};
