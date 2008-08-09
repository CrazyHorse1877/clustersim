include ../../client.mak

PROG = GE_input
UTILITY_CFILES = file.c tcp.c parse_arg.c convert_units.c
SERVER_CFILES = flight_info.c position.c time_stamp.c input_client.c client.c pdf.c orientation.c
MAIN_CFILES = $(wildcard *.c)
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

