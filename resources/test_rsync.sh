#!/bin/sh
DRY_RUN=--dry-run

if test -z "$DRY_RUN" 
then
  date >>last_rsync.txt
  svn log -q -r BASE >>last_rsync.txt
fi
rsync  -av --exclude  ".svn" --exclude="files" --exclude="settings.php" --exclude=".htaccess" $DRY_RUN . ~/public_html/ 
