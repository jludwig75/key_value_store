/*
 * DirectoryMap.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: jludwig
 */

#include "DirectoryMap.h"

#include "Log.h"
#include "kv_store.h"

#include <errno.h>

DirectoryMap::DirectoryMap() {
	// TODO Auto-generated constructor stub

}

DirectoryMap::~DirectoryMap() {
	// TODO Auto-generated destructor stub
}

int DirectoryMap::update_directory_block(unsigned block_index, uint32_t new_block, uint32_t *old_block)
{
	if (block_index >= MAXKEYS)
	{
		return -EINVAL;
	}

	// Invalidate the old block if it was used.
	*old_block = _directory_map[block_index];

	// Update the map
	_directory_map[block_index] = new_block;

	// Invalidate the old map location
	_log->invalidate_old_block(_directory_map_block);

	// Write the map to a new location
	return _log->write_block((const char *)_directory_map, &_directory_map_block);
}
