/*
 * Directory.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: jludwig
 */

#include "Directory.h"

#include "Log.h"
#include "DirectoryEntryAllocator.h"
#include "DirectoryMap.h"

#include <errno.h>


struct DirectoryEntry
{
	uint64_t key_id;
	uint32_t value_bytes;
	uint32_t value_data_block;
};

#define DIR_ENTRIES_PER_BLOCK	(LOG__DATA_BLOCK_SIZE / sizeof(DirectoryEntry))

struct DirectoryEntryBlock
{
	struct DirectoryEntry entries[DIR_ENTRIES_PER_BLOCK];
};


Directory::Directory() {
	// TODO Auto-generated constructor stub

}

Directory::~Directory() {
	// TODO Auto-generated destructor stub
}

int Directory::find_value(uint64_t key, uint32_t *value_data_block, unsigned *value_bytes)
{
	unsigned block;
	unsigned entry;
	if (!lookup_value(key, &block, &entry))
	{
		*value_data_block = Log::invalid_data_block;
		*value_bytes = 0;
		return 0;
	}

	return -ENOENT;
}

int Directory::store_value_metadata(uint64_t key, uint32_t value_data_block, unsigned value_bytes)
{
	unsigned block;
	unsigned entry;
	// See if there is already an entry for this key
	if (!lookup_value(key, &block, &entry))
	{
		// There was no entry, so we need to allocate one.
		unsigned entry_index = _entry_allocator->allocate_directory_entry();
		if (entry_index == DIR_ENTRY__INVALID_INDEX)
		{
			return -ENOENT;
		}
		block = entry_index / DIR_ENTRIES_PER_BLOCK;
		entry = entry_index % DIR_ENTRIES_PER_BLOCK;
	}

	// Update/initialize the directory entry
	_dir_entry_blocks[block].entries[entry].key_id = key;
	_dir_entry_blocks[block].entries[entry].value_data_block = value_data_block;
	_dir_entry_blocks[block].entries[entry].value_bytes = value_bytes;

	// Re-write the directory block
	return write_directory_block(block);
}

int Directory::delete_value(uint64_t key)
{
	unsigned block;
	unsigned entry;
	// Look up the entry
	if (!lookup_value(key, &block, &entry))
	{
		// The entry does not exist.
		return -ENOENT;
	}

	// Update/initialize the directory entry
	_dir_entry_blocks[block].entries[entry].key_id = 0;
	_dir_entry_blocks[block].entries[entry].value_data_block = 0;
	_dir_entry_blocks[block].entries[entry].value_bytes = 0;

	// Re-write the directory block
	return write_directory_block(block);
}

bool Directory::lookup_value(uint64_t key, unsigned *block, unsigned *entry)
{
	for(unsigned b = 0; b < _number_of_directory_blocks; b++)
	{
		for(unsigned e = 0; e < DIR_ENTRIES_PER_BLOCK; e++)
		{
			if (_dir_entry_blocks[b].entries[e].key_id == key)
			{
				*block = b;
				*entry = e;
				return true;
			}
		}
	}

	return false;
}

int Directory::write_directory_block(unsigned block)
{
	uint32_t new_block;
	int ret = _log->write_block((const char *)&_dir_entry_blocks[block], &new_block);
	if (ret != 0)
	{
		return ret;
	}

	uint32_t old_block;
	ret = _directory_map->update_directory_block(block, new_block, &old_block);
	if (ret != 0)
	{
		return ret;
	}

	if (old_block != Log::invalid_data_block)
	{
		_log->invalidate_old_block(old_block);
	}

	return 0;
}
