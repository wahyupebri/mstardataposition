//Code by WAHYU PEBRIAN, May 2023
#include "pch.h"
#include "model.h"


#include <stdio.h>

#ifndef WPTEST
//#define WPTEST 1
const int LOOP = 2;
#endif
#ifndef WPRELEASE
#define WPRELEASE 1
#endif
using namespace System;

int main(array<System::String ^> ^args)
{
#ifdef WPTEST
    printf("Testing...\n");
    for (int i = 0;i < LOOP;i++) {
#endif // WPTEST
#ifdef WPRELEASE
        printf("Code by Wahyu Pebrian, May 2023. Use for internal Tutupan only. Release version 01.\n");
        for (;;) {
#endif // WPRELEASE
            long long waitmillis = 0;
            printf("[INFO] Query is running...\n");
            getResultQuery(&waitmillis);
            if (waitmillis < 0) {
                printf("[WARNING] Interval negatif (%ld)\n", waitmillis);
                //return 1;
            }
            else
                printf("[INFO] Interval positif.\n");
            printf("[INFO] Will run next query after %ld mill secs\n", waitmillis);
            //waitForMillSecs(waitmillis);
            waitForMillSecs(10000);
            
#ifdef WPTEST
    }
#endif // WPTEST
#ifdef WPRELEASE
    }
#endif // WPRELEASE
    return 0;
}
