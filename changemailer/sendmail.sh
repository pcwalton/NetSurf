#!/bin/sh

cat /home/rjek/ns/mail.txt | mail -s "Summary of changes to `date -d "-2 days" +%F`" -s "From: netsurf-dev@rjek.com" rjek@rjek.com

echo "Sorry, too late - the mail has already been sent." > /home/rjek/ns/mail.txt

