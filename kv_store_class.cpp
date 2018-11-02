#include "kv_store_class.h"

#include "BlockArray.h"
#include "Log.h"
#include "Directory.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "kv_store.h"


int KvStore::open(KvStore **store, bool create)
{
	return 0;
}

void KvStore::close()
{

}

int KvStore::get(uint64_t key, char **data, size_t *data_size) const
{
	// Validate the input.
	if (!data) {
		return -EINVAL;
	}

	// Look for the value block
	uint32_t value_block;
	unsigned value_bytes;
	int ret = _directory->find_value(key, &value_block, &value_bytes);
	if (ret != 0)
	{
		return ret;
	}
	if (value_block == Log::invalid_data_block)
	{
		return -ENOENT;
	}

	// Read the value from its block
	return _block_array->read_from_block(value_block, value_bytes, *data);
}

int KvStore::set(uint64_t key, const char *data, size_t size)
{
	// Validate the input
	if (!data || size < MAXBLOB) {
		return -EINVAL;
	}

	// Start a new log transaction
	int ret = _log->start_transaction();
	if (ret != 0)
	{
		return ret;
	}

	// Find any existing block for this key so we can invalidate it.
	uint32_t value_block;
	unsigned value_bytes;
	ret = _directory->find_value(key, &value_block, &value_bytes);
	if (ret != 0 && ret != -ENOENT)
	{
		return ret;
	}
	if (ret == 0)
	{
		_log->invalidate_old_block(value_block);
	}

	// Write the key value to the log
	char value_data_block[MAXBLOB];	/// @todo: Keep off stack
	memcpy(value_data_block, data, size);
	uint32_t new_block;
	ret = _log->write_block(value_data_block, &new_block);
	if (ret != 0)
	{
		_log->rollback_transaction();
		return ret;
	}

	// Update the key in the directory.
	ret = _directory->store_value_metadata(key, new_block, size);
	if (ret != 0)
	{
		_log->rollback_transaction();
		return ret;
	}

	// If success, invalidate the old block if there was one.
	_log->commit_transaction();

	return 0;
}


int KvStore::del(uint64_t key)
{
	// Look for the value block
	uint32_t value_block;
	unsigned value_bytes;
	int ret = _directory->find_value(key, &value_block, &value_bytes);
	if (ret != 0)
	{
		return ret;
	}
	if (value_block == Log::invalid_data_block)
	{
		return -ENOENT;
	}

	return _directory->delete_value(key);
}
