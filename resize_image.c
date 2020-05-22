#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in

	return get_pixel(im, round(x), round(y), c);

}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)

	image resizedImage = make_image(w, h, im.c);


    double xA = im.w/(double)w;
    double yA = im.h/(double)h;
    double xB = (im.w-0.5) - xA*(w-0.5);
	double yB = -0.5 - yA*-0.5; 

    for (int c=0;c<im.c;c++) {
	    for (int y=0;y<h;y++) {
	        for (int x=0;x<w;x++) {
	        	set_pixel(resizedImage, x, y, c, nn_interpolate(im,xA*x+xB, yA*y+yB, c));
	        }
	    }
	}
    return resizedImage;
	
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
	float v1 = get_pixel(im, floor(x), floor(y), c);
	float v3 = get_pixel(im, floor(x), floor(y+1), c);
	float v2 = get_pixel(im, floor(x+1), floor(y), c);
	float v4 = get_pixel(im, floor(x+1), floor(y+1), c);
	float a1 = (floor(x+1) - x) * (floor(y+1)-y);
	float a2 = (x - floor(x)) * (floor(y+1)-y);
	float a3 = (floor(x+1) - x) * (y - floor(y));
	float a4 = (x - floor(x)) * (y - floor(y));

	return v1*a1 + v2*a2 + v3*a3 + v4*a4;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
   	image resizedImage = make_image(w, h, im.c);


    double xA = im.w/(double)w;
    double yA = im.h/(double)h;
    double xB = (im.w-0.5) - xA*(w-0.5);
	double yB = -0.5 - yA*-0.5; 

    for (int c=0;c<im.c;c++) {
	    for (int y=0;y<h;y++) {
	        for (int x=0;x<w;x++) {
	        	set_pixel(resizedImage, x, y, c, bilinear_interpolate(im,xA*x+xB, yA*y+yB, c));
	        }
	    }
	}
    return resizedImage;
}

