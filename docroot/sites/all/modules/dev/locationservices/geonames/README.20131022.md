22 October 2013 Notes

Today I did these things:

* Update geonames, geonames_adm1, and geonames_countryinfo table to current.
* Updated geonames.pl to do less and slightly better. You now have to download
yourself and put stuff into the infiles directory
* Updated the GB and DK province names to be somewhat better. GB counties are
still quite questionable. [My notes in evernote](https://www.evernote.com/shard/s15/sh/418111ff-b0df-46b4-99fa-48d921f2f07a/9dded591250a38c192c765e811479911)

I was tempted to rebuild everything so that user_location_countries and
user_location_provinces were directly tied to geonames, but got cold feet.
I suspected this would cause a lot of long-term trouble that I just wasn't
signing up for right now.

I did discover that geonames has alternatename table, which could be used for
a much better presentation. It takes forever to load. Best approach is
probably to load and then add indexes.

Nice project for loading geonames: https://github.com/codigofuerte/GeoNames-MySQL-DataImport
and my fork: https://github.com/rfay/GeoNames-MySQL-DataImport, which adds
the indexes.

I also discovered that BE, IE, and NZ probably have newish provinces also, but
since nobody has ever complained about these decided to leave well enough alone.

Sent out emails to DK and UK members asking them to update their account. Never forget to include username in subject when sending out one of these.

