#!/bin/bash
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
#    File: scripts/write_bootsect.sh                                           #
# Created: November 22, 2020                                                   #
#  Author: Wes Hampson                                                         #
#------------------------------------------------------------------------------#
# Writes the bootsector in the Niobium floppy image.                           #
#==============================================================================#

dd if=${NBDIR}/bin/bootsect of=${NBDIR}/floppy.img conv=notrunc
