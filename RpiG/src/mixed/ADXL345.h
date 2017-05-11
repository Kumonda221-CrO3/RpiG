#pragma once

#include <python2.7/Python.h>

#define ADXL345_ADDRESS             0x53

#define ADXL345_REG_DEVID           0x00
#define ADXL345_REG_DATAX0          0x32
#define ADXL345_REG_POWER_CTL       0x2D

#define ADXL345_DATARATE_0_10_HZ    0x00
#define ADXL345_DATARATE_0_20_HZ    0x01
#define ADXL345_DATARATE_0_39_HZ    0x02
#define ADXL345_DATARATE_0_78_HZ    0x03
#define ADXL345_DATARATE_1_56_HZ    0x04
#define ADXL345_DATARATE_3_13_HZ    0x05
#define ADXL345_DATARATE_6_25HZ     0x06
#define ADXL345_DATARATE_12_5_HZ    0x07
#define ADXL345_DATARATE_25_HZ      0x08
#define ADXL345_DATARATE_50_HZ      0x09
#define ADXL345_DATARATE_100_HZ     0x0A
#define ADXL345_DATARATE_200_HZ     0x0B
#define ADXL345_DATARATE_400_HZ     0x0C
#define ADXL345_DATARATE_800_HZ     0x0D
#define ADXL345_DATARATE_1600_HZ    0x0E
#define ADXL345_DATARATE_3200_HZ    0x0F

#define ADXL345_RANGE_2_G           0x00
#define ADXL345_RANGE_4_G           0x01
#define ADXL345_RANGE_8_G           0x02 
#define ADXL345_RANGE_16_G          0x03

namespace ADXL345 {
    int initialize();

    void finalize();

    void setRange(int range);

    int getRange();

    void setDataRate(int rate);

    int getDataRate();

    int* read();

    PyObject* _pFunc_setRange;

    PyObject* _pFunc_getRange;

    PyObject* _pFunc_setDataRate;

    PyObject* _pFunc_getDataRate;

    PyObject* _pFunc_read;
}

void ADXL345::setRange(int range)
{
    PyObject_CallObject(ADXL345::_pFunc_setRange, Py_BuildValue("i", range));
}

int ADXL345::getRange()
{
    return PyInt_AsLong(PyObject_CallObject(ADXL345::_pFunc_setRange, NULL));
}

int* ADXL345::read()
{
    int* res = new int[3];
    PyObject* list = PyObject_CallObject(ADXL345::_pFunc_read, NULL);
    for(int i = 0; i < 3; i++)
        res[i] = PyInt_AsLong(PyList_GetItem(list, i));
    return res;
}

void ADXL345::setDataRate(int rate)
{
    PyObject_CallObject(ADXL345::_pFunc_setDataRate, Py_BuildValue("i", rate));
}

int ADXL345::getDataRate()
{
    return PyInt_AsLong(PyObject_CallObject(ADXL345::_pFunc_getDataRate, NULL));
}

int ADXL345::initialize()
{
    Py_Initialize();

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
    PyRun_SimpleString("sys.path.append('/root/proj/mixed')");

    PyObject* pModule = PyImport_ImportModule("Adafruit_ADXL345");
    if(!pModule)
        return -2;
    
    PyObject* pFunc = PyObject_GetAttrString(pModule, "NewADXL");
    if(!pFunc)
        return -3;

    PyObject* pObject = PyEval_CallObject(pFunc, NULL);
    if(!pObject)
        return -4;

    ADXL345::_pFunc_getDataRate = PyObject_GetAttrString(pObject, "getDataRate");
    ADXL345::_pFunc_getRange    = PyObject_GetAttrString(pObject, "getRange");
    ADXL345::_pFunc_read        = PyObject_GetAttrString(pObject, "read");
    ADXL345::_pFunc_setDataRate = PyObject_GetAttrString(pObject, "setDataRate");
    ADXL345::_pFunc_setRange    = PyObject_GetAttrString(pObject, "setRange");

    return Py_IsInitialized();
}

void ADXL345::finalize()
{
    Py_Finalize();
}