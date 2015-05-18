#################################################################
#                      Door Control Script                      #
#                      James Scott McDowell                     #
#################################################################
#                          Description                          #
#  The server-side python script that validates data coming in  #
#  from the client-side door system.  If the user is granted    #
#  access to the building then a successful control byte is     #
#  returned, if not, then an unsuccessful control byte is       #
#  returned.  Additionally, this script checks for new RFID     #
#  entries.  If the RFID exists, then the program continues to  #
#  operate normally, if not, then the program adds a new RFID   #
#  number into the staging database. (new_rfid.db)              #
#  Based on code found in The Python Standard Library section   #
#  11.13. and the Quickstart section of the Flask web server    #
#  documentation website.                                       #
#################################################################
from flask import Flask, request
import sqlite3
app = Flask(__name__)

def access_permission_query(rfid, pin):
	hasAccess = "0"
	conn = sqlite3.connect('../database/tables/people.db')
	c = conn.cursor()
	c.execute("SELECT access_permission FROM people WHERE rfid='" + rfid + "' AND pin=" + pin)
	hasAccess = str(c.fetchone())
	hasAccess = hasAccess[1:-2]

	if(hasAccess != "1"):
		hasAccess = "0"

	conn.close()
	return hasAccess

def is_new_person(rfid, pin):
	conn = sqlite3.connect('../database/tables/people.db')
	c = conn.cursor()
	c.execute("SELECT * FROM people WHERE rfid='" + rfid + "' AND pin=" + pin)
	record = str(c.fetchone())
	conn.close()

	if(record != "None"):
		return False
	elif(record == "None"):
		conn = sqlite3.connect('../database/tables/new_rfid.db')
		c = conn.cursor()
		c.execute("INSERT INTO new_rfid VALUES (1,'" + rfid + "')")
		conn.commit()
		conn.close()
		print "New RFID added to database."
		return True

@app.route("/", methods=["POST"])
def access_request():
	if(request.method == "POST"):
		post_request = request.get_data()
		post_request_array = post_request.split(",");
		rfid = post_request_array[0];
		pin = post_request_array[1];
		is_new_person(rfid, pin)
		return access_permission_query(rfid, pin)

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=100, debug=True)