#ifndef _IMAGE_HEADER_
#define _IMAGE_HEADER_

#define MAX_HEIGHT 2048
#define MAX_WIDTH 1024

#include <sstream>
#include <string.h>
#include <math.h>

class Image {
	char  magic[5];
	int height;
	int width;
	int maxPixelVal;
	uint8_t Pixel[MAX_HEIGHT][MAX_WIDTH];

	public: 
	void setupImage(unsigned char inPixelArray[]);
	void readImage(char *file_name);
	void writeImage(char *file_name);
	int getHeight() {return height;}
	int getWidth() {return width;}
	int getMaxPixelVal() {return maxPixelVal;}
	/*
	   void setHeight(int h) {height = h;}
	   void setWidth(int w) {width = w;}
	   void setMaxPixelVal(int p) {maxPixelVal = p;}
	   */
	void copyImage(Image img);
	void inverseImage(Image img);
	void edgeDetect(Image img);
	int contrastCtrl(Image img);
	int brightnessCtrl(Image img, int rate);
	void pixelWrite(char file_name[]);

};

void Image::readImage(char file_name[]) 
{	
	ifstream inFile;
	inFile.open(file_name, ios::in);

	int i, j;
	unsigned char ch;
	stringstream ss;
	string inputLine = "";

	/* 1st Line MAgic Number */
	inFile.getline(magic, 3);
	/* get size of Image <height> <width> */
	getline(inFile,inputLine);
	if (inputLine[0] == '#') {
		cout << "Comment : " << inputLine << endl;
		getline(inFile,inputLine);
	}
	ss << inputLine;
	ss>>width>>height;
	/* get Maximum Pixel value 
	   getline(inFile,inputLine);
	   if (inputLine[0] == '#') {
	   cout << "Comment : " << inputLine << endl;
	   getline(inFile,inputLine);
	   }
	   ss << inputLine;
	   ss>>maxPixelVal;*/
	inFile>>maxPixelVal;
	inFile.read(reinterpret_cast<char *>(Pixel), height*width*sizeof(uint8_t));
	inFile.close();
}


void Image::writeImage(char file_name[])
{
	ofstream outFile;
	outFile.open(file_name, ios::out);

	int i, j;
	unsigned char ch;

	/* Magic Number */
	outFile<<magic<<endl;
	/* Size <height> <width> */
	outFile<<width<<' '<<height<<endl;
	/* Maximum pixel value */
	outFile<<maxPixelVal<<endl;

	outFile.write(reinterpret_cast<char *>(Pixel), height*width*sizeof(uint8_t));
	outFile.close();
}

void Image::edgeDetect(Image img)
{
	int i, j, x, y, x_val, y_val;
	int x_mask[3][3]={{1,0,-1},{2,0,-2},{1,0,-1}};
	int y_mask[3][3]={{1,2,1},{0,0,0},{-1,-2,-1}};
	unsigned char ch;

	strcpy(magic, img.magic);
	height = img.height;
	width = img.width;
	maxPixelVal = img.maxPixelVal;

	for (i = 0; i < height; i++) {
		for ( j = 0; j < width; j++) {
			x_val = 0;
			y_val = 0;
			if ( i == 0 || j == 0 || i == (height - 1) || j == (width - 1))
				Pixel[i][j] = img.Pixel[i][j];
			else{
				for (x = 0; x < 3; x++)
					for (y = 0; y < 3; y++){
						x_val += x_mask[x][y] * img.Pixel[i+x-1][j+y-1];
						y_val += y_mask[x][y] * img.Pixel[i+x-1][j+y-1];
					}
				Pixel[i][j] = (uint8_t)sqrt((x_val * x_val) + (y_val * y_val));
			}
		}
	}
}

void Image::inverseImage(Image img)
{
	int i, j;
	strcpy(magic, img.magic);
	height = img.height;
	width = img.width;
	maxPixelVal = img.maxPixelVal;

	for (i = 0; i < height; i++) {
		for ( j = 0; j < width; j++) {
			Pixel[i][j] = ~img.Pixel[i][j];
		}
	}
}
void Image::copyImage(Image img)
{
	int i, j;
	strcpy(magic, img.magic);
	height = img.height;
	width = img.width;
	maxPixelVal = img.maxPixelVal;

	for (i = 0; i < height; i++) {
		for ( j = 0; j < width; j++) {
			Pixel[i][j] = img.Pixel[i][j];
		}
	}
}
int Image::brightnessCtrl(Image img, int rate)
{
	if(rate < -100 || rate > 100)
		return -1;
	int i, j;
	float rate_value;
	strcpy(magic, img.magic);
	height = img.height;
	width = img.width;
	rate_value = img.maxPixelVal/ (100 / rate);
	maxPixelVal = img.maxPixelVal;
	for (i = 0; i < height; i++) {
		for ( j = 0; j < width; j++) {
			if(rate_value > 0)
				if ((int)img.Pixel[i][j] + rate_value > maxPixelVal)
					Pixel[i][j] = maxPixelVal;
				else
					Pixel[i][j] = img.Pixel[i][j] + rate_value;
			else
				if ((int)img.Pixel[i][j] + rate_value < 0)
					Pixel[i][j] = 0;
				else
					Pixel[i][j] = img.Pixel[i][j] + rate_value;
		}
	}

}
int Image::contrastCtrl(Image img)
{

}
void Image::pixelWrite(char file_name[])
{
	ofstream outFile;
	outFile.open(file_name, ios::out | ios::app);
	outFile.write((char *)&Pixel, height*width);
	outFile.close();
}
#endif
