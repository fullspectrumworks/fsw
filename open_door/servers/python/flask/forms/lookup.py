#################################################################
#                       Lookup Person Form                      #
#                      James Scott McDowell                     #
#################################################################
#                          Description                          #
#  Allows the admin to lookup a person from the people table.   #
#  Based on code found in The Python Standard Library section   #
#  11.13. and the Quickstart section of the Flask web server    #
#  documentation website.  Additional influences from the       #
#  "HTML Input Types" chapter of the HTML tutorial from         #
#  W3Schools.                                                   #
#################################################################
from flask import Flask, request
import sqlite3
import re
app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def lookup():
	if request.method == 'POST':
		conn = sqlite3.connect('../database/tables/people.db')
		c = conn.cursor()
		table = "<table><th colspan='7'>Members</th>"
		table += "<tr><td>Access Granted?</td><td>Present?</td><td>RFID</td><td>Last Name</td><td>First Name</td><td>Username</td><td>Birthdate</td></tr>"
		for row in c.execute("SELECT access_permission, presence_state, rfid, last_name, first_name, username, birthdate FROM people WHERE access_permission =" + request.form["access_permission"] + " OR presence_state =" + request.form["presence_state"] + " OR last_name='" + request.form["last_name"] + "' OR first_name ='" + request.form["first_name"] + "' OR username ='" + request.form["username"] + "' OR birthdate ='" + request.form["birthdate"] + "'"):
			temp_row = str(row)[1:-1]
			temp_row = re.split(", u|,", temp_row)
			temp_row[-1] = temp_row[-1].strip()
			table += "<tr>"
			for cell in temp_row:
				table += "<td>" + cell + "</td>"
			table += "</tr>"
		table += "</table>"
		conn.close()
		
		return """<html>
        <head>
            <style>
                table {
                    border-collapse: collapse;
                }

                table, td, th {
                    border: 1px solid black;
                }
            </style>
        </head>
        <body>""" + table + """
        </body>
    </html>"""
	else:
		return """
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>Lookup Page</title>
        <style>
            /*class that allows a div to act as a table*/
            div.table { display: table; }
            
            /*class that allows a div to act as a table row*/
            div.table > div{ display: table-row; }
            
            /*class that allows a div to act as a table division*/
            div.table > div > div { display: table-cell; padding: 15px;}
            
            body{ margin:0px; }
            
            /*the following classes control the styling of the header and its contents
              adjust the header's height to your needs
              modify the text alignment according to your needs
              remove the border styling since the border only serves as a demonstration*/
            header{
            	width:100%;
            	height:100px;
            	text-align:center;
            	border-style:dashed;
            	border-width:2px;
            }
            
            /*this element has a default margin, which allows the element to be distinguished between itself and the element above it.
              the margin must be set to zero in order for the header to flush with the top border of the page*/
            h1{
                font-size:30px;
                margin-top:0px;
            }
            
            h2{ font-size:20px; }
            
            /*controls the main section's appearance
              modify the padding according to your needs
              remove the border styling since the border only serves as a demonstration*/
            section{
            	width:100%;
            	padding:15px;
            	border-style:dashed;
            	border-width:1px;
            }
        </style>
    </head>
    <body>
        <header>
            <h1>[insert your custom header here]</h1>
            <h2>Lookup Page</h2>
        </header>
        <section>
            <form action="/" method="post">
                <div class="table">
                    <div>
                        <div>First Name</div><div><input type="text" name="first_name" pattern="[a-zA-Z ]*" title="Letters only please, use Roman numerals if you are the 2nd, 3rd, etc."></div>
                    </div>
                    <div>
                        <div>Last Name</div><div><input type="text" name="last_name" pattern="[a-zA-Z ]*" title="Letters only please, use Roman numerals if you are the 2nd, 3rd, etc."></div>
                    </div>
                    <div>
                        <div>Username</div><div><input type="text" name="username" pattern="[a-zA-Z0-9 ]*" title="Letters and numbers only please."></div>
                    </div>
                    <div>
                        <div>Birthdate</div><div><input type="date" name="birthdate" min="1900-01-01" max="1997-01-01" title="You are too old or too young."></div>
                    </div>
                    <div>
                        <div>Permitted Access?</div><div><input type="radio" name="access_permission" value="0" checked>No<input type="radio" name="access_permission" value="1">Yes<input type="radio" name="access_permission">N/A</div>
                    </div>
                    <div>
                        <div>Is Present?</div><div><input type="radio" name="presence_state" value="0" checked required>No<input type="radio" name="presence_state" value="1" required>Yes</div>
                    </div>
                    <div>
                        <div><input type="submit" value="submit"></div>
                    </div>
                </div>
            </form>
        </section>
    </body>
</html>"""

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=81, debug=True)