 #pragma once

#include <python2.7/Python.h>

#define MCP23017_IOCON_BANK0 0x0A
#define MCP23017_IOCON_BANK1 0x15
// for Bank 1
#define MCP23017_GPIOA 0x09
#define MCP23017_GPIOB 0x19
#define MCP23017_IODIRB 0x10

#define BUTTON_SELECT 0
#define BUTTON_RIGHT 1
#define BUTTON_DOWN 2
#define BUTTON_UP 3
#define BUTTON_LEFT 4

#define LED_OFF 0x00
#define LED_ON 0x01
#define LED_RED (0x02 | ON)
#define LED_GREEN (0x04 | ON)
#define LED_BLUE (0x08 | ON)
#define LED_YELLOW ((RED | GREEN) | ON)
#define LED_TEAL ((GREEN | BLUE) | ON)
#define LED_VIOLET ((RED | BLUE) | ON)
#define LED_WHITE ((RED | GREEN | BLUE) | ON)

// LCD commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// LCD control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// Entry modes
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Shifting
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

#define PYCALL(name) PyObject_CallObject(LCDPlate::_pFunc_##name
#define ATTR ,
#define ARGS Py_BuildValue
#define ZEROARG NULL
#define ENDCALL )

namespace LCDPlate
{
    extern "C" int initialize();

    void finalize();

    void message(char* text);

    void writeb(char b);

    void writec(char b);

    void writestr(char* str);

    void begin(int cols, int lines);

    void stop();

    void clear();

    void home();
    
    void setCursor(int col, int row);

    void display();

    void noDisplay();

    void cursor();

    void noCursor();

    void toggleCursor();

    void blink();

    void noBlink();

    void toggleBlink();

    void scrollDisplayLeft();

    void scrollDisplayRight();

    void leftToRight();

    void rightToLeft();

    void autoscroll();

    void noAutoscroll();

    void createChar(int location, unsigned char* bitmap);

    void backlight(int color);

    int buttonPressed(int button);

    int buttons();

    void pos(int line, int shift);

    PyObject* lcd;

    PyObject* _pFunc_write;

    PyObject* _pFunc_begin;

    PyObject* _pFunc_stop;

    PyObject* _pFunc_clear;

    PyObject* _pFunc_home;

    PyObject* _pFunc_setCursor;

    PyObject* _pFunc_display;

    PyObject* _pFunc_noDisplay;

    PyObject* _pFunc_cursor;

    PyObject* _pFunc_noCursor;

    PyObject* _pFunc_toggleCursor;

    PyObject* _pFunc_blink;

    PyObject* _pFunc_noBlink;

    PyObject* _pFunc_toggleBlink;

    PyObject* _pFunc_scrollDisplayLeft;

    PyObject* _pFunc_scrollDisplayRight;

    PyObject* _pFunc_leftToRight;

    PyObject* _pFunc_rightToLeft;

    PyObject* _pFunc_autoscroll;

    PyObject* _pFunc_noAutoscroll;

    PyObject* _pFunc_createChar;

    PyObject* _pFunc_message;

    PyObject* _pFunc_backlight;

    PyObject* _pFunc_buttonPressed;

    PyObject* _pFunc_buttons;
}

//

extern "C" int LCDPlate::initialize()
{
    Py_Initialize();

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
    PyRun_SimpleString("sys.path.append('/root/proj/mixed')");
    
    PyObject* pModule = PyImport_ImportModule("Adafruit_CharLCDPlate");
    if(!pModule)
        return -2;
    
    PyObject* pFunc = PyObject_GetAttrString(pModule, "NewLCD");
    if(!pFunc)
        return -3;

    PyObject* pObject = PyEval_CallObject(pFunc, NULL);
    if(!pObject)
        return -4;

    // PyObject* pDict = PyModule_GetDict(pModule);
    // if(!pDict)
    //     return -3;
            
    // PyObject* pClass = PyDict_GetItemString(pDict, "Adafruit_CharLCDPlate");
    // if(!pClass)
    //     return -4;

    // PyObject* pObject = PyInstance_New(pClass, NULL, NULL);
    // if(!pObject)
    //     return -5;
    
    LCDPlate::lcd = pObject;

    LCDPlate::_pFunc_autoscroll         = PyObject_GetAttrString(pObject, "autoscroll");
    LCDPlate::_pFunc_backlight          = PyObject_GetAttrString(pObject, "backlight");
    LCDPlate::_pFunc_begin              = PyObject_GetAttrString(pObject, "begin");
    LCDPlate::_pFunc_blink              = PyObject_GetAttrString(pObject, "blink");
    LCDPlate::_pFunc_buttonPressed      = PyObject_GetAttrString(pObject, "buttonPressed");
    LCDPlate::_pFunc_buttons            = PyObject_GetAttrString(pObject, "buttons");
    LCDPlate::_pFunc_clear              = PyObject_GetAttrString(pObject, "clear");
    LCDPlate::_pFunc_createChar         = PyObject_GetAttrString(pObject, "createChar");
    LCDPlate::_pFunc_cursor             = PyObject_GetAttrString(pObject, "cursor");
    LCDPlate::_pFunc_display            = PyObject_GetAttrString(pObject, "display");
    LCDPlate::_pFunc_home               = PyObject_GetAttrString(pObject, "home");
    LCDPlate::_pFunc_leftToRight        = PyObject_GetAttrString(pObject, "leftToRight");
    LCDPlate::_pFunc_message            = PyObject_GetAttrString(pObject, "message");
    LCDPlate::_pFunc_noAutoscroll       = PyObject_GetAttrString(pObject, "noAutoscroll");
    LCDPlate::_pFunc_noBlink            = PyObject_GetAttrString(pObject, "noBlink");
    LCDPlate::_pFunc_noCursor           = PyObject_GetAttrString(pObject, "noCursor");
    LCDPlate::_pFunc_noDisplay          = PyObject_GetAttrString(pObject, "noDisplay");
    LCDPlate::_pFunc_rightToLeft        = PyObject_GetAttrString(pObject, "rightToLeft");
    LCDPlate::_pFunc_scrollDisplayLeft  = PyObject_GetAttrString(pObject, "scrollDisplayLeft");
    LCDPlate::_pFunc_scrollDisplayRight = PyObject_GetAttrString(pObject, "scrollDisplayRight");
    LCDPlate::_pFunc_setCursor          = PyObject_GetAttrString(pObject, "setCursor");
    LCDPlate::_pFunc_stop               = PyObject_GetAttrString(pObject, "stop");
    LCDPlate::_pFunc_toggleBlink        = PyObject_GetAttrString(pObject, "ToggleBlink");
    LCDPlate::_pFunc_toggleCursor       = PyObject_GetAttrString(pObject, "ToggleCursor");
    LCDPlate::_pFunc_write              = PyObject_GetAttrString(pObject, "write");

    return Py_IsInitialized();
}

void LCDPlate::pos(int line, int shift)
{
    int hx = line == 0 ? 0x80 : 0xC0;
    int lx = shift & 0x0F;
    LCDPlate::writeb(hx | lx);
}

void LCDPlate::message(char* text)
{
    PYCALL(message) ATTR ARGS("s", text) ENDCALL;
}

void LCDPlate::writeb(char b)
{
    PYCALL(write) ATTR ARGS("ci", b, 0) ENDCALL;
}

void LCDPlate::writec(char b)
{
    PYCALL(write) ATTR ARGS("ci", b, 1) ENDCALL;
}

void LCDPlate::writestr(char* str)
{
    PYCALL(write) ATTR ARGS("si", str, 1) ENDCALL;
}

void LCDPlate::begin(int cols, int lines)
{
    PYCALL(write) ATTR ARGS("ii", cols, lines) ENDCALL;
}

void LCDPlate::stop()
{
    PYCALL(stop) ATTR ZEROARG ENDCALL;
}

void LCDPlate::clear()
{
    PYCALL(clear) ATTR ZEROARG ENDCALL;
}

void LCDPlate::home()
{
    PYCALL(home) ATTR ZEROARG ENDCALL;
}

void LCDPlate::setCursor(int col, int row)
{
    PYCALL(setCursor) ATTR ARGS("ii", col, row) ENDCALL;
}

void LCDPlate::display()
{
    PYCALL(display) ATTR ZEROARG ENDCALL;
}

void LCDPlate::noDisplay()
{
    PYCALL(noDisplay) ATTR ZEROARG ENDCALL;
}

void LCDPlate::cursor()
{
    PYCALL(cursor) ATTR ZEROARG ENDCALL; 
}

void LCDPlate::noCursor()
{
    PYCALL(noCursor) ATTR ZEROARG ENDCALL;
}

void LCDPlate::toggleCursor()
{
    PYCALL(toggleCursor) ATTR ZEROARG ENDCALL;
}

void LCDPlate::blink()
{
    PYCALL(blink) ATTR ZEROARG ENDCALL;
}

void LCDPlate::noBlink()
{
    PYCALL(noBlink) ATTR ZEROARG ENDCALL;
}

void LCDPlate::toggleBlink()
{
    PYCALL(toggleBlink) ATTR ZEROARG ENDCALL;
}

void LCDPlate::scrollDisplayLeft()
{
    PYCALL(scrollDisplayLeft) ATTR ZEROARG ENDCALL;
}

void LCDPlate::scrollDisplayRight()
{
    PYCALL(scrollDisplayRight) ATTR ZEROARG ENDCALL;
}

void LCDPlate::leftToRight()
{
    PYCALL(leftToRight) ATTR ZEROARG ENDCALL;
}

void LCDPlate::rightToLeft()
{
    PYCALL(rightToLeft) ATTR ZEROARG ENDCALL;
}

void LCDPlate::autoscroll()
{
    PYCALL(autoscroll) ATTR ZEROARG ENDCALL;
}

void LCDPlate::noAutoscroll()
{
    PYCALL(noAutoscroll) ATTR ZEROARG ENDCALL;
}

void LCDPlate::createChar(int location, unsigned char* bitmap)
{
    LCDPlate::writeb(LCD_SETCGRAMADDR | ((location & 7) << 3));
	for(int i = 0; i < 8; i++)
		LCDPlate::writec(bitmap[i]);
	LCDPlate::writeb(LCD_SETDDRAMADDR);
    // PYCALL(createChar) ATTR ARGS("iO", location, Py_BuildValue(
    //     "(bbbbbbbb)",
    //     bitmap[0], bitmap[1], bitmap[2], bitmap[3], bitmap[4], bitmap[5], bitmap[6], bitmap[7]
    // )) ENDCALL;
}

void LCDPlate::backlight(int color)
{
    PYCALL(backlight) ATTR ARGS("i", color) ENDCALL;
}

int LCDPlate::buttonPressed(int button)
{
    return (LCDPlate::buttons() >> button) & 1;
    //return PyInt_AsLong(PYCALL(buttonPressed) ATTR ARGS("i", button) ENDCALL);
}

int LCDPlate::buttons()
{
    return PyInt_AsLong(PYCALL(buttons) ATTR ZEROARG ENDCALL);
}

void LCDPlate::finalize()
{
    Py_Finalize();
}