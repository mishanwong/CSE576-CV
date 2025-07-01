#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include <iostream>

#include "image.h"

using namespace std;

// HW0 #3
// const Image& im: input image
// return the corresponding grayscale image
Image rgb_to_grayscale(const Image& im)
  {
  assert(im.c == 3); // only accept RGB images
  Image gray(im.w,im.h,1); // create a new grayscale image (note: 1 channel)
  for (int y = 0; y < im.h; y++) {
    for (int x = 0; x < im.w; x++) {
      int rLoc = im.pixel_address(x, y, 0);
      int gLoc = im.pixel_address(x, y, 1);
      int bLoc = im.pixel_address(x, y, 2);
      float r = im.data[rLoc];
      float g = im.data[gLoc];
      float b = im.data[bLoc];

      int grayLoc = gray.pixel_address(x, y, 0);
      float grayValue = 0.299 * r + 0.587 * g + 0.114 * b;
      gray.data[grayLoc] = grayValue;
    }
  } 
  return gray;
  }



// Example function that changes the color of a grayscale image
Image grayscale_to_rgb(const Image& im, float r, float g, float b)
  {
  assert(im.c == 1);
  Image rgb(im.w,im.h,3);
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    {
    rgb(q1,q2,0)=r*im(q1,q2);
    rgb(q1,q2,1)=g*im(q1,q2);
    rgb(q1,q2,2)=b*im(q1,q2);
    }
  
  return rgb;
  }




// HW0 #4
// Image& im: input image to be modified in-place
// int c: which channel to shift
// float v: how much to shift
void shift_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  for (int x = 0; x < im.w; x++) {
    for (int y = 0; y < im.h; y++) {
      int loc = im.pixel_address(x, y, c);
      float val = im.data[loc] + v;
      im.data[loc] = val;
    }
  }
  
  }

// HW0 #8
// Image& im: input image to be modified in-place
// int c: which channel to scale
// float v: how much to scale
void scale_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  // TODO: scale all the pixels at the specified channel
  
  NOT_IMPLEMENTED();
  
  }


float clamp_pixel(Image& im, int loc) {
  float val = im.data[loc];
  val = val < 0 ? 0 : val;
  val = val > 1 ? 1 : val;

  return val;
}
// HW0 #5
// Image& im: input image to be modified in-place
void clamp_image(Image& im)
  {
  // TODO: clamp all the pixels in all channel to be between 0 and 1
  
  for (int x = 0; x < im.w; x++) {
    for (int y = 0; y < im.h; y++) {
      int loc0 = im.pixel_address(x, y, 0);    
      int loc1 = im.pixel_address(x, y, 1);    
      int loc2 = im.pixel_address(x, y, 2);    
      
      float val0 = clamp_pixel(im, loc0);
      im.data[loc0] = val0;
      
      float val1 = clamp_pixel(im, loc1);
      im.data[loc1] = val1;

      float val2 = clamp_pixel(im, loc2);
      im.data[loc2] = val2;
    }
  }
  
  }

// These might be handy
float max(float a, float b, float c)
  {
  return max({a,b,c});
  }

float min(float a, float b, float c)
  {
  return min({a,b,c});
  }


// HW0 #6
// Image& im: input image to be modified in-place
void rgb_to_hsv(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
   for (int x = 0; x < im.w; x++) {
    for (int y = 0; y < im.h; y++) {
      int rLoc = im.pixel_address(x, y, 0);
      int gLoc = im.pixel_address(x, y, 1);
      int bLoc = im.pixel_address(x, y, 2);
      
      float R = im.data[rLoc];
      float G = im.data[gLoc];
      float B = im.data[bLoc];

      float m = min(R, G, B);
      float V = max(R, G, B);
      float C = V - m;

      float S = 0;
      if (V != 0) {
        S = C / V;
      } 

      float H = 0;
      if (C != 0) {
        if (V == R) {
          H = (G - B) / C;
        } else if (V == G) {
          H = ((B - R) / C) + 2;
        } else if (V == B) {
          H = ((R - G) / C) + 4;
        }
      }

      if (H < 0) {
        H = (H / 6) + 1;
      } else {
        H = (H / 6);
      }
      im.data[rLoc] = H;
      im.data[gLoc] = S;
      im.data[bLoc] = V;
    }
   }
    
  }

// HW0 #7
// Image& im: input image to be modified in-place
void hsv_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
    for (int x = 0; x < im.w; x++) {
      for (int y = 0; y < im.h; y++) {
        int hLoc = im.pixel_address(x, y, 0);
        int sLoc = im.pixel_address(x, y, 1);
        int vLoc = im.pixel_address(x, y, 2);

        float H = im.data[hLoc];
        float S = im.data[sLoc];
        float V = im.data[vLoc];

        float C = V * S;
        float X = C * (1 - abs((fmod(6 * H, 2) - 1)));
        float m = V - C;
        float R = 0;
        float G = 0;
        float B = 0;
        if (H < 1.0 / 6.0) {
          R = C;
          G = X;
        } else if (H < 2.0 / 6.0) {
          R = X;
          G = C;
        } else if (H < 3.0 / 6.0) {
          G = C;
          B = X;
        } else if (H < 4.0 / 6.0) {
          G = X;
          B = C;
        } else if (H < 5.0 / 6.0) {
          R = X;
          B = C;
        } else if (H < 1.0) {
          R = C; 
          B = X;
        }

        im.data[hLoc] = R + m;
        im.data[sLoc] = G + m;
        im.data[vLoc] = B + m;
      }
    }
    
  
  }

// HW0 #9
// Image& im: input image to be modified in-place
void rgb_to_lch(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from RGB format to LCH format
  
  
  NOT_IMPLEMENTED();
  
  }

// HW0 #9
// Image& im: input image to be modified in-place
void lch_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from LCH format to RGB format
  
  NOT_IMPLEMENTED();
  
  }



// Implementation of member functions
void Image::clamp(void) { clamp_image(*this); }
void Image::shift(int c, float v) { shift_image(*this,c,v); }
void Image::scale(int c, float v) { scale_image(*this,c,v); }

void Image::HSVtoRGB(void) { hsv_to_rgb(*this); }
void Image::RGBtoHSV(void) { rgb_to_hsv(*this); }
void Image::LCHtoRGB(void) { lch_to_rgb(*this); }
void Image::RGBtoLCH(void) { rgb_to_lch(*this); }
