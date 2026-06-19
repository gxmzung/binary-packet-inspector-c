CC=gcc
CFLAGS=-Wall -Wextra -Iinclude

SRC=src/main.c src/packet_reader.c src/packet_validator.c src/packet_classifier.c src/crc16.c src/hex_utils.c src/session_report.c
OUT=binary_packet_inspector

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)