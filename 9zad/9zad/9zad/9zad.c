#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct cvor Cvor;
typedef Cvor* position;
struct cvor {
    int element;
    position L;
    position R;
};

position addCvor(position root, int newElement);
int replace(position root);
int fprint_inorder(position root, FILE* file);

int main()
{

    position root = NULL;

    root = addCvor(root, 2);
    addCvor(root, 5);
    addCvor(root, 7);
    addCvor(root, 8);
    addCvor(root, 11);
    addCvor(root, 1);
    addCvor(root, 4);
    addCvor(root, 2);
    addCvor(root, 3);
    addCvor(root, 7);

    FILE* file = NULL;
    file = fopen("dat.txt", "w");

    fprint_inorder(root, file);
    fprintf(file, "\n");

    replace(root);
    
    fprint_inorder(root, file);
    fclose(file);

    // c)

    position randRoot = NULL;

    srand(time(NULL));

    for (int i = 0; i < 8; i++) {
        int x = rand() % 81 + 10;
        printf("Generated random number: % d\n", x);
        randRoot = addCvor(randRoot, x);
    }

    return 0;
}

position addCvor(position root, int newElement) {

    if (root == NULL) {
        root = (position)malloc(sizeof(Cvor));

        if (root == NULL) {
            printf("Error alocating memory!");
            return NULL;
        }

        root->element = newElement;
        root->L = root->R = NULL;
        printf("Added element %d \n", newElement);
    }
    else {
        if (newElement >= root->element) {
            printf("Going left from %d to add %d\n", root->element, newElement);
            root->L = addCvor(root->L, newElement);
        }
        else {
            printf("Going right from %d to add %d\n", root->element, newElement);
            root->R = addCvor(root->R, newElement);
        }
    }

    return root;
}

int replace(position root) {

    if (root == NULL) {
        return 0;
    }
    int x = root->element;

    printf("Before replace: Node %d\n", root->element);
    int leftSum = replace(root->L);
    int rightSum = replace(root->R);
    root->element = leftSum + rightSum;
    printf("After replace: Node %d updated to %d\n", x, root->element); 
    
    return x + root->element;
}

int fprint_inorder(position root, FILE* file) {

    if (root == NULL) {
        return 0;
    }
    fprint_inorder(root->L, file);
    fprintf(file, "%d, ", root->element);
    fprint_inorder(root->R, file);

    return 0;
}
