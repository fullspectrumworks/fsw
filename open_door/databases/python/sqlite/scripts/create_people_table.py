#################################################################
#                   Create People Table Script                  #
#                     James Scott McDowell                      #
#################################################################
#                          Description                          #
#  Creates a table that holds information about people.         #
#  People table fields:                                         #
#    acess_permission - a user's access to the building is      #
#                       defined either as a 1 (access), or 0    #
#                       (no access)                             #
#    pin - personal identification number                       #
#    rfid - radio-frequency identification number               #
#    last_name - user's last name                               #
#    first_name - user's first name                             #
#    username - user's username or handle                       #
#    birthdate - user's date of birth                           #
#    Based on code found in The Python Standard Library section #
#    11.13.                                                     #
#################################################################
import sqlite3
conn = sqlite3.connect('people.db')
c = conn.cursor()
c.execute('''CREATE TABLE people (access_permission integer, pin integer, rfid text, last_name text, first_name text, username text, birthdate text)''')
conn.commit()
conn.close()