#include "ai_model.h"

void* ai_model(void* arg)
{
    FILE *fp=NULL;
    float rssiSigVal=0.00;

    if (access("rssi_logger.csv",F_OK) != 0) {
        printf("\nrssi_logger.csv not found\n");
        return NULL;
    }

    // fp = fopen("rssi_logger.csv","r");
    // if (fp == NULL) {
    //     printf("\nError: rssi_logger cannot open\n");
    //     return NULL;
    // }
    
    
    // while (1)
    // {
        
        
    //     usleep(DELAY_5MS);
    // }
    
    return NULL;
}