#include "main.h"

typedef struct
{
    float rx_mbps;
    float tx_mbps;
    float latency_ms;
    // float packet_loss;
} NETWORK_STAT_t;

extern NETWORK_STAT_t network_stat;
extern pthread_mutex_t network_stat_mutx;

void get_network_stat_info(NETWORK_STAT_t* local_neteork_stat_info);
void* fetch_network_stat(void* arg);