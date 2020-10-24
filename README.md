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

