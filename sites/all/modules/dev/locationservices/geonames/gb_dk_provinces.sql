
-- country, province, description

-- Create DK provinces from geonames_adm1
DELETE from user_location_provinces WHERE country="DK";
INSERT INTO user_location_provinces SELECT * FROM geonames_adm1 WHERE country_code="DK";


-- Create GB provinces from geonames hierarchy.
DELETE from user_location_provinces WHERE country="GB";
INSERT INTO user_location_provinces SELECT "gb", CONCAT(g.admin1_code, '.', g.admin2_code), CONCAT(g.admin1_code, ':', g.name) FROM geonames g, geonames_hierarchy h
WHERE g.geonameid = h.childid AND h.parentid IN (SELECT childid FROM geonames_hierarchy WHERE parentid=2635167);

