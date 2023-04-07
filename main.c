// #define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_USER 100    
#define	LL 20

typedef struct {
    char    id[LL/2];
    char    username[LL];
    char    password[LL];
    char    name[LL];
    char    gender[LL];
    char    dob[LL];
    char    position[LL/2];
    char    phone_number[LL];
    char    profile_image[2*LL];
} Account;

void readFromAccountFile(Account* account, int* num_account) {      
    FILE* fIn = fopen("tmp.txt", "r");
    int i = 0; 
    rewind(fIn);
    fflush(stdin);
    if (fIn) {
        for (;;) {
            Account tmp;
            fscanf(fIn, "| %[^ |] | %[^ |] | %[^ |] | %[^ |] | %[^ |] | %[^ |] | %[^ |] | %[^ |] | %s\n", 
                tmp.id, tmp.username, tmp.password,
                tmp.name, tmp.gender, tmp.dob,
                tmp.position, tmp.phone_number, tmp.profile_image);
            account[i++] = tmp;

            if(feof(fIn)) {
                break;
            }   
        }
    }
    fclose(fIn);
    *num_account = i;
}

void writeToAccountFile(const Account* account, int num_account) {
   FILE* fOut = fopen("account.txt", "w+");
   int i;
   for(int t=0; t <= 200; t++) {
       fprintf(fOut,"-");
    }
    fprintf(fOut, "\n| %-10s| %-20s| %-20s| %-20s| %-10s| %-20s| %-10s| %-20s| %-20s\n",
        "ID", "Username", "Password",
        "Full Name", "Gender", "DOB", 
        "Position", "Phone Number", "Profile Image (Source)");
    for(int t = 0; t <= 200; t++) {
       fprintf(fOut, "-");
    }
    for (int i = 0; i < num_account; i++) {
        fprintf(fOut, "\n| %-10s| %-20s| %-20s| %-20s| %-10s| %-20s| %-10s| %-20s| %-20s",
            account[i].id, account[i].username, account[i].password,
            account[i].name, account[i].gender, account[i].dob,
            account[i].position, account[i].phone_number, account[i].profile_image);
    }
    fprintf(fOut, "\n");
    for(int t = 0; t <= 200; t++) {
       fprintf(fOut, "-");
    }
    fclose(fOut);
}

void dataCleaning() {
    fflush(stdin);
    char    data[MAX_USER+3][MAX_USER*2+2],
            tmp[MAX_USER+3][MAX_USER*2+2];
    FILE* fIn = fopen("account.txt", "r");
    rewind(fIn);
    int     count  = 0, 
            i      = 0,
            k      = 0;
    while (fgets(data[count], sizeof(data[0]), fIn) != NULL) {
        count++;
    }
    fclose(fIn);

    // Lưu các dòng đã lọc qua "\n" và chứa "-"
    while (1) {
        for (int j = 0; j <= count; j++) {
            if (strstr(data[j], "-") == NULL) {
                k++;
                if (k % 2 == 0 || (k>4 && k%2 != 0)) {
                    strcpy(tmp[i], data[j]);
                    i++;
                }                   
            }
            if (j==count){
                break;
            }
        }
        break;
    }
    
    // Ghi dữ liệu sạch vô file phụ để đọc
    FILE* fOut = fopen("tmp.txt", "w+");
    for (int m=1; m<i; m++) {
        fprintf(fOut, "%s", tmp[m]);
    }
    fclose(fOut);
}

void ouputToScreen(const Account* account, int num_account) {
    for(int t=0; t <= 200; t++) {
       printf("-");
    }
    printf("\n| %-10s| %-20s| %-20s| %-20s| %-10s| %-20s| %-10s| %-20s| %-20s\n",
        "ID", "Username", "Password",
        "Full Name", "Gender", "DOB", 
        "Position", "Phone Number", "Profile Image (Source)");
    for(int t = 0; t <= 200; t++) {
       printf("-");
    }
    for (int i = 0; i < num_account; i++) {
        printf("\n| %-10s| %-20s| %-20s| %-20s| %-10s| %-20s| %-10s| %-20s| %-20s",
            account[i].id, account[i].username, account[i].password,
            account[i].name, account[i].gender, account[i].dob,
            account[i].position, account[i].phone_number, account[i].profile_image);
    }
    printf("\n");
    for(int t=0; t <= 200; t++) {
       printf("-");
    }
}

void checkLogin(Account* account, int num_account, int* roleCheck, int* foundIndex) {
    char    inputUsername[20],
            inputPassword[20];
    int     checkRole = 0,
            index;
    do {
        printf("Input Username: ");
        scanf("%s", &inputUsername);
        fflush(stdin); 
        printf("Input Password: ");
        scanf("%s", &inputPassword);
        fflush(stdin);
        // Check Account and Role
        for (int k = 0; k < num_account; k++) {
            if ((strcmp(account[k].username, inputUsername) == 0) && (strcmp(account[k].password, inputPassword) == 0)) {
                index = k;
                if (strcmp(account[index].position, "Manager") == 0) {
                    checkRole = 1;
                    break;
                } else {
                    checkRole = 2;
                    break;
                }
            }
        }
    } while (checkRole == 0);
    printf("Login Succesfully!\n");
    *roleCheck    = checkRole;
    *foundIndex   = index;    
}

void updateInforAccount(Account *account, int num_account) {
    char    updateID[LL],
            newPassword[LL],
            newPhoneNumber[LL];
    int     findIndex = 0;
    UI:
    do {
        printf("What ID do you want to update information? ");
        scanf("%s", &updateID);
        for (int i = 1; i <=num_account; i++) {
            if (strcmp(account[i].id, updateID) == 0) {
                findIndex = i;
                printf("ID Found! ");
                break;
            }
        }
    } while (findIndex == 0);
    fflush(stdin);
    printf("What do you want to update?\n");
    printf("1. Update Password\n");
    printf("2. Update Phone Number\n");
    printf("3. Exit\n");
    printf("Input your selection: ");
    int choice;
    scanf("%d", &choice);
    switch(choice) {
        case 1:
                printf("Input your new password: ");
                scanf("%s", &newPassword);
                if (strcmp(account[findIndex].password, newPassword) != 0) {
                    strcpy(account[findIndex].password, newPassword);
                    main();
                } else {
                    printf("New password must be different from the old one!\n");
                }
            break;
        case 2:
                printf("Update Phone Numberl");
                scanf("%s", &newPhoneNumber);
                if (strcmp(account[findIndex].phone_number, newPhoneNumber) != 0) {
                    strcpy(account[findIndex].phone_number, newPhoneNumber);
                    main();
                } else {
                    printf("New password must be different from the old one!\n");
                }
            break;
        default:
            goto UI;
    }
    

}


void homepageMenu(Account *account, int num_account) {
    int choice;
    printf("1. Show your information\n");
    printf("2. Edit your information\n");
    printf("3. Delete your account\n");
    printf("Input your choice: ");
    scanf("%d",&choice);
    switch(choice) {
        case 1:

        case 2:
            updateInforAccount(account, num_account);
        case 3:

    }
}

int main() {
    Account account[MAX_USER];
    int     num_account = 0,    //số lượng account
            roleCheck   = 0,    //1. Manager |  2. Staff            
            foundIndex,         //Index của user trong DS
            choice      = 0;
    system("cls");
    
    dataCleaning();
    readFromAccountFile(account, &num_account);
    // writeToAccountFile(account, num_account);
    ouputToScreen(account, num_account);

    printf("\nWelcome to Management App! ");
    printf("\n1. Login");
    printf("\n2. Exit");
    printf("\nInput your choice: ");
    scanf("%d", &choice);   fflush(stdin);
    switch(choice) {
        case 1:
            checkLogin(account, num_account, &roleCheck, &foundIndex);
            break;
        default:
            exit(0);
    }
    if (roleCheck != 0) {
        homepageMenu(account, num_account);
    }
    
    return 0;
};
