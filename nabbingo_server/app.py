from flask import Flask, request, jsonify, send_file, render_template
import json
from data_access import get_data_from_file, write_data_to_file, create_data_file

DATAFILE = '/tmp/dustbin.json'

create_data_file(DATAFILE, True)

app = Flask(__name__)

@app.route('/', methods=['GET'])
def get_status():
    dustbin = get_data_from_file(DATAFILE)
    
    return jsonify(dustbin)

@app.route('/update', methods=['POST'])
def update_status():
    new_info = json.loads(request.data)
    dustbin = get_data_from_file(DATAFILE)

    dustbin['status'] = new_info['status']

    write_data_to_file(DATAFILE, dustbin)
    return jsonify(dustbin)

@app.route('/refresh_status', methods=['GET'])
def refresh_status():
    data = get_data_from_file(DATAFILE)
    data['status'] = 0

    write_data_to_file(DATAFILE, data)
    return jsonify(data)

@app.route('/index')
def index():
    dustbin = get_data_from_file(DATAFILE)
    return render_template("index.html", bin_status=int(dustbin["status"]))

if __name__ == "__main__":
    import sys
    app.run(debug=True, port=8080)
