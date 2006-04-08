<?php
$file = $_SERVER['PATH_INFO'];
if (!preg_match('/^\/[a-z]+\.zip$/', $file)) {
	header('Location: http://netsurf.sourceforge.net/builds/');
} else {
	$file = substr($file, 1);
	header("Location: http://netsurf.sourceforge.net/builds/$file");
	error_log(gmdate('Ymd.His') . " $file\n", 3, 'dl.log');
}
?>
