#ifndef RSSI_MONITOR_H
#define RSSI_MONITOR_H

typedef struct {
    float signal_level;
} RSSI_INFO_t;


void rssi_monitor(RSSI_INFO_t *rssi_info);

#endif  /* RSSI_MONITOR_H */