#!/usr/bin/lua50 

-- NetSurf weekly changes mail generastor
-- Run weekly via cron, and dump stdout somewhere for somebody to change.
-- This mail should then be sent by another cronjob.
-- Rob Kendrick <rjek@rjek.com> 2006-06-01

local lastr = (io.open(arg[1] or error "Context file param missing", "r")
		or error "Unable to open context file"):read("*l")

local f = io.popen("svn log -r ".. lastr + 1 ..":HEAD svn://semichrome.net/trunk/netsurf 2>&1")
local n = 0
local c = false -- true when we're processing the log text rather than the
                -- change meta data
local ml = 0	-- current line of the log's message

print "<html><body>Notable changes in NetSurf this week<br>"
print "------------------------------------"
print "<p>Sorry, We've not had chance to annotate this week's changes.</p>"
print "Below are the changelog comments exported directly from Subversion."

l = f:read("*l")
if string.find(l, "^svn%: No such revision") then
  print "<p>There have been no changes since last week.</p>"
else
  print "<ol>"
  repeat
    if l == string.rep("-", 72) then
      -- start of new entry.
      n = n + 1
      if n > 1 then print "<br>" end
      c = false
    elseif l ~= nil then
      if not c then
        -- this is the log's meta data.  parse it into more managable chunks
        local null, null, revision, author, time, date, messagelines = 
          string.find(l, "^r([0-9]+) %| (%S+) %| (.+) %((.+)%) %| (%S+).*$")
        print(string.format("<br><li>Change %s by %s on %s:<br>",
          revision, author, date))
        c = true
        ml = 0
      else
        ml = ml + 1
        if ml ~= 1 then
          print(string.format("%s", l))
        end
      end
    end
    l = f:read("*l")
  until not l
  print "</ol>"
end
print [[<pre>
Please test the latest version<
    http://netsurf.sourceforge.net/builds/
   
Bugs should be reported to the bug tracker
    http://sourceforge.net/tracker/?group_id=51719&atid=464312</pre>]]

print "</body></html>"

-- update the revision context file

os.execute("svn info svn://semichrome.net/trunk/netsurf | grep ^Revision | sed -e's/Revision: //' > " .. arg[1])

