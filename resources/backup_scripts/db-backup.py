#!/usr/bin/env python

import os
import glob
import json
import re
import argparse
from datetime import date, timedelta
from collections import namedtuple

def get_settings():
    # Default settings
    s3_bucket = 'warmshowers-database-backup'
    backup_dir = '/var/backups/db_backups'
    dry_run  = False

    # Parse command-line arguments
    parser = argparse.ArgumentParser(description='Sync database dumps from the last seven days.')
    parser.add_argument('--bucket', help='S3 database bucket. Default: ' + s3_bucket, default=s3_bucket)
    parser.add_argument('--path', help='Full path to backup directory. Default: ' + backup_dir, default=backup_dir)
    parser.add_argument('--dryrun', help="Perform a dry run. Default: %r" % dry_run, default=dry_run)
    args = parser.parse_args()

    if (args.dryrun != None and args.dryrun != dry_run):
        args.dryrun = True

    # Create backup settings tuple
    BackupSettings = namedtuple('BackupSettings', 's3_bucket backup_dir dry_run')
    settings = BackupSettings(s3_bucket = args.bucket, backup_dir = os.path.normpath(args.path), dry_run = args.dryrun)

    return settings;

# Initialize settings
settings = get_settings()


# Define helper functions
def get_s3_dumps():
    awscmd_db_files = 'aws s3api list-objects --bucket ' + settings.s3_bucket + ' --query Contents[].Key --output json';
    result = os.popen(awscmd_db_files).read()
    dumps = json.loads(result);
    if (dumps == None):
        dumps = []

    return dumps;

def previous_days(start, total):
    start = date.today()
    days = []
    for i in range(total):
        day = start - timedelta(i)
        days.append(day.strftime('db_backup-%d-%m-%y.sql.gz'))

    return days;

def get_local_dumps():
    return glob.glob(settings.backup_dir + '/db_backup-*.sql.gz');

def last_seven_days():
    return previous_days(date.today(), 7);

def delete_remote_db(db):
    print "Deleting remote dump " + db + "..."
    awscmd_delete_file = 'aws s3 rm s3://' + settings.s3_bucket + '/' + db
    if (not settings.dry_run):
        print os.popen(awscmd_delete_file).read()

    print "Done.\n"
    
def delete_local_db(db):
    print "Deleting local dump " + db + "..."
    if (not settings.dry_run):
        os.remove(db)

    print "Done.\n"
    
def upload_db(filename):
    if not os.path.isfile(filename):
        print "Database does not exist: " + filename
        return;

    print "Uploading " + filename + "..."
    if (not settings.dry_run):
        awscmd_upload_file = 'aws s3 cp ' + filename + ' s3://' + settings.s3_bucket + '/' + os.path.basename(filename)
        print os.popen(awscmd_upload_file).read()

    print "Done.\n"

def is_old_dump(db, days):
    db = os.path.basename(db)
    match = re.match( r'^(db_backup-\d{2}-\d{2}-\d{2}).*\.sql\.gz$', db)
    if (not match):
        raise RuntimeError("Dump filename \"%s\" is not properly formatted." % db);

    dump = match.group(1) + '.sql.gz'
    if (dump in days):
        return False
    else:
        return True

def prune_dumps(days, db_files, delete_callback):
    count = 0
    if (db_files != None):
        for db in db_files:
            if is_old_dump(db, days):
                delete_callback(db)
                count += 1

    return count

def prune_remote_dumps(days):
    count = prune_dumps(days, get_s3_dumps(), delete_remote_db)
    print "Removed %d old dumps from S3" % count

def prune_local_dumps(days):
    count = prune_dumps(days, get_local_dumps(), delete_local_db);
    print "Removed %d old local dumps" % count

def upload_new_dumps(days):
    count = 0
    remote_dumps = get_s3_dumps()
    for filename in get_local_dumps():
        db = os.path.basename(filename) 
        if ((len(remote_dumps) == 0 or db not in remote_dumps) and not is_old_dump(db, days)):
            upload_db(filename)
            count += 1

    print "Uploaded %d dumps to S3" % count 

def run():
    # Get the xyz.sql.gz dump filename for the past seven days.
    # Format: db_backup-[day]-[month]-[year].sql.gz
    # eg. db_backup-25-08-16.sql.gz
    days = last_seven_days()
        
    # Delete old database files in S3
    prune_remote_dumps(days)

    # Delete old database files on this server
    prune_local_dumps(days)

    # Upload database to S3
    upload_new_dumps(days)

run()
