#!/usr/bin/perl
use Cwd;
use Carp;
use FileHandle;
use IPC::Open2;

if ( $#ARGV != 2) {
	print STDERR "Usage: $ARGV[-1] db_username db_passwd databasename";
	exit(1);
}

($user, $pwd, $db) = @ARGV;

if  (!dir_exists("infiles"))  { mkdir("infiles") || croak "Failed to make infiles $!"; }
if (!dir_exists("outfiles")) { mkdir("outfiles") || croak "Failed to make outfiles $!"; }

$baseurl = "http://download.geonames.org/export/dump";
$indir= "infiles";
$outdir="outfiles";

# Make sure the 'infiles' directory is loaded with recent downloads
#loadurl($baseurl,"admin1Codes.txt");
#loadurl($baseurl,"countryInfo.txt");

#if ( ! -f "$indir/allCountries.zip") {
#	loadurl($baseurl,"allCountries.zip");
#} else {
#	print STDERR "Using existing allCountries.zip, not downloading. If you want to download, delete the $indir/AllCountries.zip and delete $outdir/geonames.txt``\n";
#}


$result = system('egrep -v "^#|^iso" '.  "\"$indir/countryInfo.txt\"  >\"$outdir/geonames_countryinfo.txt\"");
croak "failed to grep countryinfo: $!" if ($result != 0);


processadm1();
makeallcountriessmaller();


truncate_tables("geonames", "geonames_adm1", "geonames_countryinfo");

# Do the insertions using mysqlimport

print "\nLoading geonames:\n";
system("mysqlimport -h localhost -u$user -p$pwd --default-character-set=utf8 --fields-terminated-by='\t' --lines-terminated-by='\n' --columns='geonameid, name, ansiname, alternames, latitude, longitude, feature_class, feature_code, country_code, cc2, admin1_code, admin2_code, admin3_code,admin4_code,population, elevation, gtopo30, timezone, modification_date' --local $db $outdir/geonames.txt") ;


print "\nLoading geonames_adm1:\n";
system("mysqlimport -h localhost -u$user -p$pwd --default-character-set=utf8 --fields-terminated-by='\t' --lines-terminated-by='\n' --columns='country_code,adm1_code,name' --local $db $outdir/geonames_adm1.txt") ;


#ISO	ISO3	ISO-Numeric	fips	Country	Capital	Area(in sq km)	Population	Continent	tld	CurrencyCode	CurrencyName	Phone	Postal Code Format	Postal Code Regex	Languages	geonameid	neighbours	EquivalentFipsCode
print "\nLoading geonames_countryinfo:\n";
system("mysqlimport  --debug --default-character-set=utf8 --fields-terminated-by='\t' --lines-terminated-by='\n' --columns='iso_alpha2,iso_alpha3,iso_numeric,fips_code,name,capital,area,population,continent,\@x,currency,\@x,\@x,\@x,\@x,languages,geonameid,\@x' --local $db $outdir/geonames_countryinfo.txt") ;


truncate_tables('cache');


sub truncate_tables() {
	foreach $table (@_) {
		print STDERR "Truncating $table\n";
		domysql("truncate $table");
	}
}
sub domysql {
	($sql) = @_;
	$mysqlcmd="mysql -u $user -p$pwd -e '$sql' $db |";
	print "\nTrying: $mysqlcmd\n";
	open(MYSQL, $mysqlcmd) || croak "Failed to open mysql: $1";
	print <MYSQL>;
	close MYSQL || croak "MYSQL failed: $!";
}


sub processadm1 {
	open(ADM1, "$indir/admin1Codes.txt") || croak "Failed to open file: $!";

	open(ADMOUTPUT, ">$outdir/geonames_adm1.txt") || croak "Failed to open file: $!";

	while (<ADM1>) {
		($bigcode, $name) = split(/\t/, $_, 2);
		($country_code, $province_code) = split(/\./, $bigcode);
		print ADMOUTPUT "$country_code\t$province_code\t$name";
	}
}

sub makeallcountriessmaller {
	if ( -f "$outdir/geonames.txt" ) {
		print STDERR "Skipping processing of allCountries.zip because the file outfiles/geonames.txt already exists. Don't want to waste your time. Delete that file if you want to process it.";
		return;
	}
	open(ALLCOUNTRIES, "unzip -p \"$indir/allCountries.zip\" |") || croak "Failed to open allcountries: $!";

	open(SMALLER, "> $outdir/geonames.txt") || croak "Failed to open smallercountries: $!";

	print STDERR "Processing allcountries (about 6,602,000 entries)";
	$counter=0;
	while(<ALLCOUNTRIES>) {
		@fields=split(/\t/);
		$feature_class=$fields[6];
		$feature_code=$fields[7];
		$population=$fields[14];
		$counter++;
		if ($counter%10000 == 0) { print STDERR "$counter "; }

		if ($feature_class eq "A" || $feature_code eq "CONT"
			|| ($feature_code =~ /^PPL/ && $population > 1000)) {
			print SMALLER;
		}
	}
}

sub loadurl {
	my ($baseurl, $file) = @_;
	$mydir=cwd();
	chdir($indir);
	unlink($file) if -f $file;
	print STDERR "Trying to load $baseurl/$file\n";
	my $result = system("wget $baseurl/$file");
	if ($result != 0) {
		croak "Failed to load $file: $!";
	}
	chdir($mydir);

}

sub dir_exists {
	my ($dir) = @_;
	return opendir(DIR, $dir) ;

}
