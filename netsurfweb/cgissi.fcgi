#!/usr/bin/perl

# \file
# cgissi.fcgi -- A FastCGI script to emulate Server-Side Includes
#
# At the time of writing, it supports #include, #fsize and #flastmod
#

use warnings;
use strict;

use Digest::SHA;
use File::Spec;
use FCGI;
use POSIX qw(strftime);
use Time::Piece;

# Content cache to avoid regenerating pages unless it's absolutely necessary
#
# This hash maps between the full local path to the URL data 
# and a hash containing cache data. For example:
#
# { "/foo/bar/baz" => 
# 	{ last_modified => 0123456789,
# 	  data_digest => "base64(SHA($data))"
# 	  data_last_modified => 0123456789,
#	  data => "generated page content",
#	  dependencies => 
#	  	{ "/foo/bar/bat" => 
#	  		{ last_modified => 0123456789,
#			  command => "include" | "fsize" | "flastmod",
#			  start_offset = 123,
#			  end_offset = 456
#			}
#		}
#	} 
# }
#
my %cache;

# Request object
my $request = FCGI::Request();

# Loop until the server tells us to quit
while ($request->Accept() >= 0) {
	# Get cacheing-related headers
	my $ims = $ENV{HTTP_IF_MODIFIED_SINCE};
	my $ius = $ENV{HTTP_IF_UNMODIFIED_SINCE};
	my $im = $ENV{HTTP_IF_MATCH};
	my $inm = $ENV{HTTP_IF_NONE_MATCH};

	my $inm_pass = 1;
	my $request_failed = 0;

	# Get document root for this request
	my $docroot = $ENV{DOCUMENT_ROOT};

	# Get path of item for this request
	# Yes, this is hacky. It appears that REDIRECT_PATH contains
	# the destination document resulting from content negotiation.
	# As this is an absolute path, we strip the document root from it.
	my ($path) = ($ENV{REDIRECT_PATH} =~ m/$docroot(.*)/);

	# Find item in cache
	my $cachedata = $cache{$docroot . $path};

	# Ensure that the If-Match condition can succeed
	if (defined($im)) {
		if (!defined($cachedata)) {
			# Can't possibly match
			print "Status: 412 Precondition Failed\r\n";
			$request_failed = 1;
		}

		if ($im ne "*") {
			my @parts = split(/,/, $im);
			my $match = 0;

			foreach my $part (@parts) {
				my ($trimmed) = ($part =~ 
					m/[[:space:]]*(.*)[[:space:]]*/);
				if ($trimmed eq $cachedata{data_digest}) {
					$match = 1;
					last;
				}
			}

			if (!$match) {
				# No match
				print "Status: 412 Precondition Failed\r\n";
				$request_failed = 1;
			}
		}
	}

	# Ensure that the If-None-Match condition can succeed
	if (!$request_failed && defined($inm)) {
		if ($inm eq "*" && defined($cachedata)) {
			$inm_pass = 0;
		} elsif ($inm ne "*") {
			my @parts = split(/,/, $inm);

			foreach my $part (@parts) {
				my ($trimmed) = ($part =~ 
					m/[[:space:]]*(.*)[[:space:]]*/);
				if ($trimmed eq $cachedata{data_digest}) {
					$inm_pass = 0;
					last;
				}
			}
		}
	}

	if (!$request_failed) {
		# Determine if we need to (re)generate page
		validate_cache_entry($docroot, $path, $cachedata);

		# Refetch cache entry 
		# (as it may have been created by validate_cache_entry)
		$cachedata = $cache{$docroot . $path};
	}

	# If this was a modification-conditional request, ensure the document
	# hasn't been modified since the time given in the request.
	if (!$request_failed && defined($ius)) {
		my $now = gmtime;
		my $rmt = Time::Piece->strptime($ius,
				"%a, %d %b %Y $H:$M:$S $Z");

		if (defined($rmt) && $rmt <= $$cache_data{data_last_modified}) {
				print "Status: 412 Precondition Failed\r\n";
				$request_failed = 1;
		}
	}

	# If this was a modification-conditional request, ensure the document's
	# been modified since the time given in the request. If-Modified-Since
	# is ignored if If-None-Match passes.
	if (!$request_failed && defined($ims)) {
		if (!defined($inm) || !$inm_pass) {
			my $now = gmtime;
			my $rmt = Time::Piece->strptime($ims, 
					"%a, %d %b %Y %H:%M:%S $Z");

			if (defined($rmt) && $rmt <= $now &&
					$$cache_data{data_last_modified} <= 
						$rmt) {
				if (!$inm_pass) {
					$inm_pass = 1;
				} else {
					print "Status: 304 Not Modified\r\n";
					$request_failed = 1;
				}
			}
		}
	}

	# If-None-Match failed -- return appropriate result
	if (!$request_failed && !$inm_pass) {
		if ($ENV{REQUEST_METHOD} eq "GET" || 
				$ENV{REQUEST_METHOD} == "HEAD") {
			print "Status: 304 Not Modified\r\n";
		} else
			print "Status: 412 Precondition Failed \r\n";
		}

		$request_failed = 1;
	}
	
	# Cache-related headers
	print "ETag: " . $$cachedata{data_digest} . "\r\n";
	print "Last-Modified: " . strftime("%a, %d %b %Y %H:%M:%S %Z", 
			gmtime(time)) . "\r\n";

	# Exit if the request failed
	if ($request_failed) {
		print "\r\n";
		return;
	}

	# Send Content-Type header
	print "Content-Type: text/html; charset=ISO-8859-1\r\n";
	print "\r\n";

	# And the page data
	print $$cachedata{data};
}

################################################################################

# Run an include command
#
# \param filepath  The full path of the file to include
# \param data      Data buffer to append file data to (reference)
#
sub do_include
{
	my ($filepath, $data) = @_;

	if (-e $filepath) {
		# Unlike Zeus, we don't recursively apply SSI.
		open FILE, $filepath;
		{
			local $/ = undef;
			$$data .= <FILE>;
		}
		close FILE;
	}
}

# Run a fsize command
#
# \param filepath  The full path of the file to measure the size of
# \param data      Data buffer to append size to (reference)
#
sub do_fsize
{
	my ($filepath, $data) = @_;
	my @units = ( "B", "kB", "MB", "GB", "TB" ); # should be sufficient ;)

	if (-e $filepath) {
		my $size = -s $filepath;
		my $index = 0;

		while ($size > 1000) {
			$size /= 1024;
			$index++;
		}

		$$data .= sprintf("%.1f %s", $size, $units[$index]);
	}
}

# Run a flastmod command
#
# \param filepath  The full path of the file to get the modification time of
# \param data      Data buffer to append modification time to (reference)
#
sub do_flastmod
{
	my ($filepath, $data) = @_;

	if (-e $filepath) {
		my $mod_string = 
			strftime("%d %b %Y %R", gmtime((stat($filepath))[9]));

		$$data .= $mod_string . " UTC";
	}
}

# Process an include command
#
# \param command  The command string to process
# \param data     Data buffer to append output to (reference)
# \param docroot  The document root
# \param path     Path of the current document, relative to ::docroot
# \return The full path of the included file
#
sub process_include_command
{
	my ($command, $data, $docroot, $path) = @_;
	my $filepath;

	if ($command =~ /file=/) {
		my ($relpath) = ($command =~ m/file="(.*)"/);
		my ($vol, $dirs, $file) = 
				File::Spec->splitpath($docroot . $path);
		$filepath = File::Spec->rel2abs($relpath,
				File::Spec->catpath($vol, $dirs, ""));
	} elsif ($command =~ /virtual=/) {
		my ($vpath) = ($command =~ m/virtual="(.*)"/);
		if ($vpath =~ m#^/.*#) {
			$filepath = $docroot . $vpath;
		} else {
			my ($vol, $dirs, $file) = 
					File::Spec->splitpath($docroot . $path);
			$filepath = File::Spec->rel2abs($vpath,
					File::Spec->catpath($vol, $dirs, ""));
		}
	}

	do_include($filepath, $data);

	return $filepath;
}

# Process an fsize command
#
# \param command  The command string to process
# \param data     Data buffer to append output to (reference)
# \param docroot  The document root
# \param path     Path of the current document, relative to ::docroot
# \return The full path of the included file
#
sub process_fsize_command
{
	my ($command, $data, $docroot, $path) = @_;
	my $filepath;

	if ($command =~ /file=/) {
		my ($relpath) = ($command =~ m/file="(.*)"/);
		my ($vol, $dirs, $file) = 
				File::Spec->splitpath($docroot . $path);
		$filepath = File::Spec->rel2abs($relpath,
				File::Spec->catpath($vol, $dirs, ""));
	} elsif ($command =~ /virtual=/) {
		my ($vpath) = ($command =~ m/virtual="(.*)"/);
		if ($vpath =~ m#^/.*#) {
			$filepath = $docroot . $vpath;
		} else {
			my ($vol, $dirs, $file) = 
					File::Spec->splitpath($docroot . $path);
			$filepath = File::Spec->rel2abs($vpath,
					File::Spec->catpath($vol, $dirs, ""));
		}
	}

	do_fsize($filepath, $data);

	return $filepath;
}

# Process an flastmod command
#
# \param command  The command string to process
# \param data     Data buffer to append output to (reference)
# \param docroot  The document root
# \param path     Path of the current document, relative to ::docroot
# \return The full path of the included file
#
sub process_flastmod_command
{
	my ($command, $data, $docroot, $path) = @_;
	my $filepath;

	if ($command =~ /file=/) {
		my ($relpath) = ($command =~ m/file="(.*)"/);
		my ($vol, $dirs, $file) = 
				File::Spec->splitpath($docroot . $path);
		$filepath = File::Spec->rel2abs($relpath,
				File::Spec->catpath($vol, $dirs, ""));
	} elsif ($command =~ /virtual=/) {
		my ($vpath) = ($command =~ m/virtual="(.*)"/);
		if ($vpath =~ m#^/.*#) {
			$filepath = $docroot . $vpath;
		} else {
			my ($vol, $dirs, $file) = 
					File::Spec->splitpath($docroot . $path);
			$filepath = File::Spec->rel2abs($vpath,
					File::Spec->catpath($vol, $dirs, ""));
		}
	}

	do_flastmod($filepath, $data);

	return $filepath;
}

# Complete parsing of and process a command string
#
# \param template      Open file handle of template input
# \param char          Current input character
# \param command       The command string read so far
# \param data          Buffer to append data into (reference)
# \param docroot       The document root
# \param path          Path of the current document relative to ::docroot
# \param start_offset  Offset into template input of start of SSI command
# \param cacheentry    The cache entry for the current document
# 
sub process_command
{
	my ($template, $char, $command, $data, 
		$docroot, $path, $start_offset, $cacheentry) = @_;
	my $keyword = lc($command);
	my $dash_count = 0;
	my $filepath;

	# See state machine documentation in generate_page_full for details of
	# how this function works.

	# Reject command if it's one we know nothing about
	if ($keyword ne "include" && $keyword ne "fsize" &&
			$keyword ne "flastmod") {
		# ProcessCmd -> Initial transition occurs in generate_page_full
		$$data .= "<!--#" . $command . $char;
		return;
	}

	# Process the current input character
	if ($char eq '-') {
		# ProcessCmd -> ProcCmdDash
		$dash_count++;
	} else {
		# ProcessCmd -> ProcessCmd
		$command .= $char;
	}

	# Read rest of command string, then process command
	while ($char = getc($template)) {
		if ($char eq '-') {
			# ProcessCmd -> ProcCmdDash / ProcCmdDash -> ProcCmdCT
			$dash_count++;
		} elsif ($dash_count == 2 && $char eq '>') {
			# Run command (ProcCmdCT -> Initial transition occurs
			# in generate_page_full)
			if ($keyword eq "include") {
				$filepath = process_include_command($command, 
						$data, $docroot, $path);
			} elsif ($keyword eq "fsize") {
				$filepath = process_fsize_command($command, 
						$data, $docroot, $path);
			} elsif ($keyword eq "flastmod") {
				$filepath = process_flastmod_command($command, 
						$data, $docroot, $path);
			}

			$$cacheentry{dependencies}{$filepath}{last_modified} = 
					(stat($filepath))[9];
			$$cacheentry{dependencies}{$filepath}{command} = 
					$keyword;
			$$cacheentry{dependencies}{$filepath}{start_offset} = 
					$start_offset;
			$$cacheentry{dependencies}{$filepath}{end_offset} = 
					tell($template);
			return;
		} else {
			# ProcessCmd / ProcCmdDash / ProcCmdCT -> ProcessCmd
			while ($dash_count > 0) {
				$command .= "-";
				$dash_count--;
			}
			$command .= $char;
		}
	}
}

# Fully generate a page. 
# This involves parsing the page source looking for SSI commands.
#
# \param docroot  The document root
# \param path     Path of the current document relative to ::docroot
# 
sub generate_page_full
{
	my ($docroot, $path) = @_;
	my $data = "";
	my $in_tag = 0;
	my $had_pling = 0;
	my $dash_count = 0;
	my $in_comment = 0;
	my $had_hash = 0;
	my $command = "";
	my $start_offset = 0;

	if (! -e $docroot . $path) {
		# File doesn't exist
		$cache{$docroot . $path}{data} = $data;
		$cache{$docroot . $path}{data_digest} = 
				'"' . sha1_base64($data) . '"';
		$cache{$docroot . $path}{data_last_modified} = time;
		return;
	}

	open my $template, $docroot . $path;

	# Save document modification time into cache
	my $lastmod = (stat($docroot . $path))[9];
	$cache{$docroot . $path} = { last_modified => $lastmod }; 
	my $cacheentry = $cache{$docroot . $path};

	# Parse document template, looking for SSI comments
	#
	# This is basically a state machine, thus:
	#
	# State:	Input:		Next state:	Output:
	#
	# Initial	[^<]		Initial		Input character
	# Initial	'<'		InTag		None
	#
	# InTag		[^!]		Initial		'<' followed by input
	# InTag		!		InTagPling	None
	#
	# InTagPling	[^-]		Initial		'<!' followed by input
	# InTagPling	'-'		InTagPling1	None
	#
	# InTagPling1	[^-]		Initial		'<!-' followed by input
	# InTagPling1	'-'		InComment	None
	#
	# InComment	[^#]		Initial		'<!--' followed by input
	# InComment	'#'		InCommentHash	None
	#
	# InCommentHash	[^a-zA-Z]	ProcessCmd	None
	# InCommentHash	[a-zA-Z]	InCommentHash	None [1]
	#
	# ProcessCmd	n/a		Initial		'<!--#' followed by the 
	# 						command read in 
	# 						InCommentHash followed 
	# 						by input [2]
	# ProcessCmd	[^-]		ProcessCmd	None
	# ProcessCmd	'-'		ProcCmdDash	None
	#
	# ProcCmdDash	[^-]		ProcessCmd	None [3]
	# ProcCmdDash	'-'		ProcCmdCT	None
	#
	# ProcCmdCT	[^>]		ProcessCmd	None [4]
	# ProcCmdCT	'>'		Initial		Command specific [5]
	#
	# Notes:
	#
	# 1. This transition causes the input to be buffered as a command string
	# 2. This transition only happens if the command read so far is not
	#    recognised
	# 3. A dash is appended to the command string, however
	# 4. A '>' is appended to the command string, however
	# 5. The output is determined by running the command.
	#
	while (defined(my $char = getc($template))) {
		if (!$in_comment && !$in_tag && $char eq '<') {
			# Initial -> InTag
			$start_offset = tell($template);
			$in_tag = 1;
		} elsif ($in_tag) {
			if (!$had_pling) {
				if ($char ne '!') {
					# InTag -> Initial
					$data .= "<" . $char;
					$in_tag = 0;
				} elsif ($char eq '!') {
					# InTag -> InTagPling
					$had_pling = 1;
				}
			} else {
				if ($char ne '-') {
					# InTagPling(1) -> Initial
					$data .= "<!";
					while ($dash_count > 0) {
						$data .= "-";
						$dash_count--;
					}
					$data .= $char;

					$had_pling = 0;
					$in_tag = 0;
				} else {
					# InTagPling -> InTagPling1
					$dash_count++;

					if ($dash_count == 2) {
						# InTagPling1 -> InComment
						$dash_count = 0;
						$in_comment = 1;
						$had_pling = 0;
						$in_tag = 0;
					}
				}
			}
		} elsif ($in_comment) {
			if (!$had_hash) {
				if ($char ne '#') {
					# InComment -> Initial
					$data .= "<!--" . $char;
					$in_comment = 0;
				} else {
					# InComment -> InCommentHash
					$command = "";
					$had_hash = 1;
				}
			} else {
				my $c = lc($char);

				if ('a' le $c && $c le 'z') {
					# InCommentHash -> InCommentHash
					$command .= $char;
				} else {
					# InCommentHash -> ProcessCmd
					process_command($template, $char, 
						$command, \$data, $docroot,
						$path, $start_offset, 
						$cacheentry);

					# ProcessCmd/ProcCmdCT -> Initial
					$had_hash = 0;
					$in_comment = 0;
				}
			}
		} else {
			# Initial -> Initial
			$data .= $char;
		}
	}

	close $template;

	# Save generated page data in cache
	$$cacheentry{data} = $data;
	$$cacheentry{data_digest} = '"' . sha1_base64{$data} . '"';
	$$cacheentry{data_last_modified} = time;
}

# Partially generate a page.
# This involves using cached information to inject SSI data into a page
#
# \param docroot  The document root
# \param path     The path of the current document, relative to ::docroot
sub generate_page_partial
{
	my ($docroot, $path) = @_;

	if (! -e $docroot . $path) {
		# File doesn't exist
		$cache{$docroot . $path}{data} = "";
		$cache{$docroot . $path}{data_digest} = 
				'"' . sha1_base64("") . '"';
		$cache{$docroot . $path}{data_last_modified} = time;
		return;
	}
	
	# Get dependency hash
	my $deps = $cache{$docroot . $path}{dependencies};

	# Sort hash by start_offset
	my @sorted = 
		sort { $$deps{$a}{start_offset} <=> $$deps{$b}{start_offset} } 
			keys %$deps;

	my $data = "";
	my $temp;
	my $curpos = 0;
	my $template_length = 0;

	# Open requested document
	open TEMPLATE, $docroot . $path;

	# Find its length
	seek(TEMPLATE, 0, 2);
	$template_length = tell(TEMPLATE);
	seek(TEMPLATE, 0, 0);

	# Iterate through dependencies, loading intervening chunks of template
	foreach my $dep (@sorted) {
		my $vars = $$deps{$dep};
		my $length = $$vars{start_offset} - $curpos - 1;

		# Update last_modified time of this dependency (at least one
		# must have changed for a partial regeneration to occur)
		$$vars{last_modified} = (stat($dep))[9];

		# Read template input from end of previous dependency 
		# to the start of this one
		read(TEMPLATE, $temp, $length);

		# Append it to our data buffer
		$data .= $temp;

		# Run this SSI command
		if ($$vars{command} eq "include") {
			do_include($dep, \$data);
		} elsif ($$vars{command} eq "fsize") {
			do_fsize($dep, \$data);
		} elsif ($$vars{command} eq "flastmod") {
			do_flastmod($dep, \$data);
		}

		# Skip over SSI command in template input
		$curpos = $$vars{end_offset};

		seek(TEMPLATE, $curpos, 0);
	}

	# Read any remaining template input
	read(TEMPLATE, $temp, $template_length - $curpos);

	$data .= $temp;

	close TEMPLATE;

	# Update document cache
	$cache{$docroot . $path}{data} = $data;
	$cache{$docroot . $path}{data_digest} = '"' . sha1_base64($data) . '"';
	$cache{$docroot . $path}{data_last_modified} = time;
}

# Validate a cache entry
# 
# \param docroot  The document root
# \param path     The path of the current document, relative to ::docroot
# \param entry    Current cache entry or undef if none
#
sub validate_cache_entry
{
	my ($docroot, $path, $entry) = @_;

	if (!$entry) {
		# No entry -- generate one
		generate_page_full($docroot, $path);
		return;
	}

	if ((stat($docroot . $path))[9] > $$entry{last_modified}) {
		# Page modified -- fully regenerate page
		generate_page_full($docroot, $path);
		return;
	}

	while (my ($dep, $data) = each(%{$entry->{dependencies}})) {
		if ((stat($dep))[9] > $$data{last_modified}) {
			# Dependency changed -- partially regenerate page
			generate_page_partial($docroot, $path);
			return;
		}
	}

	# Otherwise, the cached data is valid
}

