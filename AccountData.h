#include "Trim.h"

#define MAX_USER 100

typedef struct {
	gchar no[6], id[11], username[21], password[21],
		  fullName[31], gender[9], dob[9],
		  position[11], phoneNumber[16], profileImage[101];
} Account;

void loadAccountDataFromFile(Account accounts[], gint* count) {

	FILE* f = fopen("account.txt", "r");
	if (f == NULL)
	{
		printf("Cannot open the file.\n");
		exit(1);
	}

	gchar line[1024];
	gchar col0[6], col1[11], col2[21], col3[21], col4[31], col5[9], col6[9], col7[11], col8[16], col9[51];

	// Skip the first three lines 
	fgets(line, sizeof(line), f);
	fgets(line, sizeof(line), f);
	fgets(line, sizeof(line), f);

	// Start to scan data in the fourth line and attach to the Account struct array
	while (fgets(line, sizeof(line), f) != NULL)
	{
		if (sscanf(line, "| %5[^|] | %10[^|] | %20[^|] | %20[^|] | %30[^|] | %8[^|] | %8[^|] | %10[^|] | %15[^|] | %100[^|\n]",
			col0, col1, col2, col3, col4, col5, col6, col7, col8, col9) == 10) {

			strcpy(accounts[*count].no,			  trim(col0));
			strcpy(accounts[*count].id,			  trim(col1));
			strcpy(accounts[*count].username,	  trim(col2));
			strcpy(accounts[*count].password,	  trim(col3));
			strcpy(accounts[*count].fullName,	  trim(col4));
			strcpy(accounts[*count].gender,		  trim(col5));
			strcpy(accounts[*count].dob,		  trim(col6));
			strcpy(accounts[*count].position,	  trim(col7));
			strcpy(accounts[*count].phoneNumber,  trim(col8));
			strcpy(accounts[*count].profileImage, trim(col9));
			(*count)++;
		}
	}
	fclose(f);
}

void storeAccountDataToFile(Account accounts[], gint* count) {

	FILE* f = fopen("account.txt", "w");
	if (f == NULL) {
		printf("Cannot open the file.\n");
		exit(1);
	}

	gint MAX_DASH = 200;
	for (gint i = 0; i < MAX_DASH; i++) {
		fprintf(f, "-");
		if (i == MAX_DASH - 1) fprintf(f, "\n");
	}
	fprintf(f, "| %-5s | %-10s | %-20s | %-20s | %-30s | %-8s | %-8s | %-10s | %-15s | %-50s\n",
		"No", "ID", "Username", "Password",
		"Full Name", "Gender", "DOB",
		"Position", "Phone Number", "Profile Image (Source)");

	for (gint i = 0; i < MAX_DASH; i++) {
		fprintf(f, "-");
		if (i == MAX_DASH - 1) fprintf(f, "\n");
	}
	for (gint i = 0; i < *count; i++) {
		fprintf(f, "| %-5s | %-10s | %-20s | %-20s | %-30s | %-8s | %-8s | %-10s | %-15s | %-50s\n",
					accounts[i].no, accounts[i].id, accounts[i].username, accounts[i].password,
					accounts[i].fullName, accounts[i].gender, accounts[i].dob,
					accounts[i].position, accounts[i].phoneNumber, accounts[i].profileImage);
	}
	for (gint i = 0; i < MAX_DASH; i++) {
		fprintf(f, "-");
	}

	fclose(f);
}