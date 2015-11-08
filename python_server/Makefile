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

# Monitor directory
MONITOR_DIR = PyRedPitaya-1.0/monitor

.PHONY monitor:
	$(MAKE) -C $(MONITOR_DIR)
	$(MAKE) -C $(MONITOR_DIR) install INSTALL_DIR=$(abspath $(INSTALL_DIR))/usr/local/
# Install target - copy files to  in $(INSTALL_DIR) 
# executables to that location.
install: monitor
	mkdir -p $(INSTALL_DIR)/bin
	cp bin/$(APPFILE) $(INSTALL_DIR)/bin
	mkdir -p $(INSTALL_DIR)/opt/src
	cp -r $(EXAMPLE_DIR) $(INSTALL_DIR)/opt/src
	mkdir -p $(INSTALL_DIR)/etc/init.d/
	cp -r init/$(INITFILE) $(INSTALL_DIR)/etc/init.d/

# NOTE : registering init file by OS/debian/redpitaya.sh
clean:
	$(MAKE) -C $(MONITOR_DIR) clean
