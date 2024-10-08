//project.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm_io.h"
#include "image_manip.h"

// Return (exit) codes
#define RC_SUCCESS            0
#define RC_MISSING_FILENAME   1
#define RC_OPEN_FAILED        2
#define RC_INVALID_PPM        3
#define RC_INVALID_OPERATION  4
#define RC_INVALID_OP_ARGS    5
#define RC_OP_ARGS_RANGE_ERR  6
#define RC_WRITE_FAILED       7
#define RC_UNSPECIFIED_ERR    8

void print_usage();

int main (int argc, char* argv[]) {
  
  // Check that an input filename was given
  if (argv[1] == NULL) {
    fprintf(stderr, "Input filename not provided.\n");
    print_usage();
    return RC_MISSING_FILENAME;
  }
  
  // Check that an output filename was given
  if (argv[2] == NULL) {
    fprintf(stderr, "Output filename not provided.\n");
    print_usage();
    return RC_MISSING_FILENAME;
  }

  // Check that the input file opens
  FILE * file_in = fopen(argv[1], "rb");
  if (file_in == NULL) {
    fprintf(stderr, "Input file I/O error.\n");
    return RC_OPEN_FAILED;
  }

  // Check that the input file is a valid ppm file
  Image image_in = read_ppm(file_in);
  fclose(file_in);
  if (image_in.data == NULL || image_in.rows == 0 || image_in.cols == 0) {
    free_image(&image_in);
    fprintf(stderr, "The Input file cannot be read as a PPM file.\n");
    return RC_INVALID_PPM;
  }

  // Make the output image
  Image image_out = {NULL, 0, 0};

  // Check an operation was entered
  if (argc < 4) {
    free_image(&image_in);
    free_image(&image_out);
    fprintf(stderr, "Unsupported image processing operations.\n");
    print_usage();
    return RC_INVALID_OPERATION;
  }

  // Check that the operation is valid; if so, perform the operation
  if (strcmp(argv[3], "grayscale") == 0) {
    // GRAYSCALE

    // Check that enough arguments were given
    if (argc != 4) {
      free_image(&image_in);
      free_image(&image_out);
      fprintf(stderr, "Incorrect number of arguments for the specified operation.\n");
      print_usage();
      return RC_INVALID_OP_ARGS;
    }

    image_out = grayscale(image_in);
    
  } else if (strcmp(argv[3], "blend") == 0) {
    // BLEND

    // Check that enough arguments were given
    if (argc != 6) {
      free_image(&image_in);
      free_image(&image_out);
      fprintf(stderr, "Incorrect number of arguments for the specified operation.\n");
      return RC_INVALID_OP_ARGS;
    }
    
    // Check that the second input file opens                                                                                                                                                                 
    FILE * file2_in = fopen(argv[2], "rb");
    if (file2_in == NULL) {
      free_image(&image_in);
      free_image(&image_out);
      fprintf(stderr, "Input file I/O error.\n");
      return RC_OPEN_FAILED;
    }
    
    // Check that the second input file is a valid ppm file                                                                                                                                                    
    Image image2_in = read_ppm(file2_in);
    fclose(file2_in);
    if (image2_in.data == NULL || image2_in.rows == 0 || image2_in.cols == 0) {
      free_image(&image_in);
      free_image(&image_out);
      fprintf(stderr, "The Input file cannot be read as a PPM file.\n");
      return RC_INVALID_PPM;
    }
    
    // Check that an output filename was given                                                                                                                                                                 
    if (argv[4] == NULL) {
      free_image(&image_in);
      free_image(&image2_in);
      free_image(&image_out);
      fprintf(stderr, "Output filename not provided.\n");
      print_usage();
      return RC_MISSING_FILENAME;
    }

    // Check that alpha is given as a double and is in the correct range
    double alpha;
    int num_read = sscanf(argv[5], "%lf", &alpha);
    if (num_read != 1 && alpha >= 0 && alpha <= 1) {
      free_image(&image_in);
      free_image(&image2_in);
      free_image(&image_out);
      fprintf(stderr, "Invalid arguments for the specified operation.\n");
      print_usage();
      return RC_INVALID_OP_ARGS;
    }

    image_out = blend(image_in, image2_in, alpha);

    // Check that the output file opens                                                                                                                                                                      
    FILE * file_out = fopen(argv[4], "wb");
    if (file_out == NULL) {
      free_image(&image_in);
      free_image(&image2_in);
      free_image(&image_out);
      fprintf(stderr, "Output File I/O error.\n");
      return RC_WRITE_FAILED;
    }

    // Write the new image to the output file
    int pixels = write_ppm(file_out, image_out);
    if (pixels != image_out.rows * image_out.cols) {
      free_image(&image_in);
      free_image(&image2_in);
      free_image(&image_out);
      fprintf(stderr, "Output File I/O error.\n");
      return RC_WRITE_FAILED;
    }

    // Close the files and free the images
    fclose(file_out);
    free_image(&image_in);
    free_image(&image2_in);
    free_image(&image_out);
    
    return RC_SUCCESS;
    
  } else if (strcmp(argv[3], "rotate-ccw") == 0) {
    // ROTATE_CCW

    // Check that enough arguments were given
    if (argc != 4) {
      free_image(&image_in);
      free_image(&image_out);
      fprintf(stderr, "Incorrect number of arguments for the specified operation.\n");
      print_usage();
      return RC_INVALID_OP_ARGS;
    }

    image_out = rotate_ccw(image_in);
    
  } else if (strcmp(argv[3], "pointilism") == 0) {
    // POINTILISM

    // Check that enough arguments were given
    if (argc != 4) {
      free_image(&image_in);
      free_image(&image_out);
      fprintf(stderr, "Incorrect number of arguments for the specified operation.\n");
      print_usage();
      return RC_INVALID_OP_ARGS;
    }

    image_out = pointilism(image_in, 1);
    
  } else if (strcmp(argv[3], "blur") == 0) {
    // BLUR

    // Check that enough arguments were given
    if (argc != 5) {
      free_image(&image_in);
      free_image(&image_out);
      fprintf(stderr, "Incorrect number of arguments for the specified operation.\n");
      print_usage();
      return RC_INVALID_OP_ARGS;
    }

    // Check that alpha is given as a double                                                                                                                                                                   
    double sigma;
    int num_read = sscanf(argv[4], "%lf", &sigma);
    if (num_read != 1) {
      free_image(&image_in);
      free_image(&image_out);
      fprintf(stderr, "Invalid arguments for the specified operation.\n");
      return RC_INVALID_OP_ARGS;
    }

    image_out = blur(image_in, sigma);
    
  } else if (strcmp(argv[3], "saturate") == 0) {
    // SATURATE

    // Check that enough arguments were given
    if (argc != 5) {
      free_image(&image_in);
      free_image(&image_out);
      fprintf(stderr, "Incorrect number of arguments for the specified operation.\n");
      print_usage();
      return RC_INVALID_OP_ARGS;
    }

    // Check that alpha is given as a double                                                   
    double scale;
    int num_read = sscanf(argv[4], "%lf", &scale);
    if (num_read != 1) {
      free_image(&image_in);
      free_image(&image_out);
      fprintf(stderr, "Invalid arguments for the specified operation.\n");
      print_usage();
      return RC_INVALID_OP_ARGS;
    }

    image_out = saturate(image_in, scale);
    
  } else {
    // INVALID OPERATION
    free_image(&image_in);
    free_image(&image_out);
    fprintf(stderr, "Unsupported image processing operations.\n");
    print_usage();
    return RC_INVALID_OPERATION;
  }
  
  // Check that the output file opens
  FILE * file_out = fopen(argv[2], "wb");
  if (file_out == NULL) {
    free_image(&image_in);
    free_image(&image_out);
    fprintf(stderr, "Output File I/O error.\n");
    return RC_WRITE_FAILED;
  }

  // Write the new image to the output file                                                                                                                                                                
  int pixels = write_ppm(file_out, image_out);
  if (pixels != image_out.rows * image_out.cols) {
    free_image(&image_in);
    free_image(&image_out);
    fprintf(stderr, "Output File I/O error.\n");
    return RC_WRITE_FAILED;
  }
  
  // Close the files and free the images                                                                                                                                                                     
  fclose(file_out);
  free_image(&image_in);
  free_image(&image_out);
  
  return RC_SUCCESS;
}

void print_usage() {
  printf("USAGE: ./project <input-image> <output-image> <command-name> <command-args>\n");
  printf("SUPPORTED COMMANDS:\n");
  printf("   grayscale\n" );
  printf("   blend <target image> <alpha value>\n" );
  printf("   rotate-ccw\n" );
  printf("   pointilism\n" );
  printf("   blur <sigma>\n" );
  printf("   saturate <scale>\n" );
}


