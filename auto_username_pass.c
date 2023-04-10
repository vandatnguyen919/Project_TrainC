#include <stdio.h>
#include <ctype.h>
#include <string.h>
void make_uname_pass (char* name) {
	char tu[10][51]; 
    char username[101]="";
    char pass[6] = "123456";
    int i;
    for (i = 0; i < strlen(name); i ++) {
    	if (name[i] >= 'A' && name[i] <= 'Z') name[i] = name[i] + 32;
	}
    int count = 0;
    char* token = strtok(name, " ");
    while (token != NULL) {
    	strcpy(tu[count], token); 
        count++;
        token = strtok(NULL, " ");
    }
    strcat(username, tu[count-1]);
    strcat(username, tu[0]);
    printf ("Username: %s", username);
    printf ("\nPassword: %s", pass);
}
int main() {
    char name[101];
    printf("Nhap ten cua ban: ");
    scanf("%[^\n]", name);
    make_uname_pass (name);
    return 0;
}

