#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	char *p, *q;
    long size = 1;
    
	/*메모리를 할당합니다 */
    p = (char*) malloc (sizeof(char) * (1 << 20));
	while(1) {
    	q = (char*) realloc (p, size * (1 << 20));
    	if (NULL == q) {
    		printf("할당가능한 동적메모리 영역 - %ld 메가\n", size);
    		return 1;
    	}
    	p = q;
    	size ++;
	}

	return 0;
}

