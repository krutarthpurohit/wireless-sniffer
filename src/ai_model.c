/**
 * AI Model to detect: anomaly
 * 
 * Anamoly is detected when the RSSI (Received Signal Strength Indicator)
 * value suddenly changes because of listed reasons:
 * - AP device is unstable
 * - Network Inteference
 * - Distance between devices increases.
 * 
 * Eg: -45, -45, -46, -47, -46, -45, -44, -45      is normal
 * Eg: -45, -49, -52, -51, -45, -43, -39, -35      is abnormal
 * 
 * The AI model is implementes to calculate mean (m) and standard deviation (SD).
 * The condition for anamoly detection: 
 *      | x - m | = 3 x SD      where x is the latest rssi value.
 * The current deviation must be at least 3x greater than SD.
 * 
 * With having the sample size SIG_SAMPELS_SIZE, mean can be calculated
 * 
 */

#include <math.h>

#include "main.h"
#include "ai_model.h"
#include "queue.h"
#include "rssi_monitor.h"

pthread_mutex_t rssi_stat_mutx = PTHREAD_MUTEX_INITIALIZER;

RSSI_STAT_t rssi_stat = {0};

extern pthread_mutex_t queue_mutx;
extern float rssi_sig_queue[SIG_SAMPELS_SIZE];

void calcRSSI_mean_sd(void)
{
    float rssi_log_buffer[SIG_SAMPELS_SIZE];
    float std_deviation[SIG_SAMPELS_SIZE] = {0};
    float calc_variance = 0.00; //avg of sq. deviation
    float calc_mean = 0.00;
    float calc_sum = 0.00;
    float calc_deviation = 0.00;
    
    pthread_mutex_lock(&queue_mutx);
    
    for(int i=0; i < SIG_SAMPELS_SIZE ; i++)
    {
        rssi_log_buffer[i] = rssi_sig_queue[i];
    }
    
    pthread_mutex_unlock(&queue_mutx);

    /* Calcaulate mean */

    for(int i = 0; i < SIG_SAMPELS_SIZE; i++)
    {
        calc_sum += rssi_log_buffer[i];
    }
    
    calc_mean = calc_sum / SIG_SAMPELS_SIZE;

    /* Calculate standard deviation */

    for(int i = 0; i < SIG_SAMPELS_SIZE; i++)
    {
        std_deviation[i] = rssi_log_buffer[i] - calc_mean;
    }
    
    for(int i = 0; i < SIG_SAMPELS_SIZE; i++)
    {
        std_deviation[i] = std_deviation[i] * std_deviation[i];
    }

    // calculate variance
    for(int i = 0; i < SIG_SAMPELS_SIZE; i++)
    {
        calc_variance += std_deviation[i];
    }

    calc_variance = calc_variance / SIG_SAMPELS_SIZE;

    // calculate SD
    calc_deviation = sqrt(calc_variance);

    pthread_mutex_lock(&rssi_stat_mutx);

    rssi_stat.rssi_mean = calc_mean;
    rssi_stat.rssi_dtd_deviation = calc_deviation;

    pthread_mutex_unlock(&rssi_stat_mutx);
    
    return;
}

void* ai_model(void* arg)
{
    float mean = 0.00;
    float std_deviation = 0.00;
    float curr_rssi_sig_val = 0.00;

    while (1)
    {
        calcRSSI_mean_sd();

        curr_rssi_sig_val = get_rssi_sigl();

        mean = rssi_stat.rssi_mean;
        std_deviation = rssi_stat.rssi_dtd_deviation;

        if (fabsf(curr_rssi_sig_val - mean) > (3.0f * std_deviation))
        {
            printf("\nSTATUS: Anomaly\n");
            printf("Possible interference or instability\n");
        }
        else
        {
            printf("STATUS: Normal\n");
        }

        usleep(DELAY_5MS);
    }

    return NULL;
}