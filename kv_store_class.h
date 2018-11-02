#pragma once

#include <stdlib.h>
#include <stdint.h>


class BlockArray;
class Log;
class Directory;


class KvStore
{
public:
	static int open(KvStore **store, bool create);

	void close();

	int get(uint64_t key, char **data, size_t *data_size) const;

	int set(uint64_t key, const char *data, size_t size);

	int del(uint64_t key);

private:
	Log *_log;
	BlockArray *_block_array;
	Directory *_directory;
};
