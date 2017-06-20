#include <python2.7/Python.h>

static PyObject *SpamError;
static PyObject* spam_system(PyObject *self, PyObject *args)
{
    const char *command=NULL;
    const char *teststr=NULL;
    int sts;
    Py_ssize_t size = PyObject_Size(args);
    PyObject *attr;
    PyObject *fun;
    
    printf("size=%d\n", size);
    if(!PyArg_ParseTuple(args, "s|s|O|O", &command, &teststr, &attr, &fun)){
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

    //return Py_BuildValue("i", sts);
    return PyLong_FromLong(sts);
}

static PyObject* spam_set_fun(PyObject *self, PyObject *args)
{
    
}

static PyMethodDef SpamMethods[] = {
    {"system",spam_system, METH_VARARGS, "Execute a shell command"},
    {NULL,NULL,0,NULL}
};

PyMODINIT_FUNC
initspam(void)
{
    PyObject *m;

    m = Py_InitModule("spam", SpamMethods);
    if(m==NULL){
        return ;
    }
    SpamError = PyErr_NewException("spam.error", NULL, NULL);
    Py_INCREF(SpamError);
    PyModule_AddObject(m, "error", SpamError);
}
