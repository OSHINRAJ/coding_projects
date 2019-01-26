#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
int main()
{

	FILE *in, *out, *ed;
	int i, j, x, y, x_val, y_val;
	int x_mask[3][3]={{1,0,-1},{2,0,-2},{1,0,-1}};
	int y_mask[3][3]={{1,2,1},{0,0,0},{-1,-2,-1}};
						
	char magic[5];
	int width;
	int height;
	int maxPixelVal;
	uint8_t **str;
	uint8_t ch, nch;

	in = fopen("/home/vkchlt0119/Desktop/Study/prgms/Image_processing/dog.PGM", "r");
	out = fopen("/home/vkchlt0119/Desktop/Study/prgms/Image_processing/img.PGM", "w");
	ed = fopen("/home/vkchlt0119/Desktop/Study/prgms/Image_processing/ed_img.PGM", "w");
	if(!in || !out)
		return -1;
	fscanf(in, "%s %d %d %d", magic, &width, &height,&maxPixelVal);
	printf("magic :%s\nwidth :%d\theight :%d\nmaxPixelVal :%d", magic, width, height, maxPixelVal);
	fprintf(out, "%s %d %d %d\n", magic, width, height, maxPixelVal);
	fprintf(ed, "%s %d %d %d\n", magic, width, height, maxPixelVal);
	
	str = (uint8_t **)malloc(height * sizeof(uint8_t *));
	for (i = 0; i < height; i++)
		str[i] = (uint8_t *)malloc(width *sizeof(uint8_t));

	for ( i = 0; i < height; i++)
		for ( j = 0; j < width; j++) {
			fread(&ch, 1, 1, in);
			str[i][j] = ch;
			nch = ~ch;
			fwrite(&nch,1,1,out);
		}
	for (i = 0; i < height; i++) {
		for ( j = 0; j < width; j++) {
			x_val = 0;
			y_val = 0;
			if ( i == 0 || j == 0 || i == height - 1 || j == width - 1) {
				fwrite(&str[i][j], 1, 1, ed);
			}
			else{
				for (x = 0; x < 3; x++)
					for (y = 0; y < 3; y++){
						x_val += x_mask[x][y] * str[i+x-1][j+y-1];
						y_val += y_mask[x][y] * str[i+x-1][j+y-1];
					}
				ch = (int)sqrt((x_val * x_val) + (y_val * y_val));
				fwrite(&ch, 1, 1, ed);
			}
		}
		printf("\n");
	}

	fclose(in);
	fclose(out);

	return 0;
}
