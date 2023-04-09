#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <time.h> 
#define MAX_USER 1000 

typedef struct {
    char index[6], id[11], type[21], name[31],
	     quantity[9], price[11], productImage[51];
	time_t date;
} Product;

void print_menu() {
    printf("============MENU===========\n");
    printf("    Choose an option:    \n");
    printf("1. Add new product\n");
    printf("2. Delete a product\n");
    printf("3. Update a product\n"); 
    printf("4. Exit\n");
    printf("Your choice: ");
}

void loadProductDatafromFile(Product products[], int *count);
void storeToImportFile(Product products[], int *count);
void storeProductDataToFile(Product products[], int *count);
void add(Product products[], int* count);
int check(Product products[MAX_USER], int count, const char* id);
void delete_product(const char* id);
void updateProductByID(Product products[], int count); 

char* lTrim (char s[]);
char* rTrim (char s[]);
char* trim (char s[]);

int main() {
	
	Product products[MAX_USER];
	int count = 0;
	loadProductDatafromFile(products, &count);
	int i; 
	for (i = 0; i < count; i++) {
		printf("%-5s %-10s %-20s %-30s %-8s %-10s %-50s\n", products[i].index, products[i].id, products[i].type, products[i].name,
											 products[i].quantity, products[i].price, products[i].productImage);
	}
	storeProductDataToFile(products, &count);
	storeToImportFile(products, &count);
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
				updateProductByID(products, count);
				storeProductDataToFile(products, &count);
				break;
            case 4:
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
    char col0[6], col1[11], col2[21], col3[31], col4[9], col5[11], col6[51];
    int i;

    // Skip the header lines
    for (i = 0; i < 3; i++) {
        if (fgets(line, sizeof(line), f) == NULL) {
            break;
        }
    }

    // Read the data lines 
    while (fgets(line, sizeof(line), f) != NULL) {
        if (sscanf(line, "| %5[^|] | %10[^|] | %20[^|] | %30[^|] | %8[^|] | %10[^|] | %50[^|\n]",
                   col0, col1, col2, col3, col4, col5, col6) == 7) {
            strcpy(products[*count].index, trim(col0));
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
	fprintf(f, "| %-5s | %-10s | %-20s | %-30s | %-8s | %-10s | %-50s \n",
        "No", "ID", "Type", "Name",
        "Quantity", "Price", "Product Image (Source)");
        
	for (i = 0; i < MAX_DASH; i++) {
		fprintf(f, "-");
		if ( i == MAX_DASH - 1) fprintf(f, "\n");
	}
	
	for (i = 0; i < *count; i++) {
        fprintf(f, "| %-5s | %-10s | %-20s | %-30s | %-8s | %-10s | %-50s \n",
            products[i].index, products[i].id, products[i].type, products[i].name,
            products[i].quantity, products[i].price, products[i].productImage);
            
    }
    for (i = 0; i < MAX_DASH; i++) {
		fprintf(f, "-");
	}
	
	fclose(f);
}

void storeToImportFile(Product products[], int *count) {
    int i; 
    FILE* f = fopen("import.txt", "w");
    if (f == NULL) {
        printf("Cannot open the file.\n");
        exit(1);
    }

    int MAX_DASH = 200;
    for (i = 0; i < MAX_DASH; i++) {
        fprintf(f, "-");
        if (i == MAX_DASH - 1) fprintf(f, "\n");
    }
    fprintf(f, "| %-5s | %-10s | %-8s | %-10s | %-12s\n",
        "No", "ID", "Quantity", "Price", "Date");

    for (i = 0; i < MAX_DASH; i++) {
        fprintf(f, "-");
        if (i == MAX_DASH - 1) fprintf(f, "\n");
    }

    for (i = 0; i < *count; i++) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(f, "| %-5s | %-10s | %-8s | %-10s | %02d-%02d-%d\n",
            products[i].index, products[i].id, 
            products[i].quantity, products[i].price,
             tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
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
    // add product to the array
    // Tính giá tr? c?a new_index
int new_index = *count + 1;

// Thêm s?n ph?m m?i vào m?ng products
sprintf(new_product.index, "%-5d", new_index);
products[*count] = new_product;

// Tãng bi?n count lên m?t ðõn v?
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
    FILE* file = fopen("backup.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    const int MAX_PRODUCTS = 1000;
    Product products[MAX_PRODUCTS];
    int count = 0;


	loadProductDatafromFile(products, &count);
    // Find the product to delete
    int i, index = -1;
    for (i = 0; i < count; i++) {
        if (strcmp(products[i].id, id) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Product not found\n");
        return;
    }

    // Delete the product from the array
    for (i = index; i < count - 1; i++) {
        // Shift each product after the deleted product up one index
       	int num = atoi(products[i].index);
		char next_index[7];
		sprintf(next_index, "%d", num);
		strcpy(products[i].index, next_index);
        strcpy(products[i].id, products[i+1].id);
        strcpy(products[i].type, products[i+1].type);
        strcpy(products[i].name, products[i+1].name);
        strcpy(products[i].quantity, products[i+1].quantity);
        strcpy(products[i].price, products[i+1].price);
        strcpy(products[i].productImage, products[i+1].productImage);
    }
    count--;
    storeProductDataToFile(products, &count);
    printf("Product deleted.\n");
    
}


void updateProductByID(Product products[], int count) {
	char newPrice[11];
	char newQuantity[9];
    char id[11];
    printf("Enter the ID of the product you want to update: ");
    scanf("%s", id);
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(products[i].id, id) == 0) {
        	printf("Product found: %s\n", products[i].name);
            int choice;
            do{
			
		    printf("Which field do you want to update?\n");
            printf("1. Price\n");
            printf("2. Quantity\n");
            printf("Other-Quit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    printf("Current price: %s\n", products[i].price);
                    printf("Enter new price: ");
                    scanf("%s", newPrice);
                    strcpy(products[i].price, newPrice);
                    printf("Price updated successfully.\n");
                    break;
                case 2:
                    printf("Current quantity: %s\n", products[i].quantity);
                    printf("Enter new quantity: ");
                    scanf("%s", newQuantity);
                    strcpy(products[i].quantity, newQuantity);
                    printf("Quantity updated successfully.\n");
                    break;
                default:
                    printf("Invalid choice.\n");
                    break;
            }
        } while (choice > 0 && choice <3);
            return;
        }
        
    }
    printf("Product with ID %s not found.\n", id);
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
	int i,x;
	for (i=x=0; s[i]; ++i) 
		if (!isspace(s[i]) || (i > 0 && !isspace(s[i-1])))
			s[x++] = s[i];
	
	s[x] = '\0';
	return s;
}
