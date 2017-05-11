#pragma once

#include "lcdplate.h"

void init_graph()
{
	static unsigned char flat_bitmap[8] = {
		0b00001110,
		0b00010001,
		0b00000010,
		0b00000100,
		0b00001000,
		0b00011111,
		0b00000000,
		0b00000000
	};
	LCDPlate::createChar(0x01, flat_bitmap);

	static unsigned char arrow_body_bitmap[8] = {
		0b00000000,
		0b00000000,
		0b00000000,
		0b00011111,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000
	};
	LCDPlate::createChar(0x00, arrow_body_bitmap);

	static unsigned char arrow_left_bitmap[8] = {
		0b00000010,
		0b00000100,
		0b00001000,
		0b00011111,
		0b00001000,
		0b00000100,
		0b00000010,
		0b00000000
	};
	LCDPlate::createChar(0x02, arrow_left_bitmap);

	static unsigned char arrow_right_bitmap[8] = {
		0b00001000,
		0b00000100,
		0b00000010,
		0b00011111,
		0b00000010,
		0b00000100,
		0b00001000,
		0b00000000
	};
	LCDPlate::createChar(0x03, arrow_right_bitmap);

	static unsigned char arrow_up_bitmap[8] = {
		0b00000100,
		0b00001110,
		0b00010101,
		0b00000100,
		0b00000100,
		0b00000100,
		0b00000100,
		0b00000000
	};
	LCDPlate::createChar(0x04, arrow_up_bitmap);

	static unsigned char arrow_down_bitmap[8] = {
		0b00000100,
		0b00000100,
		0b00000100,
		0b00000100,
		0b00010101,
		0b00001110,
		0b00000100,
		0b00000000
	};
	LCDPlate::createChar(0x05, arrow_down_bitmap);
}

void printArrowBody()
{
	LCDPlate::writec(0x00);
}

void printArrowDown()
{
	LCDPlate::writec(0x05);
}

void printArrowUp()
{
	LCDPlate::writec(0x04);
}

void printArrowRight()
{
	LCDPlate::writec(0x03);
}

void printArrowLeft()
{
	LCDPlate::writec(0x02);
}

void printFlat()
{
	LCDPlate::writec(0x01);
}