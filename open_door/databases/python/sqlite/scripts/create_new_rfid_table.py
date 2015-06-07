#################################################################
#                  Create New-RFID Table Script                 #
#                      James Scott McDowell                     #
#################################################################
#                          Description                          #
#  Creates a table that holds newly scanned RFID card codes.    #
#  The table simply acts as a place to stage new RFID numbers   #
#  until the admin adds a new person to the people table.       #
#  Based on code found in The Python Standard Library section   #
#  11.13.                                                       #
#################################################################
import sqlite3
conn = sqlite3.connect('new_rfid.db')
c = conn.cursor()
c.execute('''CREATE TABLE new_rfid (row_number integer, rfid text)''')
conn.commit()
conn.close()