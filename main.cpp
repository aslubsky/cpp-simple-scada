#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <SerialStream.h>
#include <stdlib.h>
#include "serial_reader.h"
#include "mysql_writer.h"
#include <exception>
#include <libconfig.h>

using namespace std;

int
main( int    argc,
      char* argv[] )
{
	const char *configPath = "";
	int dataSourceId = 0;
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
	
	if(!config_lookup_int(&cfg, "id", &dataSourceId)) {
		throw "Data source ID is not defined";
	}
	
	MysqlWriter *w = new MysqlWriter(cfg);
	SerialReader* r = new SerialReader(cfg);

	try {
		r->connect();
	} catch(const char* s) {
		std::cerr << s
		          << std::endl;
		 return(EXIT_FAILURE);
	}

	std::cout << "Sart read [" << dataSourceId << "]:"
	          << std::endl;
			  
	while(true) {
		try {
			w->saveNumericValue(r->read(), dataSourceId);
		} catch(const char* s) {
			std::cerr << s
			          << std::endl;
		}
	}

	config_destroy(&cfg);
	return EXIT_SUCCESS ;
}
