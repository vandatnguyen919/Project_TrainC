
#define MAX_GOOD 100

typedef struct {
	gchar no[6], id[11], type[21], name[31],
		  quantity[9], ImPrice[16], ExPrice[16], productImage[101];
} Product;

typedef struct {
    gchar id[11], quantity[21], price[16], totalPrice[21], totalQuantity[21], time[21];
} Import;

typedef struct {
    gchar id[11], quantity[21], price[16], totalPrice[21], totalQuantity[21], time[21];
} Export;

void loadProductDataFromFile(Product products[], gint* count) {

    FILE* f = fopen("product.txt", "r");
    if (f == NULL) {
        printf("Cannot open the file.\n");
        exit(1);
    }

    gchar line[1024];
    gchar col0[6], col1[11], col2[21], col3[31], col4[9], col5[16], col6[16], col7[51];

    // Skip the header lines
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);

    // Read the data lines 
    while (fgets(line, sizeof(line), f) != NULL) {
        if (sscanf(line, "| %5[^|] | %10[^|] | %20[^|] | %30[^|] | %8[^|] | %15[^|] | %15[^|] | %100[^|\n]",
            col0, col1, col2, col3, col4, col5, col6, col7) == 8) {
            strcpy(products[*count].no,           trim(col0));
            strcpy(products[*count].id,           trim(col1));
            strcpy(products[*count].type,         trim(col2));
            strcpy(products[*count].name,         trim(col3));
            strcpy(products[*count].quantity,     trim(col4));
            strcpy(products[*count].ImPrice,      trim(col5));
            strcpy(products[*count].ExPrice,      trim(col6));
            strcpy(products[*count].productImage, trim(col7));
            (*count)++;
        }
    }

    fclose(f);
}

void storeProductDataToFile(Product products[], gint* count) {
   
    FILE* f = fopen("product.txt", "w");
    if (f == NULL) {
        printf("Cannot open the file.\n");
        exit(1);
    }

    gint MAX_DASH = 200;
    for (gint i = 0; i < MAX_DASH; i++) {
        fprintf(f, "-");
        if (i == MAX_DASH - 1) fprintf(f, "\n");
    }
    fprintf(f, "| %-5s | %-10s | %-20s | %-30s | %-8s | %-15s | %-15s | %-50s \n",
        "No", "ID", "Type", "Name",
        "Quantity", "Import Price", "Export Price", "Product Image (Source)");

    for (gint i = 0; i < MAX_DASH; i++) {
        fprintf(f, "-");
        if (i == MAX_DASH - 1) fprintf(f, "\n");
    }
    for (gint i = 0; i < *count; i++) {
        fprintf(f, "| %-5s | %-10s | %-20s | %-30s | %-8s | %-15s | %-15s | %-50s \n",
            products[i].no,       products[i].id,    products[i].type,  products[i].name,
            products[i].quantity, products[i].ImPrice, products[i].ExPrice, products[i].productImage);

    }
    for (gint i = 0; i < MAX_DASH; i++) {
        fprintf(f, "-");
    }

    fclose(f);
}

void storeProductDataToImportFile(Product product, gint import_quantity) {

    time_t t = time(NULL);
    struct tm* tm = localtime(&t);

    FILE* f = fopen("import.txt", "a+");
    if (f == NULL) {
        printf("Cannot open the file.\n");
        exit(1);
    }

    rewind(f);
    gchar line[1024];
    gint lineNum = 0;

    while (fgets(line, sizeof(line), f) != NULL) {
        lineNum++;
    }

    gint MAX_DASH = 200;
    if (lineNum == 0) {
        for (gint i = 0; i < MAX_DASH; i++) {
            fprintf(f, "-");
            if (i == MAX_DASH - 1) fprintf(f, "\n");
        }
        fprintf(f, "| %-10s | %-20s | %-15s | %-20s | %-20s | %-20s \n",
            "ID", "Import Quantity", "Price", "Total Price", "Total Quantity", "Import Time");
        for (gint i = 0; i < MAX_DASH; i++) {
            fprintf(f, "-");
            if (i == MAX_DASH - 1) fprintf(f, "\n");
        }
    }

    gint ImQuantity = import_quantity;
    gint ImPrice = atoi(product.ImPrice);
    gint TotalPrice = ImQuantity * ImPrice;

    fprintf(f, "| %-10s | %-20d | %-15s | %-20d | %-20s | %02d/%02d/%d %02d:%02d:%02d \n",
        product.id, ImQuantity, product.ImPrice,
        TotalPrice, product.quantity,
        tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900,
        tm->tm_hour, tm->tm_min, tm->tm_sec
    );

    //Reverse the every line in import file
    FILE* f_temp = fopen("import_temp.txt", "w");
    if (f_temp == NULL) {
        printf("Cannot open the file.\n");
        exit(1);
    }

    gchar lines[1024][1024];
    lineNum = 0;

    rewind(f);
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);

    while (fgets(lines[lineNum], 1024, f) != NULL) {
        lineNum++;
    }

    // Start to write information to Import temp file
    for (gint i = 0; i < MAX_DASH; i++) {
        fprintf(f_temp, "-");
        if (i == MAX_DASH - 1) fprintf(f_temp, "\n");
    }
    fprintf(f_temp, "| %-10s | %-20s | %-15s | %-20s | %-20s | %-20s \n",
        "ID", "Import Quantity", "Price", "Total Price", "Total Quantity", "Import Time");
    for (gint i = 0; i < MAX_DASH; i++) {
        fprintf(f_temp, "-");
        if (i == MAX_DASH - 1) fprintf(f_temp, "\n");
    }
    fputs(lines[lineNum - 1], f_temp);
    for (gint i = 0; i < lineNum - 1; i++) {
        fputs(lines[i], f_temp);
    }

    fclose(f);
    fclose(f_temp);

    //Remove the old file
    if (remove("import.txt") != 0) {
        printf("Cannot remove the orignal file.\n");
        exit(1);
    }
    //Rename the temp file's name to the original file's name
    if (rename("import_temp.txt", "import.txt") != 0) {
        printf("Cannot remane the temporary file.\n");
        exit(1);
    }
}

void loadImportDataFromFile(Import imports[], int* count) {

    FILE* f = fopen("import.txt", "r");
    if (f == NULL) {
        printf("Cannot open the file.\n");
        exit(1);
    }

    gchar line[1024];
    gchar col0[11], col1[21], col2[16], col3[21], col4[21], col5[21];

    // Skip the header lines
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);

    // Read the data lines 
    while (fgets(line, sizeof(line), f) != NULL) {
        if (sscanf(line, "| %10[^|] | %20[^|] | %15[^|] | %20[^|] | %20[^|] | %20[^|\n]",
            col0, col1, col2, col3, col4, col5) == 6) {
            strcpy(imports[*count].id,            trim(col0));
            strcpy(imports[*count].quantity,      trim(col1));
            strcpy(imports[*count].price,         trim(col2));
            strcpy(imports[*count].totalPrice,    trim(col3));
            strcpy(imports[*count].totalQuantity, trim(col4));
            strcpy(imports[*count].time,          trim(col5));
            (*count)++;
        }
    }
    fclose(f);

}

void storeProductDataToExportFile(Product product, gint order_quantity) {

    time_t t = time(NULL);
    struct tm* tm = localtime(&t);

    FILE* f = fopen("export.txt", "a+");
    if (f == NULL) {
        printf("Cannot open the file.\n");
        exit(1);
    }

    rewind(f);
    gchar line[1024];
    gint lineNum = 0;

    while (fgets(line, sizeof(line), f) != NULL) {
        lineNum++;
    }

    gint MAX_DASH = 200;
    if (lineNum == 0) {
        for (gint i = 0; i < MAX_DASH; i++) {
            fprintf(f, "-");
            if (i == MAX_DASH - 1) fprintf(f, "\n");
        }
        fprintf(f, "| %-10s | %-20s | %-15s | %-20s | %-20s | %-20s \n",
            "ID", "Export Quantity", "Price", "Total Price", "Total Quantity", "Export Time");
        for (gint i = 0; i < MAX_DASH; i++) {
            fprintf(f, "-");
            if (i == MAX_DASH - 1) fprintf(f, "\n");
        }
    }

    gint TotalPrice = order_quantity * atoi(product.ExPrice);

    fprintf(f, "| %-10s | %-20d | %-15s | %-20d | %-20s | %02d/%02d/%d %02d:%02d:%02d \n",
        product.id, order_quantity, product.ExPrice,
        TotalPrice, product.quantity,
        tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900,
        tm->tm_hour, tm->tm_min, tm->tm_sec
    );

    //Reverse the every line in import file
    FILE* f_temp = fopen("export_temp.txt", "w");
    if (f_temp == NULL) {
        printf("Cannot open the file.\n");
        exit(1);
    }

    gchar lines[1024][1024];
    lineNum = 0;

    rewind(f);
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);

    while (fgets(lines[lineNum], 1024, f) != NULL) {
        lineNum++;
    }

    // Start to write information to Import temp file
    for (gint i = 0; i < MAX_DASH; i++) {
        fprintf(f_temp, "-");
        if (i == MAX_DASH - 1) fprintf(f_temp, "\n");
    }
    fprintf(f_temp, "| %-10s | %-20s | %-15s | %-20s | %-20s | %-20s \n",
        "ID", "Export Quantity", "Price", "Total Price", "Total Quantity", "Export Time");
    for (gint i = 0; i < MAX_DASH; i++) {
        fprintf(f_temp, "-");
        if (i == MAX_DASH - 1) fprintf(f_temp, "\n");
    }
    fputs(lines[lineNum - 1], f_temp);
    for (gint i = 0; i < lineNum - 1; i++) {
        fputs(lines[i], f_temp);
    }

    fclose(f);
    fclose(f_temp);

    //Remove the old file
    if (remove("export.txt") != 0) {
        printf("Cannot remove the orignal file.\n");
        exit(1);
    }
    //Rename the temp file's name to the original file's name
    if (rename("export_temp.txt", "export.txt") != 0) {
        printf("Cannot remane the temporary file.\n");
        exit(1);
    }
}

void loadExportDataFromFile(Export exports[], int* count) {

    FILE* f = fopen("export.txt", "r");
    if (f == NULL) {
        printf("Cannot open the file.\n");
        exit(1);
    }

    gchar line[1024];
    gchar col0[11], col1[21], col2[16], col3[21], col4[21], col5[21];

    // Skip the header lines
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);

    // Read the data lines 
    while (fgets(line, sizeof(line), f) != NULL) {
        if (sscanf(line, "| %10[^|] | %20[^|] | %15[^|] | %20[^|] | %20[^|] | %20[^|\n]",
            col0, col1, col2, col3, col4, col5) == 6) {
            strcpy(exports[*count].id,            trim(col0));
            strcpy(exports[*count].quantity,      trim(col1));
            strcpy(exports[*count].price,         trim(col2));
            strcpy(exports[*count].totalPrice,    trim(col3));
            strcpy(exports[*count].totalQuantity, trim(col4));
            strcpy(exports[*count].time,          trim(col5));
            (*count)++;
        }
    }
    fclose(f);

}