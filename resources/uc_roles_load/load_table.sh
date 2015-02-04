#!/bin/sh

DB="warmshowers"
DELIM=","

CSV="$1"
TABLE="$2"

[ "$CSV" = "" -o "$TABLE" = "" ] && echo "Syntax: $0 csvfile tablename" && exit 1

# FIELDS=$(head -1 "$CSV" | sed -e 's/'$DELIM'/` VARCHAR(255),\n`/g' -e 's/\r//g')
FIELDS='donation_id int, date date, amount int, email VARCHAR(256)'

# echo "$FIELDS" && exit

mysql $MYSQL_ARGS $DB -e "
DROP TABLE IF EXISTS $TABLE;
CREATE TABLE $TABLE ($FIELDS);

LOAD DATA INFILE '$(pwd)/$CSV' INTO TABLE $TABLE
FIELDS TERMINATED BY '$DELIM'
IGNORE 1 LINES
;
"
