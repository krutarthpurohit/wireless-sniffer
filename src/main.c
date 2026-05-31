#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "main.h"
#include "scanner.h"
#include "rssi_monitor.h"
#include "file_mng.h"
#include "ai_model.h"
#include "queue.h"
#include "network_stat.h"

pthread_t thread_rssi;
pthread_t thread_file_mng;
pthread_t thread_ai_model;
pthread_t thread_network_stat;

int main(void)
{
	ACCESS_POINT_t aps[MAX_APS];
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

	ret = pthread_create(&thread_rssi, NULL , &rssi_monitor, NULL);
	if(ret != 0) {
		printf("\nError: Creating rssi_monitor thread\n");
		return -1;
	}

	ret = pthread_create(&thread_file_mng, NULL , &create_csv, NULL);
	if(ret != 0) {
		printf("\nError: Creating create_csv thread\n");
		return -1;
	}
	
	ret = pthread_create(&thread_ai_model, NULL , &ai_model, NULL);
	if(ret != 0) {
		printf("\nError: Creating ai_model thread\n");
		return -1;
	}

	ret = pthread_create(&thread_network_stat, NULL , &fetch_network_stat, NULL);
	if(ret != 0) {
		printf("\nError: Creating fetch_network_stat thread\n");
		return -1;
	}

	pthread_join(thread_rssi, NULL);
	pthread_join(thread_file_mng, NULL);
	pthread_join(thread_ai_model, NULL);
	pthread_join(thread_network_stat, NULL);

	return 0;
}
