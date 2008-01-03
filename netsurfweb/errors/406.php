<?php
# Prevent a 406 being sent when there is no match for the user's
# Accept-Language available. Instead, send the English version,
# or the png, as appropriate.

# It would be better to use a file from the Alternates
# response-header, but apache_response_headers() is only available
# in PHP 4.3.0 or later.

$f = substr($_SERVER["REQUEST_URI"], 1);
if (substr($f, -1) == "/" or $f == "")
    $f .= "index";

if (is_file($f . ".en")) {
    $f .= ".en";
    header("HTTP/1.1 200 OK");
    header("Content-Location: " . $f);
    header("Last-Modified: " . gmdate("D, d M Y H:i:s ", filemtime($f)) . "GMT");
    header("Content-Length: " . filesize($f));
    header("Content-Language: en");
    readfile($f);
} elseif (is_file($f . ".png")) {
    $lang = substr($f, -2);
    $f .= ".png";
    header("HTTP/1.1 200 OK");
    header("Content-Location: " . $f);
    header("Last-Modified: " . gmdate("D, d M Y H:i:s ", filemtime($f)) . "GMT");
    header("Content-Length: " . filesize($f));
    header("Content-Language: " . $lang);
    header("Content-Type: image/png");
    readfile($f);
} else {
    header("HTTP/1.1 406 Not Acceptable");
    header("Content-Language: en");
    print <<<END
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
        "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head><title>Language Negotiation Failure</title></head>
<body>
<h1>Language Negotiation Failure</h1>
<p>A problem occurred when finding the requested file in your
language. Please email james@semichrome.net quoting the following
so that the problem can be rectified.</p>
<p>REQUEST_URI: {$_SERVER["REQUEST_URI"]}<br>
Accept-Language: {$_SERVER["HTTP_ACCEPT_LANGUAGE"]}</p>
</body></html>

END;
}

?>
