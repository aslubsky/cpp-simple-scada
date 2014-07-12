#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <exception>
#include <libconfig.h>
#include <mysql.h>
#include <vector>

using namespace std;

void finish_with_error(MYSQL *lcon, MYSQL *rcon)
{
	fprintf(stderr, "Error: %s\n", mysql_error(lcon));
	fprintf(stderr, "Error: %s\n", mysql_error(rcon));
	mysql_close(lcon);
	mysql_close(rcon);
	exit(1);
}

void sync_data(char numeric_table_name[100], char *ds_id, char *max_date, MYSQL *local_conn, MYSQL *remote_conn)
{
	ostringstream q;
	q << "SELECT * FROM " << numeric_table_name << " WHERE `date` > '" << max_date << "' AND source_id = " << ds_id << " LIMIT 200";
	std::string tmp1 = q.str();
	const char* tmp2 = tmp1.c_str();
	if (mysql_query(local_conn, tmp2)) {
		finish_with_error(local_conn, remote_conn);
	}

	MYSQL_RES *result = mysql_store_result(local_conn);
	if (result == NULL) {
		finish_with_error(local_conn, remote_conn);
	}

	ostringstream qIns;
	qIns << "INSERT IGNORE INTO " << numeric_table_name << " (`date`, `source_id`, `value`) VALUES";

	MYSQL_ROW row;
	int cnt = 0;
	while ((row = mysql_fetch_row(result))) {
		qIns << "('" << row[0] << "', '" << row[1] << "', '" << row[2] << "'),";
		cnt++;
	}
	//cout << qIns.str() << "\n\n";

	mysql_free_result(result);
	if(cnt == 0) {
		return;
	}

	tmp1 = qIns.str();
	tmp1 = tmp1.substr(0, tmp1.size() - 1);
	tmp2 = tmp1.c_str();

	if (mysql_query(remote_conn, tmp2)) {
		finish_with_error(local_conn, remote_conn);
	}
	cout << ds_id << " - " << max_date << " (" << cnt << ")" << "\n\n";
}



int
main( int    argc,
#include <mysql.h>
      char* argv[] )
{
	const char *configPath = "";
	config_t cfg;

	if (argc > 1) {
		if (strcmp(argv[1], "--config") == 0) {
			configPath = argv[2];
		}
	} else {
		printf("Usage:\n  %s --config /etc/cppscada/sync.config", argv[0]);
		std::cout << std::endl;
		exit(1);
	}

	config_init(&cfg);
	if(! config_read_file(&cfg, configPath)) {
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
		        config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return(EXIT_FAILURE);
	}

	char numeric_table_name[100];

	const char *local_server;
	const char *local_user;
	const char *local_password;
	const char *local_database;
	int local_port;
	MYSQL *local_conn;

	if(!config_lookup_string(&cfg, "local_db.host", &local_server)) {
		throw "Local MySQL host is not defined";
	}
	if(!config_lookup_string(&cfg, "local_db.username", &local_user)) {
		throw "Local MySQL username is not defined";
	}
	if(!config_lookup_string(&cfg, "local_db.password", &local_password)) {
		throw "Local MySQL password is not defined";
	}
	if(!config_lookup_string(&cfg, "local_db.database", &local_database)) {
		throw "Local MySQL database is not defined";
	}
	if(!config_lookup_int(&cfg, "local_db.port", &local_port)) {
		local_port = 3306;
	}
	
	local_conn = mysql_init(NULL);
	if (!mysql_real_connect(local_conn, local_server,
	                        local_user, local_password, local_database, local_port, NULL, 0)) {
		throw mysql_error(local_conn);
	}



	const char *remote_server;
	const char *remote_user;
	const char *remote_password;
	const char *remote_database;
	int remote_port;
	MYSQL *remote_conn;

	if(!config_lookup_string(&cfg, "remote_db.host", &remote_server)) {
		throw "Remote MySQL host is not defined";
	}
	if(!config_lookup_string(&cfg, "remote_db.username", &remote_user)) {
		throw "Remote MySQL username is not defined";
	}
	if(!config_lookup_string(&cfg, "remote_db.password", &remote_password)) {
		throw "Remote MySQL password is not defined";
	}
	if(!config_lookup_string(&cfg, "remote_db.database", &remote_database)) {
		throw "Remote MySQL database is not defined";
	}
	if(!config_lookup_int(&cfg, "remote_db.port", &remote_port)) {
		remote_port = 3306;
	}

	remote_conn = mysql_init(NULL);
	if (!mysql_real_connect(remote_conn, remote_server,
	                        remote_user, remote_password, remote_database, remote_port, NULL, 0)) {
		throw mysql_error(remote_conn);
	}




	time_t rawtime;
	struct tm *timeinfo;
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(numeric_table_name, sizeof(numeric_table_name), "archive_numeric_%Y_%m", timeinfo);

	ostringstream q;
	q << "SELECT source_id AS source_id, MAX(`date`) AS max_date FROM " << numeric_table_name << " GROUP BY source_id";
	//std::cout << q.str() << '\n';

	std::string tmp1 = q.str();
	const char* tmp2 = tmp1.c_str();
	if (mysql_query(remote_conn, tmp2)) {
		finish_with_error(local_conn, remote_conn);
	}


	MYSQL_RES *ds_result = mysql_store_result(remote_conn);
	if (ds_result == NULL) {
		finish_with_error(local_conn, remote_conn);
	}


	MYSQL_ROW row;
	while ((row = mysql_fetch_row(ds_result))) {
		//printf("%s - %s\n", row[0], row[1]);
		sync_data(numeric_table_name, row[0], row[1], local_conn, remote_conn);
	}

	mysql_free_result(ds_result);


	mysql_close(local_conn);
	mysql_close(remote_conn);

	config_destroy(&cfg);
	return EXIT_SUCCESS ;
}
