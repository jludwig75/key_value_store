#include "kv_store.h"
#include "kv_store_class.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>


struct kvstor
{
	KvStore *store_instance;
};


int
kv_open(struct kvstor **storpp, bool create, int argc, char **argv)
{
	if (!storpp) {
		return -EINVAL;
	}

	*storpp = NULL;
	struct kvstor *store = new kvstor;
	int ret = KvStore::open(&store->store_instance, create);
	if (ret != 0)
	{
		return ret;
	}

	*storpp = store;
	return 0;
}

void
kv_close(struct kvstor *stor)
{
	stor->store_instance->close();
}

int
kv_get(struct kvstor *stor, const struct key *k, struct value *v)
{
	char *value_data;
	size_t value_bytes;
	int ret = stor->store_instance->get(k->id, &value_data, &value_bytes);
	if (ret != 0)
	{
		return ret;
	}

	v->size = value_bytes;
	memcpy(v->data, value_data, value_bytes);
	delete [] value_data;
	return 0;
}

int
kv_set(struct kvstor *stor, const struct key *k, const struct value *v)
{
	return stor->store_instance->set(k->id, v->data, v->size);
}
