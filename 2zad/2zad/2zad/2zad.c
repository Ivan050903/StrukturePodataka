#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 20
#define ERROR_OPENING_FILE -1
#define BUFFER_SIZE 1024
#define EXIT_SUCCESS 0

struct _person;
typedef struct _person* position;

typedef struct _person {
    char first[MAX];
    char last[MAX];
    int god;

    position next;
}person;

void inputNew(position newPerson);
int addToStart(position head);
int printList(position firstPerson);
int addToEnd(position head);
position findLast(position head);
position findByName(position firstPerson, char* lastName);
int deletePerson(position head, char* lastName);
position findPrev(position firstPerson, char* lastName);

int main()
{
    //deklaracija head
    position head = NULL;
    head = (position)malloc(sizeof(person));
    if (head == NULL)
    {
        printf("Pogreska prilikom alokacije memorije\n");
        return ERROR_OPENING_FILE;
    }
    head->next = NULL;

    //umetanje na pocetak
    addToStart(head);

    //ispis liste
    printList(head->next);

    //umetanje na kraj
    addToEnd(head);

    //ispis liste
    printList(head->next);

    printf("\nUnesite prezime za pronaci: ");
    char Last[MAX];
    char* pLast = NULL;
    pLast = &Last[0];
    fgets(Last, sizeof(Last), stdin);
    Last[strcspn(Last, "\n")] = '\0';

    position PersonLastName = findByName(head->next, pLast);

    if (PersonLastName != NULL) {
        printf("\nTrazena osoba je %s %s, roden/a %d  godine", PersonLastName->first, PersonLastName->last, PersonLastName->god);
    }
    else {
        printf("\nOsoba nije pronadena.\n");
    }
    //izbrisi po prezimenu
    printf("\nUnesite prezime za izbrisati: ");
    char DelLast[MAX];
    char* pDelLast = NULL;
    pDelLast = &DelLast[0];
    fgets(DelLast, sizeof(DelLast), stdin);
    DelLast[strcspn(DelLast, "\n")] = '\0';

    deletePerson(head, pDelLast);

    //ispis liste
    printList(head->next);

    return 0;
}

void inputNew(position newPerson)
{
    printf("\nUnesi ime \n");
    fgets(newPerson->first, MAX, stdin);
    newPerson->first[strcspn(newPerson->first, "\n")] = '\0';
    printf("Unesi prezime \n");
    fgets(newPerson->last, MAX, stdin);
    newPerson->last[strcspn(newPerson->last, "\n")] = '\0';
    printf("Unesi godinu \n");
    scanf("%d", &newPerson->god);
    while (getchar() != '\n');
}

//umetanje na pocetak liste
int addToStart(position head)
{
    //deklaracija newPerson
    position newPerson1 = NULL;
    newPerson1 = (position)malloc(sizeof(person));
    if (newPerson1 == NULL)
    {
        printf("Pogreska prilikom alokacije memorije\n");
        return ERROR_OPENING_FILE;
    }
    inputNew(newPerson1);

    //umetanje
    newPerson1->next = head->next;
    head->next = newPerson1;
    return EXIT_SUCCESS;
}


int printList(position firstPerson)
{
    position temp = firstPerson;

    while (temp)
    {
        printf("\n");
        printf("\nIme: %s", temp->first);
        printf("\nPrezime: %s", temp->last);
        printf("\nGodina: %d", temp->god);
        printf("\n");
        temp = temp->next;
    }
    return EXIT_SUCCESS;
}

int addToEnd(position head)
{
    //zadnja osoba
    position lastPerson = NULL;
    lastPerson = (position)malloc(sizeof(person));
    if (lastPerson == NULL)
    {
        printf("Pogreska prilikom alokacije memorije\n");
        return ERROR_OPENING_FILE;
    }
    lastPerson = findLast(head);

    //nova zadnja osoba
    position newPerson2 = NULL;
    newPerson2 = (position)malloc(sizeof(person));
    if (newPerson2 == NULL)
    {
        printf("Pogreska prilikom alokacije memorije\n");
        return ERROR_OPENING_FILE;
    }
    inputNew(newPerson2);
    newPerson2->next = lastPerson->next;
    lastPerson->next = newPerson2;

    return EXIT_SUCCESS;
}

position findLast(position head)
{
    position temp = head;
    while (temp->next)
    {
        temp = temp->next;
    }
    return temp;
}

position findByName(position firstPerson, char* lastName)
{
    position temp = firstPerson;

    while (temp)
    {
        if (strcmp(temp->last, lastName) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

int deletePerson(position head, char* lastName)
{
    //prethodna osoba
    position prevPerson = findPrev(head, lastName);
    if (prevPerson == NULL || prevPerson->next == NULL) {
        printf("Osoba nije pronadena.\n");
        return EXIT_FAILURE;
    }
    //osoba koju cemo brisat
    position personToDelete = prevPerson->next;

    prevPerson->next = personToDelete->next;
    free(personToDelete);

    return EXIT_SUCCESS;
}

position findPrev(position firstPerson, char* lastName)
{
    position prevPerson = firstPerson;
    position currentPerson = firstPerson->next;

    while ((currentPerson) && (strcmp(currentPerson->last, lastName) != 0))
    {
        prevPerson = currentPerson;
        currentPerson = currentPerson->next;
    }
    if (currentPerson) return prevPerson;
    return NULL;
}