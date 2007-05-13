#!/usr/bin/perl
#
# NetSurf autobuilder.
#

use strict;
use warnings;

$ENV{PATH} = '/usr/local/bin:/usr/bin:/bin';

open LOG, ">autobuild_try.log" or die "failed to open autobuild_try.log: $!\n";
$| = 1;


# find where we are being run
my $root = (command("pwd"))[0];
chomp $root;

# determine build date time
my $date = (command("date -u '+%d %b %Y %H:%M'"))[0];
chomp $date;
my $version = "Test Build ($date)";
my $pkg_version = (command("date -u '+0.%Y.%m.%d.%H%M'"))[0];
chomp $pkg_version;

# update from repository
chdir "$root/netsurf";
my @update = command("svn update --non-interactive");
chdir $root;
@update = grep !/^At revision/, @update;

# continue only if there were updates
unless (scalar @update) {
	print LOG "no updates\n";
	exit;
}

# this is a real run: make a real log
command('mv --verbose autobuild_try.log autobuild.log');

# update web documents
chdir "$root/netsurfweb";
command("svn update --non-interactive");
chdir $root;

# create version.c
save('netsurf/desktop/version.c',
		"const char * const netsurf_version = \"$version\";\n" .
		"const int netsurf_version_major = 0;\n" .
		"const int netsurf_version_minor = 0;\n");

# build RISC OS version
chdir "$root/netsurf";
command("make riscos riscos_small");
chdir $root;
command("rm --recursive --force --verbose riscos-zip/!NetSurf");
command("rsync --archive --verbose --exclude=.svn netsurf/!NetSurf riscos-zip/");

# copy docs, processing as required
sub process_html {
	my ($source, $dest, $language) = @_;
	my $html = load($source);
	$html =~ s{a href="([a-z]+)((#[a-zA-Z]+)?)"}
	          {a href="$1_$language$2"}g;
	$html =~ s{a href="([a-z]+)[.]([a-z][a-z])((#[a-zA-Z]+)?)"}
	          {a href="$1_$2$3"}g;
	$html =~ s{src="([a-z\/]+)[.]([a-z][a-z])"}
	          {src="$1_$2"}g;
	$html =~ s{"netsurf.css"}
	          {"netsurf"}g;
	$html =~ s{"([a-z]+).png"}
	          {"$1_png"}g;
	$html =~ s{href="/"}
	          {href="intro_$language"}g;
	$html =~ s{VERSION}
	          {$version}g;
	save($dest, $html);
}

chdir "$root/netsurfweb";
my @docs = glob '*';
chdir $root;
foreach my $doc (@docs) {
	my $source = "netsurfweb/$doc";
	print LOG "$source ";
	if ($doc =~ /([a-z]+)[.]([a-z][a-z])$/) { # html with language extension
		my $leaf = $1;
		my $language = $2;
		my $dest = "riscos-zip/!NetSurf/Docs/${leaf}_$language,faf";
		print LOG "=> $dest (html)\n";
		process_html($source, $dest, $language);
	} elsif ($doc =~ /(.*)[.]css$/) {
		my $dest = "riscos-zip/!NetSurf/Docs/$1,f79";
		print LOG "=> $dest\n";
		command("cp --archive --verbose $source $dest");
	} elsif ($doc =~ /(.*)[.]png$/) {
		my $dest = "riscos-zip/!NetSurf/Docs/$1_png,b60";
		print LOG "=> $dest\n";
		command("cp --archive --verbose $source $dest");
	} else {
		print LOG "(skipped)\n";
	}
}

print LOG "riscos-zip/!NetSurf/Docs/about,faf (html)\n";
process_html('riscos-zip/!NetSurf/Docs/about,faf',
	     'riscos-zip/!NetSurf/Docs/about,faf', 'en');

mkdir 'riscos-zip/!NetSurf/Docs/images', 0755;
foreach my $png (glob 'netsurfweb/images/*') {
	$png =~ /images\/(.*)[.]png$/;
	my $leaf = $1;
	$leaf =~ s/[.]/_/g;
	command("cp --archive --verbose $png riscos-zip/!NetSurf/Docs/images/$leaf,b60");
}

# create zip for regular build
my $slot_size = (command('./slotsize riscos-zip/!NetSurf/!RunImage,ff8'))[0];
my $run = load('netsurf/!NetSurf/!Run,feb');
$run =~ s/2240/$slot_size/g;
save('riscos-zip/!NetSurf/!Run,feb', $run);
chdir "$root/riscos-zip";
command('/home/riscos/cross/bin/zip -9vr, ../netsurf.zip *');
chdir $root;
command('mv --verbose netsurf.zip builds/');

# make RiscPkg package
my $control = <<END;
Package: NetSurf
Priority: Optional
Section: Web
Maintainer: NetSurf developers <netsurf-develop\@lists.sourceforge.net>
Version: $pkg_version
Depends: SharedUnixLibrary (>=1.0.7), Tinct (>=0.1.3), Iconv (>=0.0.8), RiscPkg (>=0.3.1.1)
Licence: Free
Standards-Version: 0.1.0
Description: Web browser
 NetSurf is an open-source web browser for RISC OS. Its aim is to bring
 the HTML 4 and CSS standards to the RISC OS platform.
 .
 This is a test version of NetSurf and may be unstable.
END
save('netsurfpkg/RiscPkg/Control', $control);
mkdir "$root/builds/riscpkg";
command('rm --verbose --force builds/riscpkg/netsurf-*.zip');
command('rm --recursive --verbose --force netsurfpkg/Apps/!NetSurf');
command('mv --verbose riscos-zip/!NetSurf netsurfpkg/Apps/');
chdir "$root/netsurfpkg";
command("/home/riscos/cross/bin/zip -9vr, " .
		"../builds/riscpkg/netsurf-$pkg_version.zip " .
		'Apps/!NetSurf RiscPkg/Control RiscPkg/Copyright');
chdir "$root/builds/riscpkg";
command("$root/packageindex.pl http://www.netsurf-browser.org/builds/riscpkg/ ".
		'> packages');
chdir $root;
command('mv --verbose netsurfpkg/Apps/!NetSurf ./riscos-zip/');

# create zip for small build
command('cp --archive --verbose netsurf/u!RunImage,ff8 riscos-zip/!NetSurf/!RunImage,ff8');
$slot_size = (command('./slotsize riscos-zip/!NetSurf/!RunImage,ff8'))[0];
$run = load('netsurf/!NetSurf/!Run,feb');
$run =~ s/2240/$slot_size/g;
save('riscos-zip/!NetSurf/!Run,feb', $run);
chdir "$root/riscos-zip";
command('/home/riscos/cross/bin/zip -9vr, ../unetsurf.zip *');
chdir $root;
command('mv --verbose unetsurf.zip builds/');

# TODO nstheme

# get log of recent changes
my $week_ago = (command("date --date='7 days ago' '+%F'"))[0];
chomp $week_ago;
command("svn log --verbose --revision '{$week_ago}:HEAD' --xml " .
		'svn://semichrome.net/ > log.xml');
my @log = command('xsltproc svnlog2html.xslt log.xml');

# create builds page
my $size_netsurf = sprintf "%.1fM", (-s 'builds/netsurf.zip') / 1048576;
my $size_unetsurf = sprintf "%.1fM", (-s 'builds/unetsurf.zip') / 1048576;
my $size_nstheme = sprintf "%.1fM", (-s 'builds/nstheme.zip') / 1048576;
my $format = "'+%d %b %Y %H:%M'";
my $date_nstheme = (command("date -u -r builds/nstheme.zip $format"))[0];
my @langs = map { s/.*[.]//; $_ } glob 'builds/top.*';
foreach my $lang (@langs) {
	print LOG "builds/index.$lang\n";
	my $page = load("builds/top.$lang");
	$page =~ s/SIZE_NETSURF/$size_netsurf/g;
	$page =~ s/DATE_NETSURF/$date UTC/g;
	$page =~ s/SIZE_UNETSURF/$size_unetsurf/g;
	$page =~ s/DATE_UNETSURF/$date UTC/g;
	$page =~ s/SIZE_NSTHEME/$size_nstheme/g;
	$page =~ s/DATE_NSTHEME/$date_nstheme UTC/g;
	$page .= join '', @log;
	$page .= load("builds/bottom.$lang");
	save("builds/index.$lang", $page);
}

command('cp --verbose autobuild.log builds/netsurf.log');

# rsync to website
command('rsync --verbose --compress --times --recursive ' .
		'builds/*.zip builds/index.* builds/netsurf.log ' .
		'builds/riscpkg ' .
		'netsurf@netsurf-browser.org:/home/netsurf/websites/' .
		'www.netsurf-browser.org/docroot/builds/');


sub command {
	my $cmd = shift;
	print LOG "> $cmd\n";
	my @output = `$cmd 2>&1`;
	foreach my $line (@output) {
		print LOG "| $line";
	}
	my $status = $? / 256;
	print LOG "exit status $status\n" if $status;
	print LOG "\n";
	die "$cmd:\nexit status $status\n" if $status;
	return @output;
}

sub load {
	my $path = shift;
	open FILE, $path or die "failed to open $path: $!\n";
	my $data;
	{
		local $/ = undef;
		$data = <FILE>;
	}
	close FILE;
	return $data;
}

sub save {
	my ($path, $data) = @_;
	open FILE, ">$path" or die "failed to open $path: $!\n";
	print FILE $data;
	close FILE;
}

