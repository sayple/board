#include <stdlib.h>
#include <stdio.h>

int main()
{
	char *tempName;
	char szTempName[256];
    
    /*임시 파일명 생성 */    
	tempName = tempnam(NULL, NULL);
	printf("임시 파일명 = %s\n\n", tempName);

	tempName = tempnam(NULL, "mytemp");
	printf("임시 파일명 = %s\n\n", tempName);
	
	tempName = tempnam(".", "mytemp");
	printf("임시 파일명 = %s\n\n", tempName);
	
    
    return 0;    
    
}
