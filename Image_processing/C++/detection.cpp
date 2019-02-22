#include <iostream>
#include <fstream>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
using namespace std;
#include "image.h"
/*****OPERATIONS**********
 *
 * 1:	copyImage()
 * 2:	inverseImage()
 * 3:	edgeDetect()
 * 4:	brightnessCtrl()
 * 5:	contrastCtrl()
 * 
 ************************/
int main(int argc, char *argv[])
{
	if (argc < 3) {
		cout<<"usage : ./imgProcess <operation> <image-file> <others..>"<<endl;
		cout<<"/*****OPERATIONS**********/\n1:   copyImage()\n2:   inverseImage()\n";
		cout<<"3:   edgeDetect()\n4:   brightnessCtrl()\n5:   contrastCtrl()\n/************************/\n";
		cout<<"<operation> : 1 => <others..>:output image file name\n";
		return -1;
	}
	int rate;
	int operation = atoi(argv[1]);
	if ((operation == 1 || operation == 4 || operation == 5) && argc < 4) {
		cout<<"arguments insufficent\n";
		return -1;
	}
	Image img_in, img_out;

	switch(operation) {
		case 1:	img_in.readImage(argv[2]); 
				img_out.copyImage(&img_in);
				img_out.writeImage(argv[3]);
				break;
		case 2:	img_in.readImage(argv[2]);		
				img_out.inverseImage(&img_in);
				img_out.writeImage(argv[2]);
				break;
		case 3:	img_in.readImage(argv[2]);
				img_out.edgeDetect(&img_in);
				img_out.writeImage(argv[2]);
				break;
		case 4:	rate = atoi(argv[3]);
				img_in.readImage(argv[2]);
				img_out.brightnessCtrl(&img_in, rate);
				img_out.writeImage(argv[2]);
				break;
/*		case 5:	img_in.readImage(argv[2]);
				img_out.contrastCtrl(img_in);
				img_out.writeImage(argv[2]);
				break;
*/		default: cout<<"Invalid operation\n";
	}

	return 0;
}




