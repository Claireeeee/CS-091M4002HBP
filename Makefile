TARGET = router

default: $(TARGET)
all: $(TARGET)

CC = gcc
LD = gcc

CFLAGS = -g -Wall -Iinclude
LDFLAGS = -L.

LIBS = -lipstack -lpthread

LIBIP = libipstack.a
LIBIP_SRCS = arp.c arpcache.c icmp.c ip.c packet.c rtable.c rtable_internal.c test.c
LIBIP_OBJS = $(patsubst %.c,%.o,$(LIBIP_SRCS))

HDRS = ./include/*.h ./*.h

$(LIBIP_OBJS) : %.o : %.c include/*.h
	$(CC) -c $(CFLAGS) $< -o $@

$(LIBIP): $(LIBIP_OBJS)
	ar rcs $(LIBIP) $(LIBIP_OBJS)

SRCS = main.c ip_forwarding.c
OBJS = $(patsubst %.c,%.o,$(SRCS))

$(OBJS) : %.o : %.c include/*.h
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(LIBIP) $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET) $(LIBS) 

clean:
	rm -f *.o $(TARGET) $(LIBIP)
