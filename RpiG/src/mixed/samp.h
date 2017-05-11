#pragma once

#include "simpmath.h"
#include "ADXL345.h"
#include "wiringPi.h"

#define HYPERSAMPLE_RATE_1 1
#define HYPERSAMPLE_RATE_2 2
#define HYPERSAMPLE_RATE_4 4
#define HYPERSAMPLE_RATE_8 8
#define HYPERSAMPLE_RATE_16 16

double device_voltage = 3.57D;

int base_freq = 5; // Hz

int hypersample_rate = 2;

struct Sample_t
{
    double x;

    double y;

    double z;
};

double x_base_lsb() // +/-2g
{
    static const double base = 227.875D;

    static const double k = 11.25D;

    return k * device_voltage + base;
}

double x_shift()
{
    static const double base = -0.078125D;

    static const double k = 0.03125D;

    return k * device_voltage + base;
}

double y_base_lsb() // +/-2g
{
    return x_base_lsb();
}

double y_shift()
{
    return x_shift();
}

double z_base_lsb() // +/-2g
{
    return 256;
}

double z_shift()
{
    static const double base = -0.0625D;

    static const double k = 0.025D;

    return k * device_voltage + base;
}

double x_samp_lsb()
{
    return x_base_lsb() * hypersample_rate;
}

double y_samp_lsb()
{
    return y_base_lsb() * hypersample_rate;
}

double z_samp_lsb()
{
    return z_base_lsb() * hypersample_rate;
}

void nanodelay(long _delay)
{
    _delay = _delay / 1000000;
    timespec spec, dummy;
    spec.tv_sec = (time_t)(_delay / 1000);
    spec.tv_nsec = (long)(_delay % 1000) * 1000000;
    nanosleep(&spec, &dummy);
}

Sample_t* samp()
{
    int t = (int) pow(hypersample_rate, 2); // Hz

    double* ra = new double[3];

    for(int i = 0; i < t; i++)
    {
        int* arr = ADXL345::read();
        for(int j = 0; j < 3; j++)
            ra[j] += (double) arr[j];
        delete[] arr;
        nanodelay(1000000000 / (t * base_freq));
    }

    for(int i = 0; i < 3; i++)
        ra[i] /= hypersample_rate;

    Sample_t* rt = new Sample_t;
    rt->x = ra[0] / x_samp_lsb() + x_shift();
    rt->y = ra[1] / y_samp_lsb() + y_shift();
    rt->z = ra[2] / z_samp_lsb() + z_shift();

    delete[] ra;
    return rt;
}