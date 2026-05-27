#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "main.h"
#include "scanner.h"
#include "rssi_monitor.h"
#include "file_mng.h"

pthread_t thread_rssi;
pthread_t thread_file_mng;

pthread_attr_t thread_rssi_attr;

int main(void)
{
	ACCESS_POINT_t aps[MAX_APS];
	RSSI_INFO_t rssi_info = {0};
	int ret = -1;
	
	printf("\nWireless-sniffer Program\n");

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

	ret = pthread_create(&thread_rssi, NULL , &rssi_monitor, &rssi_info);
	if(ret != 0) {
		printf("\nError: Creating rssi thread\n");
		return -1;
	}

	ret = pthread_create(&thread_file_mng, NULL , &create_csv, &rssi_info);
	if(ret != 0) {
		printf("\nError: Creating csv thread\n");
		return -1;
	}


	pthread_join(thread_rssi, NULL);
	pthread_join(thread_file_mng, NULL);
	
	return 0;
}
