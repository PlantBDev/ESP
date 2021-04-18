//information used by NTPclock.h
#ifndef NTPinfo
#define NTPinfo
  char* NTPserver = "pool.ntp.org"; //NTP server address
  int timeOffset = 10800; //time offset from UTC in seconds
  int dayStartTimeH = 8; //hour when day starts
  int dayStartTimeM = 0; //minute when time starts
#endif
