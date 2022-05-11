from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/', methods=['GET'])
def index():
    with open('tmp/dustbin.json', 'r') as f:
        data = f.read()
        dustbin = json.loads(data)
        return jsonify(dustbin)

@app.route('/update', methods=['POST'])
def update_status():
    new_info = json.loads(request.data)
    with open('tmp/dustbin.json', 'r') as f:
        data = f.read()
        dustbin = json.loads(data)

        dustbin['status'] = new_info['status']

    with open('tmp/dustbin.json', 'w') as f :
        f.write(dustbin.dumps(dustbin, indent=2))

app.run(debug=True)
