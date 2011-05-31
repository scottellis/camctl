# Use this makefile for a cross-platform OE build

ifeq ($(strip $(OETMP)),)
	OETMP = $(HOME)/overo-oe/tmp 
endif


TOOLDIR = $(OETMP)/sysroots/`uname -m`-linux/usr/armv7a/bin

STAGEDIR = $(OETMP)/sysroots/armv7a-angstrom-linux-gnueabi/usr

CC = $(TOOLDIR)/arm-angstrom-linux-gnueabi-gcc

CFLAGS = -Wall

LIBDIR = $(STAGEDIR)/lib

INCDIR = $(STAGEDIR)/include

TARGET = camctl

$(TARGET): camctl.c
	$(CC) $(CFLAGS) -I $(INCDIR) -L $(LIBDIR) camctl.c -o $(TARGET)

install:
	sudo cp $(TARGET) /exports/overo/home/root

clean:
	rm -f $(TARGET)



