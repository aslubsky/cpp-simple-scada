#include "mod_bus_reader.h"
#include <modbus.h>
#include <errno.h>

ModBusReader::ModBusReader(config_t cfg)
{
	const char *address;
	int baud;
	int slave_id;

	if(!config_lookup_string(&cfg, "reader.port", &address)) {
		throw "Reader port is not defined";
	}
	if(!config_lookup_int(&cfg, "reader.baud_rate", &baud)) {
		baud = 115200;
	}
	if(!config_lookup_int(&cfg, "reader.slave_id", &slave_id)) {
		throw "Reader slave_id is not defined";
	}

	this->mb = modbus_new_rtu(address, baud, 'N', 8, 2);
	if (this->mb == NULL) {
		throw "Unable to allocate libmodbus context";
	}

	if(!config_lookup_int(&cfg, "reader.register_number", &this->register_number)) {
		throw "Reader register_number is not defined";
	}

	//modbus_set_debug(this->mb, TRUE);
	modbus_set_error_recovery(this->mb,
	                         (modbus_error_recovery_mode)(MODBUS_ERROR_RECOVERY_LINK |
	                         MODBUS_ERROR_RECOVERY_PROTOCOL));

	modbus_set_slave(mb, slave_id);

	if (modbus_connect(mb) == -1) {
		modbus_free(mb);
		throw modbus_strerror(errno);
	}
}

ModBusReader::~ModBusReader()
{
	modbus_close(this->mb);
	modbus_free(this->mb);
}

double ModBusReader::read()
{
	int rc;
	uint16_t tab_reg[16];

	rc = modbus_read_registers(this->mb, this->register_number, 1, tab_reg);

	if (rc == -1) {
		throw modbus_strerror(errno);
	}

	//tab_reg[0]
	return 0;
}
