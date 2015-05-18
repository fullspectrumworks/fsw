#################################################################
#                      Delete Person Form                       #
#                      James Scott McDowell                     #
#################################################################
#                          Description                          #
#  Allows the admin to delete a person from the people table.   #
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
		c.execute("SELECT first_name, last_name FROM people WHERE rfid = '" + request.form["rfid"] + "'")
		member = str(c.fetchone())
		c.execute("DELETE FROM people WHERE rfid = '" + request.form["rfid"] + "'")
		conn.commit()
		conn.close()

		return member + " removed from the database."
	else:
		return """
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>Delete Page</title>
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
            <h2>Delete Page</h2>
        </header>
        <section>
            <form action="/" method="post">
                <div class="table">
                    <div>
                        <div>RFID Code</div><div><input type="text" name="rfid" pattern="[0-9A-Z]*" title="Numbers and capital letters only please." required></div>
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
    app.run(host='0.0.0.0', port=83, debug=True)