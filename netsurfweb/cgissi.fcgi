#!/usr/bin/perl

# \file
# cgissi.fcgi -- A FastCGI script to emulate Server-Side Includes
#
# At the time of writing, it supports #include, #fsize and #flastmod
#

use warnings;
use strict;

use Digest::MD5 qw(md5_base64);
use File::Spec;
use FCGI;
use POSIX qw(strftime);

# Content cache to avoid regenerating pages unless it's absolutely necessary
#
# This hash maps between the full local path to the URL data 
# and a hash containing cache data. For example:
#
# { "/foo/bar/baz" => 
# 	{ last_modified => 0123456789,
# 	  data_digest => "base64(MD5($data))"
# 	  data_last_modified => 0123456789,
#	  data => "generated page content",
#	  dependencies => 
#	  	{ "/foo/bar/bat" => 
#	  		{ last_modified => 0123456789,
#	  		  commands => 
#				[
#				  { command => "include" | "fsize" | "flastmod",
#				    start_offset = 123,
#				    end_offset = 456
#				  },
#				  { command => "include" | "fsize" | "flastmod",
#				    start_offset = 678,
#				    end_offset = 901
#				  }
#				]
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
	# Get document root for this request
	my $docroot = $ENV{DOCUMENT_ROOT};

	# Get path of item for this request
	# Yes, this is hacky. It appears that REDIRECT_PATH contains
	# the destination document resulting from content negotiation.
	# As this is an absolute path, we strip the document root from it.
	my ($path) = ($ENV{REDIRECT_PATH} =~ m/$docroot(.*)/);

	# Find item in cache
	my $cachedata = $cache{$docroot . $path};

	# Determine if we need to (re)generate page
	validate_cache_entry($docroot, $path, $cachedata);

	# Refetch cache entry 
	# (as it may have been created by validate_cache_entry)
	$cachedata = $cache{$docroot . $path};

	# TODO It would be nice to send 304 responses where appropriate
	# and also to pay attention to conditional requests namely
	# If-(None-)Match and If-(Un)Modified-Since

	# Cache-related headers
	print "ETag: " . $$cachedata{data_digest} . "\r\n";
	print "Last-Modified: " . strftime("%a, %d %b %Y %H:%M:%S %Z", 
			gmtime($$cachedata{data_last_modified})) . "\r\n";

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
			push(@{$$cacheentry{dependencies}{$filepath}{commands}},
					{ command => $keyword, 
					  start_offset => $start_offset, 
					  end_offset => tell($template)
					});
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
				'"' . md5_base64($data) . '"';
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
	$$cacheentry{data_digest} = '"' . md5_base64($data) . '"';
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
				'"' . md5_base64("") . '"';
		$cache{$docroot . $path}{data_last_modified} = time;
		return;
	}
	
	# Get dependency hash
	my $deps = $cache{$docroot . $path}{dependencies};

	# Build command map for this page
	my %cmds;
	foreach my $dep (keys %{$deps}) {
		my $index = 0;
		foreach my $cmd (@{$$deps{$dep}{commands}}) {
			# The key here is the index into the commands array
			# prepended to the dependency URL. This ensures that
			# we have unique map entries for each command.
			$cmds{$index . $dep} = $cmd;
			$index++;
		}
	}

	# Sort command map by start_offset
	my @sorted = 
		sort { # Unpack dependency URLs and command array indices
		       my ($a_index, $a_dep) = ($a =~ m/([0-9]+)(.*)/);
		       my ($b_index, $b_dep) = ($b =~ m/([0-9]+)(.*)/);
		       # Now compare the start offsets
		       $$deps{$a_dep}{commands}[$a_index]{start_offset} <=> 
		       $$deps{$b_dep}{commands}[$b_index]{start_offset} 
		     } 
			keys %cmds;

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

	# Iterate through commands, loading intervening chunks of template
	foreach my $cmd (@sorted) {
		# Unpack dependency URL and command array index
		my ($index, $dep) = ($cmd =~ m/([0-9]+)(.*)/);
		my $vars = $$deps{$dep};
		my $length = 
			$$vars{commands}[$index]{start_offset} - $curpos - 1;

		# Update last_modified time of this dependency (at least one
		# must have changed for a partial regeneration to occur)
		$$vars{last_modified} = (stat($dep))[9];

		# Read template input from end of previous dependency 
		# to the start of this one
		read(TEMPLATE, $temp, $length);

		# Append it to our data buffer
		$data .= $temp;

		# Run this SSI command
		if ($$vars{commands}[$index]{command} eq "include") {
			do_include($dep, \$data);
		} elsif ($$vars{commands}[$index]{command} eq "fsize") {
			do_fsize($dep, \$data);
		} elsif ($$vars{commands}[$index]{command} eq "flastmod") {
			do_flastmod($dep, \$data);
		}

		# Skip over SSI command in template input
		$curpos = $$vars{commands}[$index]{end_offset};

		seek(TEMPLATE, $curpos, 0);
	}

	# Read any remaining template input
	read(TEMPLATE, $temp, $template_length - $curpos);

	$data .= $temp;

	close TEMPLATE;

	# Update document cache
	$cache{$docroot . $path}{data} = $data;
	$cache{$docroot . $path}{data_digest} = '"' . md5_base64($data) . '"';
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

