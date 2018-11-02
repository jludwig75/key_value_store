/*
 * Directory.h
 *
 *  Created on: Nov 2, 2018
 *      Author: jludwig
 */

#pragma once

#include <stdint.h>

struct  DirectoryEntryBlock;
class DirectoryEntryAllocator;
class Log;
class DirectoryMap;

class Directory {
public:
	Directory();
	virtual ~Directory();

	int find_value(uint64_t key, uint32_t *value_data_block, unsigned *value_bytes);
	int store_value_metadata(uint64_t key, uint32_t value_data_block, unsigned value_bytes);
	int delete_value(uint64_t key);

private:
	bool lookup_value(uint64_t key, unsigned *block, unsigned *entry);
	int write_directory_block(unsigned block);
	unsigned _number_of_directory_blocks;
	DirectoryEntryBlock *_dir_entry_blocks;
	DirectoryEntryAllocator *_entry_allocator;
	Log *_log;
	DirectoryMap *_directory_map;
};
