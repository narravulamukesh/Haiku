#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

void *on(void *arg);
void *off(void *arg);
float wat = 7;
sem_t s;

int main() {
    int ch;
    sem_init(&s, 0, 1); 
    pthread_t p1, p2;
    int flag=0;
 
    while (1) {
        printf("Enter Your choice: \n a) 1 to turn on a high power consuming device.\n b) 2 to turn off high power consuming device. \n c) 3 to turn on low power.\n d) 4 to turn off low power. \n e) 5 to exit.\n ");
        if (scanf("%d", &ch) != 1) {
            printf("Invalid input. Please enter a number...\n");
            while (getchar() != '\n'); // Clear invalid input
            continue;
        }

        if (ch == 1) {
            if (sem_trywait(&s) == 0) { // Check if semaphore is available
                pthread_create(&p1, NULL, on, NULL);
                pthread_join(p1, NULL);
            } else {
                printf("One high power consuming device is already running...\n");
            }
        } else if (ch == 2) {
            if (sem_trywait(&s) == -1) { // Check if semaphore is unavailable
                pthread_create(&p2, NULL, off, NULL);
                pthread_join(p2, NULL);
            } else {
                printf("No high power consuming device is currently running...\n");
                sem_post(&s); // Restore the semaphore state
            }
        } else if (ch == 3) {
            wat -= 0.15;
            printf("Low power consuming device is currently running...\n");
            printf("Remaining power: %.2f\n", wat);
            flag=1;
        } else if (ch == 4) {
            if (flag==1){
                wat += 0.15;
                printf("Low power consuming device stoped running...\n");
                printf("Remaining power: %.2f\n", wat);
            }
            else{
                printf("No low power consuming device is running");
            }
            
        } else if (ch == 5) {
            break;
        } else {
            printf("Invalid choice. Please enter 1, 2, 3, 4, or 5.\n");
        }
    }

    // Destroy semaphore
    sem_destroy(&s);

    return 0;
}

void *on(void *arg) {
    float x = wat;
    x -= 3.5;
    wat = x;
    printf("Watt(in kw) after the device running: %.2f\n", wat);
    return NULL;
}

void *off(void *arg) {
    float y = wat;
    y += 3.5;
    wat = y;
    printf("Watt(in kw) after the device stoped running: %.2f\n", wat);
    sem_post(&s); // Release semaphore after turning off
    return NULL;
}
