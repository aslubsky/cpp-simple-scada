#ifndef MYSQLWRITER_H
#define MYSQLWRITER_H

#include <stdlib.h>
#include <mysql.h>
#include <libconfig.h>

class MysqlWriter
{
private:
	MYSQL *conn;
	config_t cfg;
	char numeric_table_name[100];
	
public:
	MysqlWriter(config_t cfg);
	~MysqlWriter();
	void connect();
	void saveNumericValue(double value, int dataSourceId);
};

#endif // MYSQLWRITER_H
