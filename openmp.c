#include <stdio.h>
#include <omp.h>
#include <assert.h>
#include <unistd.h>

int main() {
    //parallel region starts here
    /* #pragma omp parallel num_threads(4) */
    /* { */
    /*     //hello world from each thread */
    /*     printf("Hello World from thread %d\n", omp_get_thread_num()); */
    /* } */

    /* #pragma omp parallel for */
    /* for(int i = 0; i < 10; i++) { */
    /*     printf("Hello World from thread %d\n", omp_get_thread_num()); */
    /* } */

    /* int id_public = -1; */
    /* int id; */
    /* #pragma omp parallel num_threads(4) private(id) */
    /* { */
    /*     id = omp_get_thread_num(); */
    /*     printf("Hello World from thread %d\n", id); */
    /*     /\* if(id == 0) id_public = 0; *\/ */
    /*     assert(id_public == -1); */
    /* } */

    /* #pragma omp parallel num_threads(4) */
    /* { */
    /*     printf("Hello World from thread %d\n", omp_get_thread_num()); */
    /*     #pragma omp barrier */
    /*     printf("Hello World from thread %d\n", omp_get_thread_num()); */
    /* } */

    /* int cnt = 0; */
    /* #pragma omp parallel num_threads(10) */
    /* { */
    /*     #pragma omp atomic */
    /*     cnt++; */
    /* } */
    /* printf("cnt: %d\n", cnt); */

    int fact = 1;
    #pragma omp parallel for num_threads(2) reduction(*:fact)
    for(int i = 1; i < 5; i++) {
        printf("Hello World from thread %d %d\n", omp_get_thread_num(), i);
        fact = i;
    }
    printf("Factorial: %d\n", fact);

    /* #pragma omp parallel num_threads(1) */
    /* #pragma omp sections */
    /* { */
    /*     #pragma omp section */
    /*     { */
    /*         printf("Hello World from thread %d\n", omp_get_thread_num()); */
    /*         sleep(1); */
    /*     } */
    /*     #pragma omp section */
    /*     { */
    /*         printf("Hello World from thread %d\n", omp_get_thread_num()); */
    /*         sleep(1); */
    /*     } */
    /* } */

    /* #pragma omp parallel num_threads(4) */
    /* { */
    /*     #pragma omp single */
    /*     { */
    /*         printf("Hello World from thread %d\n", omp_get_thread_num()); */
    /*     } */

    /*     #pragma omp master */
    /*     { */
    /*         printf("Hello World from thread %d\n", omp_get_thread_num()); */
    /*     } */
    /* } */

    return 0;
}
