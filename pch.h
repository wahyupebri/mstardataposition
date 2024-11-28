// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H
#include <curl/curl.h>
#include <mysql.h>
using namespace System;
// add headers that you want to pre-compile here
#define MAXMACHINE 300 //max data to hold
//Struct data
typedef struct result {
    long machine_oid;
    char machine[10];
    char class_name[20];
    int status;
    char activity[30];
    char last_pr[40];
    double x;
    double y;
    double z;
    double accuracy;
    int bad_gps;
    double speed;
    double heading;
}dataholder;

//--------------------------
int test1();
int execConsole(char* command);
int printData(void*);
char* convertStrToChar(String^ t);
long long getMillisec();
wchar_t* getWC(const char* c);
int waitForMillSecs(long long s);
void convertCommaToPoint(void* src, void* dest);
void convertDoubleCommaToCharPoint(void* src, void* dest);
void convertDoubleToCharArray(void* src, void* dest);
int convertDataTimeFormat(char* iFormat, char* oFormat, char* input, char* output);
#endif //PCH_H
