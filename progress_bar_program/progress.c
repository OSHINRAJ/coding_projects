#include <stdio.h>
#include<unistd.h>
#define MAX_LEN 80

int do_progress(char *str, int cur, int max, int sz) 
{   
	int i = 0; 
	i = (cur * sz)/max;
	for (int j = 1; j < i+1; j++) {
		str[j] = '=';
		str[j+1] = '>';
	}
	return i;
}
void init(char *str, char ch, int sz)
{
	for (int i = 0; i < sz; i++)
		str[i] = ch;
}

int main(void)
{
	char str[75];
	int percent;
	init(str, ' ', MAX_LEN - 1);
	str[0] = '[';
	str[1] = '>';
	str[MAX_LEN - 2] = ']';
	str[MAX_LEN - 1] = '\0';
	int max = 100, cur = 0;
	for( cur = 0; cur <= max ; cur++) {
		usleep(100000);
		percent = do_progress(str, cur, max, MAX_LEN - 4);
		printf("Download :\t %s \tETA : %d%%\r", str, (percent * 100)/(MAX_LEN - 4));
		fflush(stdout);
	}
	printf("\n");
	return 1;
}

