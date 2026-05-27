
SRC_DIR:= src 

CC:= gcc

SRC ?= 	src/main.c \
		src/scanner.c \
		src/rssi_monitor.c

CFLAG:= -Wall -Wextra

OUT:= wifi-sniffer