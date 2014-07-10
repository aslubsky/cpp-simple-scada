#ifndef MEMCACHEDWRITER_H
#define MEMCACHEDWRITER_H

#include <stdlib.h>
#include <libconfig.h>
#include <libmemcached/memcached.h>

class MemcachedWriter
{
private:
	memcached_server_st *servers;
	memcached_st *memc;
	
public:
	MemcachedWriter(config_t cfg);
	~MemcachedWriter();
	void saveNumericValue(double value, int dataSourceId);
};

#endif // MEMCACHEDWRITER_H
