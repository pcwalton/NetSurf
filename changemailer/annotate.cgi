#!/usr/bin/lua50

local fn = "/home/rjek/.changemail"
local qs = os.getenv "QUERY_STRING"

if string.find(qs, "data=", 1, 1) then
  -- update is in here!
  qs = string.gsub(qs, "^data=", "")
  qs = string.gsub(qs, "+", " ")
  qs = string.gsub(qs, "%%([0-9a-fA-F][0-9a-fA-F])", function(x) return string.char(tonumber(x, 16)) end)
  cm = io.open(fn, "w")
  cm:write(qs)
  cm:close()
  updated = "<h2>Update accepted.</h2>"
else
  updated = ""
end

local cm = io.open(fn, "r"):read("*a")

io.write "Content-Type: text/html\r\nStatus: 200 Ok\r\n\r\n"

print("<html><head><title>NetSurf Change Mail Annotator</title></head><body><h1>NetSurf Change Mail Annotator</h1>" .. updated)

io.write [[
The contents of the mail will be sent out to the mailing list on
Sunday at 23:59.
<p>
<form action="annotate.cgi" method="get">
  <textarea name="data" rows="24" cols="80">
]]
io.write(cm)

io.write [[</textarea><p>
  <input type="submit" value="Update">
</form>
 
]]

print "</body></html>"

