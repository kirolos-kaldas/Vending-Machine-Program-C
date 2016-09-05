########################################################################
# COSC1076 - Advanced Programming Techniques
# Semester 2 2015 Assignment #2
# Full Name        : KIROLOS KALDAS
# Student Number   : s3545643
# Course Code      : COSC1076
# Program Code     : BP096
# Start up code provided by Paul Miller
########################################################################

CC = gcc

# Compiler flags.
CFLAGS = -Wall -ansi -pedantic -g

# C pre-processor flags.
CPPFLAGS =

# Linker flags.
LDFLAGS = -lm

SRCS = ppd_main.c ppd_menu.c ppd_options.c ppd_utility.c ppd_stock.c ppd_coin.c

OBJS = $(SRCS:.c=.o)

all: binary

binary: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^ 

clean:
	-$(RM) *.o binary >/dev/null 2>&1

.PHONY: clean

SOURCES=ppd_main.c ppd_menu.c ppd_options.c ppd_utility.c ppd_stock.c \
ppd_coin.c
HEADERS=ppd_main.h ppd_menu.h ppd_options.h ppd_utility.h ppd_stock.h \
ppd_coin.h
README=ppd_readme
MAKEFILE=Makefile

########################################################################
# Move this target to the end of the Makefile to zip up your code 
# when submitting. Do not submit your .dat files, or directories. 
# We only want the files that are part of your implementation.
########################################################################
archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) $(README) $(MAKEFILE)
