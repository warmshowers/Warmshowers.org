This directory was created for loading old donor information from donortools in February 2015.

But it might be useful for a future event.

1. Get donation csv from donortools (use past 365 days of donations)
2. Remove columns in the spreadsheet to cut it down to just the donation_id, date, amount, email in Excel
3. Change the date field to custom yyyy-mm-dd
4. Save as csv
5. Convert to unix format lines with `perl -pi.bak -e 's/\r/\n/g' donations-2015-02-03.csv`
6. ./load_table.sh donations-2015-02-03.csv tmp_donations
7. NOTE THIS destroys existing roles in the category we're working with: 
    `cd docroot && drush scr ../resources/uc_roles_load/load_roles.php`


