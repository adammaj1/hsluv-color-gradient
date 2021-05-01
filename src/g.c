/*


make

gcc g.c  hsluv.c -lm -Wall
./a.out 



*/

/*
 * HSLuv-C: Human-friendly HSL
 * <https://github.com/hsluv/hsluv-c>
 * <https://www.hsluv.org/>
 *
 * Copyright (c) 2015 Alexei Boronine (original idea, JavaScript implementation)
 * Copyright (c) 2015 Roger Tallada (Obj-C implementation)
 * Copyright (c) 2017 Martin Mitas (C implementation, based on Obj-C implementation)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/**
 * Convert HSLuv to RGB.
 *
 * @param h Hue. Between 0.0 and 360.0.
 * @param s Saturation. Between 0.0 and 100.0.
 * @param l Lightness. Between 0.0 and 100.0.
 * @param[out] pr Red component. Between 0.0 and 1.0.
 * @param[out] pg Green component. Between 0.0 and 1.0.
 * @param[out] pb Blue component. Between 0.0 and 1.0.
 void hsluv2rgb(double h, double s, double l, double* pr, double* pg, double* pb);
 
 */

#include <stdlib.h> // malloc
#include <float.h>
#include <math.h> // -lm
#include <stdio.h> // printf
#include "hsluv.h" //  HSLuv-C: Human-friendly HSL  https://github.com/hsluv/hsluv-c https://www.hsluv.org/
/* files hsluv.c and hsluv.h in the program directory */






/* input : hsl color */
double h = 0.0; /* h Hue. Between 0.0 and 360.0 */
double h_Max = 361.0;
double h_step = 60.0; 
	
double s = 100.0;  /* s Saturation. Between 0.0 and 100.0 */
//double l = 50.0; /* l Lightness. Between 0.0 and 100.0  here it is variable  = gradient position */


// complementary hue for diverging color gradient
double h_complementary;





// size of virtual 2D array of pixels
// each piexel has a RGB color
int iWidth = 600;
int iHeight ; // 
int iXmax ;
int iYmax ;


// size of the dynamic 1D array 
unsigned char * data;
size_t ColorSize = 3; // RGB = number of color components = channels
size_t ArrayLength; // number of 1D array's elements = ENumber = iWidth*iHeight*ColorSize
size_t ElementSize; // size of array's element in bytes
size_t ArraySize; // size of array in bytes = ElementSize*ArrayLength 

// ppm P6 file
size_t HeaderSize ; // size of the P6 file  header in bytes
size_t FileSize; // = HeaderSize +ArraySize [bytes]






/* Gradient type is a compination of general and joining gradient type */


const char * sGradientGeneralType[] = {"continous" , "discrete" };
// enum items are contiguous 
typedef enum  {continous = 0 , discrete = 1 } GradientGeneralType; 
size_t gMaxType; // length (size) of the array






const char * sGradientJoiningType[] = {"no" , "steps" , "tubes", "diverging"};
// enum items are contiguous 
typedef enum  {no = 0 , steps = 1 , tubes = 2, diverging = 3} GradientJoiningType; 
size_t jMaxType; // length (size) of the array







int segments = 4; // the number should be even, 2 segments create one "wave"}

// discrete = n-colour ordered gradient, 
/*
Number of data classes : https://colorbrewer2.org/
"Choosing the number of data classes is an important part of map design. Increasing the number of data classes will result in a more "information rich" map by decreasing the amount of data generalization. However, too many data classes may overwhelm the map reader with information and distract them from seeing general trends in the distribution. In addition, a large numbers of classes may compromise map legibility—more classes require more colors that become increasingly difficult to tell apart.

Many cartographers advise that you use five to seven classes for a choropleth map. Isoline maps, or choropleth maps with very regular spatial patterns, can safely use more data classes because similar colors are seen next to each other, making them easier to distinguish."
*/


int DataClasses = 7; // the number of columns in the ordered discrete gradients = number of ordered data categories
double p_step; // for discrete gradient




// ************************************************** functions *********************************************************************888



// fractional part of real (double) number
double frac(double d){

	double fraction = d - ((long)d);
	return fraction;
}


// for discrete gradient, smth like round(double>1.0) but for double in range [0,1]
double double2steps(double p){

	double s = p / p_step;
	s = s - frac(s);
	s *= p_step;
	return s;


}



// gives position of 2D point (ix,iy) in 1D array  ; uses also global variables: iWidth , ColorSize
int Give_i ( int iX, int iY)
{
  return (iX + iY * iWidth) * ColorSize;  
}








/* 
 Interactive Creation of Perceptually Uniform Color Maps by  M. Lambers 
 https://diglib.eg.org/bitstream/handle/10.2312/evs20201048/055-059.pdf
 


*/


/* 
sequential color map based on monotonically increasing lightness

Sequential map with varying lightness, single hue 
rendered in HSLuv color space

input : position in [0.0 ; 1.0] range


otput r,g,b between 0.0 and 1.0.



*/

void SequentialMapVaryingLightnessSngleHue(const double position, const double h, const double s , double rgb[] ){

	
  	// position = (double)iX/iXmax; in [0,1] range

	/* input = hsl color */
	double l = position*100.0; /* l Lightness. Between 0.0 and 100.0 */
	
	/* output = rgb color */
	double r;
	double g;
	double b;
	
	
	
		
	hsluv2rgb(h, s, l, &r, &g, &b); // convert 
	// clamp 
	if ( r<0.0) {r=0.0;}
	if ( g<0.0) {g=0.0;}
	if ( b<0.0) {b=0.0;}
	// 
	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
	
	
}






double ModifyPosition(const double position, const GradientGeneralType gTypeVar, const GradientJoiningType jTypeVar){
	
	// input position  should be in  [0,1] range 
	double p = position; // p = local copy of position
	// if position > 1 then we have repetition of colors = periodic function = wave   
	 
	
	switch(jTypeVar){
		// simple monotone function 
		case no : { break;} // return input position without modifications
		
		// periodic waves with different joinings
		case steps : {	p = p * segments; // periodic  = change range
				p = frac(p); 
				
    				break;}
    				
		case tubes : {	p = p * segments; // periodic = change range
				int ip = (int)p;
      				p = p-ip; // fractional part 
      				if (ip % 2) {p = 1.0-p;} // reverse gradient
      				
				break;}
				
		case diverging : { 	p *= 2.0;
					if (p>1.0) {p = 2.0 - p;} // reverse gradient	 
										
					break;}
		
		default:{}
	}
	if ( gTypeVar == discrete ) { p = double2steps(p); }
	
	return p; // output in [0,1] range = modified position

}








double GiveColorAndModifiedPosition(double position, const GradientGeneralType gTypeVar, const GradientJoiningType jTypeVar, const double h, const double s, double rgb[]){

	
	double mh = h; // local copy of h, it can be modified
	double mp = ModifyPosition( position, gTypeVar, jTypeVar);
	if ( jTypeVar == diverging && position > 0.5 ) { mh = h_complementary;} // diverging is made from 2 single hue gradients 
	SequentialMapVaryingLightnessSngleHue( mp, mh, s , rgb);
	

	return mp;
} 




// text file used by the gnuplot for creating images
void PrintRGBColorToTextFile( FILE *fp,  double position, double modifiedPosition, double rgb[]){


	// normalized to [0.0, 1.0] range RGB color channels
  double R = rgb[0]; 
  double G = rgb[1];
  double B = rgb[2];
  
  // normalized to [0.0, 1.0] range RGB color channels
  
  // [Relative luminance is formed as a weighted sum of linear RGB components](https://en.wikipedia.org/wiki/Luma_(video)) 
  // 
  //from function test_palette_subcommand from file gnuplot/src/command.c test_palette_subcommand
  //ntsc = 0.299 * rgb.r + 0.587 * rgb.g + 0.114 * rgb.b;
 // double Y = 0.299*R + 0.587*G + 0.114*B;
 double Y = modifiedPosition; 

   
  fprintf(fp, "%f \t %f \t  %f \t  %f \t %f\n", position, R, G, B, Y);

}




int SaveColorToArray(int iX, double rgb[]){


	int i; //  index of 1D array  
	int iY; // virtual 2D array
	for(iY=0; iY<iYmax; ++iY){
	
      		i = Give_i(iX, iY);
		
		// conversion from [0,1] range to [0,255]
		data[i]   = (unsigned char) (255*rgb[0]); /* Red*/
		data[i+1] = (unsigned char) (255*rgb[1]); /* Green */
		data[i+2] = (unsigned char) (255*rgb[2]);  /* Blue */
	}



	return 0; 
}


// --------------- save dynamic "A" array of uinsigned char to the binary ppm file ( P6 ) --------------------------------
int SaveArray2PPM (unsigned char A[], size_t ArraySize, const char* sName )
{

  FILE *fp;
  const unsigned char MaxColorComponentValue = 255;	/* color component is coded from 0 to 255 ;  it is 8 bit color file */
  
  char filename[60] ; 
  sprintf (filename, "%s.ppm", sName);
  
  

  /* save image to the pgm file  */
  fp = fopen (filename, "wb");	/*create new file,give it a name and open it in binary mode  */
  if (fp == NULL) 
    {	fprintf(stderr, "File open error"); 
      return 1;}
  else {
    fprintf (fp, "P6\n%u %u\n%u\n", iWidth, iHeight, MaxColorComponentValue);	/*write header to the file */
    fwrite (A, ArraySize, 1, fp);	// write dynamic A array to the binary file in one step  
 
    printf ("File %s saved. \n", filename);
    fclose (fp);
    return 0;}
  
}



/*

make 2 images: 
* color bar of the gradient( first  fill rgb array,  second:  save array to ppm file, third : convert ppm to png)
* 2D rgb profile ( first save txt file with data, second: use gnuplot to create png image )
 
*/ 

int MakeGradientTypeImages(const GradientGeneralType gType, const GradientJoiningType jType,  const double h, const double s){

	
  	int iX; // virtual 2D array
  	double position; // number form 0.0 to 1.0 used for color gradient
  	double modifiedPosition;
  	double rgb[3]; // output color converted from hsl 
	
  	// name of both text and ppm file 
  	char name[100]; 
  	sprintf (name, "%.0f_%.0f_%s_%s",  h, s, sGradientGeneralType[gType], sGradientJoiningType[jType]);
  	char filename[120] ; 
  	sprintf (filename, "%s.txt", name);
  	
  	
  	// compute complementary color for diverging gradient
	h_complementary = 180.0+ h;
	if (h_complementary > 360.0) { h_complementary -= 360.0;}
	// local info
	printf(" first color : hsl = [%.0f, %.0f, l]\n", h, s );
	printf(" complemntary color : hsl = [%.0f, %.0f, l]\n", h_complementary, s );
	
	
	
  
  
	FILE *fp = fopen(filename, "w");
  	if (fp==NULL) {
    		fprintf(stderr, "Error opening text file!\n");
    		return 1;
  		}
  	fprintf(fp, "# position \t\t R \t\t G \t\tB \t\tY\n"); // header of the text file
	
	
  	//
  	for(iX=0; iX<iXmax; ++iX){    
    		position = (double) iX / iXmax;
    		modifiedPosition =  GiveColorAndModifiedPosition(position, gType, jType, h, s, rgb); // compute color 
    		
    		PrintRGBColorToTextFile(fp, position, modifiedPosition, rgb); // 
      		
      		SaveColorToArray(iX, rgb);
    		
        		
         			
  	}


  	fclose(fp); // close and save text file
  	printf ("File %s saved. \n", filename);
  	SaveArray2PPM(data, ArraySize, name);
  	
	
	
	return 0;



}


// make gradient imasges for each gradient type 
void MakeGradientImages(const double h, const double s){

	GradientGeneralType gType;
  	GradientJoiningType jType;
  	
	
 	//
  	for (gType = 0; gType< gMaxType; ++gType)
  		for (jType = 0; jType < jMaxType; ++jType)
  				MakeGradientTypeImages(gType, jType, h, s);




}
 




// --------------------------------------------------------------- general  functions ----------------------------------------------





int setup(void){

	// gradient types
	gMaxType =  sizeof(sGradientGeneralType)/sizeof(sGradientGeneralType[0]); // 
	jMaxType =  sizeof(sGradientJoiningType)/sizeof(sGradientJoiningType[0]); 
	
	
	p_step = 1.0 / DataClasses; // for discrete gradient
	
	
	

	// 2D virtual array
  	iHeight = iWidth/3; 
  	iXmax = iWidth;
  	iYmax = iHeight;
  	 	
  	
  	
  	// 1D array
  	ArrayLength = iWidth*iHeight*ColorSize;
  	ElementSize = sizeof(unsigned char);
  	ArraySize = ElementSize*ArrayLength ; 
  	HeaderSize = 11 + (size_t) (log10(iHeight) +log10(iWidth)); 
	
  	FileSize = HeaderSize + ArraySize; 
	
  	/* create dynamic 1D array for RGB colors  */
  	data = malloc (ArraySize);
  	if (data == NULL ){
    		fprintf (stderr,  "Could not allocate memory for the array\n");
    		return 1;}
    		
    		
    	

	
  	return 0;
}


void info(void){


	printf(" HSLuv-C: Human-friendly HSL. See www.hsluv.org \n");
	
	printf("h = Hue is in  [0.0, 360.0] range\n");
	printf("s = Saturation is in  [0.0, 100.0] range\n");
	printf("l = Lightness is in  [0.0, 100.0] range. Here it is variable  = gradient position \n");

}



int end(void){

	
  	fprintf (stderr, " allways free memory (deallocate )  to avoid memory leaks \n"); // https://en.wikipedia.org/wiki/C_dynamic_memory_allocation
  	free (data);
  	
  	info();
  
  	return 0;

}

// ----------------------------------------------------------------------------------------------------------------------------------------------
// -------------------------------------------- main -------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------




int main(void){

	
	
	
	
	
	
	setup();
	
	// for each h make gradient  images 
	do {
   		MakeGradientImages(h,s);
   		h += h_step; // next gradient 
	} while( h < h_Max );
	
	
		
	end();
	
	
	return 0;

}
