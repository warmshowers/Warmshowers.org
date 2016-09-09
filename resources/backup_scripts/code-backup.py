#!/usr/bin/env python

import os
import argparse
from collections import namedtuple

def get_settings():
    # Default settings
    s3_bucket = 'warmshowers-files-backup'
    files_dir = '/var/www/warmshowers.org/docroot/files'
    dry_run  = False

    # Parse command-line arguments
    parser = argparse.ArgumentParser(description='Sync code backups from the last seven days.')
    parser.add_argument('--bucket', help='S3 database bucket. Default: ' + s3_bucket, default=s3_bucket)
    parser.add_argument('--path', help='Full path to the files directory. Default: ' + files_dir, default=files_dir)
    parser.add_argument('--dryrun', help="Perform a dry run. Default: %r" % dry_run, default=dry_run)
    args = parser.parse_args()

    if (args.dryrun != None and args.dryrun != dry_run):
        args.dryrun = True

    # Create backup settings tuple
    BackupSettings = namedtuple('BackupSettings', 's3_bucket files_dir dry_run')
    settings = BackupSettings(s3_bucket = args.bucket, files_dir = os.path.normpath(args.path), dry_run = args.dryrun)

    return settings;

# Initialize settings
settings = get_settings()


# Define helper functions
def sync_files():
    dry_run = '--dry-run' if (settings.dry_run) else ''
    awscmd_db_files = 'aws s3 sync %s %s s3://%s' % (dry_run, settings.files_dir, settings.s3_bucket);
    result = os.popen(awscmd_db_files).read()
    print result;

def run():
    # Sync files with S3
    sync_files()

run()
