#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include "uwnet.h"


// Run a maxpool layer on input
// layer l: pointer to layer to run
// matrix in: input to layer
// returns: the result of running the layer
matrix forward_maxpool_layer(layer l, matrix in)
{
    int outw = (l.width-1)/l.stride + 1;
    int outh = (l.height-1)/l.stride + 1;
    matrix out = make_matrix(in.rows, outw*outh*l.channels);


    // TODO: 6.1 - iterate over the input and fill in the output with max values
    int m,k, col, i, j, voffset, hoffset, theSrcInd, underVal=0;
    int left, right = 0;
  	int size = l.size;
  	int stride = l.stride;
    if(size % 2 != 0){
        left = -1*(size-1)/2;
        right = (size-1)/2;
    } else{
        left = -1 * (size/2 -1);
        right = size/2;
    }
  	for(m=0; m<in.rows; ++m){
  		image im = float_to_image(in.data + m*in.cols, l.width, l.height, l.channels);
    	int offsetCalc = m*out.cols;
      for(k=0; k<im.c; k++){
          for(voffset=0; voffset<im.h; voffset+=stride){
              for(hoffset=0; hoffset<im.w; hoffset+=stride){
                int middleOf = hoffset + voffset*im.w + k*im.w*im.h;
				        double max = -INFINITY;
                for(i=left; i<=right; i++){
                    for(j=left; j<=right; j++){
                        theSrcInd = middleOf + i*im.w + j;
						            double temp;
                        if((hoffset+j < 0 || voffset+i<0 || hoffset+j >= im.h || voffset+i >= im.w)){
                            temp = 0;
                        } else{
            							temp = im.data[theSrcInd];
            						}
            						if(temp >= max){
            							max = temp;
            						}
                    }
                }
        				out.data[offsetCalc] = max;
        				offsetCalc++;
              }
          }
      }
  	}

    l.in[0] = in;
    free_matrix(l.out[0]);
    l.out[0] = out;
    free_matrix(l.delta[0]);
    l.delta[0] = make_matrix(out.rows, out.cols);
    return out;
}

// Run a maxpool layer backward
// layer l: layer to run
// matrix prev_delta: error term for the previous layer
void backward_maxpool_layer(layer l, matrix prev_delta)
{
    matrix in    = l.in[0];
    matrix out   = l.out[0];
    matrix delta = l.delta[0];

    int outw = (l.width-1)/l.stride + 1;
    int outh = (l.height-1)/l.stride + 1;

    // TODO: 6.2 - find the max values in the input again and fill in the
    // corresponding delta with the delta from the output. This should be
    // similar to the forward method in structure.
    int k,col,m,i,j,voffset,hoffset,theSrcInd=0;
    int left,right = 0;
  	int size = l.size;
  	int stride = l.stride;
    if(size % 2 != 0){
        left = -1*(size-1)/2;
        right = (size-1)/2;
    } else{
        left = -1 * (size/2 -1 );
        right = size/2;
    }
  	if(prev_delta.data){
      	int offsetCalc = 0;
        for(m=0; m<in.rows; ++m){
          image im = float_to_image(in.data + m*in.cols, l.width, l.height, l.channels);
        	image updateD = float_to_image(prev_delta.data + m*in.cols, l.width, l.height, l.channels);
          for(k=0; k<im.c; k++){
              for(voffset=0; voffset<im.h; voffset+=stride){
                  for(hoffset=0; hoffset<im.w; hoffset+=stride){
                    int middleOf = hoffset + voffset*im.w + k*im.w*im.h;
                    double max =-INFINITY;
    				        int differentialFoo,fooFirst,fooSecond;
                    for(i=left; i<=right; i++){
                        for(j=left; j<=right; j++){
                            theSrcInd = middleOf + i*im.w + j;
                            double temp;
                            if((hoffset+j < 0 || voffset+i<0 || hoffset+j > im.h || voffset+i > im.w)){
                                temp = 0;
                            } else{
                                temp = im.data[theSrcInd];
                            }
                            if(temp >=max){
                                max = temp;
                  							differentialFoo = theSrcInd;
                  							fooFirst = i;
                  							fooSecond = j;
                            }
                        }
                    }
            				if(!(hoffset+fooSecond < 0 || voffset+fooFirst<0 || hoffset+fooSecond >= im.h || voffset+fooFirst >= im.w)){
        					    updateD.data[differentialFoo] += delta.data[offsetCalc];
            				}
                    offsetCalc++;
                  }
              }
          }
        }
  	}
}

// Update maxpool layer
// Leave this blank since maxpool layers have no update
void update_maxpool_layer(layer l, float rate, float momentum, float decay)
{
}

// Make a new maxpool layer
// int w: width of input image
// int h: height of input image
// int c: number of channels
// int size: size of maxpool filter to apply
// int stride: stride of operation
layer make_maxpool_layer(int w, int h, int c, int size, int stride)
{
    layer l = {0};
    l.width = w;
    l.height = h;
    l.channels = c;
    l.size = size;
    l.stride = stride;
    l.in = calloc(1, sizeof(matrix));
    l.out = calloc(1, sizeof(matrix));
    l.delta = calloc(1, sizeof(matrix));
    l.forward  = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    l.update   = update_maxpool_layer;
    return l;
}
