# Schrodinger's Hack 2020

![Thruster example](https://github.com/sxrodrigues/physhackathon2020/blob/main/truster.gif)

Our goal: To use generative design to optimize rocket thruster geometry. Above is an example of how the fuel of a thruster shoots out and generates thrust.
Below is an eneration of different rocket geometries that slowly get better and better and generating thrust using a genetic algorithm (fuel particles not shown).

## Dependencies
Before you can do anything, you need to install the dependencies of this project.
```sh
pip3 install -r requirements.txt
```
The last depedency you need is [pybind11](https://github.com/pybind/pybind11). If you are running macOS and have [Homebrew](https://brew.sh/) or are on Linux and have [Linuxbrew](https://docs.brew.sh/Homebrew-on-Linux), you can install pybind11 directly with that:
```sh
brew install pybind11
```
Otherwise, the simplest method is to install pybind11 via pip:
```sh
pip3 install "pybind11[global]"
```
If you get errors down the line, you cna uninstall this package and instead:
```sh
pip3 install pybind11
```
but the above command (the one with `"pybind11[global]"`) should work.

## Building

You can now build the simulator module (you must be in the root directory of this repo):
```sh
pip3 install .
```

## Running

### Matplotlib examples
There are two examples you may run if you wish. The first one, located at [`src/main.py`](https://github.com/sxrodrigues/physhackathon2020/blob/main/src/main.py), will launch a simulation of a bunch of particles in a predefined thruster. This is to show what the particle simulator is capable of.
```sh
python3 src/main.py
```

The second one, located at [`src/ga.py`](https://github.com/sxrodrigues/physhackathon2020/blob/main/src/ga.py) shows off the genetic algorithm. This will launch a matplotlib window with the best thurster design. It will update rather slowly (~1-10 seconds), so if it is taking a little while to show something at first, don't panic.
```sh
python src/ga.py
```

### Webapp
We also have created as webapp for you to see some of the code running live on the browser. You can run the server locally or [visit the webpage](http://genzthrusters.pythonanywhere.com/) where a server is already running for you.

If you wish to run the server locally, here are some instructions:

First you're going to need [django](https://www.djangoproject.com/):
```sh
pip3 install django
```

Thenl, from the root directory of the repo you can launch a dev server:
```sh
cd web_app
python3 manage.py runserver
```

You should now be able to naviaget to [http://localhost:8000/](http://localhost:8000/) to see the server running.
