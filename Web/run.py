#app.py

from flask import Flask, request, render_template #import main Flask class and request object
import sqlite3
import json
from os import path

ROOT = path.dirname(path.realpath(__file__))


app = Flask(__name__) #create the Flask app

@app.route("/")
def hello():
    #conn = sqlite3.connect('games.db')
    conn = sqlite3.connect(path.join(ROOT, "games.db"))
    cur = conn.cursor()
    query = "SELECT * from gamesTBL"
    cur.execute(query)
    lastTen = cur.fetchmany(10)
    query = "SELECT * from playerTBL"
    cur.execute(query)
    topTen = cur.fetchmany(10)
    conn.close()
    return render_template("template.html", tbl1=lastTen, tbl2=topTen)

    #return render_template('template.html', user=user_details)
    #return "Hello World!"

@app.route('/receive-data', methods=['POST']) #GET requests will be blocked
def json_example():
    text = request.data
    print(text)
    #json_acceptable = content2.replace("'", "\"")
    req_data = json.loads(text)
    #req_data = request.get_json()
    name = None
    kills = None
    deaths = None
    assists = None
    if 'kills' in req_data:
        name = req_data['name']
        kills = req_data['kills']
        deaths = req_data['deaths']
        assists = req_data['assists']

    conn = sqlite3.connect(path.join(ROOT, "games.db"))
    cur = conn.cursor()
    cur.execute("INSERT INTO gamesTBL (name, kills, deaths, assists) VALUES (?, ?, ?, ?)", (name, kills, deaths, assists))
    conn.commit()
    conn.close()
    return '''
           Kills: {}
           Deaths: {}
           Assists: {}'''.format(kills, deaths, assists)

if __name__ == '__main__':
    app.run()
    #app.run(debug=True, port=5000) #run app in debug mode on port 5000
