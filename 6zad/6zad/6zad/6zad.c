#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DATE_EARLIER -1
#define DATE_EQ 0
#define DATE_LATER 1

struct article;
typedef struct article* articleposition;
typedef struct article {
	char name[15];
	int quantity;
	double price;
	articleposition next;
}article;

struct invoice;
typedef struct invoice* invoiceposition;
typedef struct invoice {
	char date[15];
	article head;
	invoiceposition next;
}invoice;

int read_invoice_from_file(invoiceposition);
int add_invoice_sorted(invoiceposition, invoiceposition);
int compare_dates(invoiceposition, invoiceposition);
int read_article_from_file(articleposition, char[]);
int create_and_add_article_sorted(articleposition, char[], int, double);
int print_invoice_date(invoiceposition);
int print_invoice_article(invoiceposition, int);
int find_article_in_period(invoiceposition, char[], char[], char[], int*, double*);


int main() {
	invoice head;
	head.next = NULL;
	read_invoice_from_file(&head);
	print_invoice_date(&head);
	int i = 1;
	while (print_invoice_article(&head, i) == EXIT_SUCCESS) {
		i++;
	}
	char date1[15];
	char date2[15];
	char find[15];
	int quantity;
	double price;
	printf("Unesi prvi datum u formatu god-mj-dan: "); scanf("%s", date1);
	printf("Unesi drugi datum u formatu god-mj-dan: "); scanf("%s", date2);
	printf("Unesi trazeni artikl: "); scanf("%s", find);
	find_article_in_period(&head, find, date1, date2, &quantity, &price);
	printf("U periodu od %s do %s, artikla %s ima: %d, po cijeni %lf. Sveukupno na ovaj artikl potroseno: %lf.\n", date1, date2, find, quantity, price, (double)quantity * price);

	return 0;
}

int read_invoice_from_file(invoiceposition head) {
	FILE* dat = NULL;
	dat = fopen("racuni.txt", "r");
	if (!dat) {
		printf("Datoteka se nije otvorila!");
		return EXIT_FAILURE;
	}
	int i;
	char file_name[15];
	FILE* dat2 = NULL;
	invoiceposition temp = NULL;

	while (feof(dat) == 0) {
		fscanf(dat, "%[^\n]s", file_name); fgetc(dat);
		dat2 = fopen(file_name, "r");
		if (!dat2) {
			printf("Datoteka se nije otvorila!");
			fclose(dat);
			return EXIT_FAILURE;
		}
		temp = (invoiceposition)malloc(sizeof(invoice));
		if (temp == NULL) {
			printf("Alokacija memorije nije uspijela!");
			fclose(dat);
			fclose(dat2);
			return EXIT_FAILURE;
		}
		fscanf(dat2, "%[^\n]s", temp->date);
		if (add_invoice_sorted(head, temp) == EXIT_FAILURE) {
			invoiceposition help = head->next;
			while (compare_dates(help, temp) != DATE_EQ) {
				help = help->next;
			}
			free(temp);
			temp = help;
		}
		read_article_from_file(&temp->head, file_name);
		temp = NULL;
		fclose(dat2);
		dat2 = NULL;
	}
	fclose(dat);
	return EXIT_SUCCESS;
}

int add_invoice_sorted(invoiceposition head, invoiceposition R) {
	if (head->next == NULL) {
		R->next = head->next;
		head->next = R;
		R->head.next = NULL;
		return EXIT_SUCCESS;
	}
	invoiceposition temp = head;
	while (temp->next != NULL) {
		if (compare_dates(R, temp->next) == DATE_EARLIER) {
			temp = temp->next;
		}
		else if (compare_dates(R, temp->next) == DATE_LATER) {
			R->next = temp->next;
			temp->next = R;
			R->head.next = NULL;
			return EXIT_SUCCESS;
		}
		else if (compare_dates(R, temp->next) == DATE_EQ) {
			return EXIT_FAILURE;
		}
	}
	R->next = temp->next;
	temp->next = R;
	R->head.next = NULL;
	return EXIT_SUCCESS;
}

int compare_dates(invoiceposition R1, invoiceposition R2) {
	int YYYY1, YYYY2;
	int MM1, MM2;
	int DD1, DD2;
	sscanf(R1->date, "%d-%d-%d", &YYYY1, &MM1, &DD1);
	sscanf(R2->date, "%d-%d-%d", &YYYY2, &MM2, &DD2);

	if (YYYY1 > YYYY2) {
		return DATE_EARLIER;
	}
	else if (YYYY1 == YYYY2 && MM1 > MM2) {
		return DATE_EARLIER;
	}
	else if (YYYY1 == YYYY2 && MM1 == MM2 && DD1 > DD2) {
		return DATE_EARLIER;
	}
	else if (YYYY1 == YYYY2 && MM1 == MM2 && DD1 == DD2) {
		return DATE_EQ;
	}
	else {
		return DATE_LATER;
	}

}

int read_article_from_file(articleposition head, char naziv[]) {
	FILE* file_pointer = NULL;
	file_pointer = fopen(naziv, "r");
	if (!file_pointer) {
		printf("Datoteka se nije otvorila!");
		return EXIT_FAILURE;
	}
	char name[15];
	int quantity;
	double price;
	while (fgetc(file_pointer) != '\n');
	while (1) {
		if (fscanf(file_pointer, "%s %d %lf", name, &quantity, &price) != 3) {
			break;
		}
		fgetc(file_pointer);
		create_and_add_article_sorted(head, name, quantity, price);
	}
	fclose(file_pointer);
	return EXIT_SUCCESS;

}

int create_and_add_article_sorted(articleposition head, char name[], int quantity, double price) {
	articleposition newly = NULL;
	newly = (articleposition)malloc(sizeof(article));
	if (newly == NULL) {
		printf("Memorija nije alocirana!");
		return EXIT_FAILURE;
	}
	strcpy(newly->name, name);
	newly->quantity = quantity;
	newly->price = price;
	if (head->next == NULL) {
		newly->next = head->next;
		head->next = newly;
		return EXIT_SUCCESS;
	}
	articleposition temp = head;
	while (temp->next != NULL) {
		if (strcmp(newly->name, temp->next->name) > 0) {
			temp = temp->next;
		}
		else if (strcmp(newly->name, temp->next->name) < 0) {
			newly->next = temp->next;
			temp->next = newly;
			return EXIT_SUCCESS;
		}
		else if (strcmp(newly->name, temp->next->name) == 0) {
			temp->next->quantity += quantity;
			free(newly);
			return EXIT_SUCCESS;
		}
	}
	newly->next = temp->next;
	temp->next = newly;
	return EXIT_SUCCESS;
	return 0;
}

int print_invoice_date(invoiceposition head) {
	if (head->next == NULL) {
		printf("Nema racuna!");
		return EXIT_FAILURE;
	}
	while (head->next != NULL) {
		head = head->next;
		printf("%s\n", head->date);
	}
	printf("\n");
	return EXIT_SUCCESS;
}

int print_invoice_article(invoiceposition head, int red) {
	if (head->next == NULL) {
		printf("Nema racuna!");
		return EXIT_FAILURE;
	}

	for (int i = 0; i < red; i++) {
		head = head->next;
		if (head == NULL) {
			printf("Svi racuni su ispisani!\n");
			return EXIT_FAILURE;
		}
	}

	articleposition arhead = &head->head;
	if (arhead == NULL) {
		printf("Nema artikala!");
		return EXIT_FAILURE;
	}
	printf("Datum %s, racun po redu %d. Sadrzaj:\n", head->date, red);
	while (arhead->next != NULL) {
		arhead = arhead->next;
		printf("%s %d %lf\n", arhead->name, arhead->quantity, arhead->price);
	}
	printf("\n");
	return EXIT_SUCCESS;
}

int find_article_in_period(invoiceposition head, char find[], char date1[], char date2[], int* quantity, double* price) {
	if (head->next == NULL) {
		printf("Nema racuna!");
		return EXIT_FAILURE;
	}
	*quantity = 0;
	*price = 0;
	invoice temp;
	strcpy(temp.date, date1);
	invoiceposition R1 = head->next;
	invoiceposition R2;
	while (compare_dates(R1, &temp) == DATE_LATER) {
		R1 = R1->next;
		if (R1 == NULL) {
			printf("Datum je nakon posljednjeg!\n");
			return EXIT_FAILURE;
		}
	}
	R2 = R1;
	strcpy(temp.date, date2);
	if (compare_dates(R2, &temp) == DATE_EARLIER) {
		printf("Datum je prije pocetnog!\n");
		return EXIT_FAILURE;
	}
	while (compare_dates(R2, &temp) == DATE_LATER) {
		R2 = R2->next;
		if (R2->next == NULL || compare_dates(R2->next, &temp) == DATE_EARLIER) {
			break;
		}
	}
	while (R1 != R2->next) {
		articleposition temp = R1->head.next;
		if (temp == NULL) {
			printf("Nema artikala na racunu!");
			return EXIT_FAILURE;
		}
		while (strcmp(temp->name, find) != 0) {
			temp = temp->next;
			if (temp == NULL) {
				break;
			}
		}
		if (temp != NULL) {
			*quantity += temp->quantity;
			*price = temp->price;
		}
		R1 = R1->next;
	}
	return EXIT_SUCCESS;
}