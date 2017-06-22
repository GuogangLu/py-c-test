#include <python3.4/Python.h>

static PyObject *module;
static PyObject *remain=NULL;
static PyObject *SpamError;
static PyObject* spam_system(PyObject *self, PyObject *args)
{
    const char *command=NULL;
    const char *teststr=NULL;
    FILE *fp;
    int sts;
    Py_ssize_t size = PyObject_Size(args);
    PyObject *attr;
    PyObject *fun;
    
    printf("size=%d\n", size);
    if(!PyArg_ParseTuple(args, "s|s|O|O", &command, &teststr, &attr, &fun)){
        return NULL;
    }
    remain = attr;
    if(!PyArg_Parse(PyObject_Str(attr), "s", &teststr)){
        return NULL;
    }
    //if(!PyArg_ParseTuple(args+1, "s", &teststr)){
    //    return NULL;
    //}
    PyObject_CallFunctionObjArgs(fun, NULL);
    printf("teststr: %s\n", teststr);
    sts = system(command);
    if(sts<0){
        PyErr_SetString(SpamError, "system command failed");
        return NULL;
    }

    return Py_BuildValue("i", sts);
}

static PyObject* spam_remain(PyObject *self, PyObject *args)
{
    const char *vs;
    PyObject *m;

    m = PyModule_GetDict(module);
    if(m){
        if(!PyArg_Parse(PyObject_Str(m), "s", &vs)){
            return NULL;
        }
        printf("dict: %s\n", vs);
    }

    return m;
}

static PyMethodDef SpamMethods[] = {
    {"system",spam_system, METH_VARARGS, "Execute a shell command"},
    {"remain",spam_remain, METH_VARARGS, "Execute a shell command"},
    {NULL,NULL,0,NULL}
};

static struct PyModuleDef spammodule={
    PyModuleDef_HEAD_INIT,
    "spam",
    "spam_doc",
    -1,
    SpamMethods
};

PyMODINIT_FUNC
PyInit_spam(void)
{
    module = PyModule_Create(&spammodule);
    if(module!=NULL){
        SpamError = PyErr_NewException("spam.error", NULL, NULL);
        Py_INCREF(SpamError);
        PyModule_AddObject(module, "error", SpamError);
    }
    return module;
}
