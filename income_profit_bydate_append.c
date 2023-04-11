#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void calculateByDate(int day, int month, int year) {
    int total_import_money_by_date[32] = {0};
    int total_export_money_by_date[32] = {0};
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

    char line[1024];
    if (fgets(line, sizeof(line), import_fp) == NULL) {
        fclose(import_fp);
        fclose(export_fp);
        return;
    }
    if (fgets(line, sizeof(line), export_fp) == NULL) {
        fclose(import_fp);
        fclose(export_fp);
        return;
    }

    while (fgets(line, sizeof(line), import_fp) != NULL) {
        int no, total_qty, imp_qty, total_money;
        char id[11], iprice[11], import_time[20];

        if (sscanf(line, "| %d | %10s | %18d | %11s | %19d | %21d | %[^\n]", &no, id, &total_qty, iprice, &imp_qty, &total_money, import_time) == 7) {
            int day_imp, month_imp, year_imp;
            if (sscanf(import_time, "%d/%d/%d", &day_imp, &month_imp, &year_imp) == 3) {
                if (day_imp >= 1 && day_imp <= 31 && month_imp == month && year_imp == year) {
                    total_import_money_by_date[day_imp] += total_money;
                }
            }
        }
    }

    while (fgets(line, sizeof(line), export_fp) != NULL) {
        int no, total_qty, exp_qty, total_money;
        char id[11], eprice[11], export_time[20];

        if (sscanf(line, "| %d | %10s | %18d | %11s | %19d | %21d | %[^\n]", &no, id, &total_qty, eprice, &exp_qty, &total_money, export_time) == 7) {
            int day_exp, month_exp, year_exp;
            if (sscanf(export_time, "%d/%d/%d", &day_exp, &month_exp, &year_exp) == 3) {
                if (day_exp >= 1 && day_exp <= 31 && month_exp == month && year_exp == year) {
                    total_export_money_by_date[day_exp] += total_money;
                }
            }
        }
    }

    FILE* profit_fp = fopen("profit.txt", "a");
    FILE* income_fp = fopen("income.txt", "a");

    if (profit_fp == NULL) {
        printf("Cannot create file %s\n", "profit.txt");
        fclose(import_fp);
        fclose(export_fp);
        return;
    }
	fprintf(profit_fp, "| %02d/%02d/%04d           | %-20d | %-20d | %-20d\n", day, month, year, total_import_money_by_date[day], total_export_money_by_date[day], (total_export_money_by_date[day] - total_import_money_by_date[day]));
	fprintf(income_fp, "| %02d/%02d/%04d           | %-20d\n", day, month, year, total_export_money_by_date[day]);
	fclose(import_fp);
	fclose(export_fp);
	fclose(profit_fp);
	fclose(income_fp);
}
int getDaysInMonth(int month, int year) {
    int days = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        days = 30;
    } else if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
            days = 29;
        } else {
            days = 28;
        }
    }
    return days;
}

void displayData() {
    FILE* income_fp = fopen("income.txt", "r");
    if (income_fp == NULL) {
        printf("Cannot open file %s\n", "income.txt");
        return;
    }
    char line[1024];
    int i; 
     for (i = 0; i < 3; i++) {
        if (fgets(line, sizeof(line), income_fp) == NULL) {
            break;
        }
    }
    while (fgets(line, sizeof(line), income_fp) != NULL) {
        printf("%s", line);
    }
   
    fclose(income_fp);
    FILE* profit_fp = fopen("profit.txt", "r");
    if (profit_fp == NULL) {
        printf("Cannot open file %s\n", "profit.txt");
        return;
    }
    char line2[1024];

     for (i = 0; i < 3; i++) {
        if (fgets(line2, sizeof(line), profit_fp) == NULL) {
            break;
        }
    }
    while (fgets(line2, sizeof(line), profit_fp) != NULL) {
        printf("%s", line2);
    }
   
    fclose(profit_fp);
}

int main() {
    char* import_file = "import.txt";
    char* export_file = "export.txt";
	
    int start_day;int start_month;int start_year;
	printf ("Enter start date (day(dd) month(mm) year(yy)): ");
	scanf ("%d %d %d", &start_day, &start_month, &start_year); 
	int end_day;int end_month;int end_year;
	printf ("Enter end date (day(dd) month(mm) year(yy)): ");
	scanf ("%d %d %d", &end_day, &end_month, &end_year); 
	int y,m,d; 
	for (y = start_year; y <= end_year; y++) {
	    int start_m = y == start_year ? start_month : 1;
	    int end_m = y == end_year ? end_month : 12;
	    for (m = start_m; m <= end_m; m++) {
	        int start_d = (y == start_year && m == start_month) ? start_day : 1;
	        int end_d = (y == end_year && m == end_month) ? end_day : getDaysInMonth(m, y);
	        for (d = start_d; d <= end_d; d++) {
	            calculateByDate(d, m, y);
	        }
	    }
	}
	printf ("Updated!\n");
	displayData();
    return 0;
}

