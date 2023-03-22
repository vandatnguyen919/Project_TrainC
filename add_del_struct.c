#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_ARRAY 100
#define MAX_STRING 100

typedef struct {
    char ID[MAX_STRING];
    char uname[MAX_STRING];
    char pass[MAX_STRING];
    char phone[MAX_STRING];
    char fname[MAX_STRING];
    int age;
    char gender[MAX_STRING];
} User;

void signup(User users[MAX_ARRAY], int *count){
    User new_user;
    int unique = 0;
    printf("*** Enter staff information ***\n");
    do {
        unique = 1;
        printf("\t - ID: ");
        scanf("%[^\n]", new_user.ID);
        fflush(stdin);
        printf("\t - Username: ");
        scanf("%[^\n]", new_user.uname);
        fflush(stdin);
        int i;
        for (i = 0; i < *count; i++) {
            if (strcmp(users[i].uname, new_user.uname) == 0 || strcmp(users[i].ID, new_user.ID) == 0) {
                printf("Error: This ID or username is already exists. Please enter a different ID or username.\n");
                unique = 0;
                break;
            }
        }
    } while (!unique);
    printf("\t - Password: ");
    scanf("%[^\n]", new_user.pass);
    fflush(stdin);
    printf("\t - Phone number (type NULL is not given): ");
    scanf("%[^\n]", new_user.phone);
    fflush(stdin);
    printf("\t - Full name: ");
    scanf("%[^\n]", new_user.fname);
    fflush(stdin);
    printf("\t - Age: ");
    scanf("%d", &new_user.age);
    fflush(stdin);
    printf("\t - Gender (male/female): ");
    scanf("%[^\n]", new_user.gender);
    fflush(stdin);
    // add user to the array
    users[*count] = new_user;
    (*count)++;
    printf("\n\tRegistered successfully!\n");
}

void saveToFile(User users[MAX_ARRAY], int *count)
{
    fflush(stdin);
    FILE* file = fopen("user_infor.txt", "a");
    fprintf(file, "%s, %s, %s, %s, %s, %d, %s\n",users[*count-1].ID, users[*count-1].uname, users[*count-1].pass, users[*count-1].phone, users[*count-1].fname,  users[*count-1].age, users[*count-1].gender);
    fclose(file);
    printf("\n\tSaved your information!\n\n");
}

int check(User users[MAX_ARRAY], int count, const char* username, const char* ID) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(ID, users[i].ID) == 0)  {
            return 1; // Success: username and password match
        }
    }
    return 0; // Error: username or password incorrect
}

void delete_user(User users[MAX_ARRAY], int* count, const char* username, const char* ID) {
    int i, index = -1;
    for (i = 0; i < *count; i++) {
        if (strcmp(ID, users[i].ID) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("User not found\n");
        return;
    }

    // Delete user from memory
    for (i = index; i < *count - 1; i++) {
        users[i] = users[i+1];
    }
    (*count)--;

    // Delete user from file
    FILE* file = fopen("user_infor.txt", "w");
    for (i = 0; i < *count; i++) {
        fprintf(file, "%s, %s, %s, %s, %s, %d, %s\n", users[i].ID, users[i].uname, users[i].pass, users[i].phone, users[i].fname, users[i].age, users[i].gender);

    }
    fclose(file);
    printf("\nUser deleted!\n\n");
}



void print_menu() {
    printf("============MENU===========\n");
    printf("    Choose an option:    \n");
    printf("1. Add new staff account\n");
    //printf("2. Login\n");
    printf("2. Delete a staff account\n");
    printf("3. Exit\n");
    printf("Your choice: ");
}

int main() {
    User users[MAX_ARRAY];
    int count = 0;

    // Load data from file (if file exists)
    FILE* file = fopen("user_infor.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%[^,], %[^,], %[^,], %[^,], %[^,], %d, %[^\n]\n", 
                      users[count].ID, users[count].uname, users[count].pass, 
                      users[count].phone, users[count].fname, &users[count].age, 
                      users[count].gender) != EOF) {
            count++;
        }
        fclose(file);
    }

    int option,choice;
    do {
        print_menu();
        scanf("%d", &option);
        fflush(stdin);
        
        switch (option){
            case 1: // add
                signup(users, &count);
                saveToFile(users, &count);
                break;

            case 2: // delete
            {
                char username[MAX_STRING];
                char ID[MAX_STRING];
                // enter infor of acc staff 
                printf("*** Enter information of that employee: ***\n");
                printf("\t - ID: ");
                scanf("%[^\n]", ID);
                int result = check(users, count, username, ID);
                if (result) {
                    fflush(stdin);
                    printf("Are you sure to delete your account? (Y/N): ");
                    if (toupper(getchar())=='Y'){
                        delete_user(users, &count, username, ID);
                       // saveToFile(users, count);
                    }
                    else{
                        break;
                    }
                } else {
                    printf("\nError: Incorrect ID.\n");
                }
            }
            break;

            case 3:
                printf("Exiting program...\n");
                break;
			default:
            printf("Invalid option. Please try again.\n");
            break;
    }
} while (option != 3);
return 0;
           

}


