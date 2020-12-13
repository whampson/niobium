#==============================================================================#
# Copyright (C) 2020 Wes Hampson. All Rights Reserved.                         #
#                                                                              #
# This file is part of the Niobium Operating System.                           #
# Niobium is free software; you may redistribute it and/or modify it under     #
# the terms of the license agreement provided with this software.              #
#                                                                              #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   #
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL      #
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER   #
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      #
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER          #
# DEALINGS IN THE SOFTWARE.                                                    #
#==============================================================================#
#    File: Makefile                                                            #
# Created: November 24, 2020                                                   #
#  Author: Wes Hampson                                                         #
#                                                                              #
# Master Makefile for the Niobium Operating System and associated tools.       #
#                                                                              #
# Niobium is currently built using GCC 7.1.0 and Binutils 2.28 configured for  #
# i686-elf binaries. To build, you must first add these tools to your system's #
# PATH. You can find a precompiled set of the i686-elf tools here:             #
#     https://github.com/lordmilko/i686-elf-tools                              #
# The programs in tools/ are built using your system's native GCC and Binutils #
# as they are not meant to be run on Niobium.                                  #
#==============================================================================#

export TREE			:=
export TOPDIR		:= $(CURDIR)
export BINDIR		:= bin
export OBJDIR		:= obj
export INCLUDE		:= include
export IMGDIR		:= $(BINDIR)/img
export IMGFILE		:= $(IMGDIR)/niobium.img

BINUTILS_PREFIX		:= i686-elf-
GCC_WARNINGS		:= -Wall -Wextra -Wpedantic
GCC_FLAGS			:= $(GCC_WARNINGS) -g

export AS			:= $(BINUTILS_PREFIX)gcc
export ASFLAGS		:= $(GCC_FLAGS) -D__ASSEMBLY__ -m32
export CC			:= $(BINUTILS_PREFIX)gcc
export CFLAGS		:= $(GCC_FLAGS) -ffreestanding -fno-exceptions \
						-fno-unwind-tables -fno-asynchronous-unwind-tables
export LD			:= $(BINUTILS_PREFIX)ld
export LDFLAGS		:=
export MAKEFLAGS	:= --no-print-directory
export MKDIR		:= mkdir -p
export OBJCOPY		:= $(BINUTILS_PREFIX)objcopy
export RM			:= rm -f

.PHONY: all img boot kernel tools wipe clean-tools

all: tools img

img: boot kernel
	@echo '> Creating floppy image...'
	@$(MKDIR) -p $(IMGDIR)
	@fatfs -i $(IMGFILE) create
	@fatfs -i $(IMGFILE) add $(BINDIR)/init.sys
	@fatfs -i $(IMGFILE) add $(BINDIR)/nbos.sys
	@echo '> Writing boot sector...'
	@dd if=$(BINDIR)/boot.bin of=$(IMGFILE) conv=notrunc status=none
	@echo 'OUT $(IMGFILE)'

boot: dirs
	@$(MAKE) -C boot

kernel: dirs
	@$(MAKE) -C kernel

tools: dirs
	@$(MAKE) -C tools

wipe: clean clean-tools

clean-tools:
	@$(RM) -r tools/bin
	@$(RM) -r tools/obj

include $(TOPDIR)/include/make/Rules.mk
