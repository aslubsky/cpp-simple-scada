#include "mysql_writer.h"
#include <mysql.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream> // for ostringstream
#include <stdio.h>

MysqlWriter::MysqlWriter(config_t cfg)
{
	//const char *server, const char *user, const char *password, const char *database
	const char *server;
	const char *user;
	const char *password;
	const char *database;
	
	if(!config_lookup_string(&cfg, "db.host", &server)) {
		throw "MySQL host is not defined";
	}
	if(!config_lookup_string(&cfg, "db.username", &user)) {
		throw "MySQL username is not defined";
	}
	if(!config_lookup_string(&cfg, "db.password", &password)) {
		throw "MySQL password is not defined";
	}
	if(!config_lookup_string(&cfg, "db.database", &database)) {
		throw "MySQL database is not defined";
	}

	this->conn = mysql_init(NULL);
	if (!mysql_real_connect(this->conn, server,
	                        user, password, database, 0, NULL, 0)) {
		throw mysql_error(conn);
	}
	
	time_t rawtime;
	struct tm *timeinfo;
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(this->numeric_table_name, sizeof(this->numeric_table_name), "archive_numeric_%Y_%m", timeinfo);
	
}

MysqlWriter::~MysqlWriter()
{
	mysql_close(this->conn);
}

void MysqlWriter::saveNumericValue(double value, int dataSourceId)
{
	std::ostringstream q;  
	q << "INSERT INTO " << this->numeric_table_name << " (date, source_id, value) VALUES(NOW(), " << dataSourceId << ", " << value << ")";
	//std::cout << q.str() << '\n';
	std::string tmp1 = q.str();
	const char* tmp2 = tmp1.c_str();
  
	if (mysql_query(this->conn, tmp2)) {
		throw mysql_error(conn);
	}
}