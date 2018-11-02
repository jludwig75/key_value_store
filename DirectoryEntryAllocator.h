/*
 * DirectoryEntryAllocator.h
 *
 *  Created on: Nov 2, 2018
 *      Author: jludwig
 */

#pragma once

#include <stdint.h>


#define	DIR_ENTRY__INVALID_INDEX	UINT32_MAX


class DirectoryEntryAllocator {
public:
	DirectoryEntryAllocator();
	virtual ~DirectoryEntryAllocator();

	unsigned allocate_directory_entry();
	void mark_directory_entry_as_used(unsigned director_entry_number);
	void free_directory_entry(unsigned director_entry_number);
};
