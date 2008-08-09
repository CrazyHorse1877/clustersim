#ifndef _TIME_STAMP_H_
#define _TIME_STAMP_H_


typedef struct{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	double millisecond;
}Time_stamp;

Time_stamp new_time_stamp(int year, int month, int day, int hour, int minute, int second, double millisecond);
void print_time_stamp(Time_stamp time);

#endif

