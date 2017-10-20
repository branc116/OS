#include <stdio.h>
#include <pthread.h>
#define N 10000
int a[10000];

struct tree {
    int data;
    struct tree* right;
    struct tree* left;
};
typedef struct tree tree_t;

void randomize(int n){
	int i,index,random,temp;
	for(i=0;i<10000;++i){
		temp=a[i];
		index=rand();
		random=a[index];
		a[index]=temp;
		a[i]=random;
	}
}

void insert_to_tree(tree_t** root,int data) {
    if (*root == NULL) {
        (*root) = malloc(sizeof(tree_t));
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
    
}

void fill() {
    int i=0;
    for (;i<N;i++) {
        a[i] = i;
    }
    return;
}

void *thread1(void* a){
    pthread_t pointer;
    int b = *((int *)a) + 1;
    while(1){
        pthread_create(&pointer, NULL, thread1, &b);
        printf("HEllo %d", b);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t pointer;
    int a = 0;
    while(1){
        pthread_create(&pointer, NULL, thread1, &a);
    }
    pthread_exit(NULL);
    return 0;
}