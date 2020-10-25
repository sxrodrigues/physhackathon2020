# Schrodinger's Hack 2020

## Building and Running
First, make sure you have [pybind11](https://github.com/pybind/pybind11) installed. You should be able to do so with pip:
```sh
pip3 install pybind11
```

Then, you can build the simulator module (you must be in the root directory of this repo):
```sh
pip3 install .
```

Finally, you can test the code to make sure it works:
```sh
python3 src/main.py
```
This should print the value 3 to the console.


## Running the WebApp

First to install Django run:
```sh
pip3 install Django
```

Then enter the "web_app" directory and run:
```sh
python3 manage.py runserver
```

That should run the web application in your browser as a local host.

