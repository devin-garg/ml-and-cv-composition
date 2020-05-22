#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    float sum = 0;
    for (int c = 0; c <= im.c-1; c++)
    {
        for (int y = 0; y <= im.h-1; y++)
        {   
            for (int x = 0; x <= im.w-1; x++)
            {
                sum += get_pixel(im,x,y,c);
            }
        }
    }
    for (int c = 0; c <= im.c-1; c++)
    {
        for (int y = 0; y <= im.h-1; y++)
        {   
            for (int x = 0; x <= im.w-1; x++)
            {
                set_pixel(im,x,y,c, get_pixel(im,x,y,c)/sum);
            }
        }
    }
}

image make_box_filter(int w)
{
    // TODO

    image boxFilter = make_image(w,w,1);
    for (int y = 0; y < w; y++)
    {   
        for (int x = 0; x < w; x++)
        {
            set_pixel(boxFilter, x, y, 0, 1.0);
        }
    }
    l1_normalize(boxFilter);
    return boxFilter;
}


image conPreservedImage(image im, image filter){
    image conImg = make_image(im.w, im.h, im.c);
    int filterChannelToUse;
    for (int c = 0; c <= im.c-1; c++)
    {
        filterChannelToUse = 0;
        if (filter.c == im.c)
        {
            
            filterChannelToUse = c;
        } 
        for (int y = 0; y <= im.h-1; y++)
        {   
            for (int x = 0; x <= im.w-1; x++)
            {
                float convolvedVal = 0.0;
                for (int j = 0; j < filter.h; j++)
                {
                    for (int i = 0; i < filter.w; i++)
                    {

                        convolvedVal += get_pixel(im,x - (filter.w/2) + i,y - (filter.w/2) + j,c)
                                *get_pixel(filter,i, j, filterChannelToUse);

                    }

                }                
                set_pixel(conImg, x, y, c, convolvedVal);
            }
        }
    }

    return conImg;
}


image convolve_image(image im, image filter, int preserve)
{
    // TODO
    assert(filter.c == im.c || filter.c == 1);
    if (preserve == 1)
    {
        return conPreservedImage(im,filter);
    } else{
        image myRetImg = make_image(im.w, im.h, 1);
        int offset = filter.w/2;
        for (int row = 0; row < im.h; row++) {
            for (int col = 0; col < im.w; col++) {
                float summation = 0.0;
                for (int ch = 0; ch < im.c; ++ch)
                {
                    for (int rowLoop = 0; rowLoop < filter.h; rowLoop++) {
                        for (int colLoop = 0; colLoop < filter.w; colLoop++) {
                            
                            float imVal=get_pixel(im, col - offset + colLoop, row - offset + rowLoop, ch);
                            summation += imVal*get_pixel(filter, colLoop, rowLoop, ch);
                        }
                    }
                }

                set_pixel(myRetImg, col, row, 0, summation);
            }
        }

        return myRetImg;

    }
    
}


image make_highpass_filter()
{
    // TODO
    image hpImg = make_image(3,3,1);
    set_pixel(hpImg, 0,0,0, 0);
    set_pixel(hpImg, 1,0,0, -1);
    set_pixel(hpImg, 2,0,0, 0);
    set_pixel(hpImg, 0,1,0, -1);
    set_pixel(hpImg, 1,1,0, 4);
    set_pixel(hpImg, 2,1,0, -1);
    set_pixel(hpImg, 0,2,0, 0);
    set_pixel(hpImg, 1,2,0, -1);
    set_pixel(hpImg, 2,2,0, 0);

    return hpImg;
}

image make_sharpen_filter()
{
    // TODO
    image hpImg = make_image(3,3,1);
    set_pixel(hpImg, 0,0,0, 0);
    set_pixel(hpImg, 1,0,0, -1);
    set_pixel(hpImg, 2,0,0, 0);
    set_pixel(hpImg, 0,1,0, -1);
    set_pixel(hpImg, 0,2,0, 0);
    set_pixel(hpImg, 1,1,0, 5);
    set_pixel(hpImg, 1,2,0, -1);
    set_pixel(hpImg, 2,1,0, -1);
    set_pixel(hpImg, 2,2,0, 0);

    return hpImg;
}

image make_emboss_filter()
{
    // TODO
    image hpImg = make_image(3,3,1);
    set_pixel(hpImg, 0,0,0, -2);
    set_pixel(hpImg, 1,0,0, -1);
    set_pixel(hpImg, 2,0,0, 0);
    set_pixel(hpImg, 0,1,0, -1);
    set_pixel(hpImg, 0,2,0, 0);
    set_pixel(hpImg, 1,1,0, 1);
    set_pixel(hpImg, 1,2,0, 1);
    set_pixel(hpImg, 2,1,0, 1);
    set_pixel(hpImg, 2,2,0, 2);

    return hpImg;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: Preserve should be done to keep the color and that is for sharpen image and emboss filter that preserves its changed pixel values while 
// don't preserve for high pass filter as the image could be used for edge detection and has gray result.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: Clamping all the above filters should be done so the pixel values are within a range that we could see color values.

image make_gaussian_filter(float sigma)
{

    // TODO
    int dimOfImg = ceil(sigma*6);
    if (dimOfImg % 2 == 0){
        dimOfImg++;
    }
    image myFilter = make_image(dimOfImg, dimOfImg, 1);
    for (int yy = 0; yy < dimOfImg; yy++)
    {   
        for (int xx = 0; xx < dimOfImg; xx++)
        {
            int x = xx - myFilter.w/2;
            int y = yy - myFilter.h/2;
            set_pixel(myFilter, xx, yy, 0, (1/(TWOPI*sigma*sigma))*exp(((-x*x-y*y)/(2*sigma*sigma))));
        }
    }
    l1_normalize(myFilter);
    return myFilter;
}

image add_image(image a, image b)
{
    // TODO
    image addedImg = make_image(a.w, a.h, a.c);
    for (int c = 0; c <= a.c-1; c++)
    {
        for (int y = 0; y <= a.h-1; y++)
        {   
            for (int x = 0; x <= a.w-1; x++)
            {
                set_pixel(addedImg, x, y, c, get_pixel(a, x, y, c) + get_pixel(b, x, y, c) );
            }
        }
    }
    return addedImg;
}

image sub_image(image a, image b)
{
    // TODO

    image subImg = make_image(a.w, a.h, a.c);
    for (int c = 0; c <= a.c-1; c++)
    {
        for (int y = 0; y <= a.h-1; y++)
        {   
            for (int x = 0; x <= a.w-1; x++)
            {
                set_pixel(subImg, x, y, c, get_pixel(a, x, y, c) - get_pixel(b, x, y, c) );
            }
        }
    }
    return subImg;
}

image make_gx_filter()
{
    // TODO
    image hpImg = make_image(3,3,1);
    set_pixel(hpImg, 0,0,0, -1.0);
    set_pixel(hpImg, 1,0,0, 0.0);
    set_pixel(hpImg, 2,0,0, 1.0);
    set_pixel(hpImg, 0,1,0, -2.0);
    set_pixel(hpImg, 1,1,0, 0.0);
    set_pixel(hpImg, 2,1,0, 2.0);
    set_pixel(hpImg, 0,2,0, -1.0);
    set_pixel(hpImg, 1,2,0, 0.0);
    set_pixel(hpImg, 2,2,0, 1.0);

    return hpImg;

}

image make_gy_filter()
{
    // TODO
    image hpImg = make_image(3,3,1);
    set_pixel(hpImg, 0,0,0, -1.0);
    set_pixel(hpImg, 1,0,0, -2.0);
    set_pixel(hpImg, 2,0,0, -1.0);
    set_pixel(hpImg, 0,1,0, 0.0);
    set_pixel(hpImg, 1,1,0, 0.0);
    set_pixel(hpImg, 2,1,0, 0.0);
    set_pixel(hpImg, 0,2,0, 1.0);
    set_pixel(hpImg, 1,2,0, 2.0);    
    set_pixel(hpImg, 2,2,0, 1.0);

    return hpImg;

}

void feature_normalize(image im)
{
    // TODO 
    float min = INFINITY;
    float max = -INFINITY;
    for (int c = 0; c <= im.c-1; c++)
    {
        for (int y = 0; y <= im.h-1; y++)
        {   
            for (int x = 0; x <= im.w-1; x++)
            {
                float pixelVal = get_pixel(im, x, y, c);
                if (pixelVal < min)
                {
                    /* code */
                    min = pixelVal;
                }

                if (pixelVal > max)
                {
                    /* code */
                    max = pixelVal;
                }
            }
        }
    }

    for (int c = 0; c <= im.c-1; c++)
    {
        for (int y = 0; y <= im.h-1; y++)
        {   
            for (int x = 0; x <= im.w-1; x++)
            {
                set_pixel(im, x, y, c, (get_pixel(im, x, y, c)-min)/(max-min));
            }
        }
    }
}

image *sobel_image(image im)
{
    // TODO
    image gx = convolve_image(im, make_gx_filter(), 0);
    image gy = convolve_image(im, make_gy_filter(), 0);
    image mag = make_image(im.w, im.h, 1);
    image ang = make_image(im.w, im.h, 1);

    for (int y = 0; y <= im.h-1; y++)
    {   
        for (int x = 0; x <= im.w-1; x++)
        {
            float gxVal = get_pixel(gx,x,y,0);
            float gyVal = get_pixel(gy,x,y,0);
            set_pixel(mag, x, y, 0, sqrt(gxVal*gxVal + gyVal*gyVal));
            set_pixel(ang, x, y, 0, atan2(gyVal,gxVal));
        }
    }
    image* ptr = (image*) calloc(2, sizeof(image));
    ptr[0] = mag;
    ptr[1] = ang;
    return ptr;
}

image colorize_sobel(image im)
{
    // TODO
    image* sobelImg = sobel_image(im);
    image finalImg = make_image(im.w,im.h,im.c);
    feature_normalize(*sobelImg);
    feature_normalize(*(sobelImg+1));
    for (int c = 0; c <= im.c-1; c++)
    {
        for (int y = 0; y <= im.h-1; y++)
        {   
            for (int x = 0; x <= im.w-1; x++)
            {
                if (c == 0)
                {
                    set_pixel(finalImg, x, y, c, get_pixel(*(sobelImg+1), x, y, c));
                } else{
                    set_pixel(finalImg, x, y, c, get_pixel(*sobelImg, x, y, c));

                }
                
            }
        }
    }
    hsv_to_rgb(finalImg);
    feature_normalize(finalImg);
    clamp_image(finalImg);
    return finalImg;
}
