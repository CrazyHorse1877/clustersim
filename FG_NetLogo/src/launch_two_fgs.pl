#!/usr/bin/perl

$fg_path = "/Users/elijordan/Documents/THESIS/thesis_stuff/Applications/FlightGear.app/Contents/Resources";

$multiplay_args1 = "--callsign=ELI-1 --multiplay=in,10,,5000 --multiplay=out,10,129.94.207.123,5000";
$multiplay_args2 = "--callsign=ELI-2 --multiplay=in,10,,5001 --multiplay=out,10,129.94.207.123,5000";

$telnet_args1 = "--telnet=5501";
$telnet_args2 = "--telnet=5502";

$position_args = "--airport=VG001";
$common_args = "--timeofday=noon --aircraft=j3cub";

$log_args1 = "--nmea=file,out,1,/Users/elijordan/Desktop/data1.txt";
$log_args2 = "--nmea=file,out,1,/Users/elijordan/Desktop/data2.txt";

system "cd $fg_path; ./fgfs.sh $position_args $common_args $log_args1 $multiplay_args1 $telnet_args1 &";
system "cd $fg_path; ./fgfs.sh $position_args $common_args $log_args2 $multiplay_args2 $telnet_args2 &";

