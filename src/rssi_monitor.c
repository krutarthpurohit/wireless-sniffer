#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "rssi_monitor.h"
#include "queue.h"

pthread_mutex_t sig_lvl_mutx = PTHREAD_MUTEX_INITIALIZER;
RSSI_INFO_t rssi_info = {0};

void set_rssi_sigl(float sigVal)
{
    float tempStorage = sigVal;
 
    pthread_mutex_lock(&sig_lvl_mutx);

    rssi_info.signal_level = tempStorage;
    // printf("\n[set_rssi_sigl] rssi_info.signal_level = %0.2f\n",rssi_info.signal_level);
    
    pthread_mutex_unlock(&sig_lvl_mutx);

    return;
}

float get_rssi_sigl(void)
{
    float retRssiSigVal = 0.00;
 
    pthread_mutex_lock(&sig_lvl_mutx);

    retRssiSigVal = rssi_info.signal_level;
    // printf("\n[get_rssi_sigl] retRssiSigVal=%0.2f", retRssiSigVal);
    
    pthread_mutex_unlock(&sig_lvl_mutx);
    
    return retRssiSigVal;
}

void *rssi_monitor(void *arg)
{
    FILE* fp                = NULL;
    char line[2048]         = "\0";
    float curr_signal_lvl   = 0;
    
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
                if(sscanf(signal_lvl,"Signal level=%f", &curr_signal_lvl) == 1)
                {
                    printf("Fetched signal level= %0.2f\n",curr_signal_lvl);
                    set_rssi_sigl(curr_signal_lvl);
                    enqueue(curr_signal_lvl);
                }
            }

        }
        pclose(fp);
        usleep(DELAY_5MS);    //500ms delay
    }

    return NULL;
}