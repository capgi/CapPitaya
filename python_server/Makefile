##
# $Id: Makefile  2015-11-04 20:21:40Z Capgi $
#
# (c) Red Pitaya  http://www.redpitaya.com
#
# To Intergrate Pierre Clade works around RedPitaya ((https://github.com/clade/RedPitaya/)) on version 0.94 of RedPitaya acquire 
#
# This project file is written for GNU/Make software. For more details please 
# visit: http://www.gnu.org/software/make/manual/make.html
# GNU Compiler Collection (GCC) tools are used for the compilation and linkage. 
# For the details about the usage and building please visit:
# http://gcc.gnu.org/onlinedocs/gcc/
#

# Versioning system
VERSION ?= 0.00-0000
REVISION ?= devbuild

# AppFile
APPFILE = rpyc_server
# init.d file
INITFILE = python_server
# Example dir
EXAMPLE_DIR = Examples

# Install target - copy files to  in $(INSTALL_DIR) 
# executables to that location.
install:
	mkdir -p $(INSTALL_DIR)/bin
	cp bin/$(APPFILE) $(INSTALL_DIR)/bin
	mkdir -p $(INSTALL_DIR)/src
	cp -r $(EXAMPLE_DIR) $(INSTALL_DIR)/src
	mkdir -p $(INSTALL_DIR)/etc/init.d/
	cp -r init/$(INITFILE) $(INSTALL_DIR)/etc/init.d/

# NOTE : registering init file in OS/debian/redpitaya.sh
