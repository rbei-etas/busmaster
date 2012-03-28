#!/usr/bin/perl -w

use strict;
use File::Glob qw(bsd_glob);

my $astyle_args = "-s4 -S"; # indent by four spaces, indent switches

# for astyle:
$ENV{'PATH'} .= ":/c/Program\ Files/AStyle/bin";
# for diff and diffstat:
$ENV{'PATH'} .= ":/c/MinGW/msys/1.0/bin";
#$ENV{'PATH'} .= ":/c/cygwin/bin";

my $infile;
my $outfile;

# print header
open($outfile, ">report.html");
print $outfile "<html>\n";
print $outfile "<head>\n";
print $outfile "<title>BUSMASTER Coding Style Report</title>\n";
print $outfile "</head>\n";
print $outfile "<body>\n";
print $outfile "<img src=\"../../Documents/Logo/BUSMASTER_Logo_311x64.png\"/>\n";
print $outfile "<h1>BUSMASTER coding style report</h1>\n";

# print astyle per file
my @dirs;
push(@dirs, bsd_glob("../../Sources/*/"));
push(@dirs, bsd_glob("../../Sources/Format Converter/*/"));
my $total_files = 0;
my $total_inserted = 0;
my $total_modified = 0;
my $total_deleted = 0;
foreach(sort(@dirs)) {
  my $dir = $_;
  my @files = bsd_glob("$dir"."*.{cpp,c,h}");
  next if @files == 0;

  print $outfile "<h2>$dir</h2>\n";
  print $outfile "<table border=\"1\">\n";
  print $outfile "<tr>\n";
  print $outfile "<th>Filename</th>\n";
  print $outfile "<th>Inserted</th>\n";
  print $outfile "<th>Deleted</th>\n";
  print $outfile "<th>Modified</th>\n";
  print $outfile "</tr>\n";

  foreach(sort(@files))
  {
    my $filename = $_;
    next if $filename eq "DIL_Datatypes.h"; # don't know why AStyle runs amok...
    unlink("new.txt");
    print "astyle $astyle_args \"$filename\"\n";
    print `astyle $astyle_args < \"$filename\" > new.txt`;
    my @diffstat = `diff -Naur "$filename" new.txt | diffstat -t`;
    unlink("new.txt");

    my $inserted = 0;
    my $deleted = 0;
    my $modified = 0;
    if (@diffstat > 1) {
      ($inserted, $deleted, $modified) = split(/,/, $diffstat[1]);
    }
    $total_files++;
    $total_inserted += $inserted;
    $total_modified += $modified;
    $total_deleted += $deleted;

    print $outfile "<tr>\n";
    print $outfile "<td>$filename</td>\n";
    if ($inserted == 0) {
      print $outfile "<td style='background-color:#77FF77'>$inserted</td>\n";
    } else {
      print $outfile "<td style='background-color:#FF7777'>$inserted</td>\n";
    }
    if ($modified == 0) {
      print $outfile "<td style='background-color:#77FF77'>$modified</td>\n";
    } else {
      print $outfile "<td style='background-color:#FF7777'>$modified</td>\n";
    }
    if ($deleted == 0) {
      print $outfile "<td style='background-color:#77FF77'>$deleted</td>\n";
    } else {
      print $outfile "<td style='background-color:#FF7777'>$deleted</td>\n";
    }
    print $outfile "</tr>\n";
  }

  print $outfile "<tr>\n";
  print $outfile "<td style='background-color:#CCCCCC'>$total_files</td>\n";
  print $outfile "<td style='background-color:#CCCCCC'>$total_inserted</td>\n";
  print $outfile "<td style='background-color:#CCCCCC'>$total_modified</td>\n";
  print $outfile "<td style='background-color:#CCCCCC'>$total_deleted</td>\n";
  print $outfile "</tr>\n";
  print $outfile "</table>\n";
}

# print footer
print $outfile "</body>\n";
print $outfile "</html>\n";
close($outfile);
