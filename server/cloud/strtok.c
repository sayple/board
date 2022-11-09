#include <stdio.h>
#include <string.h>

char * inputLine(void);

int main(void)
{
	char *line;
	char *token;
	
	while(line=inputLine()) {
		token = strtok(line, " \t\n");
		while(token) {
			printf("token : %s\n", token);
			token = strtok( NULL, " \t\n");
		}
	}
	return 0;
}