#include <stdio.h>
#include <string.h>

#define NAME_SIZE		200
#define ADDRESS_SIZE	1000
#define ARRAY_SIZE		2000

typedef struct _tagData{
	char szName[NAME_SIZE];
	char address[ARRAY_SIZE][ADDRESS_SIZE];
} DATA;

int main(int argc, char** argv)
{
	DATA data;
	char s1[] = "Hello world!";
	char s2[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	char s3[] = "Hello world!";
	char s4[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	int a = 49;
	int b = 20;
	int result;

	printf("s1:%p, s2:%p\n", s1, s2);
	printf("isalnum('1') -> %d\n", isalnum('1'));
	printf("isalnum('ÿ') -> %d\n", isalnum('*'));
	printf("isalnum('*') -> %d\n", isalnum('a'));
	printf("isalnum('*') -> %d\n", isalnum(-1));
	printf("isalnum('*') -> %d\n", isalnum(97));

	printf("before memcpy : %s\n", s2);
	memcpy(s2, s1, 40);
	printf("after memcpy : %s\n", s1);
	printf("after memcpy : %s\n", s2);

	printf("\nbefore memmove : %s\n", s4);
	memmove(s4, s3, 40);
	printf("after memmove: %s\n", s3);
	printf("after memmove: %s\n", s4);

    /* ¹®ÀÚ¿­ÀÇ ³»¿ëÀ» ÀüºÎ 'H'·Î º¯°æÇÕŽÏŽÙ */
	printf("\nbefore memset : %s\n", s1);
    memset(s1, 'H', sizeof(s1));
    s1[sizeof(s1)-1] = '\0';
	printf("after memset : %s\n", s1);

	result = memcmp(&a, &b, sizeof(int));

	if (result < 0) {
		outputLine("\na ÀÇ °ªÀÌ bºžŽÙ ÀÛœÀŽÏŽÙ.");
	} else if (result == 0) {
		outputLine("\na ¿Í bÀÇ °ªÀÌ °°œÀŽÏŽÙ.");
	} else {
		outputLine("\nb ÀÇ °ªÀÌ aºžŽÙ ÀÛœÀŽÏŽÙ.");
	}

	return 0;
}

