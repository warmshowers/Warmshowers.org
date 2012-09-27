#!/bin/bash

echo "This script will give dummy emails to all but a few key accounts. It's intended for warmshowers.thefays.us. If you want to use it elsewhere, you'll need to change \$db."
db=warmshowers_thefays_us

echo "update users set mail=concat('user_', uid, '@localhost') where uid not in (1,1165, 12075, 8088, 36456, 18358);" | mysql $db 
