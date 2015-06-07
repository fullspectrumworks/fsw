#################################################################
#                      Insert Person Script                     #
#                      James Scott McDowell                     #
#################################################################
#                          Description                          #
#  A test script to insert a new user into the people table     #
#  Based on code found in The Python Standard Library section   #
#  11.13.                                                       #
#################################################################
import sqlite3
conn = sqlite3.connect('people.db')
c = conn.cursor()
c.execute("INSERT INTO people VALUES (0,0,1234,'0F030387CA','Doe','John','username','01/01/1990')")
conn.commit()
conn.close()