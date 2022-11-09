/*
char line[10] = "Hello";
char *start = line + 1;
char *stop  = line + 1;

markLine(line, start, stop );
[0] = 0; ' '
[1] = 0; '^'
[2] = 0; ' '
[3] = 0; ' '
[4] = 0; ' '
[5] = 0; ' '
[6] = 0; '\0'
[7] = 0; 
[8] = 0;
[9] = 0;





*/

void markLine(char *line, char *start, char *stop)
{
    char *p;

    for (p = line; p < start; p++)
        *p = ' ';

    for (p = start; p <= stop; p++)
        *p = '^';

    for (p = stop+1; *p != '\0'; p++)
        *p = ' ';
}
