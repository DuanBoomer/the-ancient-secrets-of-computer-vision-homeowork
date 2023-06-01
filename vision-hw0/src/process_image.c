#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    if (y >= im.h)
        y = im.h - 1;
    else if (y < 0)
        y = 0;
    if (x >= im.w)
        x = im.w - 1;
    else if (x < 0)
        x = 0;
    if (c > im.c)
        c = im.c - 1;
    else if (c < 0)
        c = 0;
    float pixel = im.data[im.w * ((im.h * c) + y) + x];

    return pixel;
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if (y >= im.h)
        y = im.h - 1;
    else if (y < 0)
        y = 0;
    if (x >= im.w)
        x = im.w - 1;
    else if (x < 0)
        x = 0;
    if (c >= im.c)
        c = im.c - 1;
    else if (c < 0)
        c = 0;
    im.data[im.w * ((im.h * c) + y) + x] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    for (int i = 0; i < im.c; i++)
    {
        for (int j = 0; j < im.h; j++)
        {
            for (int k = 0; k < im.w; k++)
            {
                copy.data[im.w * ((im.h * i) + j) + k] = im.data[im.w * ((im.h * i) + j) + k];
            }
        }
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for (int j = 0; j < im.h; j++)
    {
        for (int k = 0; k < im.w; k++)
        {
            float r = im.data[im.w * ((im.h * 0) + j) + k];
            float g = im.data[im.w * ((im.h * 1) + j) + k];
            float b = im.data[im.w * ((im.h * 2) + j) + k];

            // gray.data[im.w * ((im.h * 0) + j) + k] = (r + g + b)/3;
            gray.data[im.w * ((im.h * 0) + j) + k] = 0.299 * r + 0.587 * g + 0.114 * b;
        }
    }

    return gray;
}

void shift_image(image im, int c, float v)
{
    for (int j = 0; j < im.h; j++)
    {
        for (int k = 0; k < im.w; k++)
        {
            im.data[im.w * ((im.h * c) + j) + k] += v;
        }
    }
}

void clamp_image(image im)
{
    for (int i = 0; i < im.c; i++)
    {
        for (int j = 0; j < im.h; j++)
        {
            for (int k = 0; k < im.w; k++)
            {
                float pixel = im.data[im.w * ((im.h * i) + j) + k];
                if (pixel > 1)
                {
                    pixel = 1;
                }
                if (pixel < 0)
                {
                    pixel = 0;
                }

                im.data[im.w * ((im.h * i) + j) + k] = pixel;
            }
        }
    }
}

// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

void rgb_to_hsv(image im)
{
    for (int j = 0; j < im.h; j++)
    {
        for (int k = 0; k < im.w; k++)
        {
            float r = im.data[im.w * ((im.h * 0) + j) + k];
            float g = im.data[im.w * ((im.h * 1) + j) + k];
            float b = im.data[im.w * ((im.h * 2) + j) + k];

            float v = three_way_max(r, g, b);
            float c = v - three_way_min(r, g, b);
            float s = c / v;
            float h0 = 0;

            if (c == 0)
            {
                h0 = 0;
            }
            else if (v == r)
            {
                h0 = (g - b) / c;
            }
            else if (v == g)
            {
                h0 = ((b - r) / c) + 2;
            }
            else if (v == b)
            {
                h0 = ((r - g) / c) + 4;
            }

            float h = 0;
            if (h0 < 0)
            {
                h = h0 / 6 + 1;
            }
            else
            {
                h = h0 / 6;
            }

            im.data[im.w * ((im.h * 0) + j) + k] = h;
            im.data[im.w * ((im.h * 1) + j) + k] = s;
            im.data[im.w * ((im.h * 2) + j) + k] = v;
        }
    }
}

void hsv_to_rgb(image im)
{
    for (int j = 0; j < im.h; j++)
    {
        for (int k = 0; k < im.w; k++)
        {
            float h = im.data[im.w * ((im.h * 0) + j) + k];
            float s = im.data[im.w * ((im.h * 1) + j) + k];
            float v = im.data[im.w * ((im.h * 2) + j) + k];

            float c = v * s;
            float h0 = 6 * h;
            float x = c * (1 - abs(fmod(h0, 2) - 1));

            float r1 = c;
            float g1 = x;
            float b1 = 0;

            if (h0 >= 0 && h0 < 1)
            {
                r1 = c;
                g1 = x;
                b1 = 0;
            }
            else if (h0 >= 1 && h0 < 2)
            {
                r1 = x;
                g1 = c;
                b1 = 0;
            }
            else if (h0 >= 2 && h0 < 3)
            {
                r1 = 0;
                g1 = c;
                b1 = x;
            }
            else if (h0 >= 3 && h0 < 4)
            {
                r1 = 0;
                g1 = x;
                b1 = c;
            }
            else if (h0 >= 4 && h0 < 5)
            {
                r1 = x;
                g1 = 0;
                b1 = c;
            }
            else if (h0 >= 5)
            {
                r1 = c;
                g1 = 0;
                b1 = x;
            }

            float m = v - c;
            float r = r1 + m;
            float g = g1 + m;
            float b = b1 + m;

            im.data[im.w * ((im.h * 0) + j) + k] = r;
            im.data[im.w * ((im.h * 1) + j) + k] = g;
            im.data[im.w * ((im.h * 2) + j) + k] = b;
        }
    }
}
