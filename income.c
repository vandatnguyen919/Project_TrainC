#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

void mergeFiles(const char* file1, const char* file2, const char* merged_file) {
    FILE* f1 = fopen(file1, "r");
    FILE* f2 = fopen(file2, "r");
    FILE* out = fopen(merged_file, "w");
    char line[MAX_LINE_LENGTH];

    if (f1 == NULL || f2 == NULL || out == NULL) {
        printf("Cannot open file.\n");
        return;
    }

    // copy contents of file1 to merged_file
    while (fgets(line, MAX_LINE_LENGTH, f1)) {
        fprintf(out, "%s", line);
    }

    // copy contents of file2 to merged_file
    while (fgets(line, MAX_LINE_LENGTH, f2)) {
        fprintf(out, "%s", line);
    }

    fclose(f1);
    fclose(f2);
    fclose(out);
}

void trim(char* str) {
    int len = strlen(str);
    int i;
    for (i = 0; i < len; i++) {
        if (!isspace(str[i])) {
            break;
        }
    }

    if (i == len) {
        str[0] = '\0';
        return;
    }

    int j;
    for (j = len - 1; j >= 0; j--) {
        if (!isspace(str[j])) {
            break;
        }
    }

    str[j + 1] = '\0';
    if (i > 0) {
        memmove(str, str + i, j - i + 2);
    }
}

void calculateDailyIncome(int day, int month, int year) {
    float total_import_money_by_date[32] = {0};
    float total_export_money_by_date[32] = {0};
    FILE* import_fp = fopen("import.txt", "r");
    if (import_fp == NULL) {
        printf("Cannot open file %s\n", "import.txt");
        return;
    }
    FILE* export_fp = fopen("export.txt", "r");
    if (export_fp == NULL) {
        printf("Cannot open file %s\n", "export.txt");
        fclose(import_fp);
        return;
    }

    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), import_fp) == NULL) {
        printf("Invalid file format.\n");
        fclose(import_fp);
        fclose(export_fp);
        return;
    }
    if (fgets(line, sizeof(line), export_fp) == NULL) {
        printf("Invalid file format.\n");
        fclose(import_fp);
        fclose(export_fp);
        return;
    }

    while (fgets(line, sizeof(line), import_fp) != NULL) {
        int no, total_qty, imp_qty;
        float total_money;
        char id[11], iprice[11], import_time[20];

        if (sscanf(line, "| %d | %10s | %18d | %10s | %d | %f | %[^\n]", &no, id, &total_qty, iprice, &imp_qty, &total_money, import_time) == 7) {
            int day_imp, month_imp, year_imp;
            if (sscanf(import_time, "%d/%d/%d", &day_imp, &month_imp, &year_imp) == 3) {
                if (day_imp >= 1 && day_imp <= 31 &&
while (fgets(line, sizeof(line), export_fp) != NULL) {
    int no, total_qty, exp_qty;
    float total_money;
    char id[11], eprice[11], export_time[20];

    if (sscanf(line, "| %d | %10s | %18d | %10s | %d | %f | %[^\n]", &no, id, &total_qty, eprice, &exp_qty, &total_money, export_time) == 7) {
        int day_exp, month_exp, year_exp;
        if (sscanf(export_time, "%d/%d/%d", &day_exp, &month_exp, &year_exp) == 3) {
            if (day_exp >= 1 && day_exp <= 31 && month_exp >= 1 && month_exp <= 12 && year_exp >= 1900 && year_exp <= 9999) {
                if (day_exp == day && month_exp == month && year_exp == year) {
                    total_export_money_by_date[day_exp] += total_money;
                }
            }
        }
    }
}

fclose(import_fp);
fclose(export_fp);

printf("Total income on %d/%d/%d: %0.2f\n", day, month, year, total_export_money_by_date[day] - total_import_money_by_date[day]);
}

int main() {
mergeFiles("import1.txt", "import2.txt", "import.txt");
mergeFiles("export1.txt", "export2.txt", "export.txt");int day, month, year;
printf("Enter date (dd/mm/yyyy): ");
scanf("%d/%d/%d", &day, &month, &year);

calculateDailyIncome(day, month, year);

return 0;
}
