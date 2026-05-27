#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rssi_monitor.h"

void rssi_monitor(RSSI_INFO_t* rssi_info)
{
    FILE* fp                = NULL;
    char line[2048]         = "\0";
    float curr_signal_lvl   = 0;

    fp = popen("iwconfig wlan0", "r");
    if (fp == NULL)
    {
        perror("[rssi_monitor] Error: popen failed");
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {

        char* signal_lvl = strstr(line,"Signal level=");
        if(signal_lvl != NULL)
        {
            if(sscanf(signal_lvl,"Signal level=%f", &curr_signal_lvl) > 0)
            {
                printf("Fetched signal level= %0.2f\n",curr_signal_lvl);
                rssi_info->signal_level = curr_signal_lvl;
            }
        }

    }
    
    pclose(fp);
    
}