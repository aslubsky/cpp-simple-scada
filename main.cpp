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
	const char* configPath = "";
	if (argc > 1) {
		if (strcmp(argv[1], "--config") == 0) {
			configPath = argv[2];
		}
	} else {
		printf("Usage:\n  %s --config /etc/cppscada/config", argv[0]);
		std::cout << std::endl;
		exit(1);
	}

	config_t cfg;

	config_init(&cfg);

	
	if(! config_read_file(&cfg, configPath)) {
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
		        config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return(EXIT_FAILURE);
	}

	MysqlWriter *w = new MysqlWriter(cfg);
//	w->saveNumericValue(5.5, 1);

	SerialReader* r = new SerialReader(cfg);//"/dev/ttyUSB0"

	/*SerialReader* r = new SerialReader("/dev/ttyUSB0");
	try {
		r->connect();
	} catch(const char* s) {
		std::cerr << s
		          << std::endl;
		 return(EXIT_FAILURE);
	}

	std::cout << "Read:"
	          << std::endl;
	for(int i=0; i<100; i++) {
		try {
			std::cout << r->read()
			          << std::endl;
			std::cout.flush();
		} catch(const char* s) {
			std::cerr << s
			          << std::endl;
		}
		sleep(1);
	}*/

	config_destroy(&cfg);
	return EXIT_SUCCESS ;
}
