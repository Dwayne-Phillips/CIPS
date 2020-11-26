
       /**********************************************
       *
       *   file lambert.c
       *
       *   Functions: This file contains
       *
       *   Purpose:
       *      These functions perform the lambert
       *      operations.
       *
       *   External Calls:
       *      none
       *
       *   Modifications:
       *      26 November 1999 - created
       *
       ************************************************/


       /******************************************
       *
       *   The k constants are for diffuse and
       *   specular reflectivity.  They are
       *   constants for a surface between 0 and 1.
       *
       *   eta is a factor for the reflectance
       *   of the object.  A large eta is for
       *   a shiny object that approaches a perfect
       *   mirror.  A small eta is for a dull
       *   surface.
       *
       ******************************************/

#include "cips.h"

int cross_product();
int angle_between();
int magnitude_of();

#undef NEVER

#define SOURCE         100
#define AMBIENT        200
#define DEGREESRADIANS 57.29577951
#define NINETYDEGREES  1.570796327
#define ONEEIGHTY      3.141592654


int lambert(the_image, out_image,
        k_diffuse, k_specular, eta,
        L,
        rows, cols)

   float  eta, k_diffuse, k_specular, *L;
   short  **the_image,
          **out_image;
   long   cols, rows;
{
   char   response[80];
   float  theta1, theta2, theta3, theta4;
   float  diffuse_term, specular_term;
   float  max_diffuse_term, max_specular_term;
   float  nl, vr;
   float  N[3], R[3], V[3], v1[3], v2[3];
   float  l, n, r, v;
   int    i, j;

   char line[100];
   FILE *log_file;


/****** log file ********************************/
   if((
   log_file = fopen("logfile", "wt")) == NULL){
      printf("\ntoc: Error opening paged file\n");
      exit(4);
   }

   max_diffuse_term  = 0.0;
   max_specular_term = 0.0;


      /******************************************
      *
      *   V, the viewer, is straight up
      *   v1 is the vector down one row
      *   v2 is the vector right one column
      *
      ******************************************/

   V[0] =  0.0;
   V[1] =  0.0;
   V[2] = -1.0;



   for(i=1; i<rows-1; i++){
      for(j=1; j<cols-1; j++){

            /*************************************
            *
            *   Calculate the vector N that is
            *   normal to the triangle whose
            *   corners are
            *   [i][j]    [i][j+1]
            *   [i+1][j]
            *
            *************************************/

         /*****************
         v1[0] = 0.0;
         v1[1] = 1.0;
         v1[2] = (float)(the_image[i+1][j] - the_image[i][j]);
         v2[0] = 1.0;
         v2[1] = 0.0;
         v2[2] = (float)(the_image[i][j+1] - the_image[i][j]);
         ****************/

         v1[0] = 0.0;
         v1[1] = 1.0;
         v1[2] = (float)(the_image[i][j] - the_image[i+1][j]);
         v2[0] = 1.0;
         v2[1] = 0.0;
         v2[2] = (float)(the_image[i][j] - the_image[i][j+1]);

         cross_product(v1, v2, N);

            /*************************************
            *
            *   Calculate the dot product of
            *   N and V.  N and V are both unit
            *   vectors (their magnitudes are 1).
            *   The dot product becomes the cosine
            *   of the angle between them.
            *
            *************************************/


            /*************************************
            *
            *   If theta1 > 90 degrees, the
            *   specular term will be zero.
            *   This is because there is no
            *   reflectance.
            *
            *   I am calculating vr the simple
            *   way so I don't need to find
            *   the values of R(x,y,z).
            *
            *   theta1 = angle between L and N
            *   theta2 = angle between N and V
            *   theta3 = angle between L and V
            *   theta4 = angle between V and R
            *
            *************************************/

         angle_between(L, N, &theta1);
         theta1 = ONEEIGHTY - theta1;
         angle_between(N, V, &theta2);
         angle_between(L, V, &theta3);
         theta3 = ONEEIGHTY - theta3;
         if(theta3 >= theta1)
            theta4 = theta1 - theta2;
         else
            theta4 = theta1 + theta2;

/************************
if( (i%20==0)  && (j%20==0) ){
printf("\ni=%d j=%d t1=%f t2=%f t3=%f t4=%f vector N is",
 i,j,theta1,theta2,theta3,theta4);
print_vector(N);
gets(response);
}
**************************/


         vr = cos(theta4);
         if(vr <= 0.0)
            vr = 0.0;
         vr = pow(vr, eta);
         specular_term = k_specular*vr;

         /****nl = dot_product(N, L);***/
         magnitude_of(N, &n);
         magnitude_of(L, &l);
         /*nl = cos(theta1)*n*l;*/
         nl = cos(theta1);
         diffuse_term = k_diffuse*nl;

            /* If the surface is in the shadow
               of the light, the diffuse and
               specular terms are zero. */
         if(theta1 >= NINETYDEGREES){
/*************
printf("\ntheta1=%5.2f i=%d j=%d and the normal vector is:",theta1,i,j);
print_vector(N);
*************/
            diffuse_term  = 0.0;
            specular_term = 0.0;
         }  /* ends if */


            /* Write to logfile */
         if((i%50) == 0){
         sprintf(line,"\nAMBIENT SOURCE   i   j   vr specterm nl    diffterm  T1  T4");
         fputs(line,log_file);
         sprintf(line,"\n%d      %d     %d  %d %4.2f %5.2f    %4.2f %5.2f  %5.2f  %5.2f",
                 AMBIENT, SOURCE, i, j, vr, specular_term, nl, diffuse_term, theta1, theta4);
         fputs(line,log_file);
         }


         if(specular_term > max_specular_term)
            max_specular_term = specular_term;
         if(diffuse_term > max_diffuse_term)
            max_diffuse_term = diffuse_term;

         out_image[i][j] = k_diffuse*AMBIENT +
                           SOURCE*(specular_term+diffuse_term);
         if(out_image[i][j] < 0)
            out_image[i][j] = 0;
         if(out_image[i][j] > GRAY_LEVELS-1)
            out_image[i][j] = GRAY_LEVELS-1;

/*******************
if( i==j && (i%20) == 0 ){
printf("\nPress Enter to continue");
printf("\nAMBIENT=%d SOURCE=%d kdiffuse=%f kspecular=%f",
      AMBIENT,SOURCE,k_diffuse, k_specular);
printf("\nSpecular=%f  Diffuse=%f  (nl=%f) out[%d][%d]=%d",
      specular_term, diffuse_term, nl, i, j, out_image[i][j]);
gets(response);
}
*******************/

/**************************
if( i==j && (i%20) == 0 ){
printf("\ni=%d j=%d image[%d][%d]=%d image[%d][%d]=%d image[%d][%d]=%d",
      i, j, i, j, the_image[i][j], i+1, j, the_image[i+1][j],
      i, j+1, the_image[i][j+1]);
printf("\nv1[2] should be %d-%d=%d, it is %f",
         the_image[i+1][j],the_image[i][j],
         the_image[i+1][j] - the_image[i][j], v1[2]);
printf("\nv2[2] should be %d-%d=%d, it is %f",
         the_image[i][j+1],the_image[i][j],
         the_image[i][j+1] - the_image[i][j], v2[2]);

printf("\ntheta1=%f theta2=%f theta3=%f",theta1,theta2,theta3);
printf("\nVector v1");
print_vector(v1);
printf("\nVector v2");
print_vector(v2);
printf("\nVector Normal");
print_vector(N);
printf("\nSpecular=%f  Diffuse=%f  out[%d][%d]=%d",
      specular_term, diffuse_term, i, j, out_image[i][j]);
printf("\nPress Enter to continue");
gets(response);
}
********************************/

      }  /* ends loop over j */
   }  /* ends loop over i */


      /* fix the edges of the output image */
   for(i=0; i<rows; i++){
      for(j=1; j>0; j--){
       out_image[i][j-1] = out_image[i][j];
       out_image[i][cols-j] = out_image[i][cols-j-1];
      }
   }

   for(j=0; j<cols; j++){
      for(i=1; i>0; i--){
       out_image[i-1][j] = out_image[i][j];
       out_image[rows-i][j] = out_image[rows-i-1][j];
      }
   }

      /* close the log file */
   fclose(log_file);

printf("\n\nmax_specular=%f    max_diffuse=%f",
max_specular_term, max_diffuse_term);

return(1);
}  /* ends lambert */





     /*******************************************
     *
     *   magnitude_of(...
     *
     *
     *******************************************/

int magnitude_of(v, answer)
   float v[], *answer;
{
   double d, n;
   n  = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
   d  = sqrt(n);
   *answer = d;
return(1);
}  /* ends dot_product */



     /*******************************************
     *
     *   dot_product(...
     *
     *   The dot product assumes the two
     *   vectors start at the same point.
     *
     *******************************************/

int dot_product(v1, v2)
   float v1[], v2[];
{
   float result = 0.0;
   result = v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
   return(result);
return(1);
}  /* ends dot_product */




     /*******************************************
     *
     *   cross_product(...
     *
     *
     *******************************************/

int cross_product(v1, v2, result)
   float v1[], v2[], result[];
{
   result[0] = v1[1]*v2[2] - v2[1]*v1[2];
   result[1] = v1[2]*v2[0] - v2[2]*v1[0];
   result[2] = v1[0]*v2[1] - v2[0]*v1[1];
return(1);
}  /* ends cross_product */




     /*******************************************
     *
     *   angle_between(...
     *
     *
     *******************************************/

int angle_between(v1, v2, angle)
   float *angle, v1[], v2[];
{
   double d, d1, d2, dt, n, result = 0.0, t;
   char   response[80];

/*******************
if(i == 2 && j>127){
   printf("\nangle_between>> i=%d j=%d", i,j);
   print_vector(v1);
   print_vector(v2);
}
printf("\nangle_between>> ");
   printf("\n %f  %f  %f",v1[0],v1[1],v1[2]);
   printf("\n %f  %f  %f",v2[0],v2[1],v2[2]);
*****************/

   n  = v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
   dt = v1[0]*v1[0] + v1[1]*v1[1] + v1[2]*v1[2];
   d1 = sqrt(dt);
   dt = v2[0]*v2[0] + v2[1]*v2[1] + v2[2]*v2[2];
   d2 = sqrt(dt);

   d      = d1*d2;


/**************
if(i == 2 && j>127){
   printf("\n n=%f d1=%f d2=%f d=%f",n,d1,d2,d);
}
if(d < 0.1){
   printf("\nd1=%f d2=%f",d1,d2);
   print_vector(v1);
   print_vector(v2);
   gets(response);
}
*****************/

   t      = n/d;
   if(t <= -1.0) t = -0.999;
   if(t >   1.0) t =  1.0;
   result = acos(t);
   *angle = result;
/**************
   printf("\nangle_between>> result=%f",result);
**************/
return(1);
}  /* ends angle_between */





int print_vector(x)
   float *x;
{
   printf("\n %f  %f  %f",x[0],x[1],x[2]);
return(1);
}  /* ends print_vector */



#ifdef NEVER


   /* test dot product */
   float result = 0;
   L[0] = 1.0;
   L[1] = 2.0;
   L[2] = 3.0;
   N[0] = 4.0;
   N[1] = 5.0;
   N[2] = 6.0;
   print_vector(L);
   print_vector(N);
   result = dot_product(L, N);
printf("\nresult is %f",result);
   /* ends test dot product */



   /* test cross product */
   float result = 0;
   L[0] = 0.0;
   L[1] = 1.0;
   L[2] =-2.0;
   N[0] = 1.0;
   N[1] = 0.0;
   N[2] = 9.0;
   print_vector(L);
   print_vector(N);
   cross_product(L, N, V);
   print_vector(V);
   /* ends test cross product */


   /* test angle between */
   double result = 0.0;
   L[0] = 0.0;
   L[1] = 1.0;
   L[2] = 0.0;
   N[0] = 1.0;
   N[1] = 1.0;
   N[2] = 0.0;
   print_vector(L);
   print_vector(N);
   angle_between(1, 1, L, N, &vr);
   printf("\nthe angle between is %f", vr);
   /* ends test angle between */


   /* test magnitude of */
   float result = 0.0;
   L[0] = 0.0;
   L[1] = 1.0;
   L[2] = 0.0;
   print_vector(L);
   result = magnitude_of(L);
   printf("\nthe magnitude is %f", result);
   /* ends test magnitude of */

#endif
