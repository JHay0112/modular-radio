# Makefile for modular radio


# Definitions
CC = gcc
CFLAGS = -Wall -Werror
DEL = rm
MKDIR = mkdir
OBJDIR = ./build
SRCDIR = ./src


# Targets

all: channel am_demod

channel: $(OBJDIR)/channel.out
am_demod: $(OBJDIR)/am_demod.out


# Recipes

$(OBJDIR)/utils/queue.o: $(SRCDIR)/utils/queue.c $(SRCDIR)/utils/queue.h | setup
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/channel.o: $(SRCDIR)/channel.c | setup
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/channel.out: $(OBJDIR)/channel.o | setup
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(OBJDIR)/am_demod.o: $(SRCDIR)/am_demod.c $(SRCDIR)/utils/queue.h | setup
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/am_demod.out: $(OBJDIR)/am_demod.o $(OBJDIR)/utils/queue.o | setup
	$(CC) $(CFLAGS) $^ -o $@ -lm -lpthread


# Phony targets

# Sets up the project for being built
# Ensures that the build directory is present
.PHONY: setup
setup:
	$(MKDIR) -p $(OBJDIR)
	$(MKDIR) -p $(OBJDIR)/utils

# Cleans up the build directory after use
.PHONY: clean
clean: 
	$(DEL) -rf $(OBJDIR)