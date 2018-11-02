/*
 * Log.h
 *
 *  Created on: Nov 2, 2018
 *      Author: jludwig
 */

#pragma once

#include <stdint.h>


#define	LOG__DATA_BLOCK_SIZE	4096


class Log {
public:
	static const uint32_t invalid_data_block;
	static const unsigned data_block_size;

	Log();
	virtual ~Log();

	int start_transaction();
	int write_block(const char *block_data, uint32_t *new_block);
	int invalidate_old_block(uint32_t old_block);
	void commit_transaction();
	void rollback_transaction();
};
