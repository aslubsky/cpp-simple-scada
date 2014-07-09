#! /bin/bash

now=$(date +%Y_%m)
tpl="CREATE TABLE IF NOT EXISTS \`archive_numeric_$now\` ("
tpl+="  \`date\` datetime NOT NULL,"
tpl+="  \`source_id\` smallint(10) unsigned NOT NULL,"
tpl+="  \`value\` float NOT NULL,"
tpl+="  PRIMARY KEY (\`date\`,\`source_id\`),"
tpl+="  KEY \`name\` (\`source_id\`)"
tpl+=") ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;"
echo $tpl