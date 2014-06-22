#include <string>
#include "serial_reader.h"
#include <exception>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>


SerialReader::SerialReader(config_t cfg)
{
	this->_connected = false;
	if(!config_lookup_string(&cfg, "reader.port", &this->_address)) {
		throw "Reader port is not defined";
	}
	
	if(!config_lookup_bool(&cfg, "main.debug", &this->debug)) {
		this->debug = false;
	}
}

SerialReader::~SerialReader()
{
	this->_serial_port.Close();
}

void SerialReader::connect()
{
	this->_serial_port.Open(this->_address) ;
	if ( ! this->_serial_port.good() ) {
		throw "Error: Could not open serial port.";
	}

	this->_serial_port.SetBaudRate(SerialStreamBuf::BAUD_38400);
	if (!this->_serial_port.good()) {
		throw "Error: Could not set the baud rate.";
	}

	this->_serial_port.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
	if (!this->_serial_port.good()) {
		throw "Error: Could not set the character size.";
	}

	this->_serial_port.SetParity(SerialStreamBuf::PARITY_NONE);
	if (!this->_serial_port.good()) {
		throw "Error: Could not disable the parity.";
	}

	this->_serial_port.SetNumOfStopBits(1);
	if (!this->_serial_port.good()) {
		throw "Error: Could not set the number of stop bits.";
	}

	this->_serial_port.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_HARD);
	if (!this->_serial_port.good()) {
		throw "Error: Could not use hardware flow control.";
	}
	this->_connected = true;
}

double SerialReader::read()
{
	if(this->_connected == false) {
		this->connect();
	}
	char val[20];
	char next_byte;
	int b = 0;
	int limit = 1000;
	bool started = false;
	bool success = false;
	for(int i = 0; i < limit; i++) {
		this->_serial_port.get(next_byte);
		if(next_byte == '\r' && started == true) {
			success = true;
			break;
		}
		if(next_byte == '\r' && started == false) {
			started = true;
		}
		if(started == true) {
			if(next_byte == 'c') {
				throw "Board error: Check probe.";
			}
			val[b] = next_byte;
			b++;
		}
	}
	if(success == false) {
		//this->_connected = false;
		throw "Error: Unable to read board value.";
	}
	if(this->debug == 1) {
		std::cout << this->_address << ": " << atof(val) << std::endl;
	}
	return atof(val);
}
