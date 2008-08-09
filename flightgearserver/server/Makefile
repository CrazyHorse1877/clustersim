include ../client.mak

PROG = server
UTILITY_DIR = ../utility
UTILITY_CFILES = tcp.c file.c parse_arg.c
MAIN_CFILES = main.c server.c flight_info.c pdf.c time_stamp.c position.c orientation.c
CFILES = $(MAIN_CFILES) $(patsubst %,$(UTILITY_DIR)/%,$(UTILITY_CFILES)) $(patsubst %,$(SERVER_DIR)/%,$(SERVER_CFILES))

OBJS = $(CFILES:.c=.o)

.PHONY: all run clean clear mkdepend

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(OBJS) -o $(PROG)

run: all
	./$(PROG)

valgrind: all
	valgrind --leak-check=full --suppressions=suppressions ./$(PROG)

clean:
	-rm $(OBJS) $(PROG) depend

depend:
	-$(CC) -M $(CFLAGS) $(CFILES) > depend

mkdepend:
	-rm -f depend
	-$(MAKE) depend
	
include depend

