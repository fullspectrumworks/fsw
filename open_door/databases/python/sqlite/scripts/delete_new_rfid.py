#################################################################
#                     Delete New RFID Script                    #
#                      James Scott McDowell                     #
#################################################################
#                          Description                          #
#  Deletes an rfid number from the new_rfid staging table       #
#  Based on code found in The Python Standard Library section   #
#  11.13.                                                       #
#################################################################
import sqlite3
conn = sqlite3.connect('new_rfid.db')
c = conn.cursor()
c.execute("DELETE FROM new_rfid WHERE row_number = 1")
conn.commit()
conn.close()