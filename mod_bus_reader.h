#ifndef MODBUSREADER_H
#define MODBUSREADER_H

#include <libconfig.h>
#include <modbus.h>

class ModBusReader
{
private:
	modbus_t *mb;
	int register_number;
	
public:
	ModBusReader(config_t cfg);
	~ModBusReader();
	double read();
};

#endif // MODBUSREADER_H
