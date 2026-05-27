#ifndef SCANNER_H
#define SCANNER_H

#define MAX_APS 64

typedef struct {
    char ssid[128];
    float signal;
} ACCESS_POINT_t;

int scan_wifi(ACCESS_POINT_t *aps);

#endif  /* SCANNER_H */