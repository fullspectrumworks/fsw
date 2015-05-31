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
#sudo apt-get install python-pip
#sudo pip install flask
clear
echo ".------------------------------------------------------------------------------."
echo "|                            Open Door Installer                               |"
echo "|------------------------------------------------------------------------------|"
echo "| - The Open Door installation was successful!                                 |"
echo "| - Press [Enter] to exit this installer.                                      |"
echo "'------------------------------------------------------------------------------'"
read -p ""
clear
