#include "memcached_writer.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

using namespace std;

MemcachedWriter::MemcachedWriter(config_t cfg)
{
	const char *host;
	int port = 11211;

	if(!config_lookup_string(&cfg, "memcached.host", &host)) {
		host = "localhost";
	}
	if(!config_lookup_int(&cfg, "memcached.port", &port)) {
		port = 11211;
	}

	memcached_server_st *memcached_servers_parse (char *server_strings);
	memcached_return rc;

	this->memc = memcached_create(NULL);
	this->servers = memcached_server_list_append(this->servers, host, port, &rc);

	rc = memcached_server_push(memc, servers);
	if (rc != MEMCACHED_SUCCESS) {
		throw memcached_strerror(memc, rc);
	}
}

MemcachedWriter::~MemcachedWriter()
{
}

void MemcachedWriter::saveNumericValue(double value, int dataSourceId)
{
	memcached_return rc;

	char key[32];
	snprintf(key, sizeof(key), "%d", dataSourceId);
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%g", value);

	rc= memcached_set(this->memc, key, strlen(key), buffer, strlen(buffer), (time_t)0, (uint32_t)0);
	if (rc != MEMCACHED_SUCCESS) {
		throw "Key stored successfully";
	}
}
