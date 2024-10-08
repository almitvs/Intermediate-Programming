#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "ppm_io.h"



/* helper function for read_ppm, takes a filehandle
 * and reads a number, but detects and skips comment lines
 */
int read_num( FILE *fp ) {
  assert(fp);

  int ch;
  while((ch = fgetc(fp)) == '#') { // # marks a comment line
    while( ((ch = fgetc(fp)) != '\n') && ch != EOF ) {
      /* discard characters til end of line */
    }
  }
  ungetc(ch, fp); // put back the last thing we found

  int val;
  if (fscanf(fp, "%d", &val) == 1) { // try to get an int
    while(isspace(ch = fgetc(fp))) {
      // drop trailing whitespace
    }
    ungetc(ch, fp);
    return val; // we got a value, so return it
  } else {
    fprintf(stderr, "Error:ppm_io - failed to read number from file\n");
    return -1;
  }
}

Image read_ppm( FILE *fp ) {
  Image im = { NULL , 0 , 0 };
  
  /* confirm that we received a good file handle */
  if( !fp ){
	fprintf( stderr , "Error:ppm_io - bad file pointer\n" );
	return im;
  }

  int rows=-1 , cols=-1;

  /* read in tag; fail if not P6 */
  char tag[20];
  tag[19] = '\0';
  fscanf( fp , "%19s\n" , tag );
  if( strncmp( tag , "P6" , 20 ) ) {
	fprintf( stderr , "Error:ppm_io - not a PPM (bad tag)\n" );
	return im;
  }


  /* read image dimensions */

  //read in columns
  cols = read_num( fp ); // NOTE: cols, then rows (i.e. X size followed by Y size)
  //read in rows
  rows = read_num( fp );

  //read in colors; fail if not 255
  int colors = read_num( fp );
  if( colors!=255 ){
	fprintf( stderr , "Error:ppm_io - PPM file with colors different from 255\n" );
	return im;
  }

  //confirm that dimensions are positive
  if( cols<=0 || rows<=0 ){
	fprintf( stderr , "Error:ppm_io - PPM file with non-positive dimensions\n" );
	return im;
  }

  /* Allocate the new image */
  im = make_image( rows , cols );
  if( !im.data ){
	fprintf( stderr , "Error:ppm_io - Could not allocate new image\n" );
	return im;
  }
  /* finally, read in Pixels */

  /* read in the binary Pixel data */
  if( fread( im.data , sizeof(Pixel) , im.rows * im.cols , fp ) != (size_t)(im.rows * im.cols) ) {
    fprintf(stderr, "Error:ppm_io - failed to read data from file!\n");
	free_image( &im );
    return im;
  }

  //return the image struct pointer
  return im;
}



/* Write given image to disk as a PPM; assumes fp is not null */
int write_ppm( FILE *fp , const Image im ) {
  // Print the tag
  fprintf(fp, "P6\n");
  // Print the column and row numbers
  fprintf(fp, "%d %d\n", im.cols, im.rows);
  // Print the color
  fprintf(fp, "255\n");
  // Write the pixel values to the file in binary
  fwrite(im.data, sizeof(unsigned char), 3 * im.rows * im.cols, fp);
  return im.rows * im.cols;
}


/* allocate a new image of the specified size;
 * doesn't initialize pixel values */
Image make_image( int rows , int cols ) {
  // Create the image
  Image im = {NULL, 0, 0};
  // Set the rows and columns
  im.rows = rows;
  im.cols = cols;
  // Allocate the data array pointer
  im.data = malloc(rows * cols * sizeof(Pixel));
  return im;
}


/* output dimensions of the image to stdout */
void output_dims( const Image im ) {
  printf( "cols = %d, rows = %d" , im.cols , im.rows );
}

/* free_image
 * utility function to free inner and outer pointers, 
 * and set to null 
 */
void free_image( Image *im ) {
  // Free the data array
  free(im->data);
  im = NULL;
}
