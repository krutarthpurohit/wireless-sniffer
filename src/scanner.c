#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Project Specific Header File*/
#include "scanner.h"

int scan_wifi(ACCESS_POINT_t *aps)
{
    FILE *fp            = NULL;
    char line[2048]     = "\0";
    char curr_ssid[128] = "\0";
    float curr_signal   = 0;
    int count           = 0;

    fp = popen("sudo iw dev wlan0 scan ap-force 2>&1","r");

    if(fp == NULL)
    {
        printf("\n[scan_wifi] Error: popen failed\n");
    }

    while(fgets(line,sizeof(line),fp) != NULL) {
        
        /* parse signal */
        char* signal_ptr = strstr(line,"signal: ");
        if(signal_ptr != NULL)
        {
            if(sscanf(line," signal: %f", &curr_signal) > 0)
            {
                // printf("Fetched signal = %0.2f\n",curr_signal);
            }

        }

        /* parse SSID */
        char* ssid_ptr = strstr(line,"SSID: ");
        if(ssid_ptr != NULL)
        {
            ssid_ptr += 6;  // "SSID: " = 6 characters
            
            ssid_ptr[strcspn(ssid_ptr, "\n")] = '\0';
            strncpy(curr_ssid, ssid_ptr, sizeof(curr_ssid) - 1);
            curr_ssid[sizeof(curr_ssid) - 1] = '\0';

            // printf("SSID Extracted: %s",curr_ssid);

            /* Store complete AP information in structure */
            aps[count].signal = curr_signal;
            strncpy(aps[count].ssid, curr_ssid , 128);
            count++;

        }

    }
    
    pclose(fp);

    return count;

}