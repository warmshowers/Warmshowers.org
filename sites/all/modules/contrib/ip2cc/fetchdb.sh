#!/bin/sh

OUTFILE=ipdb.mysql
URL=http://ip-to-country.webhosting.info/downloads/ip-to-country.csv.zip

#if [ ! -e ip-to-country.csv ] ; then
	wget $URL
	unzip ip-to-country.csv.zip
  chmod +r ip-to-country.csv
	rm ip-to-country.csv.zip
#fi
