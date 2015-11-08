################################################################################
# Authors:
# - Pavel Demin <pavel.demin@uclouvain.be>
# - Iztok Jeras <iztok.jeras@redpitaya.com>
# License:
# https://raw.githubusercontent.com/RedPitaya/RedPitaya/master/COPYING
################################################################################

# Copy files to the boot file system
unzip ecosystem*.zip -d $BOOT_DIR

# Systemd services
install -v -m 664 -o root -d                                                         $ROOT_DIR/var/log/redpitaya_nginx
install -v -m 664 -o root -D $OVERLAY/etc/systemd/system/redpitaya_discovery.service $ROOT_DIR/etc/systemd/system/redpitaya_discovery.service
install -v -m 664 -o root -D $OVERLAY/etc/systemd/system/redpitaya_nginx.service     $ROOT_DIR/etc/systemd/system/redpitaya_nginx.service
install -v -m 664 -o root -D $OVERLAY/etc/systemd/system/redpitaya_scpi.service      $ROOT_DIR/etc/systemd/system/redpitaya_scpi.service
install -v -m 664 -o root -D $OVERLAY/etc/systemd/system/redpitaya_heartbeat.service $ROOT_DIR/etc/systemd/system/redpitaya_heartbeat.service
install -v -m 664 -o root -D $OVERLAY/etc/sysconfig/redpitaya                        $ROOT_DIR/etc/sysconfig/redpitaya
# JMC Add autostart RedPitaya python_server
install -v -m 754 -o root -D $OVERLAY/bin/rpyc_server		                     $ROOT_DIR/bin/rpyc_server
install -v -m 754 -o root -D $OVERLAY/etc/init.d/python_server                       $ROOT_DIR/etc/init.d/python_server
install -v -m 754 -o root -D $OVERLAY/usr/local/lib/libmonitor.so                    $ROOT_DIR/usr/local/lib/libmonitor.so

# TODO: this Wyliodrin service is only here since wyliodrin.sh can not be run in a virtualized environment
# Wyliodrin service
install -v -m 664 -o root -D $OVERLAY/etc/systemd/system/redpitaya_wyliodrin.service $ROOT_DIR/etc/systemd/system/redpitaya_wyliodrin.service

chroot $ROOT_DIR <<- EOF_CHROOT
systemctl enable redpitaya_discovery
systemctl enable redpitaya_nginx
#systemctl enable redpitaya_scpi
systemctl enable redpitaya_heartbeat

apt-get -y install libluajit-5.1 lua-cjson unzip
apt-get -y install libboost-system1.55.0 libboost-regex1.55.0 libboost-thread1.55.0

# JMC : Add Python, pip, numpy, PyRedPitaya
apt-get -y install python python-dev python-pip python-numpy
pip install PyRedPitaya
# config RedPitaya Python server autostart
update-rc.d python_server defaults
EOF_CHROOT

# profile for PATH variables, ...
install -v -m 664 -o root -D $OVERLAY/etc/profile.d/profile.sh   $ROOT_DIR/etc/profile.d/profile.sh
install -v -m 664 -o root -D $OVERLAY/etc/profile.d/alias.sh     $ROOT_DIR/etc/profile.d/alias.sh
install -v -m 664 -o root -D $OVERLAY/etc/profile.d/redpitaya.sh $ROOT_DIR/etc/profile.d/redpitaya.sh

# remove existing MOTD and replace it with a link to Red Pitaya version.txt
rm $ROOT_DIR/etc/motd
ln -s /opt/redpitaya/version.txt $ROOT_DIR/etc/motd 
