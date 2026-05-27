#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "scanner.h"
#include "rssi_monitor.h"

int main(void)
{
	ACCESS_POINT_t aps[MAX_APS];
	RSSI_INFO_t* rssi_info;
	
	printf("\nWireless-info Program\n");

	int total = scan_wifi(aps);
	
	if(total < 0) {
		printf("\nWifi scan failed\n");
		return -1;
	}

	for (int i = 0; i < total; i++)
	{
		// if(aps[i].ssid[0] == '\0')
		// continue;
		printf("%-30s %f dBm\n", aps[i].ssid, aps[i].signal);
	}

	printf("\nTotal Network Found: %d\n", total);

	rssi_monitor(rssi_info);
	printf("RSSI: Signal level=%f",rssi_info->signal_level);
	
	return 0;
}
