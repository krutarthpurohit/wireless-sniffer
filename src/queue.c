#include "queue.h"

float rssi_sig_queue[SIG_SAMPELS_SIZE] = {0};
pthread_mutex_t queue_mutx = PTHREAD_MUTEX_INITIALIZER;

void enqueue(float value)
{
    float tmpVal = value;

    pthread_mutex_lock(&queue_mutx);
    
    /* shift each element by 1 indx to right */
    for(int i = SIG_SAMPELS_SIZE - 1 ; i >= 0 ; i--)
    {
        rssi_sig_queue[i] = rssi_sig_queue[i-1];
    }

    rssi_sig_queue[0] = tmpVal; //always appen new value at indx 0

    pthread_mutex_unlock(&queue_mutx);

    return;

}

void view_queue(void)
{
    float rssi_log_buffer[SIG_SAMPELS_SIZE] = {0};
    
    pthread_mutex_lock(&queue_mutx);
    
    for(int i=0; i < SIG_SAMPELS_SIZE ; i++)
    {
        rssi_log_buffer[i] = rssi_sig_queue[i];
    }
    
    pthread_mutex_unlock(&queue_mutx);

    printf("\nRSSI Log in Q: ");
    for(int i = 0 ; i < SIG_SAMPELS_SIZE ; i++)
    {
        printf("%0.2f ", rssi_log_buffer[i]);
    }

    return;
}