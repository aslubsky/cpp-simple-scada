#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <SerialStream.h>
#include <libconfig.h>

using namespace LibSerial;

class SerialReader
{
private:
	SerialStream _serial_port;
	bool _connected;
	const char *_address;

public:
	SerialReader(config_t cfg);
	~SerialReader();
	void connect();
	double read();
};

#endif // SERIALREADER_H
