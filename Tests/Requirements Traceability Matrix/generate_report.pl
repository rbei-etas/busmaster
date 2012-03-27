#!/usr/bin/perl -w

use strict;
use File::Glob qw(bsd_glob);

my $infile;
my $outfile;

my %mods;
#$mods{'01'} = "Error Logger";
#$mods{'02'} = "License Validating Module";
#$mods{'03'} = "Utility library";
#$mods{'04'} = "DataType library";
$mods{'05'} = "Simulation Engine";
$mods{'06'} = "Project Configuration";
#$mods{'07'} = "DIL_FlexRay";
#$mods{'08'} = "Main UI (Control unit)";
$mods{'09'} = "ConfigDialogsDIL";
#$mods{'10'} = "Fibex Editor";
#$mods{'11'} = "Resilience (aspect)";
$mods{'12'} = "Frame Logger";
$mods{'13'} = "Node Simulation";
$mods{'14'} = "DIL_CAN";
#$mods{'15'} = "DIL_MCNet";
$mods{'16'} = "Message Window";
$mods{'17'} = "Frame Transmission Window";
$mods{'18'} = "Signal Watch Window";
$mods{'19'} = "Session Replay Window";
$mods{'20'} = "Filter";
$mods{'21'} = "Common Class";
$mods{'22'} = "Signal Graph Window";
$mods{'23'} = "DIL_Interface";
$mods{'24'} = "Bus Statistics Window";
$mods{'25'} = "Installer";
$mods{'26'} = "DIL_J1939";
$mods{'27'} = "Test Automation";

my %req_tags; # all existing tags

my %req_tags_2; # html links to the locations where tag is found
sub get_from_requirement_specification()
{
  foreach(bsd_glob("../../Documents/2 Requirement Specification/out/topics/*.html"))
  {
    my $filename = $_;
    open($infile, "<$filename");
    while(<$infile>) {
      foreach(/(RS[I]*_[0-9]+_[0-9]+)/g) {
	my $tag = $_;
        $req_tags{$tag}++;
	$req_tags_2{$tag} .= "<a href=\"$filename\">*</a>";
      }
    }
    close($infile);
  }
}

my %req_tags_3; # html links to the locations where tag is found
sub get_from_design_document()
{
  foreach(bsd_glob("../../Documents/3 Design Document/out/topics/*.html"))
  {
    my $filename = $_;
    open($infile, "<$filename");
    while(<$infile>) {
      foreach(/(RS[I]*_[0-9]+_[0-9]+)/g) {
	my $tag = $_;
        $req_tags{$tag}++;
	$req_tags_3{$tag} .= "<a href=\"$filename\">*</a>";
      }
    }
    close($infile);
  }
}

my %req_tags_5;
sub get_from_source_code_documentation()
{
  foreach(bsd_glob("../../Documents/5 Source Code Documentation/out/*/html/req.html"))
  {
    my $filename = $_;
    open($infile, "<$filename");
    while(<$infile>) {
      foreach(/(RS[I]*_[0-9]+_[0-9]+)/g) {
	my $tag = $_;
        $req_tags{$tag}++;
	$req_tags_5{$tag} .= "<a href=\"$filename\">*</a>";
      }
    }
    close($infile);
  }
}

sub req_sort()
{
  my ($a_rsx, $a_mod, $a_idx) = split(/_/, $a);
  my ($b_rsx, $b_mod, $b_idx) = split(/_/, $b);

  return  1 if ($a_mod > $b_mod);
  return -1 if ($a_mod < $b_mod);
  return  1 if ($a_rsx eq "RSI") and ($b_rsx eq "RS" );
  return -1 if ($a_rsx eq "RS" ) and ($b_rsx eq "RSI");
  return  1 if ($a_idx > $b_idx);
  return -1 if ($a_idx < $b_idx);
  return 0;
}

sub print_coverage()
{
  my ($_mod) = @_;

  # print header
  print $outfile "<table>\n";
  print $outfile "<table border=\"1\">\n";
  print $outfile "<tr>\n";
  print $outfile "<th>Requirement Tag</th>\n";
  print $outfile "<th>Requirement Specification</th>\n";
  print $outfile "<th>Design Document</th>\n";
  print $outfile "<th>Help</th>\n";
  print $outfile "<th>Source Code Documentation</th>\n";
  print $outfile "<th>Test</th>\n";
  print $outfile "</tr>\n";

  # print coverage for module(s)
  my $cnt_total = 0;
  my $cnt_2 = 0;
  my $cnt_3 = 0;
  my $cnt_5 = 0;
  foreach(sort(req_sort keys(%req_tags))) {
    my $mod = (split(/_/, $_))[1];
    next if (defined $_mod) and ($mod != $_mod);
    my $tag = $_;
    print $outfile "<tr>\n";
    print $outfile "<td>$tag</td>\n";
    $cnt_total++;
    if (exists $req_tags_2{$tag}) {
      print $outfile "<td style='background-color:#77FF77;'>";
      print $outfile $req_tags_2{$tag};
      print $outfile "</td>\n";
      $cnt_2++;
    } else {
      print $outfile "<td style='background-color:#FF7777;'>";
      print $outfile "&nbsp;";
      print $outfile "</td>\n";
    }
    if (exists $req_tags_3{$tag}) {
      print $outfile "<td style='background-color:#77FF77;'>";
      print $outfile $req_tags_3{$tag};
      print $outfile "</td>\n";
      $cnt_3++;
    } else {
      print $outfile "<td style='background-color:#FF7777;'>";
      print $outfile "&nbsp;";
      print $outfile "</td>\n";
    }
    print $outfile "<td style='background-color:#CCCCCC;'>";
    print $outfile "&nbsp;";
    print $outfile "</td>\n";
    if (exists $req_tags_5{$tag}) {
      print $outfile "<td style='background-color:#77FF77;'>";
      print $outfile $req_tags_5{$tag};
      print $outfile "</td>\n";
      $cnt_5++;
    } else {
      print $outfile "<td style='background-color:#FF7777;'>";
      print $outfile "&nbsp;";
      print $outfile "</td>\n";
    }
    print $outfile "<td style='background-color:#CCCCCC;'>";
    print $outfile "&nbsp;";
    print $outfile "</td>\n";
    print $outfile "</tr>\n";
  }

  # print summary
  print $outfile "<tr>\n";
  print $outfile "<td style='background-color:#CCCCCC;'>";
  print $outfile $cnt_total;
  print $outfile "</td>\n";
  print $outfile "<td style='background-color:#CCCCCC;'>";
  print $outfile $cnt_2;
  print $outfile "</td>\n";
  print $outfile "<td style='background-color:#CCCCCC;'>";
  print $outfile $cnt_3;
  print $outfile "</td>\n";
  print $outfile "<td style='background-color:#CCCCCC;'>";
  print $outfile "&nbsp;";
  print $outfile "</td>\n";
  print $outfile "<td style='background-color:#CCCCCC;'>";
  print $outfile $cnt_5;
  print $outfile "</td>\n";
  print $outfile "<td style='background-color:#CCCCCC;'>";
  print $outfile "&nbsp;";
  print $outfile "</td>\n";
  print $outfile "</tr>\n";

  # print footer
  print $outfile "</table>\n";
}

&get_from_requirement_specification();
&get_from_design_document();
&get_from_source_code_documentation();

# print header
open($outfile, ">report.html");
print $outfile "<html>\n";
print $outfile "<head>\n";
print $outfile "<title>BUSMASTER Requirements Traceability Matrix</title>\n";
print $outfile "</head>\n";
print $outfile "<body>\n";
print $outfile "<img src=\"../../Documents/Logo/BUSMASTER_Logo_311x64.png\"/>\n";
print $outfile "<h1>BUSMASTER requirements traceability matrix</h1>\n";

# print coverage per module
foreach(sort(keys(%mods))) {
  my $mod = $_;
  print $outfile "<h2>RS_$mod - ".$mods{$_}."</h2>\n";
  &print_coverage($mod);
}

# print footer
print $outfile "</body>\n";
print $outfile "</html>\n";
close($outfile);
