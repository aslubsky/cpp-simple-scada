#! /bin/bash

rm -rf Build
mkdir Build

g++  -c  "./main.cpp" -g -O0 -Wall  -o ./Build/main.o -I. -I. -I/usr/local/include -I/usr/include/mysql -I/usr/include -I/usr/include/modbus
g++  -c  "./mysql_writer.cpp" -g -O0 -Wall  -o ./Build/mysql_writer.o -I. -I. -I/usr/local/include -I/usr/include/mysql -I/usr/include -I/usr/include/modbus
g++  -c  "./memcached_writer.cpp" -g -O0 -Wall  -o ./Build/memcached_writer.o -I. -I. -I/usr/local/include -I/usr/include/mysql -I/usr/include -I/usr/include/modbus
g++  -c  "./serial_reader.cpp" -g -O0 -Wall  -o ./Build/serial_reader.o -I. -I. -I/usr/local/include -I/usr/include/mysql -I/usr/include -I/usr/include/modbus
g++  -c  "./mod_bus_reader.cpp" -g -O0 -Wall  -o ./Build/mod_bus_reader.o -I. -I. -I/usr/local/include -I/usr/include/mysql -I/usr/include -I/usr/include/modbus
g++  -c  "./data_reader.cpp" -g -O0 -Wall  -o ./Build/data_reader.o -I. -I. -I/usr/local/include -I/usr/include/mysql -I/usr/include -I/usr/include/modbus
g++ -o ./Build/cpp_simple_scada ./Build/main.o ./Build/mysql_writer.o ./Build/memcached_writer.o ./Build/serial_reader.o ./Build/mod_bus_reader.o ./Build/data_reader.o -L. -L/usr/local/lib -L/usr/lib/mysql -L/usr/lib  -lserial -lmysqlclient -lconfig -lmodbus -lmemcached
