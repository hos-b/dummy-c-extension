#define PY_SSIZE_T_CLEAN
#include <Python.h>
#define DUMMY_MODULE
#include <dummy_header.h>

#ifdef __cplusplus
extern "C" {
#endif

static PyObject *DummyError;

// C-API function
static void
PyDummy_Print(const char* str) {
    printf("dummy module says: %s\n", str);
}

// Python function
static PyObject *
dummy_print(PyObject *self, PyObject *args)
{
    const char *str;
    if (!PyArg_ParseTuple(args, "s", &str)) {
        return NULL;
    } else if (strlen(str) == 0) {
        PyErr_SetString(DummyError, "empty string passed as argument.");
        return NULL;
    }
    PyDummy_Print(str);
    Py_RETURN_NONE;
}

PyMODINIT_FUNC
PyInit_dummy(void)
{
    // create module
    PyObject *module;
    module = PyModule_Create(&dummy_module);
    if (module == NULL) {
        return NULL;
    }
    // add capsule
    static void *PyDummy_API[PyDummy_API_Pointers];
    PyObject *c_api_object;
    PyDummy_API[PyDummy_Print_INDEX] = (void*)PyDummy_Print;
    c_api_object = PyCapsule_New((void *)PyDummy_API, "spam._C_API", NULL);
    if (PyModule_AddObject(module, "_C_API", c_api_object) < 0) {
        Py_XDECREF(c_api_object);
        Py_DECREF(module);
        return NULL;
    }   
    // add custom exception
    DummyError = PyErr_NewException("dummy.error", NULL, NULL);
    Py_XINCREF(DummyError);
    if (PyModule_AddObject(module, "error", DummyError) < 0) {
        Py_XDECREF(DummyError);
        Py_CLEAR(DummyError);
        Py_DECREF(module);
        return NULL;
    }

    return module;
}

static PyMethodDef DummyMethods[] = {
    {"print",  dummy_print, METH_VARARGS, "prints a string object."},
    {NULL, NULL, 0, NULL} // sentinel
};

static struct PyModuleDef dummy_module = {
    PyModuleDef_HEAD_INIT,
    "dummy",
    NULL,
    -1,
    DummyMethods
};

#ifdef __cplusplus
} // end of extern "C"
#endif
