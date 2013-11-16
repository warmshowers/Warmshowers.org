GEONAMES UPDATE

This area is to update our information from geonames. The update must
be done manually.

To do it:

perl geonames.pl


If infiles/AllCountries.zip exists, it doesn't get downloaded again.
So if you want it to be downloaded, delete it.

Note that if you have changed anything directly in the database 
(like user_location_provinces or user_location_countries) 
you MUST re-update the two text files there.

mysqldump -U <user> -P<password> --default-character-set=utf8 -T <path_to_dump> roger_wsdrupal user_location_countries

mysqldump -U <user> -P<password> --default-character-set=utf8 roger_wsdrupal user_location_provinces >user_location_provinces.sql

After doing these you have to copy in the user_location_countries.txt file.
