#include <python3.4/Python.h>

static PyObject *module;
static PyObject *remain=NULL;
static PyObject *SpamError;
static PyObject* spam_system(PyObject *self, PyObject *args)
{
    const char *command=NULL;
    const char *teststr=NULL;
    const char *dictstr=NULL;
    const char *argvstr=NULL;
    FILE *fp;
    int sts;
    Py_ssize_t size = PyObject_Size(args);
    PyObject *attr;
    PyObject *fun;
    PyObject *sys;
    PyObject *pyargv;
    PyObject *dict;
    
    printf("size=%d\n", size);
    //通过c导入外部模块测试
    sys = PyImport_ImportModule("sys");
    if(sys==NULL){
        printf("sys=NULL\n");
        return NULL;
    }
    printf("sys Ok\n");

    //通过属性读取argv
    pyargv = PyObject_GetAttrString(sys, "argv");
    if(pyargv==NULL){
        printf("pyargv=NULL\n");
        return NULL;
    }
    if(!PyList_Check(pyargv)){
        printf("pyargv not list\n");
        return NULL;
    }
    printf("pyargv ok\n");
    if(!PyArg_Parse(PyObject_Str(pyargv), "s", &argvstr)){
        return NULL;
    }
    printf("argvstr:\n%s\n", argvstr);

    //通过字典读取argv
    dict = PyModule_GetDict(sys);
    if(dict==NULL){
        printf("dict=NULL\n");
        return NULL;
    }
    if(!PyArg_Parse(PyObject_Str(dict), "s", &dictstr)){
        return NULL;
    }
    printf("dictstr:\n%s\n", dictstr);
    pyargv = PyDict_GetItemString(dict, "argv");
    if(pyargv==NULL){
        printf("pyargv=NULL\n");
        return NULL;
    }
    if(!PyList_Check(pyargv)){
        printf("pyargv not list\n");
        return NULL;
    }
    printf("pyargv ok\n");
    if(!PyArg_Parse(PyObject_Str(pyargv), "s", &argvstr)){
        return NULL;
    }
    printf("argvstr:\n%s\n", argvstr);

    //读取python脚本输入的参数，并转化为c格式参数
    if(!PyArg_ParseTuple(args, "s|s|O|O", &command, &teststr, &attr, &fun)){
        return NULL;
    }
    remain = attr;
    if(!PyArg_Parse(PyObject_Str(attr), "s", &teststr)){
        return NULL;
    }
    if(PyUnicode_Check(attr)){
        printf("attr is bytes\n");
    }
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
