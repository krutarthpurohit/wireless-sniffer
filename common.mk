
SRC_DIR:= src 

CC:= gcc

SRC ?= 	src/main.c \
		src/scanner.c \
		src/rssi_monitor.c \
		src/file_mng.c \
		src/ai_model.c \
		src/queue.c

CFLAG:= -Wall -Wextra

LDFLAGS = -lpthread

OUT:= wifi-sniffer