#!/bin/sh

/home/rjek/ns/genmail.lua /home/rjek/ns/genmail-state | w3m -dump > /home/rjek/ns/mail.txt

echo "The change mail has been updated.  Please update it by visiting\nhttp://netsurf.rjek.com/annotate.cgi before 12:00 on Tuesday should you wish\nto make any annotations." | mail -s "New change mail is awaiting annotation" -a "From: netsurf-dev@rjek.com" rjek@rjek.com

