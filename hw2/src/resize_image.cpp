#include <cmath>
#include "image.h"
#include <iostream>

using namespace std;

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the nearest neighbor to pixel (x,y,c)
float Image::pixel_nearest(float x, float y, int c) const
  {
  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)
  int x1 = round(x);
  int y1 = round(y);
  return clamped_pixel(x1, y1, c);
  }

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the bilinearly interpolated pixel (x,y,c)
float Image::pixel_bilinear(float x, float y, int c) const
  {
  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)
    int x1 = floor(x);
    int y1 = floor(y);
    int x2 = ceil(x);
    int y2 = ceil(y);

    float x1y1 = clamped_pixel(x1, y1, c);
    float x2y1 = clamped_pixel(x2, y1, c);
    float x1y2 = clamped_pixel(x1, y2, c);
    float x2y2 = clamped_pixel(x2, y2, c);

    float xp1 = std::fabs(x - x1) * x2y1 + std::fabs(x - x2) * x1y1;
    float xp2 = std::fabs(x - x1) * x2y2 + std::fabs(x - x2) * x1y2; 

    float pixVal = std::fabs(y - y1) * xp2 + std::fabs(y - y2) * xp1;
    
  return pixVal;
  }

// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image nearest_resize(const Image& im, int w, int h)
  {
  Image ret(w,h,im.c); 
    float scale_x = im.w / (float)w;
    float scale_y = im.h / (float)h;
  for (int ch = 0; ch < ret.c; ch++) {
    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {

        float src_x = (x + 0.5f) * scale_x - 0.5f;
        float src_y = (y + 0.5f) * scale_y - 0.5f;

        float pixVal = im.pixel_nearest(src_x, src_y, ch); 
        int loc = ret.pixel_address(x, y, ch);
        ret.data[loc] = pixVal;
      }
    }
  }
  return ret;
  }


// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image bilinear_resize(const Image& im, int w, int h)
  {
    Image ret(w, h, im.c);
    for (int ch = 0; ch < ret.c; ch++) {
      for (int y = 0; y < h; y++) {                          
        for (int x = 0; x < w; x++) {                        
          float scale_x = im.w / (float)w;
          float scale_y = im.h / (float)h;
  
          float src_x = (x + 0.5f) * scale_x - 0.5f;
          float src_y = (y + 0.5f) * scale_y - 0.5f;
  
          float pixVal = im.pixel_bilinear(src_x, src_y, ch); 
          int loc = ret.pixel_address(x, y, ch);
          ret.data[loc] = pixVal;
        }
      }
    }
    return ret;
  }