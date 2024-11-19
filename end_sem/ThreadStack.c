#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pthread_attr_t attr;
    size_t stacksize;

    // Initialize the thread attributes
    pthread_attr_init(&attr);

    // Set a custom stack size
    pthread_attr_setstacksize(&attr, stacksize); // 1 MB

    // Get the stack size
    int result = pthread_attr_getstacksize(&attr, &stacksize);
    if (result != 0) {
        perror("pthread_attr_getstacksize");
        return EXIT_FAILURE;
    }

    printf("Stack size: %zu bytes\n", stacksize);

    // Clean up
    pthread_attr_destroy(&attr);
    return 0;
}
