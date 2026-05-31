#include <time.h>

#include "main.h"
#include "file_mng.h"
#include "rssi_monitor.h"
#include "queue.h"
#include "network_stat.h"

pthread_mutex_t local_nwtk_stat = PTHREAD_MUTEX_INITIALIZER;

void* create_csv(void *arg)
{
    FILE *fp=NULL;
    time_t raw_time;
    char timestamp[64] = "\0";
    struct timespec ts;
    struct tm *time_info = {0};
    float receivedRssiVal = 0.00;
    NETWORK_STAT_t local_neteork_stat_info = {0};
    
    if(access("rssi_logger.csv", F_OK) == 0)
    {
        if(unlink("rssi_logger.csv") == 0) printf("\nFile 'rssi_logger.csv' deleted successfully!\n");
    }

    fp = fopen("rssi_logger.csv","a");
    if(fp == NULL){
        printf("\nError: rssi_logger cannot open\n");
        return NULL;
    }
    
    fseek(fp,0,SEEK_END);
    if(ftell(fp) == 0)
    {
        fprintf(fp, "Timestamp,rssi,tx_mbps,rx_mbps\n");
    }
    
    while (1)
    {
        clock_gettime(CLOCK_REALTIME, &ts);
        time_info = localtime(&ts.tv_sec);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);
        snprintf(timestamp + strlen(timestamp),sizeof(timestamp)-strlen(timestamp),".%03ld",ts.tv_nsec / 1000000);
        receivedRssiVal = get_rssi_sigl();
        get_network_stat_info(&local_neteork_stat_info);

        fprintf(fp, "%s,%.2f,%0.4f,%0.4f\n", timestamp, receivedRssiVal, local_neteork_stat_info.tx_mbps, local_neteork_stat_info.rx_mbps);
        fflush(fp);
        // view_queue();
        usleep(DELAY_500MS);
    }

    fclose(fp);

    return NULL;
}