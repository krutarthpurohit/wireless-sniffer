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
    char timestamp[20] = "\0";
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
        time(&raw_time);
        time_info = localtime(&raw_time);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);
        
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