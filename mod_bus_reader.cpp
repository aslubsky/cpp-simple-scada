#include "mod_bus_reader.h"
#include <modbus.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

ModBusReader::ModBusReader(config_t cfg)
{
	if(!config_lookup_bool(&cfg, "main.debug", &this->debug)) {
		this->debug = false;
	}
	
	if(!config_lookup_string(&cfg, "reader.port", &this->_address)) {
		throw "Reader port is not defined";
	}
	if(!config_lookup_int(&cfg, "reader.baud_rate", &this->baud)) {
		baud = 115200;
	}
	if(!config_lookup_int(&cfg, "reader.slave_id", &this->slave_id)) {
		throw "Reader slave_id is not defined";
	}

	if(!config_lookup_int(&cfg, "reader.register_number", &this->register_number)) {
		throw "Reader register_number is not defined";
	}
}

ModBusReader::~ModBusReader()
{
	modbus_close(this->mb);
	modbus_free(this->mb);
}

void ModBusReader::connect()
{
	this->mb = modbus_new_rtu(this->_address, this->baud, 'N', 8, 2);
	if (this->mb == NULL) {
		throw "Unable to allocate libmodbus context";
	}

	//modbus_set_debug(this->mb, TRUE);
	modbus_set_error_recovery(this->mb,
	                         (modbus_error_recovery_mode)(MODBUS_ERROR_RECOVERY_LINK |
	                         MODBUS_ERROR_RECOVERY_PROTOCOL));

	modbus_set_slave(mb, this->slave_id);

	if (modbus_connect(mb) == -1) {
		modbus_free(mb);
		throw modbus_strerror(errno);
	}
}

double ModBusReader::read()
{
	int rc;
	uint16_t tab_reg[16];

	rc = modbus_read_registers(this->mb, this->register_number, 1, tab_reg);

	if (rc == -1) {
		throw modbus_strerror(errno);
	}

	if(this->debug == 1) {
		std::cout << this->_address << ": " << tab_reg[0] << std::endl;
	}
	return tab_reg[0];
}
