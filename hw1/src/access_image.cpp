#include "image.h"
#include <iostream>

int get_bound(int idx, int size);
// HW0 #1
// const Image& im: input image
// int x,y: pixel coordinates
// int ch: channel of interest
// returns the 0-based location of the pixel value in the data array
int pixel_address(const Image& im, int x, int y, int ch)
  {
  return (ch * im.w * im.h) + (y * im.w) + x;
  }

// HW0 #1
// const Image& im: input image
// int x,y,ch: pixel coordinates and channel of interest
// returns the value of the clamped pixel at channel ch
float get_clamped_pixel(const Image& im, int x, int y, int ch)
  {
  x = get_bound(x, im.w);
  y = get_bound(y, im.h);
  ch = get_bound(ch, 3);
  // std::cout << "x: " << x << " y: " << y << " ch: " << ch << std::endl;
  int loc = pixel_address(im, x, y, ch);
  // std::cout << "pixel loc: " << loc << std::endl;
  return im.data[loc];
  }

int get_bound(int idx, int size)
{
  idx = idx < 0 ? 0 : idx;
  idx = idx > size - 1 ? size - 1 : idx;
  return idx;
}
// HW0 #1
// Image& im: input image
// int x,y,ch: pixel coordinates and channel of interest
void set_pixel(Image& im, int x, int y, int c, float value)
  {
    if (x < 0 | x >= im.w | y < 0 | y >= im.h | c < 0 | c > 2) {
      return;
    }
  
    int loc = pixel_address(im, x, y, c);
    im.data[loc] = value;
  }



// HW0 #2
// Copies an image
// Image& to: destination image
// const Image& from: source image
void copy_image(Image& to, const Image& from)
  {
  // allocating data for the new image
  to.data=(float*)calloc(from.w*from.h*from.c,sizeof(float));
  to.c=from.c;
  to.w=from.w;
  to.h=from.h;
  std::memcpy(to.data, from.data, from.c * from.w * from.h * sizeof(float)); 
  }
