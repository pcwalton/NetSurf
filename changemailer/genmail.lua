#!/usr/bin/lua50 

-- NetSurf weekly changes mail generastor
-- Run weekly via cron, and dump stdout somewhere for somebody to change.
-- This mail should then be sent by another cronjob.
-- Rob Kendrick <rjek@rjek.com> 2006-06-01


local f = io.popen "svn log svn://semichrome.net/trunk/netsurf"
--local f = io.open("/home/rjek/nslog", "r")
local n = 0
local c = false -- true when we're processing the log text rather than the
                -- change meta data
local ml = 0	-- current line of the log's message

print "<html><body>Notable changes in NetSurf this week<br>"
print "------------------------------------"
print "<p>Sorry, We've not had chance to annotate this week's changes.</p>"
print "Below are the changelog comments exported directly from Subversion."
print "<ol>"

repeat
  l = f:read("*l")
  if l == string.rep("-", 72) then
    -- start of new entry.
    n = n + 1
    if n > 1 then print "<br>" end
    c = false
  elseif l ~= nil then
    if not c then
      -- this is the log's meta data.  parse it into more managable chunks
      local null, null, revision, author, time, messagelines = 
        string.find(l, "^r([0-9]+) %| (%S+) %| (.+) %| (%S+).*$")
      print(string.format("<br><li>Change %s by %s:<br>",
        revision, author))
      c = true
      ml = 0
    else
      ml = ml + 1
      if ml ~= 1 then
        print(string.format("%s", l))
      end
    end
  end
until not l

print "</ol></body></html>"

