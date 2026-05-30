#include "main.h"

typedef struct
{
    float rssi_mean;
    float rssi_dtd_deviation;
} RSSI_STAT_t;

extern RSSI_STAT_t rssi_stat;
extern pthread_mutex_t rssi_stat_mutx;

void calcRSSI_mean_sd(void);
void* ai_model(void* arg);
