#include "file_mng.h"
#include "rssi_monitor.h"

void* create_csv(void *arg)
{
    FILE *fp=NULL;
    time_t raw_time;
    char timestamp[20] = "\0";
    
    RSSI_INFO_t* info = (RSSI_INFO_t*) arg;

    time(&raw_time);
    struct tm *time_info = localtime(&raw_time);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);

    fp = fopen("rssi_logger.csv","a");
    if(fp == NULL){
        printf("\nError: rssi_logger cannot open\n");
        return NULL;
    }

    // while (1)
    {
        fprintf(fp, "%s,%f", timestamp, info->signal_level);
    }

    fclose(fp);

    return NULL;
}