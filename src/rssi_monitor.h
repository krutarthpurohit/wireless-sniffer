#ifndef RSSI_MONITOR_H
#define RSSI_MONITOR_H

typedef struct {
    float signal_level;
} RSSI_INFO_t;

extern pthread_mutex_t sig_lvl_mutx;
extern RSSI_INFO_t rssi_info;

void set_rssi_sigl(float sigVal);
float get_rssi_sigl(void);

void *rssi_monitor(void *arg);

#endif  /* RSSI_MONITOR_H */