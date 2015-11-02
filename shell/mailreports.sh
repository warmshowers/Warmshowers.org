#!/usr/bin/env bash

# Send admin reports to admins

if [ -f ./deploy_config.sh ] ; then
  . ./deploy_config.sh
fi

user_report_emails=usercreationreport@warmshowers.org
comment_report_emails=martinmemartin@warmshowers.org,mrtour@hotmail.com
forum_report_emails=martinmemartin@warmshowers.org,mrtour@hotmail.com
feedback_report_emails=feedbackreport@warmshowers.org

reportdate=yesterday

# export TESTING=1  # if you want test behavior
test_email=chris.andrews.russo@gmail.com
if [ $TESTING ] ; then
  user_report_emails=$test_email
  comment_report_emails=$test_email
  forum_report_emails=$test_email
  feedback_report_emails=$test_email
  reportdate="2015-09-15"
fi

datetool=date
machine_date=$($datetool --date="$reportdate" "+%Y%m%d")
friendly_date=$($datetool --date="$reportdate" "+%Y-%m-%d")

drush --root="${DEPLOY_TO}" --user=1 mav --view=admin_content_vbo --display=page_1 --to="$forum_report_emails" --subject="WS Forum Posts for $friendly_date" --args=$machine_date
drush --root="${DEPLOY_TO}" --user=1 mav --view=admin_comments_review --display=page --to="$comment_report_emails" --subject="WS Comments for $friendly_date" --args=$machine_date
drush --root="${DEPLOY_TO}" --user=1 mav --view=admin_user_vbo_2 --display=admin_new_user_report --to="$user_report_emails" --subject="WS New User Report for $friendly_date" --args=$machine_date
drush --root="${DEPLOY_TO}" --user=1 mav --view=member_feedback --display=page_1 --to="$feedback_report_emails" --subject="WS Feedback for $friendly_date" --args=$machine_date
