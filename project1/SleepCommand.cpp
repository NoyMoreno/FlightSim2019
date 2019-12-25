//
// Created by noy on ٢٣‏/١٢‏/٢٠١٩.
//

#include <unistd.h>
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
    sleep(timeSleep/1000);
    return 1;
}