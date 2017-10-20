#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define N 10000
int a[N];

struct tree {
    int data;
    struct tree* right;
    struct tree* left;
};
typedef struct tree tree_t;

struct dretva_params {
    int from;
    int to;
    tree_t** drvo;
};
typedef struct dretva_params dretva_params_t;

void randomize(int n){
    int i,index,random,temp;
    for(i=0;i<(N - 2);++i){
        temp=a[i];
        index=rand() % N;
        random=a[index];
        a[index]=temp;
        a[i]=random;
    }
}

void insert_to_tree(tree_t** root,int data) {
    if (*root == NULL) {
        (*root) = malloc(sizeof(tree_t));
        (*root)->data = data;
        return;
    }
    if (data > (*root)->data) {
        insert_to_tree(&(((*(root))->right)), data);
        return;
    }
    insert_to_tree(&(((*(root))->left)), data);
}

void print_tree(tree_t* root) {
    if (root == NULL)
        return;
    print_tree(root->left);
    printf("%d ", root->data);
    print_tree(root->right);
}


void array_to_drvo(tree_t* drvo, int od_index, int do_index) {
    
    int i=od_index;
    for(;i<do_index;i++) {
        insert_to_tree(&drvo, a[i]);
    }
    // print_tree(drvo);
    return;
}


void fill() {
    int i=0;
    for (;i<N;i++) {
        a[i] = i;
    }
    return;
}
void print_array(){
    int i=0;
    for (;i<N;i++) {
        printf("%d ", a[i]);
    }
    printf("\n DONE \n");
    return;
}

void *thread1(void* a){
    dretva_params_t b = *((dretva_params_t *)a);
    printf("array to drvo from %d to %d\n", b.from, b.to);
    array_to_drvo(*(b.drvo), b.from, b.to);
    printf("zavrseno\n");
    // print_tree(*(b.drvo));
    // return &b;
}

void *crash(void *a) {
    pthread_t firstPart;
    int b = (*(int *)a) + 1;
    printf("HEllo %d ", b);
    while(1)
        pthread_create(&firstPart, NULL, thread1, &b);
}

int main() {
    pthread_t firstPart, secondPart;
    dretva_params_t b, c;
    tree_t** drvo = malloc(sizeof(tree_t *));
    int ardy = 0;
    crash(&ardy);
    b.drvo = &drvo;
    b.from = 0;
    b.to = N/2;
    c.drvo = &drvo;
    c.from = N/2;
    c.to = N;
    
    srand(time(NULL));
    fill();
    randomize(2000000);
    pthread_create(&firstPart, NULL, thread1, &b);
    pthread_create(&secondPart, NULL, thread1, &c);
    pthread_exit(NULL);
    print_tree(drvo);
    return 0;
}