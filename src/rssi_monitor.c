#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "rssi_monitor.h"

void *rssi_monitor(void *arg)
{
    FILE* fp                = NULL;
    char line[2048]         = "\0";
    float curr_signal_lvl   = 0;
    RSSI_INFO_t *info = (RSSI_INFO_t*) arg;
    
    while (1)
    {
        fp = popen("iwconfig wlan0", "r");
        if (fp == NULL)
        {
            perror("[rssi_monitor] Error: popen failed");
            return NULL;
        }

        while (fgets(line, sizeof(line), fp) != NULL)
        {

            char* signal_lvl = strstr(line,"Signal level=");
            if(signal_lvl != NULL)
            {
                if(sscanf(signal_lvl,"Signal level=%f", &info->signal_level) == 1)
                {
                    printf("Fetched signal level= %0.2f\n",info->signal_level);
                }
            }

        }
        pclose(fp);
        usleep(DELAY_5MS);    //500ms delay
    }

    return NULL;
}