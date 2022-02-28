#define PY_SSIZE_T_CLEAN
#include <Python.h>
#define DUMMY_MODULE_INCLUDE
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

// dummy.print
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

static PyMethodDef DummyMethods[] = {
    {"print",  dummy_print, METH_VARARGS, "prints a string object."},
    {NULL, NULL, 0, NULL} // sentinel
};

static struct PyModuleDef dummy_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "dummy",
    .m_doc = "Example module that creates an extension type.", // doc
    .m_size = -1,
    .m_methods = DummyMethods
};

static PyTypeObject DummyType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "dummy.Dummy",
    .tp_doc = "Dummy class",
    .tp_basicsize = sizeof(struct Dummy),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
};

PyMODINIT_FUNC
PyInit_dummy(void)
{
    // create module
    PyObject *module;
    module = PyModule_Create(&dummy_module);
    if (module == NULL) {
        return NULL;
    }
    // add custom type
    if (PyType_Ready(&DummyType) < 0) {
        return NULL;
    }
    Py_INCREF(&DummyType);
    if (PyModule_AddObject(module, "Dummy", (PyObject *)&DummyType) < 0) {
        Py_DECREF(&DummyType);
        Py_DECREF(module);
        return NULL;
    }
    // add capsule
    static void *PyDummy_API[PyDummy_API_Pointers];
    PyObject *c_api_object;
    PyDummy_API[PyDummy_Print_INDEX] = (void*)PyDummy_Print;
    c_api_object = PyCapsule_New((void *)PyDummy_API, "dummy._C_API", NULL);
    if (PyModule_AddObject(module, "_C_API", c_api_object) < 0) {
        Py_XDECREF(c_api_object);
        Py_DECREF(module);
        return NULL;
    }   
    // add custom exception class
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


#ifdef __cplusplus
} // end of extern "C"
#endif
