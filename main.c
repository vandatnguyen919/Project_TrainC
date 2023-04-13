#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_USER 100    
#define	LL 20
typedef struct {
    char    order[3];
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

void storeFromAccountFile(Account* account, int* num_account) {   
    FILE* fIn = fopen("account.txt", "r");
    int i = 0; 
    fflush(stdin);
    fflush(stdin);      
    rewind(fIn);
    for (int j = 0; j < 3; j++) {
        char line[1000];
        fgets(line, sizeof(line), fIn);
    }

    while (fscanf(fIn, "| %[^ |] | %[^ |] | %[^ |] | %[^ |] | %[^|] | %[^ |] | %[^ |] | %[^ |] | %[^ |] | %s\n", 
            account[i].order, account[i].id, account[i].username, account[i].password,
            account[i].name, account[i].gender, account[i].dob,
            account[i].position, account[i].phone_number, account[i].profile_image) == 10) {
        i++;
        if(feof(fIn)) {
            break;
        }   
    }
    fclose(fIn);
    *num_account = i;
    // Trim cho Full Name
    int t;
    char s[LL];
    for (int k = 0; k < i; k++) {       // qu�t qua t?t c? t�i kho?n
        strcpy(s, account[k].name);
        t = 0;
        for (int m = 0; t <= 2; m++) {     // qu�t qua t?t c? k� t? c?a t?ng t�i kho?n
            if(isspace(s[m]) && isspace(s[m+1])) {
                t++;
                s[m] = '\0';
                strcpy(account[k].name, s);
            }    
        }
    }
}

void writeToAccountFile(Account* account, int num_account) {
    FILE* fOut = fopen("account.txt", "w+");
    int i;
    for(int t=0; t <= 200; t++) {
       fprintf(fOut,"-");
    }
    
    fprintf(fOut, "\n| %-6s| %-10s| %-20s| %-20s| %-20s| %-10s| %-20s| %-10s| %-20s| %-20s\n",
        "STT", "ID", "Username", "Password",
        "Full Name", "Gender", "DOB", 
        "Position", "Phone Number", "Profile Image (Source)");
    for(int t = 0; t <= 200; t++) {
       fprintf(fOut, "-");
    }
    
    for (int i = 0; i < num_account; i++) {
        fprintf(fOut, "\n| %-6s| %-10s| %-20s| %-20s| %-20s| %-10s| %-20s| %-10s| %-20s| %-20s",
            account[i].order, account[i].id, account[i].username, account[i].password,
            account[i].name, account[i].gender, account[i].dob,
            account[i].position, account[i].phone_number, account[i].profile_image);
    }

    fprintf(fOut, "\n");
    for(int t = 0; t <= 200; t++) {
       fprintf(fOut, "-");
    }
    fclose(fOut);
}

void ouputToScreen(const Account* account, int num_account) {
    for(int t=0; t <= 200; t++) {
       printf("-");
    }

    printf("\n| %-6s| %-10s| %-20s| %-20s| %-20s| %-10s| %-20s| %-10s| %-20s| %-20s\n",
        "STT", "ID", "Username", "Password",
        "Full Name", "Gender", "DOB", 
        "Position", "Phone Number", "Profile Image (Source)");
    
    for(int t = 0; t <= 200; t++) {
       printf("-");
    }

    for (int i = 0; i < num_account; i++) {
        printf("\n| %-6s| %-10s| %-20s| %-20s| %-20s| %-10s| %-20s| %-10s| %-20s| %-20s",
            account[i].order, account[i].id, account[i].username, account[i].password,
            account[i].name, account[i].gender, account[i].dob,
            account[i].position, account[i].phone_number, account[i].profile_image);
    }
    putchar('\n');
    
    for(int t=0; t <= 200; t++) {
       printf("-");
    }
}

void checkLogin(Account* account, int num_account, int* roleCheck, int* foundIndex) {
    char    inputID[20],
            inputUsername[20],
            inputPassword[20];
    int     checkRole = 0,
            index;
    do {
        printf("Input ID: ");
        scanf("%s", &inputID);
        printf("Input Username: ");
        scanf("%s", &inputUsername);
        fflush(stdin); 
        printf("Input Password: ");
        scanf("%s", &inputPassword);
        fflush(stdin);
        // Check Account and Role
        for (int k = 0; k < num_account; k++) {
            if ((strcmp(account[k].id, inputID) == 0 && strcmp(account[k].username, inputUsername) == 0) && (strcmp(account[k].password, inputPassword) == 0)) {
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
            newPhoneNumber[LL/2];
    int     findIndex = -1;
    UI:
    do {
        printf("What ID do you want to update information? ");
        scanf("%s", &updateID);
        for (int i = 0; i < num_account; i++) {
            if (strcmp(account[i].id, updateID) == 0) {
                findIndex = i;
                printf("ID Found! ");
                break;
            }
        }
    } while (findIndex == -1);
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
                    writeToAccountFile(account, num_account);
                } else {
                    printf("New password must be different from the old one!\n");
                }
            break;
        case 2:
                printf("Update Phone Number: ");
                scanf("%s", &newPhoneNumber);
                if (strcmp(account[findIndex].phone_number, newPhoneNumber) != 0) {
                    strcpy(account[findIndex].phone_number, newPhoneNumber);
                    writeToAccountFile(account, num_account);
                } else {
                    printf("New password must be different from the old one!\n");
                }
            break;
        default:
            goto UI;
    }
    
}

void addNewAccount(Account* account, int* num_account) {
    char    inputID[LL],
            inputUsername[LL],
            inputPassword[LL],
            inputFullName[LL+10],
            inputDOB[LL],
            inputPosition[LL/2],
            inputPhoneNumber[LL/2],
            inputProfileImage[LL];
    int     gender,
            position;

    printf("Input ID: ");
    scanf("%s", &inputID);
    fflush(stdin);
    printf("Input name: ");
    scanf("%[^\n]s", &inputFullName);
    strcpy(account[*num_account].name, inputFullName);

    fflush(stdin);
    printf("What is his/her gender? (1.Male / 2.Female): ");
    scanf("%d", &gender);
    if (gender == 1) {
        strcpy(account[*num_account].gender, "Male");
    } else {
        strcpy(account[*num_account].gender, "Female");
    }
    fflush(stdin);
    printf("Input dob: ");
    scanf("%s", &inputDOB);
    fflush(stdin);
    printf("What is his/her postion? (1.Manager / 2.Staff): ");
    scanf("%d", &position);
    if (position == 1) {
        strcpy(account[*num_account].position, "Manager");
    } else {
        strcpy(account[*num_account].position, "Staff");
    }
    fflush(stdin);
    printf("Input phone number: ");
    scanf("%s", &inputPhoneNumber);
    fflush(stdin);
    printf("Input profile image (source): ");
    scanf("%s", &inputProfileImage);
    fflush(stdin);
    (*num_account)++;
    sprintf(account[*num_account-1].order, "%d", *num_account);
    strcpy(account[*num_account-1].id, inputID);
    strcpy(account[*num_account-1].password, "123456");
    
    strcpy(account[*num_account-1].dob, inputDOB);
    strcpy(account[*num_account-1].phone_number, inputPhoneNumber);
    strcpy(account[*num_account-1].profile_image, inputProfileImage);

    char tu[10][51]; 
    for (int i = 0; i < strlen(inputFullName); i ++) {
    	if (inputFullName[i] >= 'A' && inputFullName[i] <= 'Z') inputFullName[i] = inputFullName[i] + 32;
	}
    int count = 0;
    char* token = strtok(inputFullName, " ");
    while (token != NULL) {
    	strcpy(tu[count], token); 
        count++;
        token = strtok(NULL, " ");
    }
    strcat(account[*num_account-1].username, tu[count-1]);
    strcat(account[*num_account-1].username, tu[0]);

    printf("Your ID is: %s", account[*num_account-1].id);
    printf("\nYour username is: %s", account[*num_account-1].username);
    printf("\nYour password is: %s", account[*num_account-1].password);
    printf("\nPlease note down carefully");
}

void deleteAccount(Account* account, int* num_account, int roleCheck, int foundIndex) {
    char checkAgain[LL];
    if (roleCheck == 1) {
        int foundID, tmp;
        char inputID[LL];
        printf("Do you sure want to delete an account?\n");
        printf("Type \"sure\" to delete or \"exit\" get back: ");
        scanf("%s", &checkAgain);
        if (strcmp(checkAgain, "sure") == 0) {
            do {
                printf("Input ID you want to delete: ");
                fflush(stdin);
                scanf("%[^\n]s", &inputID);
                for (int i = 0; i < num_account; i++) {
                    if (strcmp(account[i].id, inputID) == 0) {
                        foundID = 1;
                        printf("ID found!\n");
                        for(int j = i; j < *num_account; j++) {
                            account[j] = account[j+1];
                            sprintf(account[j].order, "%d", j+1);
                        } 
                        break;
                    }
                }
            } while (foundID != 1);
            --(*num_account);
            printf("Delete Successfully!\n");
        } else if (strcmp(checkAgain, "exit") == 0) {
            printf("Get back to menu page\n");
        }
        
    } else {
        printf("Do you sure want to delete your account?\n");
        printf("Type \"sure\" to delete or \"exit\" get back: ");
        scanf("%s", &checkAgain);
        if (strcmp(checkAgain, "sure") == 0) {
            for(int j = foundIndex; j < *num_account; j++) {
                account[j] = account[j+1];
                sprintf(account[j].order, "%d", j+1);
            }
            --(*num_account);
            printf("Delete Successfully!\n");
        } else if (strcmp(checkAgain, "exit") == 0) {
            printf("Get back to menu page\n");
        }

    }
}

void homepageMenu(Account *account, int num_account, int roleCheck, int foundIndex) {
    Menu:
    int choice;
    if (roleCheck == 1) {
        printf("\n1. Add new account\n");
        printf("2. Delete an account\n");
        printf("3. Update your information\n");
        printf("Input your choice: ");
        scanf("%d", &choice);
        switch(choice) {    
            case 1:
                addNewAccount(account, &num_account);
                ouputToScreen(account, num_account);
                writeToAccountFile(account, num_account);
                goto Menu;
                break;
            case 2:
                deleteAccount(account, &num_account, roleCheck, foundIndex);
                ouputToScreen(account, num_account);
                writeToAccountFile(account, num_account);
                goto Menu;
                break;
            case 3:
                updateInforAccount(account, num_account);
                ouputToScreen(account, num_account);
                writeToAccountFile(account, num_account);
                goto Menu;
                break;
            default:
                break;
        }
    } else {
        printf("\n1. Update your information\n");
        printf("2. Delete your account\n");
        printf("Input your choice: ");
        scanf("%d", &choice);
        switch(choice) {    
            case 1:
                updateInforAccount(account, num_account);
                ouputToScreen(account, num_account);
                writeToAccountFile(account, num_account);
                goto Menu;
                break;
            case 2:
                deleteAccount(account, &num_account, roleCheck, foundIndex);
                ouputToScreen(account, num_account);
                writeToAccountFile(account, num_account);
                goto Menu;
                break;
            default:
                break;
        }
    }
    
}

int main() {
    Account account[MAX_USER];
    int     num_account = 0,    //s? lu?ng account
            roleCheck   = 0,    //1. Manager |  2. Staff            
            foundIndex,         //Index c?a user trong DS
            choice      = 0;
    system("cls");
    fflush(stdin);
    storeFromAccountFile(account, &num_account);
    ouputToScreen(account, num_account);

    Menu:
    printf("\nWelcome to Management App! For further access, please login!");
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
        homepageMenu(account, num_account, roleCheck, foundIndex);
    }
    
    return 0;
}
