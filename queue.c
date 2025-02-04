#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 10

//what is typedef 
typedef struct{
    int vehicles[MAX_SIZE];
    int front;
    int rear;
    int size;
} Queue;

void initQueue(Queue* q){
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}

bool isEmpty(Queue *q){
    return q->size = 0;
}

bool isFull(Queue *q){
    return q->size == MAX_SIZE;
}

bool enqueue(Queue *q, int vehicle){
    if(isFull(q)){
        printf("Queue is full\n");
        return false;
    }

    q->rear = (q->rear + 1)&MAX_SIZE;
    q->vehicles[q->rear] = vehicle;
    q->size++;
    return true;
}

bool dequeue(Queue* q){
    if(isEmpty(q)){
        printf("Queue is empty\n");
        return -1;
    }
    int vehicle = q->vehicles[q->front];
    q->front = (q->front + 1) % MAX_SIZE;
    q->size--;
}

void displayQueue(Queue* q){
    if(isEmpty(q)){
        printf("Queue is empty\n");
        return;
    }
    printf("Queue:\n");
    for(int i = 0; i<q->size; i++){
        int index = (q->front + i) % MAX_SIZE;
        printf("%d ", q->vehicles[index]);
    }
    printf("\n");
}

void simulateTraffic(Queue* A, Queue* B, Queue* C, Queue* D, int steps){
    int timepervehicle = 2;
    
}