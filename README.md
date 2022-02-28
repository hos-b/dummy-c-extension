# Introduction
this repo demonstrates a basic C <-> Python interface. it exposes C types in Python:

* C struct <--> Python class
* C basic type <--> Python basic type
* C array <--> NumPy array
* C get/set function <--> Python @property

* C error handling <--> Python exception class
* C API <--> Python capsule

# Build & Install
## windows
add mingw64/bin to `PATH` and run:
```bash
python setup.py build -c mingw32
python setup.py install
```

## linux
TBA