/*
 * DirectoryMap.h
 *
 *  Created on: Nov 2, 2018
 *      Author: jludwig
 */

#pragma once

#include <stdint.h>

class Log;


class DirectoryMap {
public:
	DirectoryMap();
	virtual ~DirectoryMap();
	int update_directory_block(unsigned block_index, uint32_t new_block, uint32_t *old_block);

private:
	uint32_t *_directory_map;
	uint32_t _directory_map_block;
	Log *_log;
};
