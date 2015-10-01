#!/bin/bash

db=wsupg
echo "This script will give dummy emails to all but a few key accounts. It's intended for the database $db. If you want to use it elsewhere, you'll need to change \$db."

echo "update users set mail=concat('user_', uid, '@localhost') where uid not in (1,1165, 12075, 8088, 36456, 18358, 36076) AND uid > 0;" | mysql $db
echo "update users set init=mail; " | mysql $db
