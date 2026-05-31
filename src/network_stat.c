#include <stdlib.h>
#include <stdint.h>

#include "main.h"
#include "network_stat.h"

#define TX_STAT_PATH "/sys/class/net/wlan0/statistics/tx_bytes"
#define RX_STAT_PATH "/sys/class/net/wlan0/statistics/rx_bytes"

NETWORK_STAT_t network_stat = {0};

pthread_mutex_t network_stat_mutx = PTHREAD_MUTEX_INITIALIZER;
extern pthread_mutex_t local_nwtk_stat;

void get_network_stat_info(NETWORK_STAT_t* local_neteork_stat_info)
{
    pthread_mutex_lock(&local_nwtk_stat);

    local_neteork_stat_info->tx_mbps = network_stat.tx_mbps;
    local_neteork_stat_info->rx_mbps = network_stat.rx_mbps;

    pthread_mutex_unlock(&local_nwtk_stat);

    return;
}

uint64_t read_counter(const char* stat_path)
{
    FILE* fp = NULL;
    uint64_t value = 0;

    fp = fopen(stat_path, "r");
    if(fp == NULL) {
        printf("\nError: fopen stat_path\n");
        return 0;
    }

    fscanf(fp,"%lu",&value);

    fclose(fp);

    return value;
}

void* fetch_network_stat(void* arg)
{
    FILE* fp_tx_stat = NULL;
    FILE* fp_rx_stat = NULL;

    uint64_t curr_tx = 0;
    uint64_t curr_rx = 0;
    uint64_t prev_tx = 0;
    uint64_t prev_rx = 0;
    uint64_t delta_tx = 0;
    uint64_t delta_rx = 0;
    double tx_mbps = 0.00;
    double rx_mbps = 0.00;

    while (1)
    {
        curr_tx = read_counter(TX_STAT_PATH);
        curr_rx = read_counter(RX_STAT_PATH);

        delta_tx = curr_tx - prev_tx;
        delta_rx = curr_rx - prev_rx;

        tx_mbps = (delta_tx * 0.8) / 1000000.0;
        rx_mbps = (delta_rx * 0.8) / 1000000.0;

        printf("TX = %0.2f Mbps\n",tx_mbps);
        printf("RX = %0.2f Mbps\n",rx_mbps);

        pthread_mutex_lock(&network_stat_mutx);

        network_stat.tx_mbps = tx_mbps;
        network_stat.rx_mbps = rx_mbps;

        pthread_mutex_unlock(&network_stat_mutx);

        prev_tx = curr_tx;
        prev_rx = curr_rx;
        
        usleep(DELAY_1SEC);
    }
    
}
