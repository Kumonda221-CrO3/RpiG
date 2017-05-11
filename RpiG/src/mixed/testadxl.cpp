#include <stdio.h>
#include "ADXL345.h"
#include "lcdplate.h"
#include "wiringPi.h"
#include "simpmath.h"
#include "samp.h"
#include "graph.h"

#define FREQ_1HZ 1
#define FREQ_2HZ 2
#define FREQ_4HZ 4
#define FREQ_5HZ 5
#define FREQ_10HZ 10
#define FREQ_20HZ 20

#define UNIT_MG 0
#define UNIT_MPSFLAT 1
#define UNIT_CMPSFLAT 2
#define UNIT_G 3

typedef void (* Callback)(int);

Callback detect_callback;

Callback update;

char* fix_symbol[6] = {
	"%.0f", "%.1f", "%.2f", "%.3f", "%.4f", "%.5f"
};

void display_mainpage();

void callback_mainpage(int button);

void mode_AXIS_G();

int update_count;

int fix = 1;

Sample_t* last_samp;

void printUnit(int edge = 1)
{
	if(edge)
		LCDPlate::writec('(');
	LCDPlate::writestr("m/s");
	printFlat();
	if(edge)
		LCDPlate::writec(')');
}

void newpage()
{
	LCDPlate::clear();
	LCDPlate::home();
}

void newline()
{
	LCDPlate::writeb(0xC0);
}

int allow_update()
{
	return 1;
}

void backToMainPage()
{
	display_mainpage();
	update = 0;
	detect_callback = callback_mainpage;
}

void display_mode_AXIS_G()
{
	newpage();
	LCDPlate::writestr("U:AxisX  D:AxisY");
	newline();
	LCDPlate::writestr("L:AxisZ  S:Menu");
}

int $um_AXIS_G;

float* umAGD_read(int mode)
{
	double v;
	char pref;
	switch(mode)
	{
	case -1:
		newpage();
		LCDPlate::writestr("ERROR: -1");
		return 0;

	case BUTTON_UP: // X
		v = -last_samp->x;
		pref = 'X';
		break;

	case BUTTON_DOWN: // Y
		v = -last_samp->y;
		pref = 'Y';
		break;

	case BUTTON_LEFT: // Z
		v = -last_samp->z;
		pref = 'Z';
		break;
	}

	float fV = ((float) v);
	//float aV2 = fV / 110;
	float aV2 = fV * 9.80665;
	v = aV2;

	float* r = new float[2];
	r[0] = v;
	r[1] = pref;
	return r;
}

int $cmAGD_has_const;

float $cmAGD_const;

void update_mode_AXIS_G_DISPLAY(int unused)
{
	if(!allow_update())
		return;

	char* val = new char[16];
	float* v = umAGD_read($um_AXIS_G);
	char pref = (char) v[1];

	newpage();
	LCDPlate::writec(pref);
	LCDPlate::writestr(": ");

	sprintf(val, fix_symbol[fix], $cmAGD_has_const == 1 ? v[0] - $cmAGD_const : v[0]);
	LCDPlate::writestr(val);
	newline();
	if($cmAGD_has_const)
		LCDPlate::writestr("CST");
	else
		LCDPlate::writestr("   ");
	LCDPlate::pos(1, 9);
	printUnit();

	delete[] v;
	delete[] val;
}

void callback_mode_AXIS_G_DISPLAY(int button)
{
	switch(button)
	{
	case BUTTON_SELECT:
		$cmAGD_has_const = 0;
		mode_AXIS_G();
		break;
	case BUTTON_DOWN:
		if(!$cmAGD_has_const)
		{
			$cmAGD_const = umAGD_read($um_AXIS_G)[0];
			$cmAGD_has_const = 1;
		}
		else
			$cmAGD_has_const = 0;
		break;
	}
}

void callback_mode_AXIS_G(int button)
{
	switch(button)
	{
	case BUTTON_UP: // Axis-X
	case BUTTON_DOWN: // Axis-Y
	case BUTTON_LEFT: // Axis-Z
		$um_AXIS_G = button;
		update = update_mode_AXIS_G_DISPLAY;
		detect_callback = callback_mode_AXIS_G_DISPLAY;
		break;
	case BUTTON_SELECT: // Menu
		backToMainPage();
		break;
	}
}

void mode_AXIS_G()
{
	update = 0;
	detect_callback = callback_mode_AXIS_G;
	display_mode_AXIS_G();
}

void callback_mode_Orgn(int button)
{
	switch(button)
	{
	case BUTTON_SELECT:
		backToMainPage();
	default:
		return;
	}
}

void callback_mode_Orgn_update(int unused)
{
	if(!allow_update())
		return;
	
	char* x = new char[7];
	char* y = new char[7];
	char* z = new char[7];

	int* xyz = ADXL345::read();

	sprintf(x, "%d", xyz[0]);
	sprintf(y, "%d", xyz[1]);
	sprintf(z, "%d", xyz[2]);

	newpage();
	LCDPlate::writestr("X: ");
	LCDPlate::writestr(x);
	LCDPlate::writec(' ');
	LCDPlate::writestr("Y: ");
	LCDPlate::writestr(y);
	newline();
	LCDPlate::writestr("Z: ");
	LCDPlate::writestr(z);

	delete[] x;
	delete[] y;
	delete[] z;
	delete[] xyz;
}

void display_options_Freq()
{
	newpage();
	LCDPlate::writestr("U:4Hz   D:5Hz");
	newline();
	LCDPlate::writestr("L:10Hz  R:20Hz");
}

void callback_options_Freq(int button)
{
	switch(button)
	{
	case BUTTON_UP:		base_freq = FREQ_4HZ;			break;
	case BUTTON_DOWN:	base_freq = FREQ_5HZ;			break;
	case BUTTON_LEFT:	base_freq = FREQ_10HZ;			break;
	case BUTTON_RIGHT:	base_freq = FREQ_20HZ; 			break;
	}
	backToMainPage();
}

void jump_options_Freq()
{
	update = 0;
	detect_callback = callback_options_Freq;
	display_options_Freq();
}

void display_option_Range()
{
	newpage();
	LCDPlate::writestr("U:2Mlt  D:4Mlts");
	newline();
	LCDPlate::writestr("L:8Mlts R:16Mlts");
}

void callback_options_Range(int button)
{
	switch(button)
	{
	case BUTTON_UP:		hypersample_rate = HYPERSAMPLE_RATE_2;	break;
	case BUTTON_DOWN:	hypersample_rate = HYPERSAMPLE_RATE_4;	break;
	case BUTTON_LEFT:	hypersample_rate = HYPERSAMPLE_RATE_8;	break;
	case BUTTON_RIGHT:	hypersample_rate = HYPERSAMPLE_RATE_16;	break;
	}
	backToMainPage();
}

void jump_options_Range()
{
	update = 0;
	detect_callback = callback_options_Range;
	display_option_Range();
}

void display_options_Fix()
{
	newpage();
	LCDPlate::writestr("Fix 0~5?");
	newline();
	LCDPlate::writestr("               ");
	
	char* buff = new char[2];
	sprintf(buff, "%d", fix);
	LCDPlate::writec(buff[0]);
}

void callback_options_Fix(int button)
{
	switch(button)
	{
	case BUTTON_LEFT:
		if(fix == 0)
			return;
		fix--;
		break;
	case BUTTON_RIGHT:
		if(fix == 5)
			return;
		fix++;
		break;
	case BUTTON_SELECT:
		backToMainPage();
		return;
	}
	display_options_Fix();
}

void jump_options_Fix()
{
	update = 0;
	detect_callback = callback_options_Fix;
	display_options_Fix();
}

void display_options()
{
	newpage();
	LCDPlate::writestr("U:Freq  D:Unit");
	newline();
	LCDPlate::writestr("L:HySmp R:Fix");
}

void callback_options(int button)
{
	switch(button)
	{
	case BUTTON_UP:			jump_options_Freq();	break;
	case BUTTON_DOWN:								break;
	case BUTTON_LEFT:		jump_options_Range();	break;
	case BUTTON_RIGHT:		jump_options_Fix();		break;
	case BUTTON_SELECT:		backToMainPage();		break;
	}
}

int $umAX_axis;

int $umAX_CST_mode;

float $umAX_CST_value;

int $umAX_STA_mode;

float $umAX_STA_max = (float) -0x7FFFFFFF;

float $umAX_STA_min = (float) 0x7FFFFFFF;

void init_STA()
{
	$umAX_STA_mode = 0;
	$umAX_STA_max = (float) -0x7FFFFFFF;
	$umAX_STA_min = (float) 0x7FFFFFFF;
}

void jump_mode_AxisEx_Axis();

void update_mode_AxisEx_Axis(int unused)
{
	if(!allow_update())
		return;

	float* arr = umAGD_read($umAX_axis);
	char pref = (char) arr[1];

	arr[0] = (float) ((int) (arr[0] * powTen(fix)));
	arr[0] /= powTen(fix);

	newpage();
	LCDPlate::writec(pref);
	LCDPlate::writec(':');

	char* val = new char[16];
	sprintf(val, fix_symbol[fix], arr[0] = ($umAX_CST_mode == 1 ? arr[0] - $umAX_CST_value : arr[0]));
	LCDPlate::writestr(val);

	if($umAX_STA_mode == 1)
	{
		$umAX_STA_max = max(arr[0], $umAX_STA_max);
		$umAX_STA_min = min(arr[0], $umAX_STA_min);
	}

	LCDPlate::pos(0, 11);
	printUnit(0);

	newline();
	
	if($umAX_CST_mode == 1)
		LCDPlate::writestr("CST");
	else
		LCDPlate::writestr("   ");
	LCDPlate::writec(' ');
	if($umAX_STA_mode == 1)
		LCDPlate::writestr("STA");
	else
		LCDPlate::writestr("   ");

	LCDPlate::pos(1, 13);
	switch($umAX_axis)
	{
	case BUTTON_UP: // X
		if(arr[0] == 0)
		{
			printArrowBody();
			printArrowBody();
			printArrowBody();
		}
		else if(arr[0] > 0)
		{
			printArrowBody();
			printArrowBody();
			printArrowRight();
		}
		else
		{
			printArrowLeft();
			printArrowBody();
			printArrowBody();
		}
		break;

	case BUTTON_DOWN: // Y
		if(arr[0] == 0)
		{
			printArrowBody();
			printArrowBody();
			printArrowBody();
		}
		else if(arr[0] > 0)
		{
			printArrowDown();
			printArrowDown();
			printArrowDown();
		}
		else
		{
			printArrowUp();
			printArrowUp();
			printArrowUp();
		}
		break;

	case BUTTON_LEFT: // Z
		if(arr[0] == 0)
		{
			printArrowBody();
			printArrowBody();
			printArrowBody();
		}
		else if(arr[0] < 0)
		{
			printArrowDown();
			printArrowDown();
			printArrowDown();
		}
		else
		{
			printArrowUp();
			printArrowUp();
			printArrowUp();
		}
	}

	delete[] val;
	delete[] arr;
}

void update_mode_AxisEx_Axis_STA_DISPLAY(int);

void callback_mode_AxisEx_Axis_STA_DISPLAY(int button)
{
	if(button == BUTTON_SELECT)
		jump_mode_AxisEx_Axis();
	else if(button == BUTTON_DOWN)
	{
		init_STA();
		$umAX_STA_mode = 1;
		update_mode_AxisEx_Axis_STA_DISPLAY(0);
	}
}

void update_mode_AxisEx_Axis_STA_DISPLAY(int unused)
{
	if(!allow_update())
		return;

	char* val;

	newpage();
	LCDPlate::writestr("Max: ");
	if($umAX_STA_max == (float) 0xFFFFFFFF)
		LCDPlate::writestr("UNDEF");
	else
	{
		val = new char[16];
		sprintf(val, "%.5f", $umAX_STA_max);
		LCDPlate::writestr(val);
		delete[] val;
	}

	newline();
	LCDPlate::writestr("Min: ");
	if($umAX_STA_min == (float) 0x7FFFFFFF)
		LCDPlate::writestr("UNDEF");
	else
	{
		val = new char[16];
		sprintf(val, "%.5f", $umAX_STA_min);
		LCDPlate::writestr(val);
		delete[] val;
	}
}

int cst_delta = 0;

float get_cst_delta()
{
	return 1.F / powTen(cst_delta);
}

void jump_mode_AxisEx_Axis_STA_DISPLAY()
{
	update = update_mode_AxisEx_Axis_STA_DISPLAY;
	detect_callback = callback_mode_AxisEx_Axis_STA_DISPLAY;
}

void display_mode_AxisEx_CST_DISPLAY()
{
	newpage();
	LCDPlate::writestr("CST: ");
	char* val = new char[16];
	sprintf(val, "%.5f", $umAX_CST_value);
	LCDPlate::writestr(val);

	delete[] val;

	newline();
	LCDPlate::writestr("Delta: ");
	val = new char[16];
	sprintf(val, "%.5f", get_cst_delta());
	LCDPlate::writestr(val);
	delete[] val;
}

void callback_mode_AxisEx_Axis_CST_DISPLAY(int button)
{
	switch(button)
	{
	case BUTTON_SELECT:
		jump_mode_AxisEx_Axis();
		cst_delta = 0;
		break;
	case BUTTON_UP:
		$umAX_CST_value += get_cst_delta();
		display_mode_AxisEx_CST_DISPLAY();
		break;
	case BUTTON_DOWN:
		$umAX_CST_value -= get_cst_delta();
		display_mode_AxisEx_CST_DISPLAY();
		break;
	case BUTTON_LEFT:
		if(cst_delta == 0)
			break;
		cst_delta--;
		display_mode_AxisEx_CST_DISPLAY();
		break;
	case BUTTON_RIGHT:
		if(cst_delta == 5)
			break;
		cst_delta++;
		display_mode_AxisEx_CST_DISPLAY();
		break;
	}
}

void jump_mode_AxisEx_Axis_CST_DISPLAY()
{
	update = 0;
	detect_callback = callback_mode_AxisEx_Axis_CST_DISPLAY;
	display_mode_AxisEx_CST_DISPLAY();
}

void jump_mode_AxisEx();

void callback_mode_AxisEx_Axis(int button)
{
	switch(button)
	{
	case BUTTON_DOWN: // CST
		if($umAX_CST_mode == 0)
		{
			
			float* arr = umAGD_read($umAX_axis);
			arr[0] = (float) ((int) (arr[0] * powTen(fix)));
			arr[0] /= powTen(fix);
			$umAX_CST_value = arr[0];
			$umAX_CST_mode = 1;
			delete[] arr;
		}
		else
			$umAX_CST_mode = 0;
		break;

	case BUTTON_UP: // CST_DISPLAY
		jump_mode_AxisEx_Axis_CST_DISPLAY();
		break;

	case BUTTON_LEFT: // STA
		if($umAX_STA_mode == 0)
			$umAX_STA_mode = 1;
		else
			$umAX_STA_mode = 0;
		break;

	case BUTTON_RIGHT: // STA_DISPLAY
		jump_mode_AxisEx_Axis_STA_DISPLAY();
		break;

	case BUTTON_SELECT: // exit
		init_STA();
		$umAX_CST_mode = 0;
		jump_mode_AxisEx();
		break;
	}
}

void jump_mode_AxisEx_Axis()
{
	update = update_mode_AxisEx_Axis;
	detect_callback = callback_mode_AxisEx_Axis;
}

void display_mode_AxisEx()
{
	newpage();
	LCDPlate::writestr("U:AxisX  D:AxisY");
	newline();
	LCDPlate::writestr("L:AxisZ  R:Graph");
}

void callback_mode_AxisEx(int button)
{
	switch(button)
	{
	case BUTTON_RIGHT:							break;
	case BUTTON_LEFT:
	case BUTTON_DOWN:
	case BUTTON_UP:
		$umAX_axis = button;
		jump_mode_AxisEx_Axis();
		break;
	case BUTTON_SELECT:		backToMainPage();	break;
	}
}

void jump_mode_AxisEx()
{
	update = 0;
	detect_callback = callback_mode_AxisEx;
	display_mode_AxisEx();
}

void display_mainpage()
{
	newpage();
	LCDPlate::writestr("U:Orgn  D:Axis-G");
	newline();
	LCDPlate::writestr("L:Optns R:AxisEX");
}

void callback_mainpage(int button)
{
	switch(button)
	{
	case BUTTON_UP:
		update = callback_mode_Orgn_update;
		detect_callback = callback_mode_Orgn;
		break;
	case BUTTON_DOWN:
		mode_AXIS_G();
		break;
	case BUTTON_LEFT:
		update = 0;
		detect_callback = callback_options;
		display_options();
		break;
	case BUTTON_RIGHT:
		jump_mode_AxisEx();
		break;
	default:
		return;
	}
}

void detect(Callback callback)
{
	if(!callback)
		return;
	
	if(LCDPlate::buttonPressed(BUTTON_SELECT))
		(* callback)(BUTTON_SELECT);
	else if(LCDPlate::buttonPressed(BUTTON_UP))
		(* callback)(BUTTON_UP);
	else if(LCDPlate::buttonPressed(BUTTON_LEFT))
		(* callback)(BUTTON_LEFT);
	else if(LCDPlate::buttonPressed(BUTTON_DOWN))
		(* callback)(BUTTON_DOWN);
	else if(LCDPlate::buttonPressed(BUTTON_RIGHT))
		(* callback)(BUTTON_RIGHT);
	else return;
	delay(800);
}

int main()
{
	ADXL345::initialize();
	LCDPlate::initialize();

	ADXL345::setDataRate(ADXL345_DATARATE_3200_HZ);

	init_graph();
	
	// main loop
	LCDPlate::begin(16, 02);
	LCDPlate::backlight(LED_OFF);
	detect_callback = callback_mainpage;
	display_mainpage();
	
	while(1)
	{
		last_samp = samp();
		if(update)
			(* update)(0);
		detect(detect_callback);
		// delay(50);
		delete last_samp;
	}
}
