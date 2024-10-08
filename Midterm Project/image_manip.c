#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "image_manip.h"
#include "ppm_io.h"


////////////////////////////////////////
// Definitions of the functions       //
// declared in image_manip.h go here! //
////////////////////////////////////////

/*
 * Perform grayscale on an image and return the resulting image
 */
Image grayscale(const Image in) {
  // Create a new image for the grayscale operation
  Image out = make_image(in.rows, in.cols);
  
  // Calculate the pixel values based on the in image and store them in out
  for (int i = 0; i < in.rows * in.cols; i++) {
    unsigned char gray = (unsigned char) (0.3 * in.data[i].r + 0.59 * in.data[i].g + 0.11 * in.data[i].b);
    out.data[i].r = gray;
    out.data[i].g = gray;
    out.data[i].b = gray;
  }
  
  return out;
}

/*                                                                                                                                                                                                            
 * Blend two images together where they overlap and return the resulting image                                                                                                                                
 */
Image blend(const Image in1, const Image in2, double alpha) {

  //Caculate the rows and columns of the output image
  int outRows = (in1.rows > in2.rows) ? in1.rows : in2.rows;
  int outCols = (in1.cols > in2.cols) ? in1.cols : in2.cols;

  Image out = make_image(outRows, outCols);
  
  for (int i = 0; i < out.rows; i++){
    for (int j = 0; j < out.cols; j++){

      //initial lize two pixel layers
      Pixel pixel1 = {0,0,0};
      Pixel pixel2 = {0,0,0};

      //test pixel layers to see if they are within the bound
      if (i < in1.rows && j < in1.cols){
	pixel1 = in1.data[i * in1.cols + j];
      }
      if (i < in2.rows && j < in2.cols){
	pixel2 = in2.data[i * in2.cols + j];
      }

      //blend the pixels together
      Pixel blendedPixel;

      //blend them if in the two layers overlap
      if (i < in1.rows && j < in1.cols && i < in2.rows && j < in2.cols) {
	
        blendedPixel.r = (unsigned char)((pixel1.r * alpha) + (pixel2.r * (1 - alpha)));
        blendedPixel.g = (unsigned char)((pixel1.g * alpha) + (pixel2.g * (1 - alpha)));
        blendedPixel.b = (unsigned char)((pixel1.b * alpha) + (pixel2.b * (1 - alpha)));

	//else just output the direct value of each image 
      } else if (i < in1.rows && j < in1.cols) {
	blendedPixel = pixel1;
      } else if (i < in2.rows && j < in2.cols) {
	blendedPixel = pixel2;
      }

      
      //itnerate through the pixels
      out.data[i * outCols + j] = blendedPixel;
    }
  }

  //output the pixels that form a picture
  return out;
}

/*                                                                                                                                                                                                             
 * Rotate an image counter-clockwise and return the resulting image
 */
Image rotate_ccw(const Image in) {
  // Create a new image for the grayscale operation
  // The image is rotated so out has the column number of in's rows and vice versa 
  Image out = make_image(in.cols, in.rows);
  
  // Iterate through the pixels in in and assign them to out based on their new positions
  int index = 0;
  for (int j = in.cols - 1; j > -1; j--) {
    for (int i = 0; i < in.rows; i++) {
      out.data[index++] = in.data[i * in.cols + j];
    }
  }
  
  return out;
}

/*                                                                                                                                                                                                             
 * Perform a pointism affect on a given image and return the resulting image by picking pixels at random and setting a radius of pixels to the same colors
 */
Image pointilism(const Image in, unsigned int seed) {
  
  //set seed
  srand(seed);

  //calculate total pixels and the pixels to modify
  int totalPixels = in.rows * in.cols;
  int pixelsToModify = (int)(totalPixels * 0.03);

  //generate output image of the same dimension
  Image out = make_image(in.rows, in.cols);

  // initialize the pixels so they are black
  for (int i = 0; i < in.rows * in.cols; i++) {
    out.data[i].r = 0;
    out.data[i].g = 0;
    out.data[i].b = 0;
  }
  
  for(int i = 0; i < pixelsToModify; i++) {

    //select random rows and cols within the range
    const int randCol = rand() % in.cols;
    const int randRow = rand() % in.rows;

    //generate random raidus from 1 to 5
    const int radius = (rand() % 5) + 1;

    //test if the pixel is in range of circle 
    for (int y = randRow - radius; y <= randRow + radius; y++) {
      for (int x = randCol - radius; x <= randCol + radius; x++) {

	//if the pixel is around the circle of the center, set it to the same rgb of the center
	if(x >= 0 && x < in.cols && y >= 0 && y < in.rows){
	  int dx = x - randCol;
	  int dy = y - randRow;

	  //based on the function x^2 + y^2 <= r^2
	  if( (dx * dx + dy * dy) <= (radius * radius)) {
	      out.data[y * in.cols + x] = in.data[randRow * in.cols + randCol];
	  }
	}
      }
    }
  }
  return out;
}  

/*                                                                                                                                                                                                             
 * Blur an image with a given value for sigma and return the blurred image
 */
Image blur(const Image in, double sigma) {
  // Create a new image for the grayscale operation                                                                                                                                                            
  Image out = make_image(in.rows, in.cols);

  // Calculate N, the dimension of the square matrix                                                                                                                                                         
  int N	= (int)	(sigma * 10);
  // Add 1 if N is even                                                                                                                                                                                        
  if (N % 2 == 0) {
    N++;
  }
  
  // Generate the Gaussian matrix
  double ** gauss = gaussian_matrix(N, sigma);

  // Apply the filer to each pixel
  for (int i = 0; i < in.rows; i++) {
    for (int j = 0; j < in.cols; j++) { 
      out.data[i * in.cols + j] = filter_pixel(i, j, in, N, gauss);
    }
  }

  // Free the Gaussian matrix
  for (int i = 0; i < N; i++) {
    free(gauss[i]);
  }
  free(gauss);
  
  return out;
}

/*
 * Function to create a gaussian matrix for a given value of sigma to be used in blurring an image
 */
double ** gaussian_matrix(int N, double sigma) {
  // Allocate the matrix
  double ** gauss = malloc(N * sizeof(double*));
  for (int i = 0; i < N; i++) {
    gauss[i] = malloc(N * sizeof(double));
  }
  int center = N / 2;
  // Calculate g for each entry based on distance from the center
  for (int dy = -1 * center; dy < center + 1; dy++) {
    for (int dx = -1 * center; dx < center + 1; dx++) {
      gauss[center + dy][center + dx] = (1.0 / (2.0 * PI * sq(sigma))) * exp( -(sq(dx) + sq(dy)) / (2 * sq(sigma)));
    }
  }
  
  return gauss;
}

/*
 * Helper function for the blur function to apply a gauss matrix filter to a given pixel and its surroundings and return the averaged pixel
 */
Pixel filter_pixel(const int i, const int j, const Image in, const int N, double ** gauss) {
  // The center of the gaussian matrix is at (N/2, N/2)
  int center = N / 2;
  // The index of the pixel in the fake 2D image data array
  int index = i * in.cols + j;
  // The index of the current pixel being examined
  int cur = 0;
  // The total weight some of each color channel before normalization
  double sum_r = 0;
  double sum_g = 0;
  double sum_b = 0;
  // The total some of the filter pixels used
  double filter_sum = 0;
  
  // Iterate through the pixels in the filer
  for (int dy = -1 * center; dy < center + 1; dy++) {
    for (int dx = -1 * center; dx < center + 1; dx++) {
      // Skip if the filter is out of bounds
      if (i + dy < 0 || i + dy > in.rows - 1) {
	continue;
      } else if (j + dx < 0 || j + dx > in.cols - 1){
	continue;
      } else {
	// Calculate the index of the current pixel being examined
	cur = index + in.cols * dy + dx;
	sum_r += in.data[cur].r * gauss[center + dy][center + dx];
	sum_g += in.data[cur].g * gauss[center + dy][center + dx];
	sum_b += in.data[cur].b * gauss[center + dy][center + dx];
	filter_sum += gauss[center + dy][center + dx];
      }
    }
  }
  
  // Instatiate the new blurred pixel with the normalized weighted sums for each color
  Pixel blurred_pixel = {0, 0, 0};
  blurred_pixel.r = (unsigned char) (sum_r / filter_sum);
  blurred_pixel.g = (unsigned char) (sum_g / filter_sum);
  blurred_pixel.b = (unsigned char) (sum_b / filter_sum);
  
  return blurred_pixel;
}

/*                                                                                                                                                                                                           
 * Saturate the colors on a given image with a given scale and return the new image
 */
Image saturate(const Image in, double scale) {

  //make an output file with the same dimension as the input file
  Image out = make_image(in.rows, in.cols);

  
  for (int i = 0; i < in.rows * in.cols; i++) {

    //calculate the grayscale value
    unsigned char gray = (0.3 * in.data[i].r + 0.59 * in.data[i].g + 0.11 * in.data[i].b);

    //calculate the saturation value
    double diffR = ((double)in.data[i].r - gray) * scale + gray;
    double diffG = ((double)in.data[i].g - gray) * scale + gray;
    double diffB = ((double)in.data[i].b - gray) * scale + gray;

    //check if saturation value it is out bound; if out of bound, clamp it to the bound; if in bound, assign them to rgb of output
    out.data[i].r =(unsigned char) (diffR < 0 ? 0: (diffR > 255 ? 255 : diffR));
    out.data[i].g =(unsigned char) (diffG < 0 ? 0: (diffG > 255 ? 255 : diffG));
    out.data[i].b =(unsigned char) (diffB < 0 ? 0: (diffB > 255 ? 255 : diffB));
  }

  //return output value
  return out;
}
