/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/*
 * Please fill in the following student struct:
 */
const team_t team = {
    "adte5487@colorado.edu", //Replace this with your email address.
    ""                   //Replace this with your partner's email address. Leave blank if working alone.
};

/***************
 * FLIP KERNEL
 ***************/

/******************************************************
 * Your different versions of the flip kernel go here
 ******************************************************/

/*
 * naive_flip - The naive baseline version of flip
 */
char naive_flip_descr[] = "naive_flip: Naive baseline implementation";
void naive_flip(int dim, pixel *src, pixel *dst)
{
    int i, j;
    for (i = 0; i < dim; i++){
        for (j = 0; j < dim; j++){
            dst[RIDX_F(i, j, dim)].red   = src[RIDX(i, j, dim)].red;
            dst[RIDX_F(i, j, dim)].green = src[RIDX(i, j, dim)].green;
            dst[RIDX_F(i, j, dim)].blue  = src[RIDX(i, j, dim)].blue;
        }
    }
}

/*
 * flip - Your current working version of flip
 * IMPORTANT: This is the version you will be graded on
 */
char flip_descr[] = "flip: Current working version";
void flip(int dim, pixel *src, pixel *dst)
{
  int i, j, ii, jj, blockSize, dimMinusOne;
  blockSize = 32;
  dimMinusOne = dim - 1;
  for (i = 0; i < dim; i+=blockSize)
  {
    for (j = 0; j < dim; j+=blockSize)
    {
      for (ii = i; ii < i + blockSize; ii++)
      {
        for (jj = j; jj < j + blockSize; jj++)
        {
          dst[RIDX(dimMinusOne-jj, ii, dim)] = src[RIDX(ii, jj, dim)];
        }
      }
    }
  }
}

char flip2_descr[] = "flip2: Unrolling Attemp";
void flip2(int dim, pixel *src, pixel *dst)
{
  int i, j;
  for (i = 0; i < dim; i++)
  {
    for (j = 0; j < dim; j+=2)
    {
      // dst[RIDX(dim-1-j, i, dim)].red = src[RIDX(i, j, dim)].red;
      // dst[RIDX(dim-1-j, i, dim)].green = src[RIDX(i, j, dim)].green;
      // dst[RIDX(dim-1-j, i, dim)].blue = src[RIDX(i, j, dim)].blue;
      dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
      dst[RIDX(dim-2-j, i, dim)] = src[RIDX(i, j + 1, dim)];

      /////////////////////////////////////////////////////////////

    }
  }
}

char flip3_descr[] = "flip3: Blocking Attemp w/ blocksize 8";
void flip3(int dim, pixel *src, pixel *dst)
{

}

/*********************************************************************
 * register_flip_functions - Register all of your different versions
 *     of the flip kernel with the driver by calling the
 *     add_flip_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_flip_functions()
{
    add_flip_function(&flip, flip_descr);
    //add_flip_function(&flip2, flip2_descr);
    //add_flip_function(&flip3, flip3_descr);
    add_flip_function(&naive_flip, naive_flip_descr);
    /* ... Register additional test functions here */
}


/***************
 * CONVOLVE KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the convolve function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute a pixel value */
typedef struct {
    float red;
    float green;
    float blue;
    float weight;
} pixel_sum;

/******************************************************
 * Your different versions of the convolve kernel go here
 ******************************************************/

/*
 * naive_convolve - The naive baseline version of convolve
 */
char naive_convolve_descr[] = "naive_convolve: Naive baseline implementation";
void naive_convolve(int dim, pixel *src, pixel *dst)
{
    int i, j, ii, jj, curI, curJ;
    pixel_sum ps;

    for (j = 0; j < dim; j++){
        for (i = 0; i < dim; i++){
            ps.red    = 0.0;
            ps.green  = 0.0;
            ps.blue   = 0.0;
            ps.weight = 0.0;
            for (jj = -2; jj <= 2; jj++){
                for (ii = -2; ii <= 2; ii++){
                    curJ = j+jj;
                    if(curJ<0 || curJ>=dim){
                        continue;
                    }
                    curI = i+ii;
                    if(curI<0 || curI>=dim){
                        continue;
                    }
                    ps.red   += src[RIDX(curI, curJ, dim)].red *   kernel[ii+2][jj+2];
                    ps.green += src[RIDX(curI, curJ, dim)].green * kernel[ii+2][jj+2];
                    ps.blue  += src[RIDX(curI, curJ, dim)].blue *  kernel[ii+2][jj+2];
                    ps.weight += kernel[ii+2][jj+2];
                }
            }
            dst[RIDX(i,j,dim)].red   = (unsigned short)(ps.red/ps.weight);
            dst[RIDX(i,j,dim)].green = (unsigned short)(ps.green/ps.weight);
            dst[RIDX(i,j,dim)].blue  = (unsigned short)(ps.blue/ps.weight);
        }
    }
}

/*
 * convolve - Your current working version of convolve.
 * IMPORTANT: This is the version you will be graded on
 */

void static inline solveRegion(int dim, pixel *src, pixel *dst, int initRows, int endRows, int initColumns, int endColumns)
{

  int i, j, ii, jj, curI, curJ, iip2, jjp2, r, g, b, w;
  //pixel_sum ps;

  for (i = initRows; i < endRows; i++){
      for (j = initColumns; j < endColumns; j++){
          r = 0;
          g = 0;
          b = 0;
          w = 0;

          for (ii = -2; ii <= 2; ii++){
              iip2 = ii + 2;
              curI = i+ii;
              if(curI<0 || curI>=dim){
                  continue;
              }

              for (jj = -2; jj <= 2; jj++){
                  curJ = j+jj;
                  if(curJ<0 || curJ>=dim){
                      continue;
                  }

                  jjp2 = jj + 2;
                  r += src[RIDX(curI, curJ, dim)].red *   kernel[iip2][jjp2];
                  g += src[RIDX(curI, curJ, dim)].green * kernel[iip2][jjp2];
                  b += src[RIDX(curI, curJ, dim)].blue *  kernel[iip2][jjp2];
                  w += kernel[iip2][jjp2];
              }
          }
          dst[RIDX(i,j,dim)].red   = (unsigned short)(r/w);
          dst[RIDX(i,j,dim)].green = (unsigned short)(g/w);
          dst[RIDX(i,j,dim)].blue  = (unsigned short)(b/w);
      }
  }
}

char convolve_descr[] = "convolve: Current working version";
void convolve(int dim, pixel *src, pixel *dst)
{
  int i, j, ip2, ip1, im1, im2, jp1, jp2, jm1, jm2, blockSize, ii, jj, dimMinusTwo, r, g, b;
  //pixel_sum ps;

  blockSize = 4;
  dimMinusTwo = dim - 2;
  for (i = 2; i < dimMinusTwo; i+=blockSize)
  {
      for (j = 2; j < dimMinusTwo; j+=blockSize)
      {
          for (ii = i; ii < i + blockSize; ii++)
          {
            for (jj = j; jj < j + blockSize; jj++)
            {
              r = 0;
              g = 0;
              b = 0;


              ip2 = ii + 2;
              ip1 = ii + 1;
              im2 = ii - 2;
              im1 = ii - 1;
              jp2 = jj + 2;
              jp1 = jj + 1;
              jm2 = jj - 2;
              jm1 = jj - 1;

              r += ~(src[RIDX(im2, jm2, dim)].red) + 1;
              g += ~(src[RIDX(im2, jm2, dim)].green) + 1;
              b += ~(src[RIDX(im2, jm2, dim)].blue) +1;


              r += ~(src[RIDX(im2, jm1, dim)].red) +1;
              g += ~(src[RIDX(im2, jm1, dim)].green) +1;
              b += ~(src[RIDX(im2, jm1, dim)].blue) +1;


              r += ~(src[RIDX(im2, jj, dim)].red) +1;
              g += ~(src[RIDX(im2, jj, dim)].green) +1;
              b += ~(src[RIDX(im2, jj, dim)].blue) +1;


              r += ~(src[RIDX(im2, jp1, dim)].red) +1;
              g += ~(src[RIDX(im2, jp1, dim)].green) +1;
              b += ~(src[RIDX(im2, jp1, dim)].blue) +1;


              // r += src[RIDX(im2, jp2, dim)].red * kernel[0][4];
              // g += src[RIDX(im2, jp2, dim)].green * kernel[0][4];
              // b += src[RIDX(im2, jp2, dim)].blue * kernel[0][4];
              // w += kernel[0][4];

              r += ~(src[RIDX(im1, jm2, dim)].red) +1;
              g += ~(src[RIDX(im1, jm2, dim)].green) +1;
              b += ~(src[RIDX(im1, jm2, dim)].blue) +1;


              r += ~(src[RIDX(im1, jm1, dim)].red << 4) +1;
              g += ~(src[RIDX(im1, jm1, dim)].green << 4) +1;
              b += ~(src[RIDX(im1, jm1, dim)].blue << 4) +1;


              r += src[RIDX(im1, jj, dim)].red << 2;
              g += src[RIDX(im1, jj, dim)].green << 2;
              b += src[RIDX(im1, jj, dim)].blue << 2;


              // r += src[RIDX(im1, jp1, dim)].red * kernel[1][3];
              // g += src[RIDX(im1, jp1, dim)].green * kernel[1][3];
              // b += src[RIDX(im1, jp1, dim)].blue * kernel[1][3];
              // w += kernel[1][3];

              r += src[RIDX(im1, jp2, dim)].red;
              g += src[RIDX(im1, jp2, dim)].green;
              b += src[RIDX(im1, jp2, dim)].blue;


              r += ~(src[RIDX(ii, jm2, dim)].red) +1;
              g += ~(src[RIDX(ii, jm2, dim)].green) +1;
              b += ~(src[RIDX(ii, jm2, dim)].blue) +1;


              r += ~(src[RIDX(ii, jm1, dim)].red << 2) +1;
              g += ~(src[RIDX(ii, jm1, dim)].green << 2) +1;
              b += ~(src[RIDX(ii, jm1, dim)].blue << 2) +1;


              r += src[RIDX(ii, jj, dim)].red;
              g += src[RIDX(ii, jj, dim)].green;
              b += src[RIDX(ii, jj, dim)].blue;


              r += src[RIDX(ii, jp1, dim)].red << 2;
              g += src[RIDX(ii, jp1, dim)].green << 2;
              b += src[RIDX(ii, jp1, dim)].blue << 2;


              r += src[RIDX(ii, jp2, dim)].red;
              g += src[RIDX(ii, jp2, dim)].green;
              b += src[RIDX(ii, jp2, dim)].blue;


              r += ~(src[RIDX(ip1, jm2, dim)].red) +1;
              g += ~(src[RIDX(ip1, jm2, dim)].green) +1;
              b += ~(src[RIDX(ip1, jm2, dim)].blue) +1;


              // r += src[RIDX(ip1, jm1, dim)].red * kernel[3][1];
              // g += src[RIDX(ip1, jm1, dim)].green * kernel[3][1];
              // b += src[RIDX(ip1, jm1, dim)].blue * kernel[3][1];
              // w += kernel[3][1];

              r += src[RIDX(ip1, jj, dim)].red << 2;
              g += src[RIDX(ip1, jj, dim)].green << 2;
              b += src[RIDX(ip1, jj, dim)].blue << 2;


              r += src[RIDX(ip1, jp1, dim)].red << 4;
              g += src[RIDX(ip1, jp1, dim)].green << 4;
              b += src[RIDX(ip1, jp1, dim)].blue << 4;


              r += src[RIDX(ip1, jp2, dim)].red;
              g += src[RIDX(ip1, jp2, dim)].green;
              b += src[RIDX(ip1, jp2, dim)].blue;


              // r += src[RIDX(ip2, jm2, dim)].red * kernel[4][0];
              // g += src[RIDX(ip2, jm2, dim)].green * kernel[4][0];
              // b += src[RIDX(ip2, jm2, dim)].blue * kernel[4][0];
              // w += kernel[4][0];

              r += src[RIDX(ip2, jm1, dim)].red;
              g += src[RIDX(ip2, jm1, dim)].green;
              b += src[RIDX(ip2, jm1, dim)].blue;


              r += src[RIDX(ip2, jj, dim)].red;
              g += src[RIDX(ip2, jj, dim)].green;
              b += src[RIDX(ip2, jj, dim)].blue;


              r += src[RIDX(ip2, jp1, dim)].red;
              g += src[RIDX(ip2, jp1, dim)].green;
              b += src[RIDX(ip2, jp1, dim)].blue;


              r += src[RIDX(ip2, jp2, dim)].red;
              g += src[RIDX(ip2, jp2, dim)].green;
              b += src[RIDX(ip2, jp2, dim)].blue;


              dst[RIDX(ii,jj,dim)].red   = (unsigned short)(r/9);
              dst[RIDX(ii,jj,dim)].green = (unsigned short)(g/9);
              dst[RIDX(ii,jj,dim)].blue  = (unsigned short)(b/9);

            }
          }
      }
  }

  solveRegion(dim, src, dst, 0, 2, 0, dim); // Top rows
  solveRegion(dim, src, dst, dimMinusTwo, dim, 0, dim); // Bottom rows
  solveRegion(dim, src, dst, 2, dimMinusTwo, 0, 2); // Left Columns
  solveRegion(dim, src, dst, 2, dimMinusTwo, dimMinusTwo, dim); // Right Columns

}

/*********************************************************************
 * register_convolve_functions - Register all of your different versions
 *     of the convolve kernel with the driver by calling the
 *     add_convolve_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_convolve_functions() {
    add_convolve_function(&convolve, convolve_descr);
    add_convolve_function(&naive_convolve, naive_convolve_descr);
    /* ... Register additional test functions here */
}
