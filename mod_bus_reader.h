#ifndef MODBUSREADER_H
#define MODBUSREADER_H

#include <libconfig.h>
#include <modbus.h>
#include "data_reader.h"

class ModBusReader : public DataReader
{
private:
	modbus_t *mb;
	int debug;
	const char *_address;
	int baud;
	int slave_id;
	int register_number;
	
public:
	ModBusReader(config_t cfg);
	~ModBusReader();
	void connect();
	double read();
};

#endif // MODBUSREADER_H
