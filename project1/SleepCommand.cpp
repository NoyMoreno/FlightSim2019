//
// Created by noy on ٢٣‏/١٢‏/٢٠١٩.
//
//#define WINDOWS_USE
#ifndef WINDOWS_USE
#include <unistd.h>
#else
#include <windows.h>
#endif

#include "SleepCommand.h"
SleepCommand :: SleepCommand() {}
int SleepCommand :: execute(std::vector<std::string> commands, int pos) {
    std::string time_sleep_str = commands[pos];
    // need time in seconds, 1 second = 1000 millisecond
    // need expression
    int timeSleep = atoi(time_sleep_str.c_str());
    if (timeSleep < 1000){
        timeSleep = 1000;
    }
#ifndef WINDOWS_USE
	sleep(timeSleep / 1000);
#else
	Sleep(timeSleep / 1000);
#endif
    return 1;
}