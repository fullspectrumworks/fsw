#################################################################
#                     Inser New RFID Script                     #
#                      James Scott McDowell                     #
#################################################################
#                          Description                          #
#  Inserts a new RFID number into the new_rfid staging table    #
#  Based on code found in The Python Standard Library section   #
#  11.13.                                                       #
#################################################################
import sqlite3
conn = sqlite3.connect('new_rfid.db')
c = conn.cursor()
c.execute("INSERT INTO new_rfid VALUES (1,'0F030387CA')")
conn.commit()
conn.close()
