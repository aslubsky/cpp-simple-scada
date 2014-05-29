#ifndef MYSQLWRITER_H
#define MYSQLWRITER_H

#include <stdlib.h>
#include <mysql.h>
#include <libconfig.h>

class MysqlWriter
{
private:
	MYSQL *conn;
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	
public:
	MysqlWriter(config_t cfg);
	~MysqlWriter();
	void saveNumericValue(double value, int dataSourceId);
};

#endif // MYSQLWRITER_H
