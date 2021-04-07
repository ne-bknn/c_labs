#if defined(__clang_)
#include <Python.h>
#else
#include </usr/include/python3.9/Python.h>
#endif

static PyObject* helloworld(PyObject* self) {
   return Py_BuildValue("s", "Hello, Python extensions!!");
}

static char helloworld_docs[] =
   "helloworld( ): Any message you want to put here!!\n";

static PyMethodDef module_methods[] = {
   {"helloworld", (PyCFunction)helloworld, 
      METH_NOARGS, helloworld_docs},
      {NULL}
};

static struct PyModuleDef Helloworld = {
	PyModuleDef_HEAD_INIT,
	"helloworld",
	"usage: you know",
	-1,
	module_methods
};

PyMODINIT_FUNC PyInit_pystrassen(void) {
	return PyModule_Create(&Helloworld);
}
