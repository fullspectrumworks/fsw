#!/bin/bash
#################################################################
#                      Open Door Installer                      #
#                      James Scott McDowell                     #
#################################################################
#                          Instructions                         #
#  Run the following two commands to use this installer:        #
#    chmod u+x unix_python_flask_sqlite.sh                      #
#    ./unix_python_flask_sqlite.sh                              #
#################################################################
clear
echo ".------------------------------------------------------------------------------."
echo "|                            Open Door Installer                               |"
echo "|------------------------------------------------------------------------------|"
echo "| - Press [Enter] to continue or press [Ctrl] + [c] to quit.                   |"
echo "'------------------------------------------------------------------------------'"
read -p ""
sudo mkdir -p /var/open_door/{controllers,database/tables,forms}
sudo cp ../servers/python/flask/controllers/door_1.py /var/open_door/controllers
sudo cp ../servers/python/flask/forms/add.py /var/open_door/forms
sudo cp ../servers/python/flask/forms/delete.py /var/open_door/forms
sudo cp ../servers/python/flask/forms/edit.py /var/open_door/forms
sudo cp ../servers/python/flask/forms/lookup.py /var/open_door/forms
sudo python ../databases/python/sqlite/scripts/create_people_table.py
sudo python ../databases/python/sqlite/scripts/create_new_rfid_table.py
sleep 1
sudo mv people.db /var/open_door/database/tables
sudo mv new_rfid.db /var/open_door/database/tables
sudo apt-get install python-pip
sudo pip install flask
# open door network change and activate static ip network
echo auto lo                  >  /etc/network/interfaces
echo iface lo inet loopback  >> /etc/network/interfaces
echo auto eth0               >> /etc/network/interfaces
echo iface eth0 inet static  >> /etc/network/interfaces
echo address 192.168.1.4     >> /etc/network/interfaces
echo gateway 192.168.1.1     >> /etc/network/interfaces
echo netmask 255.255.255.0   >> /etc/network/interfaces
echo network 192.168.1.0     >> /etc/network/interfaces
echo broadcast 192.168.1.255 >> /etc/network/interfaces
sudo /etc/init.d/networking restart
clear
echo ".------------------------------------------------------------------------------."
echo "|                            Open Door Installer                               |"
echo "|------------------------------------------------------------------------------|"
echo "| - The Open Door installation was successful!                                 |"
echo "| - Press [Enter] to exit this installer.                                      |"
echo "'------------------------------------------------------------------------------'"
read -p ""
clear
