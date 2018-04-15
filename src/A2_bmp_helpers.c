/* FILE: A2_bmp_helpers.c is where you will code your answers for Assignment 2.
 * 
 * Each of the functions below can be considered a start for you. 
 *
 * You should leave all of the code as is, except for what's surrounded
 * in comments like "REPLACE EVERTHING FROM HERE... TO HERE.
 *
 * The assignment document and the header A2_bmp_headers.h should help
 * to find out how to complete and test the functions. Good luck!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

int bmp_open( char* bmp_filename,        unsigned int *width, 
              unsigned int *height,      unsigned int *bits_per_pixel, 
              unsigned int *padding,     unsigned int *data_size, 
              unsigned int *data_offset, unsigned char** img_data ){

  //Opens the BMP file to read from it
  FILE *bmp_file = fopen(bmp_filename, "rb");

  //Checks if the file is of the right format
  char b, m;
  fread(&b, 1, 1, bmp_file);
  fread(&m, 1, 1, bmp_file);
  if (b != 'B' || m != 'M'){
    printf("The file provided is not of the right format. Please provide a BMP image.");
    return -1;
  }

  //Reads the size of the file and closes the file to stop reading 
  unsigned int size;
  fread(&size, 1, sizeof(unsigned int), bmp_file);
  *data_size = size;
  
  //Advancing 4 bytes
  char useless1;
  for (int i = 0; i < 4; i++) {
    fread (&useless1, 1, sizeof (char), bmp_file);
  }

  //data_offset
  unsigned int offset;
  fread (&offset,1,sizeof (unsigned int),bmp_file);
  *data_offset= offset;

  //Advancing 1 byte
  int useless2 = 0;
  fread (&useless2, 1, sizeof (int), bmp_file);

  //width of the image
  unsigned int w;
  fread (&w, 1, sizeof(unsigned int), bmp_file);
  *width = w;
  
  //height of the image
  unsigned int h;
  fread (&h, 1, sizeof(unsigned int), bmp_file);
  *height = h;

  //Advancing 2 bytes
  for (int i = 0; i < 2; i++) {
    fread (&useless1, 1, sizeof(char), bmp_file);
  }
  
  //bits per pixel
  unsigned int bpp;
  fread (&bpp, 1, sizeof (unsigned int), bmp_file);
  *bits_per_pixel = bpp;  

  //padding
  unsigned int p;
  p = ( (*width) % (*bits_per_pixel) ) % 4;
  *padding = p;

  fclose(bmp_file);
        
  bmp_file = fopen (bmp_filename, "rb");

  //allocating memory and copying the image data into the heap
  *img_data = (unsigned char*)malloc(*data_size);
  fread (*img_data, 1, *data_size, bmp_file); 
  
  fclose (bmp_file);

  return 0;  
}

// We've implemented bmp_close for you. No need to modify this function
void bmp_close( unsigned char **img_data ){

  if( *img_data != NULL ){
    free( *img_data );
    *img_data = NULL;
  }
}

int bmp_mask( char* input_bmp_filename, char* output_bmp_filename, 
              unsigned int x_min, unsigned int y_min, unsigned int x_max, unsigned int y_max,
              unsigned char red, unsigned char green, unsigned char blue )
{
  unsigned int img_width;
  unsigned int img_height;
  unsigned int bits_per_pixel;
  unsigned int data_size;
  unsigned int padding;
  unsigned int data_offset;
  unsigned char* img_data    = NULL;
  
  int open_return_code = bmp_open( input_bmp_filename, &img_width, &img_height, &bits_per_pixel, &padding, &data_size, &data_offset, &img_data ); 
  
  if( open_return_code ){ printf( "bmp_open failed. Returning from bmp_mask without attempting changes.\n" ); return -1; }
 
  // YOUR CODE FOR Q2 SHOULD REPLACE EVERYTHING FROM HERE
  //printf( "BMP_MASK is not yet implemented. Please help complete this code!\n" );
  // TO HERE!
  
  //Allocates memory for the new image and copies the old image into it
  unsigned char *new_image = (unsigned char*)(malloc(data_size));
  memcpy(new_image, img_data, data_size);

  //sets variables to navigate through the pixels of the image
  int x_count = 0;
  int y_count = 0;

  for (int i = 0 ; (x_count <= img_width) && (y_count < img_height) ; i += 3){

    //restarts the counters if we reach the end of a line
    if ((x_count != 0) && (x_count/(img_width) == 1)){

      if (padding != 0){
        for(int j = 0; j < padding; j++){
          new_image[data_offset + j] = 0;
        }
        i += padding;
      }

      y_count++;
      x_count = 0;
    }

    //Checks if the counters corresponds to the x and y boundaries set by the user
    if ((x_count <= x_max) && (x_count >= x_min)){
      if ((y_count <= y_max) && (y_count >= y_min)){
        //Updates image coordinates to the color set by the user
        new_image[data_offset + i] = blue;
        new_image[data_offset + i + 1] = green;
        new_image[data_offset + i + 2] = red;

      }
    }

    x_count++;
  }

  //Writes new file with new image data
  FILE *fp = fopen (output_bmp_filename, "w");
  fwrite(new_image, data_size, 1, fp);

  bmp_close( &img_data );
  
  return 0;
}         


int bmp_collage( char* bmp_input1, char* bmp_input2, char* bmp_result, int x_offset, int y_offset ){

  unsigned int img_width1;
  unsigned int img_height1;
  unsigned int bits_per_pixel1;
  unsigned int data_size1;
  unsigned int padding1;
  unsigned int data_offset1;
  unsigned char* img_data1    = NULL;
  
  int open_return_code = bmp_open( bmp_input1, &img_width1, &img_height1, &bits_per_pixel1, &padding1, &data_size1, &data_offset1, &img_data1 ); 
  
  if( open_return_code ){ printf( "bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n", bmp_input1 ); return -1; }
  
  unsigned int img_width2;
  unsigned int img_height2;
  unsigned int bits_per_pixel2;
  unsigned int data_size2;
  unsigned int padding2;
  unsigned int data_offset2;
  unsigned char* img_data2    = NULL;
  
  open_return_code = bmp_open( bmp_input2, &img_width2, &img_height2, &bits_per_pixel2, &padding2, &data_size2, &data_offset2, &img_data2 ); 
  
  if( open_return_code ){ printf( "bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n", bmp_input2 ); return -1; }
  
  int new_width = 0;
  int new_height = 0;

  //Initializing new_width according to the x_offset
  if (x_offset > 0){

    if((img_width2 + x_offset) > img_width1)
      new_width = img_width2 + x_offset;
    else
      new_width = img_width1;

  } 

  if (x_offset < 0){

    if(img_width2>img_width1)
      new_width = img_width2;
    else
      new_width = img_width1 - x_offset;
    
  } 

  if (x_offset == 0){

    if(img_width2>img_width1)
      new_width = img_width2;
    else
      new_width = img_width1;
  }


  //Initializing new_height according to y_offset
  if (y_offset > 0){

    if ((img_height2 + y_offset) > img_height1)
      new_height = img_height2 + y_offset;
    else
      new_height = img_height1;
    

  } 

  if (y_offset < 0){

    if (img_height2 > img_height1)
      new_height = img_height2;
    else
      new_height = img_height1 - y_offset;
    

  } 

  if (y_offset == 0){

    if (img_height2 > img_height1)
      new_height = img_height2;
    else
      new_height = img_height1;

  }


  //Computes the padding
  int new_padding = 0;
  int x = new_width;
  int y = bits_per_pixel1;

  for (int i = 0 ; i < 4 ; i++){

    unsigned int temp = x * y/8;

    if ((temp + i) % 4 == 0)
      new_padding = i;

  }

  //Computes the memory necessary for the new image
  int new_data_space = (new_width * bits_per_pixel1 + new_padding) * new_height + data_offset1;

  //Computes where both collated images will be on the new canvas
  int x_start_img1 = 0;
  int x_start_img2 = 0;
  int x_end_img1 = 0;
  int x_end_img2 = 0;
  int y_start_img1 = 0;
  int y_start_img2 = 0;
  int y_end_img1 = 0;
  int y_end_img2 = 0;

  //x-axis
  if(x_offset > 0){

    x_start_img1 = 0;
    x_start_img2 = x_offset;
    x_end_img1 = img_width1;
    x_end_img2 = img_width2 + x_offset;

  } else if (x_offset < 0) {

    x_start_img1 = -x_offset;
    x_start_img2 = 0;
    x_end_img1 = img_width1 - x_offset;
    x_end_img2 = img_width2;

  } else {

    x_start_img1 = 0;
    x_start_img2 = 0;
    x_end_img1 = img_width1;
    x_end_img2 = img_width2;

  }

  //y-axis
  if(y_offset > 0){

    y_start_img1 = 0;
    y_start_img2 = y_offset;
    y_end_img1 = img_height1;
    y_end_img2 = img_height2 + y_offset;

  } else if (y_offset < 0) {

    y_start_img1 = -y_offset;
    y_start_img2 = 0;
    y_end_img1 = img_height1 - y_offset;
    y_end_img2 = img_height2;

  } else {

    y_start_img1 = 0;
    y_start_img2 = 0;
    y_end_img1 = img_height1;
    y_end_img2 = img_height2;
    
  }   

  //allocates memory for the new image
  char *new_img_data = (char*) malloc(new_data_space);
  memcpy(new_img_data, img_data1, data_offset1);  

  //Changes header values
  //data size
  new_img_data [2] = (char) (new_data_space);
  new_img_data [3] = (char) (new_data_space >> 8);
  new_img_data [4]= (char) (new_data_space >> 16);
  new_img_data [5] = (char) (new_data_space >> 24);

  //width
  new_img_data [18]= (char) (new_width);
  new_img_data [19]= (char) (new_width >> 8);
  new_img_data [20]= (char) (new_width >> 16);
  new_img_data [21]= (char) (new_width >> 24);

  //height
  new_img_data [22]= (char) (new_height);
  new_img_data [23]= (char) (new_height >> 8);
  new_img_data [24]= (char) (new_height >> 16);
  new_img_data [25]= (char) (new_height >> 24);

  //counters to keep track of image coordinates and the current position
  int x1 = 0;
  int x2 = 0;
  int y1 = 0;
  int y2 = 0;

  int p1 = 0;
  int p2 = 0;

  int bpr = new_width * 3 + new_padding;
  
  //loops through the whole canvas
  for (int i = 0 ; i < new_height ; i++){

    for (int j = 0 ; j < new_width ; j++){

      //declares some status variables
      int pixel = 0;
      int position = data_offset1 + i * bpr + j * (bits_per_pixel1 / 8);

      //Checks if current position is in image 1
      if ((j >= x_start_img1) && (j < x_end_img1) && (i >= y_start_img1) && (i < y_end_img1)){

        p1 = data_offset1 + y1 * (img_width1 * (bits_per_pixel1 / 8) + padding1) + x1 * (bits_per_pixel1 / 8);
        new_img_data [position] = img_data1 [p1];
        new_img_data [position + 1] = img_data1 [p1 + 1];
        new_img_data [position + 2] = img_data1 [p1 + 2];
        x1++;

        if (x1 == img_width1) {
          x1 = 0;
          y1++;
        }
        pixel = 1;

      }

      //Checks if current position is in image 2
      if ((j >= x_start_img2) && (j < x_end_img2) && (i >= y_start_img2) && (i < y_end_img2)){

        p2 = data_offset2 + y2 * (img_width2 * (bits_per_pixel2 / 8) + padding2) + x2 * (bits_per_pixel2 / 8);
        new_img_data [position] = img_data2 [p2];
        new_img_data [position + 1] = img_data2 [p2 + 1];
        new_img_data [position + 2] = img_data2 [p2 + 2];
        x2++;

        if (x2 == img_width2) {
          x2 = 0;
          y2++;
        }
        pixel = 1;
      }

      //Checks if the pixel was modified (black if not)
      if (pixel == 0){
        new_img_data [position] = 0;
        new_img_data [position + 1] = 0;
        new_img_data [position + 2] = 0;
      }

    }

  }
  
  //Creates and writes on the file
  FILE * final = fopen (bmp_result, "w");
  fwrite (new_img_data, new_data_space, 1, final);

  //Closes the files 
  fclose (final);
  unsigned char* final_memory = (unsigned char*) new_img_data;
  bmp_close (&final_memory);
  
  bmp_close( &img_data1 );
  bmp_close( &img_data2 );

  
  return 0;
}                  
