#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <SerialStream.h>
#include <stdlib.h>
#include "data_reader.h"
#include "serial_reader.h"
#include "mod_bus_reader.h"
#include "mysql_writer.h"
#include "memcached_writer.h"
#include <exception>
#include <libconfig.h>


using namespace std;

int
main( int    argc,
      char* argv[] )
{
	const char *configPath = "";
	int readerType = 0;
	int dataSourceId = 0;
	int attemptsLimit;
	int attemptsCount = 0;
	config_t cfg;
	
	if (argc > 1) {
		if (strcmp(argv[1], "--config") == 0) {
			configPath = argv[2];
		}
	} else {
		printf("Usage:\n  %s --config /etc/cppscada/config", argv[0]);
		std::cout << std::endl;
		exit(1);
	}

	config_init(&cfg);	
	if(! config_read_file(&cfg, configPath)) {
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
		        config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return(EXIT_FAILURE);
	}
	
	if(!config_lookup_int(&cfg, "main.attempts_limit", &attemptsLimit)) {
		attemptsLimit = 100;
	}
	if(!config_lookup_int(&cfg, "main.id", &dataSourceId)) {
		throw "Data source ID is not defined";
	}
	if(!config_lookup_int(&cfg, "reader.type", &readerType)) {
		throw "Data reader type is not defined";
	}
	
	DataReader *r;
	if (readerType == 1) {
		r = new SerialReader(cfg);
	} else if (readerType == 2) { 
		r = new ModBusReader(cfg);
	} else {
		throw "Data reader type is not defined";
	}
	
	MysqlWriter *w = new MysqlWriter(cfg);
	MemcachedWriter *mw = new MemcachedWriter(cfg);

	try {
		r->connect();
	} catch(const char* s) {
		std::cerr << s
		          << std::endl;
		 return(EXIT_FAILURE);
	}

	std::cout << "Start read [" << dataSourceId << "]:"
	          << std::endl;

	double v;
	while(true) {
		try {
			v = r->read();
			w->saveNumericValue(v, dataSourceId);
			mw->saveNumericValue(v, dataSourceId);
		} catch(const char* s) {
			std::cerr << s
			          << std::endl;
			attemptsCount++;
		    sleep(2);
		}
		if(attemptsCount >= attemptsLimit) {
			std::cerr << "Overflow attemps limit. Try restart."
			          << std::endl;
			return(EXIT_FAILURE);
		}
		sleep(1);
	}

	config_destroy(&cfg);
	return EXIT_SUCCESS ;
}
