#!/usr/bin/perl
#
# NetSurf autobuilder.
#

use strict;
use warnings;

use File::Find;
use File::Spec;
use Getopt::Long;

$ENV{PATH} = '/usr/local/bin:/usr/bin:/bin';

open LOG, ">autobuild_try.log" or die "failed to open autobuild_try.log: $!\n";
$| = 1;

# Process command line options
my $release_build = 0;
my $version = "3.0 (Dev)";
GetOptions('release=s' => sub { my ($name, $value) = @_; 
	   			$release_build = 1; 
				$version = $value;
			      }
	  );

# A bunch of useful definitions, in case the website structure changes again
# Output directory path (relative to site root)
my $outputdir = $release_build ? "downloads/releases" : "downloads/development";
# Website host name
my $websitehost = "www.netsurf-browser.org";

# find where we are being run
my $root = (command("pwd"))[0];
chomp $root;

# update from repository
chdir "$root/netsurf";
my @update = command("svn update --non-interactive");
chdir $root;
my @revlines = grep / revision /, @update;
my ($revno) = ($revlines[0] =~ /(\d+)/);     # get revison for version later
@update = grep !/^At revision/, @update;

if (!$release_build) {
	# continue only if there were updates
	unless (scalar @update) {
		print LOG "no updates\n";
		exit;
	}
}

# determine build date time and version
my $date = (command("date -u '+%d %b %Y %H:%M'"))[0];
chomp $date;
my $full_version = "$version ($date)";
if (!$release_build) {
	$full_version .= " r$revno";
}
my $pkg_version = (command("date -u '+0.%Y.%m.%d.%H%M'"))[0];
chomp $pkg_version;

# this is a real run: make a real log
command('mv --verbose autobuild_try.log autobuild.log');

# update web documents
chdir "$root/netsurfweb";
command("svn update --non-interactive");
chdir $root;

if (!$release_build) {
	# create version.c -- keep major/minor in sync with $version
	save('netsurf/desktop/version.c',
		"const char * const netsurf_version = \"$full_version\";\n" .
		"const int netsurf_version_major = 3;\n" .
		"const int netsurf_version_minor = 0;\n");
}

# build libraries
chdir "$root/libsvgtiny";
command("svn update --non-interactive");
command("make TARGET=riscos install");

chdir "$root/libharu";
command("svn update --non-interactive");
command("make TARGET=riscos PREFIX=/home/riscos/env");

chdir "$root/libparserutils";
command("svn update --non-interactive");
command("make TARGET=riscos uninstall");
command("make TARGET=riscos install");

chdir "$root/libwapcaplet";
command("svn update --non-interactive");
command("make TARGET=riscos uninstall");
command("make TARGET=riscos install");

chdir "$root/hubbub";
command("svn update --non-interactive");
command("make TARGET=riscos uninstall");
command("make TARGET=riscos install");

chdir "$root/libcss";
command("svn update --non-interactive");
command("make TARGET=riscos uninstall");
command("make TARGET=riscos OPTCFLAGS=-O2 install");

chdir "$root/libnsbmp";
command("svn update --non-interactive");
command("make TARGET=riscos uninstall");
command("make TARGET=riscos install");

chdir "$root/libnsgif";
command("svn update --non-interactive");
command("make TARGET=riscos uninstall");
command("make TARGET=riscos install");

# build RISC OS version
chdir "$root/netsurf";
command("make TARGET=riscos clean");
command("make TARGET=riscos");
chdir $root;
command("rm --recursive --force --verbose riscos-zip/!NetSurf");
command("rsync --archive --verbose --exclude=.svn netsurf/!NetSurf riscos-zip/");
command("rm --force --verbose riscos-zip/ReadMe");
command("mv --verbose riscos-zip/!NetSurf/ReadMe riscos-zip/");

# copy HTML document, preprocessing as required
sub process_html {
	my ($source, $dest, $language) = @_;
	my $html = load($source);
	# Append "_xx" to all anchors which don't already encode the language
	$html =~ s{a href="([a-z\/]+[a-z]+)((#[a-zA-Z]+)?)"}
	          {a href="$1_$language$2"}g;
	# Rewrite anchors from (e.g) "index.en" to "index_en"
	$html =~ s{a href="([a-z\/]+[a-z]+)[.]([a-z][a-z])((#[a-zA-Z]+)?)"}
	          {a href="$1_$2$3"}g;
	# Rewrite image @src to from (e.g) "foo/index.en" to "foo/index_en"
	$html =~ s{src="([a-z\/]+[a-z]+)[.]([a-z][a-z])"}
	          {src="$1_$2"}g;
	# Rewrite "/netsurf.css" to "../netsurf"
	$html =~ s{"/netsurf.css"}
	          {"../netsurf"}g;
	# Rewrite all image links from "img.png" to "img_png"
	$html =~ s{"([a-z\/]+[a-z]+).png"}
	          {"$1_png"}g;
	# Rewrite "/netsurf.png" to "../netsurf_png"
	$html =~ s{"/netsurf.png"}
		  {"../netsurf_png"}g;
	# Append _png to all documentation image links
	$html =~ s{src="images/([a-z\/]+[a-z]+)"}
		  {src="images/$1_png"}g;
	# Rewrite all links to the document root to "../welcome/index_xx"
	$html =~ s{href="/"}
	          {href="../welcome/index_$language"}g;
	# Rewrite all local directory links to "../dir/index_xx"
	$html =~ s{href="/(contact|documentation|welcome)/((#[a-zA-Z]+)?)"}
		  {href="../$1/index_$language$2"}g;
	# Rewrite all local file links to "../dir/..."
	$html =~ s{href="/(contact|documentation|welcome)/(.+)"}
		  {href="../$1/$2"}g;
	# Rewrite all non-local links to be absolute
	$html =~ s{href="/(.+)"}
		  {href="http://$websitehost/$1"}g;
	# Rewrite navigation div to prevent its display
	$html =~ s{div class="navigation"}
		  {div class="navigation" style="display: none"}g;
	# Rewrite content div's class to use full page width
	$html =~ s{div class="content"}
		  {div class="onlycontent"}g;
	# Remove the search box
	$html =~ s{div class="searchbox"}
		  {div class="searchbox" style="display:none"}g;
	# Welcome page heading
	$html =~ s{(<h2 class="version">Welcome to NetSurf)(</h2>)}
		  {$1 $version$2}g;
	# Substitute the version into those documents that require it
	$html =~ s{VERSION}
	          {$full_version}g;
	save($dest, $html);
}

# Process CSS files
sub process_css
{
	my ($source, $dest) = @_;
	my $css = load($source);

	$css .= 'a[href ^="http://"], a[href ^="https://"], ' .
		'a[href ^="ftp://"] { ' .
		'background: url(online) center right no-repeat; ' .
		'padding-right: 13px; }';

	save($dest, $css);
}

# process each item found in the website tree
sub process_item {
	my $destroot = "riscos-zip/!NetSurf/Docs";

	# Get item name (relative to root of website tree)
	my ($item) = ($File::Find::name =~ /$root\/netsurfweb\/(.*)/);
	$item = "" if !defined($item);

	# Get current directory (relative to root of website tree)
	my ($dir) = ($File::Find::dir =~ /$root\/netsurfweb\/(.*)/);
	$dir = "" if !defined($dir);

	# Ignore item if it's part of an SVN metadata tree
	return 0 if $item =~ /.*[.]svn.*/;

	# Ignore item if it's not in the contact or documentation directories
	# or if it's not the website stylesheet or NetSurf logo image
	if ($item !~ /contact.*/ && $item !~ /documentation.*/ &&
			$item !~ /welcome.*/ && $item ne "netsurf.css" && 
			$item ne "netsurf.png") {
		return 0;
	}

	if (-d $File::Find::name) {
		# Directory -- mirror it in destination tree
		command("mkdir -m 755 -p $destroot/$item");
	} elsif (-f $File::Find::name) {
		# File -- process it appropriately
		my ($volume, $directories, $doc) = File::Spec->splitpath($item);

		die "Bad item $item" if $doc eq "";

		my $source = $directories eq "" 
				? "netsurfweb/$doc" 
				: "netsurfweb/$directories$doc";

		print LOG "$source ";

		if ($doc =~ /([a-z]+)[.]([a-z][a-z])$/) { 
			# HTML with language extension
			my $leaf = $1;
			my $language = $2;
			my $dest = "$destroot/$dir/${leaf}_$language,faf";
			print LOG "=> $dest (html)\n";
			process_html($source, $dest, $language);
		} elsif ($doc =~ /(.*)[.]css$/) {
			# CSS document
			my $dest = "$destroot/$dir/$1,f79";
			print LOG "=> $dest\n";
			process_css($source, $dest);
		} elsif ($doc =~ /(.*)[.]png$/) {
			# PNG image
			my $dest = "$destroot/$dir/$1_png,b60";
			print LOG "=> $dest\n";
			command("cp --archive --verbose $source $dest");
		} else {
			# anything else
			print LOG "(skipped)\n";
		}
	}
}

# clone website into Docs directory
chdir $root;
find({ wanted => \&process_item, no_chdir => 1, follow => 1}, 
		"$root/netsurfweb");

# Perform applicable processing upon about page
my @langs = map { s/.*_(.*),faf/$1/; $_ } glob 'riscos-zip/!NetSurf/Docs/about/index_*';
foreach my $lang (@langs) {
	print LOG "riscos-zip/!NetSurf/Docs/about/index_$lang,faf (html)\n";
	process_html("riscos-zip/!NetSurf/Docs/about/index_$lang,faf",
			"riscos-zip/!NetSurf/Docs/about/index_$lang,faf", 
			$lang);
}

# create zip for regular build
chdir "$root/riscos-zip";
command('/home/riscos/env/bin/zip -9vr, ../netsurf.zip *');
chdir $root;
command("mv --verbose netsurf.zip $outputdir/");

if (!$release_build) {
	# make RiscPkg package
my $control = <<END;
Package: NetSurf
Priority: Optional
Section: Web
Maintainer: NetSurf developers <netsurf-develop\@lists.sourceforge.net>
Version: $pkg_version
Depends: SharedUnixLibrary (>=1.0.7), Tinct (>=0.1.3), Iconv (>=0.1.0), RiscPkg (>=0.3.1.1)
Licence: Free
Standards-Version: 0.1.0
Description: Web browser
 NetSurf is an open-source web browser for RISC OS. Its aim is to bring
 the HTML 4 and CSS standards to the RISC OS platform.
 .
 This is a test version of NetSurf and may be unstable.
END
	save('netsurfpkg/RiscPkg/Control', $control);
	mkdir "$root/$outputdir/riscpkg";
	command("rm --verbose --force $outputdir/riscpkg/netsurf-*.zip");
	command('rm --recursive --verbose --force netsurfpkg/Apps/!NetSurf');
	command('mv --verbose riscos-zip/!NetSurf netsurfpkg/Apps/');
	chdir "$root/netsurfpkg";
	command("/home/riscos/env/bin/zip -9vr, " .
			"../$outputdir/riscpkg/netsurf-$pkg_version.zip " .
			'Apps/!NetSurf RiscPkg/Control RiscPkg/Copyright');
	chdir "$root/$outputdir/riscpkg";
	command("$root/packageindex.pl ".
			"http://$websitehost/$outputdir/riscpkg/ ".
			'> packages');
	chdir $root;
	command('mv --verbose netsurfpkg/Apps/!NetSurf ./riscos-zip/');
}

# TODO nstheme

# build source tarball
my $source_tree = (command("svn info netsurf | grep '^URL' | cut -c 6-"))[0];
chomp $source_tree;
command("rm --verbose --force $outputdir/netsurf-*.tar.gz");
command('rm --recursive --verbose --force export');
mkdir "$root/export";
chdir "$root/export";
command("svn export --non-interactive $source_tree netsurf");
command("tar czf netsurf-r$revno.tar.gz netsurf");
chdir $root;
command("mv --verbose export/netsurf-*.tar.gz $outputdir/");

# Create a fragment of an HTML page containing details of a download link
sub create_download_fragment {
	my ($dest, $title, $filename) = @_;
	my $size = sprintf "%.1fM", (-s "$outputdir/$filename") / 1048576;
	my $date = (command("date -u -r $outputdir/$filename '+%d %b %Y %H:%M'"))[0];
	my $html = "<li><a href=\"/$outputdir/$filename\">$title</a> <span>$size</span> <span>$date UTC</span>";

	save($dest, $html);	
}

if (!$release_build) {
	# create page fragments
	create_download_fragment("$outputdir/source.inc", 
			"SVN source code (r$revno)", "netsurf-r$revno.tar.gz");

	# get log of recent changes
	my $week_ago = (command("date --date='7 days ago' '+%F'"))[0];
	chomp $week_ago;
	command("svn log --verbose --revision '{$week_ago}:HEAD' --xml " .
		'svn://source.netsurf-browser.org/ > log.xml');
	command("xsltproc svnlog2html.xslt log.xml >$outputdir/svnlog.txt");

	# and a shorter log for the developers page
	command("svn log --verbose --revision 'HEAD:{$week_ago}' --xml " .
		"--limit 5 svn://source.netsurf-browser.org/ > log.xml");
	command("xsltproc svnlog2html.xslt log.xml >$outputdir/shortlog.txt");
}

if (!$release_build) {
	# Copy build log ready for upload
	command("cp --verbose autobuild.log $outputdir/netsurf.log");

	# rsync to website
	command("rsync --verbose --compress --times --recursive " .
		"$outputdir/*.zip $outputdir/netsurf.log " .
		"$outputdir/svnlog.txt $outputdir/shortlog.txt " .
		"$outputdir/riscpkg $outputdir/*.tar.gz $outputdir/*.inc " .
		"netsurf\@netsurf-browser.org:/home/netsurf/websites/" .
		"$websitehost/docroot/$outputdir/");
} else {
	# Rename binary zip file to encode version number
	command("mv --verbose $outputdir/netsurf.zip " .
		"$outputdir/netsurf-$version.zip");

	# Similarly for the source tarball
	command("mv --verbose $outputdir/netsurf-r$revno.tar.gz " .
		"$outputdir/netsurf-$version-src.tar.gz");

	# Stage ChangeLog
	command("cp --verbose riscos-zip/ChangeLog $outputdir/ChangeLog.txt");

	# rsync to website
	command("rsync --verbose --compress --times --recursive " .
		"$outputdir/*.zip $outputdir/*.tar.gz " .
		"$outputdir/ChangeLog.txt " .
		"netsurf\@netsurf-browser.org:/home/netsurf/websites/" .
		"$websitehost/docroot/$outputdir/");
}


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

