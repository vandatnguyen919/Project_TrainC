#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USER 100

typedef struct {
    char id[11], type[21], name[31],
	     quantity[9], price[11], productImage[51];
} Product;
void print_menu() {
    printf("============MENU===========\n");
    printf("    Choose an option:    \n");
    printf("1. Add new product\n");
    printf("2. Delete a product\n");
    printf("3. Exit\n");
    printf("Your choice: ");
}

void loadProductDatafromFile(Product products[], int *count);
void storeProductDataToFile(Product products[], int *count);
void add(Product products[], int* count);
void delete_product(const char* id);

char* lTrim (char s[]);
char* rTrim (char s[]);
char* trim (char s[]);

int main() {
	
	Product products[MAX_USER];
	int count = 0;
	loadProductDatafromFile(products, &count);
	int i; 
	for (i = 0; i < count; i++) {
		printf("%-10s %-20s %-30s %-8s %-10s %-51s\n", products[i].id, products[i].type, products[i].name,
											 products[i].quantity, products[i].price, products[i].productImage);
	}
	
	storeProductDataToFile(products, &count);
	   int option,choice;
  do {
        print_menu();
        scanf("%d", &option);
        fflush(stdin);
        switch (option){
            case 1: // add
                add(products, &count);
                storeProductDataToFile(products, &count);
                break;
            case 2: // delete
            {
			    char id[MAX_USER];
                // enter infor 
                printf("*** Enter information of that product: ***\n");
                printf("\t - ID: ");
                scanf("%[^\n]", id);
                int result = check(products, count, id);
                if (result) {
                    fflush(stdin);
                    printf("Are you sure to delete this product? (Y/N): ");    
                    if (toupper(getchar()) == 'Y') {
                        delete_product(id);
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

void loadProductDatafromFile(Product products[], int *count) {
    FILE *f = fopen("product.txt", "r");
    if (f == NULL) {
        printf("Cannot open the file.\n");
        exit(1);
    }

    char line[1024];
    char col1[11], col2[21], col3[31], col4[9], col5[11], col6[51];
    int i;

    // Skip the header lines
    for (i = 0; i < 3; i++) {
        if (fgets(line, sizeof(line), f) == NULL) {
            break;
        }
    }

    // Read the data lines
    while (fgets(line, sizeof(line), f) != NULL) {
        if (sscanf(line, "| %10[^|] | %20[^|] | %30[^|] | %8[^|] | %10[^|] | %50[^|\n]",
                   col1, col2, col3, col4, col5, col6) == 6) {
            strcpy(products[*count].id, trim(col1));
            strcpy(products[*count].type, trim(col2));
            strcpy(products[*count].name, trim(col3));
            strcpy(products[*count].quantity, trim(col4));
            strcpy(products[*count].price, trim(col5));
            strcpy(products[*count].productImage, trim(col6));
            (*count)++;
        }
    }

    fclose(f);
}


void storeProductDataToFile(Product products[], int *count) {
	int i; 
	FILE* f = fopen("product.txt", "w");
	if (f == NULL) {
		printf("Cannot open the file.\n");
		exit(1);
	}
	
	int MAX_DASH = 200;
	for (i = 0; i < MAX_DASH; i++) {
		fprintf(f, "-");
		if ( i == MAX_DASH - 1) fprintf(f, "\n");
	}
	fprintf(f, "| %-10s | %-20s | %-30s | %-8s | %-10s | %-50s \n",
        "ID", "Type", "Name",
        "Quantity", "Price", "Product Image (Source)");
        
	for (i = 0; i < MAX_DASH; i++) {
		fprintf(f, "-");
		if ( i == MAX_DASH - 1) fprintf(f, "\n");
	}
	
	for (i = 0; i < *count; i++) {
        fprintf(f, "| %-10s | %-20s | %-30s | %-8s | %-10s | %-50s \n",
            products[i].id, products[i].type, products[i].name,
            products[i].quantity, products[i].price, products[i].productImage);
            
    }
    for (i = 0; i < MAX_DASH; i++) {
		fprintf(f, "-");
	}
	
	fclose(f);
}

void add(Product products[MAX_USER], int *count){
    Product new_product;
    int unique = 0;
    printf("*** Enter product information ***\n");
    do {
        unique = 1;
        printf("\t - ID: ");
        scanf("%[^\n]", new_product.id);
        fflush(stdin);
        int i;
        for (i = 0; i < *count; i++) {
            if (strcmp(products[i].id, new_product.id) == 0) {
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
    scanf("%[^\n]", new_product.quantity);
    fflush(stdin);
    printf("\t - Price: ");
    scanf("%[^\n]", new_product.price);
    fflush(stdin);
    printf("\t - Image source: ");
    scanf("%[^\n]", new_product.productImage);
    fflush(stdin);
    // add user to the array
    products[*count] = new_product;
    (*count)++;
    printf("\n\tAdded product successfully!\n");
}
int check(Product products[MAX_USER], int count, const char* id) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(id, products[i].id) == 0)  {
            return 1; 
        }
    }
    return 0; // Error: username or password incorrect
}
void delete_product(const char* id) {
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
        if (strstr(lines[i], id) != NULL) {
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


char* lTrim (char s[]) {
	int i = 0;
	while (s[i] == ' ') i++;
	if (i > 0) strcpy(&s[0], &s[i]);
	return s;
}

char* rTrim (char s[]) {
	int i = strlen(s) - 1;
	while (s[i] == ' ') i--;
	s[i+1] = '\0';
	return s;
}

char* trim (char s[]) {
	rTrim(lTrim(s));
	char *ptr = strstr(s, " ");
	while (ptr != NULL) {
		strcpy(ptr, ptr+1);
		ptr = strstr(s, " ");
	}
	return s;
}
