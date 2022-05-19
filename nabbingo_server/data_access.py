import json, pathlib

def get_data_from_file(datafile: str):
    with open(datafile, 'r') as f:
        data = f.read()
        return json.loads(data)

def write_data_to_file(datafile:str, data):
    with open(datafile, 'w') as f:
        f.write(json.dumps(data, indent=2))

def create_data_file(datafile:str, overwrite=False):
    file_path = pathlib.Path(datafile)
    try:
        file_path.touch(0o666, exist_ok=overwrite)
        write_data_to_file(datafile, {'status': 0})
    except:
        print("data file already exists: Not creating.")