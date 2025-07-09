#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#include <iostream>
#include <chrono>
#include <thread>

#define M_PI 3.14159265358979323846

// HW1 #2.1
// Image& im: image to L1-normalize
void l1_normalize(Image& im)
  {
  int N = im.w * im.h;
  float sum = 0.0f;
  for (int ch = 0; ch < im.c; ch++) {
    for (int y = 0; y < im.h; y++) {
      for (int x = 0; x < im.w; x++) {
        sum += im.data[im.pixel_address(x, y, ch)];      
      }
    }
  }

  for (int ch = 0; ch < im.c; ch++) {
    for (int y = 0; y < im.h; y++) {
      for (int x = 0; x < im.w; x++) {
        im.data[im.pixel_address(x, y, ch)] /= sum;
      }
    }
  }
  
  
  }

// HW1 #2.1
// int w: size of filter
// returns the filter Image of size WxW
Image make_box_filter(int w)
  {
  assert(w%2); // w needs to be odd
  
  int N = w * w;
    Image ret(w, w, 1);

    for (int y = 0; y < w; y++) {
      for (int x = 0; x < w; x++) {
        ret.data[ret.pixel_address(x, y, 0)] = (float) 1.0 / N;
      }
    }
  return ret;
  }

// HW1 #2.2
// const Image&im: input image
// const Image& filter: filter to convolve with
// bool preserve: whether to preserve number of channels
// returns the convolved image
Image convolve_image(const Image& im, const Image& filter, bool preserve)
  {
  assert(filter.c==1);
  Image ret;
  ret = Image(im.w, im.h, preserve ? im.c : 1);
 
  if (preserve) {
    for (int ch = 0; ch < im.c; ch++) {
      for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
          // Perform convolution here
          float pixVal = 0;
          for (int fy = 0; fy < filter.h; fy++) {
            for (int fx = 0; fx < filter.w; fx++) {
              int im_x = x + fx - filter.w / 2;
              int im_y = y + fy - filter.h / 2;
              float image_pix = im.clamped_pixel(im_x, im_y, ch);
              float filter_pix = filter.clamped_pixel(fx, fy, 0);
              pixVal += image_pix * filter_pix;
            }
          }

          // Save pixel value to output image
          int loc =  ret.pixel_address(x, y, ch);
          ret.data[loc] = pixVal;
        }
      }
    }
  } else {
    for (int y = 0; y < im.h; y++) {
      for (int x = 0; x < im.w; x++) {
        float pixVal = 0;
        for (int ch = 0; ch < im.c; ch++) {
                for (int fy = 0; fy < filter.h; fy++) {
                    for (int fx = 0; fx < filter.w; fx++) {
                        int im_x = x + fx - filter.w / 2;
                        int im_y = y + fy - filter.h / 2;
                        float image_pix = im.clamped_pixel(im_x, im_y, ch);
                        float filter_pix = filter.clamped_pixel(fx, fy, 0);
                        pixVal += image_pix * filter_pix;
                    }
                }
            }
        int loc = ret.pixel_address(x, y, 0);
        ret.data[loc] = pixVal;
      }
    }
  }
  return ret;
  }

// HW1 #2.3
// returns basic 3x3 high-pass filter
Image make_highpass_filter()
  {
    Image filter;
    filter = Image(3, 3, 1);
    filter.data[0] = 0;
    filter.data[1] = -1;
    filter.data[2] = 0;
    filter.data[3] = -1;
    filter.data[4] = 4;
    filter.data[5] = -1;
    filter.data[6] = 0;
    filter.data[7] = -1;
    filter.data[8] = 0;
  return filter;
  
  }

// HW1 #2.3
// returns basic 3x3 sharpen filter
Image make_sharpen_filter()
  {
    Image filter;
    filter = Image(3, 3, 1);
    if (!filter.data) {
      std::cout << "ret.data is NULL!" << std::endl;
      exit(1);
    }
    filter.data[0] = 0;
    filter.data[1] = -1;
    filter.data[2] = 0;
    filter.data[3] = -1;
    filter.data[4] = 5;
    filter.data[5] = -1;
    filter.data[6] = 0;
    filter.data[7] = -1;
    filter.data[8] = 0;
  return filter;
  
  
  }

// HW1 #2.3
// returns basic 3x3 emboss filter
Image make_emboss_filter()
  {
    Image filter;
    filter = Image(3, 3, 1);
    if (!filter.data) {
      std::cout << "ret.data is NULL!" << std::endl;
      exit(1);
    }
    filter.data[0] = -2;
    filter.data[1] = -1;
    filter.data[2] = 0;
    filter.data[3] = -1;
    filter.data[4] = 1;
    filter.data[5] = 1;
    filter.data[6] = 0;
    filter.data[7] = 1;
    filter.data[8] = 2;
  return filter;
  
  
  }

// HW1 #2.4
// float sigma: sigma for the gaussian filter
// returns basic gaussian filter
Image make_gaussian_filter(float sigma)
  {
    int sixSigma = ceil(sigma * 6);
    int w = sixSigma % 2 == 1 ? sixSigma : sixSigma + 1;
    int center = w / 2;
    Image ret(w, w, 1);
    float sum = 0;
    for (int y = 0; y < w; y++) {
      for (int x = 0; x < w; x++) {
        int dx = x - center;
        int dy = y - center;
        float numerator = -((dx * dx) + (dy * dy));
        float denominator = 2.0f * sigma * sigma;
        float exponent = numerator / denominator;
        float pixVal = (1.0f / (2.0f * M_PI * sigma * sigma)) * std::exp(exponent);
        ret.data[ret.pixel_address(x, y, 0)] = pixVal;
      }
    }
    // l1_normalize(ret);

    for (int y = 0; y < w; y++) {
      for (int x = 0; x < w; x++) {
        sum += ret.data[ret.pixel_address(x, y, 0)];
      }
    }
    return ret;
  }


// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their sum
Image add_image(const Image& a, const Image& b)
  {
  assert(a.w==b.w && a.h==b.h && a.c==b.c); // assure images are the same size
  

    Image ret(a.w, a.h, a.c);
    for (int ch = 0; ch < a.c; ch++) {
      for (int y = 0; y < a.h; y++) {
        for (int x = 0; x < a.w; x++) {
          float pixA = a.data[a.pixel_address(x, y, ch)];
          float pixB = b.data[b.pixel_address(x, y, ch)];
          ret.data[a.pixel_address(x, y, ch)] = pixA + pixB;
        }
      }
    }
  return ret;
  
  }

// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their difference res=a-b
Image sub_image(const Image& a, const Image& b)
  {
  assert(a.w==b.w && a.h==b.h && a.c==b.c); // assure images are the same size
    Image ret(a.w, a.h, a.c);
    for (int ch = 0; ch < a.c; ch++) {
      for (int y = 0; y < a.h; y++) {
        for (int x = 0; x < a.w; x++) {
          float pixA = a.data[a.pixel_address(x, y, ch)];
          float pixB = b.data[b.pixel_address(x, y, ch)];
          ret.data[a.pixel_address(x, y, ch)] = pixA - pixB;
        }
      }
    }
  
  return ret;
  
  }

// HW1 #4.1
// returns basic GX filter
Image make_gx_filter()
  {
  
    Image filter;
    filter = Image(3, 3, 1);
    filter.data[0] = -1;
    filter.data[1] = 0;
    filter.data[2] = 1;
    filter.data[3] = -2;
    filter.data[4] = 0;
    filter.data[5] = 2;
    filter.data[6] = -1;
    filter.data[7] = 0;
    filter.data[8] = 1;
    return filter;
  }

// HW1 #4.1
// returns basic GY filter
Image make_gy_filter()
  {
    Image filter;
    filter = Image(3, 3, 1);
    filter.data[0] = -1;
    filter.data[1] = -2;
    filter.data[2] = -1;
    filter.data[3] = 0;
    filter.data[4] = 0;
    filter.data[5] = 0;
    filter.data[6] = 1;
    filter.data[7] = 2;
    filter.data[8] = 1;
    return filter;
  }

// HW1 #4.2
// Image& im: input image
void feature_normalize(Image& im)
  {
  assert(im.w*im.h); // assure we have non-empty image
    for (int ch = 0; ch < im.c; ch++) {
      
      float ch_min = im.data[im.pixel_address(0, 0, ch)];
      float ch_max = ch_min;

      for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
          float curVal = im.data[im.pixel_address(x, y, ch)];
          if (curVal > ch_max) ch_max = curVal;
          if (curVal < ch_min) ch_min = curVal;
        }
      }
      float range = ch_max - ch_min;

      if (range == 0.0f) {
        for (int y = 0; y < im.h; y++) {
          for (int x = 0; x < im.w; x++) {
            im.data[im.pixel_address(x, y, ch)] = 0.0f;
          }
        }
      } else {
      for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
          float curVal = im.data[im.pixel_address(x, y, ch)];
          float normVal = (curVal - ch_min) / range;
          im.data[im.pixel_address(x, y, ch)] = normVal;
        }
      }
    }
  }
  }


// Normalizes features across all channels
void feature_normalize_total(Image& im)
  {
  assert(im.w*im.h*im.c); // assure we have non-empty image
  
  int nc=im.c;
  im.c=1;im.w*=nc;
  
  feature_normalize(im);
  
  im.w/=nc;im.c=nc;
  
  }


// HW1 #4.3
// Image& im: input image
// return a pair of images of the same size
pair<Image,Image> sobel_image(const Image& im)
  {
    Image gx = make_gx_filter();
    Image gy = make_gy_filter();

    Image Gx = convolve_image(im, gx, false);
    Image Gy = convolve_image(im, gy, false);

    Image im_magnitude(im.w, im.h, 1);
    Image im_direction(im.w, im.h, 1);

      for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
          int loc = im.pixel_address(x, y, 0);
          float GxVal = Gx.data[loc];
          float GyVal = Gy.data[loc];
          
          float magnitude = sqrt(GxVal * GxVal + GyVal * GyVal);
          im_magnitude.data[loc] = magnitude;

          float angle = atan2(GyVal, GxVal);
          im_direction.data[loc] = 0.5f + (angle / (2 * M_PI));
        }
      }
    feature_normalize(im_magnitude);
    feature_normalize(im_direction);
    return {im_magnitude, im_direction};
  }


// HW1 #4.4
// const Image& im: input image
// returns the colorized Sobel image of the same size
Image colorize_sobel(const Image& im)
  {
    Image gauss_im(im.w, im.h, 1);
    
    Image gauss_f = make_gaussian_filter(4);

    gauss_im = convolve_image(im, gauss_f, true);

    // Use magnitude as saturation and value
    pair<Image, Image> sobel = sobel_image(gauss_im); 
    Image magnitude = sobel.first;

    Image direction = sobel.second;
    
    for (int y = 0; y < direction.h; y++) {
      for (int x = 0; x < direction.w; x++) {
        int loc =  direction.pixel_address(x, y, 0);
        float pixVal = direction.data[loc];
        pixVal = 0.5 + pixVal / (2 * M_PI);
        direction.data[loc] = pixVal;
      }
    }

    // Apply hsv_to_rgb
    Image hsv(im.w, im.h, im.c);
     for (int y = 0; y < im.h; ++y) {
        for (int x = 0; x < im.w; ++x) {
            int loc = im.pixel_address(x, y, 0);
            float h = direction.data[loc];  // Hue from direction
            float s = magnitude.data[loc];  // Saturation from magnitude
            float v = magnitude.data[loc];  // Value from magnitude

            hsv.data[hsv.pixel_address(x, y, 0)] = h;
            hsv.data[hsv.pixel_address(x, y, 1)] = s;
            hsv.data[hsv.pixel_address(x, y, 2)] = v;
        }
    }
    hsv_to_rgb(hsv);

  return hsv;
  }

Image make_range_filter(const Image& im, int filter_size, int x, int y, float sigma) 
{
  float sigmaSquared = sigma * sigma;
  int distance = filter_size / 2;
  int numCh = im.c;
  Image filter = Image(filter_size, filter_size, 1);

  float center_r, center_g, center_b;
  center_r = im.clamped_pixel(x, y, 0);
  if (numCh > 1) {
    center_g = im.clamped_pixel(x, y, 1);
    center_b = im.clamped_pixel(x, y, 2);
  }
  for (int dy = -distance; dy <=  distance; dy++) {
    for (int dx = -distance; dx <= distance; dx++) {
      int i = x + dx;
      int j = y + dy;

      float diff_r, diff_g, diff_b;
      diff_r = im.clamped_pixel(i, j, 0) - center_r;
      if (numCh > 1) {
        diff_g = im.clamped_pixel(i, j, 1) - center_g;
        diff_b = im.clamped_pixel(i, j, 2) - center_b; 
      }
      float color_distance_squared = diff_r * diff_r + diff_g * diff_g + diff_b * diff_b;

      float weight = std::exp(-color_distance_squared / (2 * sigmaSquared));
      filter.data[filter.pixel_address(dx + distance, dy + distance, 0)] = weight;
    }
  }
  return filter;
}

Image make_bilateral_filter(const Image& gaussian_filter, const Image& range_filter)
{
  Image bi_filter = Image(gaussian_filter.w, gaussian_filter.h, 1);

  for (int y = 0; y < gaussian_filter.h; y++) {
    for (int x = 0; x < gaussian_filter.w; x++) {
      int loc = bi_filter.pixel_address(x, y, 0);
      bi_filter.data[loc] = gaussian_filter.data[loc] * range_filter.data[loc];
    }
  }

  return bi_filter;
}

float compute_bilateral_pixel_value(const Image& im, const Image& bi_filter, int x, int y, int ch)
{
  int distance = bi_filter.w / 2;
  float weighted_sum = 0.0f;
  float total_weight = 0.0f;
  for (int dy = -distance; dy <= distance; dy++) {
    for (int dx = -distance; dx <= distance; dx++) {
      int ix = x + dx;
      int iy = y + dy;

      float neighbor_val = im.clamped_pixel(ix, iy, ch);

      int fx = dx + distance;
      int fy = dy + distance;

      float weight = bi_filter.data[bi_filter.pixel_address(fx, fy, 0)];

      weighted_sum += neighbor_val * weight;
      total_weight += weight;
    }
  } 
  if (total_weight == 0.0f) return 0.0f;
  return weighted_sum / total_weight;
}
// HW1 #4.5
// const Image& im: input image
// float sigma1,sigma2: the two sigmas for bilateral filter
// returns the result of applying bilateral filtering to im
Image bilateral_filter(const Image& im, float sigma1, float sigma2)
  {
  Image result = Image(im.w, im.h, im.c);
  
  Image gaussian_filter = make_gaussian_filter(sigma1);

  for (int ch = 0; ch < im.c; ch++) {
    for (int y = 0; y < im.h; y++) {
      for (int x = 0; x < im.w; x++) {
        Image range_filter = make_range_filter(im, gaussian_filter.w, x, y, sigma2);
        Image bi_filter = make_bilateral_filter(gaussian_filter, range_filter);
        float pixVal = compute_bilateral_pixel_value(im, bi_filter, x, y, ch);
        int loc = im.pixel_address(x, y, ch);
        if (pixVal < 0) pixVal = 0;
        if (pixVal > 1) pixVal = 1;

        result.data[loc] = pixVal;
      }
    }
  }

  return result;
  }



// HELPER MEMBER FXNS

void Image::feature_normalize(void) { ::feature_normalize(*this); }
void Image::feature_normalize_total(void) { ::feature_normalize_total(*this); }
void Image::l1_normalize(void) { ::l1_normalize(*this); }

Image operator-(const Image& a, const Image& b) { return sub_image(a,b); }
Image operator+(const Image& a, const Image& b) { return add_image(a,b); }
