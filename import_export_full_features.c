#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <stdbool.h> 
#include <time.h> 
#define MAX_USER 1000 
int import_count = 0; 
typedef struct {
    char index[6], import_index[6], export_index[6], id[11], type[21], name[31],
	     quantity[9], iprice[11], eprice[11], productImage[51], total[20], amount[9];
	time_t date;
} Product;

void print_menu() {
    printf("============MENU===========\n");
    printf("    Choose an option:    \n");
    printf("1. Add new product\n");
    printf("2. Delete a product\n");
    printf("3. Import an existed product \n"); 
    printf("4. Export an existed product\n");
    printf("5. Change price of a product\n");
    printf("6. Exit\n");
    printf("Your choice: ");
}

void loadProductDatafromFile(Product products[], int *count);
void loadAndPrintImportDatafromFile(const char *filename, int *count);
void loadAndPrintExportDatafromFile(const char *filename, int *count);

void storeToImportFile(Product product);
void storeToExportFile(Product product); 
void storeProductDataToFile(Product products[], int *count);

void add(Product products[], int* count);
int check(Product products[MAX_USER], int count, const char* id);
void delete_product(const char* id);
void importExistedProduct(Product products[], int count); 
void exportExistedProduct(Product products[], int count);
void changePriceofProduct(Product products[], int count);

char* lTrim (char s[]);
char* rTrim (char s[]);
char* trim (char s[]);

int main() {
	
	Product products[MAX_USER];
	int count = 0;
	loadProductDatafromFile(products, &count);
	int i; 
	for (i = 0; i < count; i++) {
		printf("%-5s %-10s %-20s %-30s %-8s %-10s %-10s %-50s\n", products[i].index, products[i].id, products[i].type, products[i].name,
											 products[i].quantity, products[i].iprice, products[i].eprice,products[i].productImage);
	}
	loadAndPrintImportDataFromFile("import.txt", 1);
	loadAndPrintExportDataFromFile("export.txt", 1);
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
				importExistedProduct(products, count);
				storeProductDataToFile(products, &count);
				break;
			case 4:
				exportExistedProduct(products, count);
				storeProductDataToFile(products, &count);
				break;
			case 5:
				changePriceofProduct(products, count);
				storeProductDataToFile(products, &count);
				break;
            case 6:
                printf("Exiting program...\n");
                exit(0); 
                break;
			default:  
				printf("Invalid option. Please try again.\n");
                break;
	}
} while (option != 6);

	return 0;
}

void loadProductDatafromFile(Product products[], int *count) {
    FILE *f = fopen("product.txt", "r");
    if (f == NULL) {
        printf("Cannot open the file.\n");
        exit(1);
    }

    char line[1024];
    char col0[6], col1[11], col2[21], col3[31], col4[9], col5[11], col6[11], col7[51];
    int i;

    // Skip the header lines
    for (i = 0; i < 3; i++) {
        if (fgets(line, sizeof(line), f) == NULL) {
            break;
        }
    }

    // Read the data lines 
    while (fgets(line, sizeof(line), f) != NULL) {
        if (sscanf(line, "| %5[^|] | %10[^|] | %20[^|] | %30[^|] | %8[^|] | %10[^|] | %10[^|] | %50[^|\n]",
                   col0, col1, col2, col3, col4, col5, col6, col7) == 8) {
            strcpy(products[*count].index, trim(col0));
            strcpy(products[*count].id, trim(col1));
            strcpy(products[*count].type, trim(col2));
            strcpy(products[*count].name, trim(col3));
            strcpy(products[*count].quantity, trim(col4));
            strcpy(products[*count].iprice, trim(col5));
            strcpy(products[*count].eprice, trim(col6));
            strcpy(products[*count].productImage, trim(col7));
            (*count)++;
        }
    }

    fclose(f);
}

void loadAndPrintImportDataFromFile(const char* filename, int start_index) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    // Skip the header line
    char line[1024];
    if (fgets(line, sizeof(line), fp) == NULL) {
        printf("Invalid file format.\n");
        fclose(fp);
        return;
    }

    printf ("\tImport Data: \n"); 
    int row = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        int no, total_qty, imp_qty, total_money;
        char id[11], iprice[11], import_time[20];

        if (sscanf(line, "| %d | %10s | %18d | %11s | %19d | %21d | %[^\n]", &no, id, &total_qty, iprice, &imp_qty, &total_money, import_time) == 7) {
            printf("%-7d %-10s %-18d %-11s %-19d %-21d %s\n", start_index + row, id, total_qty, iprice, imp_qty, total_money, import_time);
            row++;
        }
    }

    printf("Read %d rows from file %s\n", row, filename);

    fclose(fp);
}

void loadAndPrintExportDataFromFile(const char* filename, int start_index) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    // Skip the header line
    char line[1024];
    if (fgets(line, sizeof(line), fp) == NULL) {
        printf("Invalid file format.\n");
        fclose(fp);
        return;
    }

    printf ("\tExport Data: \n"); 
    int row = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        int no, total_qty, exp_qty, total_money;
        char id[11], eprice[11], export_time[20];

        if (sscanf(line, "| %d | %10s | %18d | %11s | %19d | %21d | %[^\n]", &no, id, &total_qty, eprice, &exp_qty, &total_money, export_time) == 7) {
            printf("%-7d %-10s %-18d %-11s %-19d %-21d %s\n", start_index + row, id, total_qty, eprice, exp_qty, total_money, export_time);
            row++;
        }
    }

    printf("Read %d rows from file %s\n", row, filename);

    fclose(fp);
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
	fprintf(f, "| %-5s | %-10s | %-20s | %-30s | %-8s | %-15s | %-15s | %-50s \n",
        "No", "ID", "Type", "Name",
        "Quantity", "Import Price", "Export Price","Product Image (Source)");
        
	for (i = 0; i < MAX_DASH; i++) {
		fprintf(f, "-");
		if ( i == MAX_DASH - 1) fprintf(f, "\n");
	}
	
	for (i = 0; i < *count; i++) {
        fprintf(f, "| %-5s | %-10s | %-20s | %-30s | %-8s | %-15s | %-15s | %-50s \n",
            products[i].index, products[i].id, products[i].type, products[i].name,
            products[i].quantity, products[i].iprice, products[i].eprice, products[i].productImage);
            
    }
    for (i = 0; i < MAX_DASH; i++) {
		fprintf(f, "-");
	}
	
	fclose(f);
}

void storeToImportFile(Product product) {
    FILE *f;
    f = fopen("import.txt", "a");

    int MAX_DASH = 200,i;
   	 
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    fprintf(f, "| %-5s | %-10s | %-17s | %-10s | %-17s | %-20s | %02d/%02d/%d %02d:%02d:%02d \n",
        product.import_index, product.id, 
        product.quantity, product.iprice, product.amount, product.total,
        tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900,
        tm->tm_hour, tm->tm_min, tm->tm_sec
        );
    fclose(f);
}

void storeToExportFile(Product product) {
    FILE *f;
    f = fopen("export.txt", "a");

    int MAX_DASH = 200,i;
   	 
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    fprintf(f, "| %-5s | %-10s | %-17s | %-10s | %-17s | %-20s | %02d/%02d/%d %02d:%02d:%02d \n",
        product.export_index, product.id, 
        product.quantity, product.eprice, product.amount, product.total,
        tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900,
        tm->tm_hour, tm->tm_min, tm->tm_sec
        );
    fclose(f);
}


int getNextImportIndex() {
    FILE *f;
    f = fopen("import.txt", "r");

    char line[500];
    int lastIndex = 0;

    // Ð?c t?ng d?ng trong file
    while (fgets(line, sizeof(line), f)) {
        // B? qua d?ng header và d?ng g?ch ngang
        if (strstr(line, "No") != NULL || strstr(line, "-") != NULL) {
            continue;
        }

        // Tách các trý?ng b?ng "|" và l?y giá tr? index t? trý?ng ð?u tiên
        char *token = strtok(line, "|");
        int currentIndex = atoi(trim(token));
        if (currentIndex > lastIndex) {
            lastIndex = currentIndex;
        }
    }

    fclose(f);

    return lastIndex + 1;
}

int getNextExportIndex() {
    FILE *f;
    f = fopen("export.txt", "r");

    char line[500];
    int lastIndex = 0;

    // Ð?c t?ng d?ng trong file
    while (fgets(line, sizeof(line), f)) {
        // B? qua d?ng header và d?ng g?ch ngang
        if (strstr(line, "No") != NULL || strstr(line, "-") != NULL) {
            continue;
        }

        // Tách các trý?ng b?ng "|" và l?y giá tr? index t? trý?ng ð?u tiên
        char *token = strtok(line, "|");
        int currentIndex = atoi(trim(token));
        if (currentIndex > lastIndex) {
            lastIndex = currentIndex;
        }
    }

    fclose(f);

    return lastIndex + 1;
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
    printf("\t - Import Price: ");
    scanf("%[^\n]", new_product.iprice);
    fflush(stdin);
    printf("\t - Export Price: ");
    scanf("%[^\n]", new_product.eprice);
    fflush(stdin);
    printf("\t - Image source: ");
    scanf("%[^\n]", new_product.productImage);
    fflush(stdin);
    int num1 = atoi(new_product.quantity);
    float num2 = strtof(new_product.iprice, NULL); 
    sprintf(new_product.total, "%.0f", num1*num2);

    strcpy(new_product.amount, new_product.quantity);
    int new_index = *count + 1;
    // them sp moi  
    sprintf(new_product.index, "%-5d", new_index);
    products[*count] = new_product;
    (*count)++;
    sprintf(new_product.import_index, "%-5d", getNextImportIndex());
    storeToImportFile(new_product);
    printf("\n\tAdded product successfully!\n");
}


int check(Product products[MAX_USER], int count, const char* id) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(id, products[i].id) == 0)  {
            return 1; 
        }
    }
    return 0; 
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
        strcpy(products[i].iprice, products[i+1].iprice);
        strcpy(products[i].eprice, products[i+1].eprice);
        strcpy(products[i].productImage, products[i+1].productImage);
    }
    count--;
    storeProductDataToFile(products, &count);
    printf("Product deleted.\n");
    
}


void importExistedProduct(Product products[], int count) {
	int flag = 0;
	char newQuantity[9];
	int amount;
    char id[11];
    
    printf("Enter the ID of the product you want to import more: ");
    scanf("%s", id);
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(products[i].id, id) == 0) {
        	flag = 1;
        	printf("Product found: %s\n", products[i].name);

                    printf("Current quantity: %s\n", products[i].quantity);
                    printf("Import Amount: ");
                    scanf("%d", &amount);
                    int num1 = atoi(products[i].quantity);
					sprintf(newQuantity, "%d", num1+amount);
                    strcpy(products[i].quantity, newQuantity);
                    printf("Quantity updated successfully.\n");
                    sprintf(products[i].amount, "%d", amount);
				    float num2 = strtof(products[i].iprice, NULL); 
				    sprintf(products[i].total, "%.0f", amount*num2);
				    char *prev_import_index_str = strrchr(products[i].import_index, ' ');
		            int prev_import_index = atoi(prev_import_index_str);
		            int next_import_index = getNextImportIndex();
		            sprintf(products[i].import_index, "%-5d", next_import_index);

                    storeToImportFile(products[i]);
        }
        
    }
    if (flag = 0) printf("Product with ID %s not found.\n", id);
}

void exportExistedProduct(Product products[], int count) {
	char newQuantity[9];
	int amount;
    char id[11];
    int flag = 0;
    printf("Enter the ID of the product you want to export: ");
    scanf("%s", id);
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(products[i].id, id) == 0) {
        	flag = 1;
        	printf("Product found: %s\n", products[i].name);
            printf("Current quantity: %s\n", products[i].quantity);
                    printf("Export Amount: ");
                    scanf("%d", &amount);
                    int num1 = atoi(products[i].quantity);
					sprintf(newQuantity, "%d", num1-amount);
                    strcpy(products[i].quantity, newQuantity);
                    printf("Quantity updated successfully.\n");
                    sprintf(products[i].amount, "%d", amount);
				    float num2 = strtof(products[i].eprice, NULL); 
				    sprintf(products[i].total, "%.0f", amount*num2);
				    char *prev_export_index_str = strrchr(products[i].export_index, ' ');
		            int prev_export_index = atoi(prev_export_index_str);
		            int next_export_index = getNextExportIndex();
		            sprintf(products[i].export_index, "%-5d", next_export_index);
                    storeToExportFile(products[i]);
            }
        
    }
    if (flag = 0) printf("Product with ID %s not found.\n", id);
}

void changePriceofProduct (Product products[], int count) {
	char newPrice[11];
	char id[11];
	int flag = 0;
	printf("Enter the ID of the product you want to change price: ");
    scanf("%s", id);
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(products[i].id, id) == 0) {
        	flag = 1;
        	printf("Product found: %s\n", products[i].name);
        	printf("Current price: %s\n", products[i].eprice);
                    printf("Enter new price: ");
                    scanf("%s", newPrice);
                    strcpy(products[i].eprice, newPrice);
                    printf("Price updated successfully.\n");
        }
        
    }
    if (flag = 0) printf("Product with ID %s not found.\n", id);
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
