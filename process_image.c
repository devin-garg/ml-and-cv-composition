#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

float get_pixel(image im, int x, int y, int c)
{

    x = max(min(x, im.w-1), 0);
    y = max(min(y, im.h-1), 0);
    c = max(min(c, im.c-1), 0);
    return im.data[x + y*im.w + c*im.w*im.h];
}


void set_pixel(image im, int x, int y, int c, float v)
{

    if ((x > im.w - 1 || x < 0) || (y > im.h - 1 || y < 0) || (c > im.c - 1 || c < 0))
    {
        return;
    }
    im.data[x + y*im.w + c*im.w*im.h] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in

    for (int c = 0; c <= im.c-1; c++)
    {
        for (int y = 0; y <= im.h-1; y++)
        {   
            for (int x = 0; x <= im.w-1; x++)
            {
                set_pixel(copy, x, y, c, get_pixel(im, x, y, c));
            }
        }
    }

    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in

    for (int y = 0; y <= im.h-1; y++)
    {   
        for (int x = 0; x <= im.w-1; x++)
        {
            set_pixel(gray, x, y, 0, 0.299*get_pixel(im, x, y, 0) + 0.587*get_pixel(im, x, y, 1) + .114*get_pixel(im, x, y, 2));
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in

    for (int y = 0; y < im.h-1; y++)
    {   
        for (int x = 0; x < im.w-1; x++)
        {
            set_pixel(im, x, y, c, get_pixel(im, x, y, c) + v);
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int c = 0; c <= im.c-1; c++)
    { 
        for (int y = 0; y <= im.h-1; y++)
        {   

            for (int x = 0; x <= im.w-1; x++)
            {

                float myPixelValue = get_pixel(im, x, y, c);
                if (myPixelValue > 1.0)
                {
                    set_pixel(im, x, y, c, 1.0);
                } else if (myPixelValue < 0.0)
                {
                    set_pixel(im, x, y, c, 0.0);
                }
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    for (int y = 0; y <= im.h-1; y++)
    {   
        for (int x = 0; x <= im.w-1; x++)
        {

            float redValue = get_pixel(im, x, y, 0);
            float greenValue = get_pixel(im, x, y, 1);
            float blueValue = get_pixel(im, x, y, 2);
            float V = three_way_max(redValue, greenValue, blueValue);
            float S = 0;
            float C = V - three_way_min(redValue, greenValue, blueValue);
            if (V != 0)
            {
                S = C/V;
            } 
            float H;
            if (C == 0)
            {
                H = 0;
            } else{
                float hPrimeValue;
                if (V == redValue)
                {
                    hPrimeValue = (greenValue - blueValue)/C;
                } else if (V == greenValue)
                {
                    hPrimeValue = 2 + ((blueValue - redValue)/C);
                } else{
                    hPrimeValue = 4 + ((redValue - greenValue)/C);
                }
                if (hPrimeValue < 0)
                {
                    H = 1 + (hPrimeValue/6);
                } else{
                    H = hPrimeValue/6;
                }
            }
            set_pixel(im, x, y, 0, H);
            set_pixel(im, x, y, 1, S);
            set_pixel(im, x, y, 2, V);
        }
    }

}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    for (int y = 0; y <= im.h-1; y++)
    {       
        for (int xx = 0; xx <= im.w-1; xx++)
        {
       
            float h = get_pixel(im, xx, y, 0);
            float S = get_pixel(im, xx, y, 1);
            float V = get_pixel(im, xx, y, 2);
            double r = 0, g = 0, b = 0;

            if (S < 0)
            {
           
                set_pixel(im, xx, y, 0, r);
                set_pixel(im, xx, y, 1, g);
                set_pixel(im, xx, y, 2, b);
                return;
            }

             if (h > 360)
             {
                 h = 0;
             } else{
                h = h*6;
             }
            float c = V*S;
            double x = c * (1 - fabs(fmod(h, 2) - 1.0));

            if (h >= 0.0 && h < 1)
            {
                r = c;
                g = x;
                b = 0;

            }
            else if (h >= 1 && h < 2)
            {
                r = x;
                g = c;
                b = 0;
            }
            else if (h >= 2 && h < 3)
            {
                r = 0;
                g = c;
                b = x;
            }
            else if (h >= 3 && h < 4)
            {
                r = 0;
                g = x;
                b = c;
            }
            else if (h >= 4 && h < 5)
            {
                r = x;
                g = 0;
                b = c;
            }
            else if (h >= 5 && h < 6)
            {
                r = c;
                g = 0;
                b = x;
            }
            else
            {
                r = 0;
                g = 0;
                b = 0;
            }
            float m = V - c;
            set_pixel(im, xx, y, 0, r+m);
            set_pixel(im, xx, y, 1, g+m);
            set_pixel(im, xx, y, 2, b+m);
        }
    }

}
