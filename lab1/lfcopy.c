#include <stdio.h>
#include <string.h>


int i, num;

/* readline : read a line from standard input, return its length or 0
*/
int readline(char line[], int max)
{
	if (fgets(line, max, stdin) == NULL){
		return 0;
	}
	else{
		return strlen(line);
}
}

/* writeline: write line to standard output, return number of chars
written */
int writeline(const char line[]){
	fputs(line, stdout);
	return strlen(line);
}

int main(){


	char str[100];
	num = readline(str,100);
	writeline(str);
	printf("%d\n",num);
	
	return 0;

} 
