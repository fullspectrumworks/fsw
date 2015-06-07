#################################################################
#                     Select New RFID Script                    #
#                      James Scott McDowell                     #
#################################################################
#                          Description                          #
#  Select the newly swiped RFID number from the new_rfid        #
#  staging table.                                               #
#  Based on code found in The Python Standard Library section   #
#  11.13.                                                       #
#################################################################
import sqlite3
conn = sqlite3.connect('new_rfid.db')
c = conn.cursor()
c.execute('SELECT rfid FROM new_rfid')
print c.fetchone()
conn.close()
