#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_CARS 3

struct direction {
    char name[20];
    int cur_cars;
    pthread_mutex_t lock_dir;
};
typedef struct direction direction_t;

struct car_thread {
    char name[50];
    pthread_t t;
    direction_t *dir;
};
typedef struct car_thread car_thread_t;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
direction_t dirs[] = {
    {.cur_cars = 0, .name= "north", .lock_dir = PTHREAD_MUTEX_INITIALIZER}, 
    {.cur_cars = 0, .name = "north west", .lock_dir = PTHREAD_MUTEX_INITIALIZER}
};
char car_names[][20] = {"BMW", "Tesla", "Mercedes", "Unknown"};

void *car_over_bridge(void *args) {
    int waited = 0;
    int pass_time;
    car_thread_t* car = ((car_thread_t *)args);
    printf("Car %s wants to drive in a direction of %s.\n", car->name, car->dir->name);
    pthread_mutex_lock(&(car->dir->lock_dir));
    while (car->dir->cur_cars >= MAX_CARS) {
        sleep(1);
        waited++;
    }
    car->dir->cur_cars++;
    pthread_mutex_unlock(&(car->dir->lock_dir));
    pass_time = rand() % 20 +3;
    printf("Car %s: Has waited for %d, will need %ds to pass the bridge in a direction of %s. He is traveling with %d other cars in the same direction.\n",
            car->name, waited, pass_time, car->dir->name, car->dir->cur_cars - 1);
    sleep(pass_time);
    car->dir->cur_cars--;
    printf("Car %s has passed the bridge.\n", car->name);
    free(args);
    return NULL;
}

int main(void) {
    int rand_num;
    car_thread_t* car;
    srand(time(NULL));
    while(1) {
        car = (car_thread_t *)malloc(sizeof(car_thread_t));
        sprintf(car->name, "%s %c%d", car_names[rand() %(sizeof(car_names)/20)], (char)(rand()%26) + 'a', rand()%1000);
        car->dir = &dirs[rand() % (sizeof(dirs) /sizeof(direction_t))];
        pthread_create(&(car->t), NULL, car_over_bridge, car);
        sleep(5);
    }
    return 0;
}