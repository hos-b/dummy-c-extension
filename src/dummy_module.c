#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>
#define DUMMY_MODULE_INCLUDE
#include <dummy_header.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
// version string definition
#if !defined(MAJOR_VERSION) || !defined(MINOR_VERSION) || !defined(BUGFIX_VERSION)
#warning "warning: version strings are not defined. using 0.0.0 instead..."
#define VERSION_STRING "0.0.0"
#else
#define _STR(x) #x
#define STRINGIFY(x) _STR(x)
#define VERSION_STRING STRINGIFY(MAJOR_VERSION) "." STRINGIFY(MINOR_VERSION) "." STRINGIFY(BUGFIX_VERSION)
#endif

#define DUMMY_CAST(x) ((struct Dummy*)x)

// custom exception type
static PyObject *DummyError;

// C-API function
static void
PyDummy_Print(const char* str)
{
    printf("dummy module says: %s\n", str);
}
// dummy.print
static PyObject*
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
// dummy.__dealloc__
static void
Dummy_dealloc(struct Dummy* self)
{
    Py_TYPE(self)->tp_free((PyObject *) self);
}
// dummy.__new__
static PyObject*
Dummy_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    struct Dummy *self;
    self = (struct Dummy*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->rand_seed = 0;
        self->toss_count = 1;
        self->randint_min = INT_MIN;
        self->randint_max = INT_MAX;
        self->randfloat_min = __FLT_MIN__;
        self->randfloat_max = __FLT_MAX__;
    }
    return (PyObject*)self;
}
// dummy.__repr__
static PyObject *
Dummy_repr(PyObject *self)
{
    // this function does not support unicode from float!
    PyObject* float_min = PyFloat_FromDouble(DUMMY_CAST(self)->randfloat_min);
    PyObject* float_max = PyFloat_FromDouble(DUMMY_CAST(self)->randfloat_max);
    PyObject* unicode = PyUnicode_FromFormat(
        "Dummy data type:\nseed = %d, toss count = %d\n"
        "irand range[%i, %i]\nfrand range [%R, %R]",
        DUMMY_CAST(self)->rand_seed, DUMMY_CAST(self)->toss_count,
        DUMMY_CAST(self)->randint_min, DUMMY_CAST(self)->randint_max,
        float_min, float_max
    );
    Py_XDECREF(float_min);
    Py_XDECREF(float_max);
    return unicode;
}
// dummy.__init__
static int
Dummy_init(struct Dummy *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"seed", "toss_count",
                             "min_int", "max_int",
                             "min_float", "max_float",
                             NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "II|iiff", kwlist,
                                     &self->rand_seed, &self->toss_count,
                                     &self->randint_min, &self->randint_max,
                                     &self->randfloat_min, &self->randfloat_max)) {
        return -1;
    }
    if (self->toss_count == 0) {
        PyErr_SetString(DummyError, "cannot use 0 for toss_count");
        return -1;
    }
    srand(self->rand_seed);
    return 0;
}
// dummy general getter
static PyObject*
Dummy_getter(struct Dummy *self, void *closure)
{
    return Py_BuildValue("I", *(uint32_t*)((char*)self + (size_t)closure));
}
// dummy general setter
static int
Dummy_setter(struct Dummy *self, PyObject *arg, void *closure)
{
    uint32_t value;
    if (arg == NULL) {
        PyErr_SetString(PyExc_TypeError, "cannot delete class attributes.");
        return -1;
    }
    if (!PyArg_ParseTuple(arg, "I", &value)) {
        return -1;
    }
    *(uint32_t*)((char*)self + (size_t)closure) = value;
    return 0;
}

// module method definition
static PyMethodDef module_methods[] = {
    {"print",  dummy_print, METH_VARARGS, "prints a string object."},
    {NULL, NULL, 0, NULL} // sentinel
};
// module definition
static struct PyModuleDef dummy_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "dummy",
    .m_doc = "Example module that creates an extension type.", // doc
    .m_size = -1,
    .m_methods = module_methods
};
// custom type members
static struct PyMemberDef Dummy_members[] = {
    {"randi_min", T_INT,   offsetof(struct Dummy, randint_min),   0, "minimum for random ints"},
    {"randi_max", T_INT,   offsetof(struct Dummy, randint_max),   0, "maximum for random ints"},
    {"randf_min", T_FLOAT, offsetof(struct Dummy, randfloat_min), 0, "minimum for random floats"},
    {"randf_max", T_FLOAT, offsetof(struct Dummy, randfloat_max), 0, "maximum for random floats"},
    {NULL}  /* Sentinel */
};
// custom type functions
static PyMethodDef Dummy_methods[] = {
    // {"name", (PyCFunction) Custom_name, METH_NOARGS, "Return the name, combining the first and last name"},
    {NULL}  /* Sentinel */
};
// custom type getsetters
static PyGetSetDef Dummy_getsetters[] = {
    {"seed",       (getter)Dummy_getter, (setter)Dummy_setter, "random seed value", (void*)offsetof(struct Dummy, rand_seed)},
    {"toss_count", (getter)Dummy_getter, (setter)Dummy_setter, "random toss count", (void*)offsetof(struct Dummy, toss_count)},
    {NULL}  /* Sentinel */
};
// custom type definition
static PyTypeObject DummyType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "dummy.Dummy",
    .tp_doc = "Dummy class",
    .tp_basicsize = sizeof(struct Dummy),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = Dummy_new,
    .tp_init = (initproc)Dummy_init,
    .tp_dealloc = (destructor)Dummy_dealloc,
    .tp_members = Dummy_members,
    .tp_methods = Dummy_methods,
    .tp_getset  = Dummy_getsetters,
    .tp_repr    = Dummy_repr,
    .tp_str     = Dummy_repr,
};

// module initialization function
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
    // add string to module
    if (PyModule_AddStringConstant(module, "__version__", VERSION_STRING) < 0) {
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


#undef _STR
#undef STRINGIFY