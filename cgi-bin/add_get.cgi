#!/usr/bin/perl -Tw

use strict;
use CGI;

my($cgi) = new CGI;

print $cgi->header;
print $cgi->h1("This page method is GET");
my($result) = ($cgi->param('number_1')+$cgi->param('number_2')) if defined $cgi->param('number_1');

print $cgi->h1("Result is $result");
print $cgi->end_html;
