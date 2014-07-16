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
	config_t cfg;
	
public:
	MemcachedWriter(config_t cfg);
	~MemcachedWriter();
	void connect();
	void saveNumericValue(double value, int dataSourceId);
};

#endif // MEMCACHEDWRITER_H
