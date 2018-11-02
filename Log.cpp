/*
 * Log.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: jludwig
 */

#include "Log.h"


const uint32_t Log::invalid_data_block = UINT32_MAX;

const unsigned Log::data_block_size = 4096;

Log::Log() {
	// TODO Auto-generated constructor stub

}

Log::~Log() {
	// TODO Auto-generated destructor stub
}

int Log::start_transaction()
{
	return 0;
}

int Log::write_block(const char *block_data, uint32_t *new_block)
{
	return 0;
}

int Log::invalidate_old_block(uint32_t old_block)
{
	return 0;
}

void Log::commit_transaction()
{

}

void Log::rollback_transaction()
{

}
