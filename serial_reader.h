#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <SerialStream.h>
#include <libconfig.h>
#include "data_reader.h"

using namespace LibSerial;

class SerialReader : public DataReader
{
private:
	SerialStream _serial_port;
	int debug;
	bool _connected;
	const char *_address;

public:
	SerialReader(config_t cfg);
	~SerialReader();
	void connect();
	double read();
};

#endif // SERIALREADER_H
