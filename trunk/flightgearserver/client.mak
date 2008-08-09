CC = gcc
UTILITY_DIR = ../../utility
SERVER_DIR = ../../server

WARNFLAGS = -Wall -W -Werror -pedantic
CFLAGS = -g -O3 -std=c99 $(WARNFLAGS) -I$(SERVER_DIR) -I$(UTILITY_DIR)

