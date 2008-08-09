#include <stdio.h>
#include "time_stamp.h"


Time_stamp new_time_stamp(int year, int month, int day, int hour, int minute, int second, double millisecond){
	Time_stamp time;

	time.year = year;
	time.month = month;
	time.day = day;
	time.hour = hour;
	time.minute = minute;
	time.second = second;
	time.millisecond = millisecond;

	return time;
}


void print_time_stamp(Time_stamp time){
	printf("time: '%d-%d-%d  %d:%d:%d:%f'\n",time.year, time.month, time.day, time.hour, time.minute, time.second, time.millisecond);

}
