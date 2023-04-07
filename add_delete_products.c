#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_ARRAY 100
#define MAX_STRING 100
typedef struct {
    char ID[MAX_STRING];
    char type[MAX_STRING];
    char name[MAX_STRING];
    int quantity;
    int price;
    char imsrc[MAX_STRING];
} Product;


void loadFromFile(Product products[MAX_ARRAY], int *count) {
    FILE *fp;
    char buffer[MAX_STRING];
    fp = fopen("product.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    while (fgets(buffer, MAX_STRING, fp)) {
        Product new_product;
        sscanf(buffer, "| %s | %s | %s | %d | %d | %[^\n]",
               new_product.ID, new_product.type, new_product.name,
               &new_product.quantity, &new_product.price, new_product.imsrc);
        products[*count] = new_product;
        (*count)++;
    }
    fclose(fp);
}

void printProducts() {
    FILE *fp;
    char buffer[MAX_STRING];
    fp = fopen("product.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    while (fgets(buffer, MAX_STRING, fp)) {
        printf("%s", buffer);
    }
    fclose(fp);
}


void add(Product products[MAX_ARRAY], int *count){
    Product new_product;
    int unique = 0;
    printf("*** Enter product information ***\n");
    do {
        unique = 1;
        printf("\t - ID: ");
        scanf("%[^\n]", new_product.ID);
        fflush(stdin);
        int i;
        for (i = 0; i < *count; i++) {
            if (strcmp(products[i].ID, new_product.ID) == 0) {
                printf("Error: This ID is already exists. Please enter a different ID of product.\n");
                unique = 0;
                break;
            }
        }
    } while (!unique);
    printf("\t - type: ");
    scanf("%[^\n]", new_product.type);
    fflush(stdin);
    printf("\t - Name: ");
    scanf("%[^\n]", new_product.name);
    fflush(stdin);
    printf("\t - Quantity: ");
    scanf("%d", &new_product.quantity);
    fflush(stdin);
    printf("\t - Price: ");
    scanf("%d", &new_product.price);
    fflush(stdin);
    printf("\t - Image source: ");
    scanf("%[^\n]", new_product.imsrc);
    fflush(stdin);
    // add user to the array
    products[*count] = new_product;
    (*count)++;
    printf("\n\tAdded product successfully!\n");
}



    
void saveToFile(Product products[MAX_ARRAY], int *count)
{
    FILE* file = fopen("product.txt", "a");
    if (file != NULL) {
        fprintf(file, "| %-11s| %-6s | %-22s | %-10d | %-7d | %s\n",
                products[*count-1].ID, products[*count-1].type, products[*count-1].name,
                products[*count-1].quantity, products[*count-1].price, products[*count-1].imsrc);
        fclose(file);
        printf("\n\tSaved product information!\n\n");
    } else {
        printf("Failed to open file for writing.\n");
    }
}

int check(Product products[MAX_ARRAY], int count, const char* ID) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(ID, products[i].ID) == 0)  {
            return 1; 
        }
    }
    return 0; // Error: username or password incorrect
}
void delete_product(const char* ID) {
    FILE* file = fopen("product.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Read file into an array
    const int MAX_LINES = 1000;
    char lines[MAX_LINES][1000];
    int count = 0;
    while (fgets(lines[count], sizeof(lines[0]), file) != NULL) {
        count++;
    }
    fclose(file);

    // Find the line to delete
    int i, index = -1;
    for (i = 0; i < count; i++) {
        if (strstr(lines[i], ID) != NULL) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Product not found\n");
        return;
    }

    // Delete the line from the array
    for (i = index; i < count - 1; i++) {
        strcpy(lines[i], lines[i+1]);
    }
    count--;

    // Write the array back to file
    file = fopen("product.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    for (i = 0; i < count; i++) {
        fprintf(file, "%s", lines[i]);
    }
    fclose(file);
    printf("Product deleted.\n");
}

void print_menu() {
    printf("============MENU===========\n");
    printf("    Choose an option:    \n");
    printf("1. Add new product\n");
    printf("2. Delete a product\n");
    printf("3. Exit\n");
    printf("Your choice: ");
}

int main() {
    Product products[MAX_ARRAY];
    int count = 0;
    loadFromFile(products, &count);
    printProducts();
    int option,choice;
  do {
        print_menu();
        scanf("%d", &option);
        fflush(stdin);
        switch (option){
            case 1: // add
                add(products, &count);
                saveToFile(products, &count);
                break;
            case 2: // delete
            {
			    char ID[MAX_STRING];
                // enter infor 
                printf("*** Enter information of that product: ***\n");
                printf("\t - ID: ");
                scanf("%[^\n]", ID);
                int result = check(products, count, ID);
                if (result) {
                    fflush(stdin);
                    printf("Are you sure to delete this product? (Y/N): ");    
                    if (toupper(getchar()) == 'Y') {
                        delete_product(ID);
                    }
                    else{
                        break;
                    }
                } else {
                    printf("\nError: Incorrect ID.\n");
                }         
            break;
		}
            case 3:
                printf("Exiting program...\n");
                exit(0); 
                break;
			default:  
				printf("Invalid option. Please try again.\n");
                break;
	}
} while (option != 3);

return 0;  
}


