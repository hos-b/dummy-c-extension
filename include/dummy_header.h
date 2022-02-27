#ifndef __DUMMY_HEADER__
#define __DUMMY_HEADER__

#ifdef __cplusplus
extern "C" {
#endif



// C-API defs
#define PyDummy_API_Pointers 1
#define PyDummy_Print_INDEX 0
#define PyDummy_Print_RETURN void
#define PyDummy_Print_PROTO (const char* str)

#ifdef DUMMY_MODULE
static PyDummy_Print_RETURN PyDummy_Print PyDummy_Print_PROTO;
#else
static void ** PyDummy_API;
#define PyDummy_Print \
 (*(PyDummy_Print_RETURN (*)PyDummy_Print_PROTO) PyDummy_API[PyDummy_Print_INDEX])
static int
import_dummy(void) {
    PyDummy_API = (void **)PyCapsule_Import("dummy._C_API", 0);
    return (PyDummy_API != NULL) ? 0 : -1;
}
#endif

struct Dummy {

};

#ifdef __cplusplus
} // end of extern "C"
#endif

#endif