include common.mk

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -rf $(OUT)
