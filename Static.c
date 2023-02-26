#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define n 1000
#define maxIter n
#define xOffset 200

typedef struct complex {
    double real;
    double img;
} complex;

int ms(complex c);

int main(int argc, char** argv) {

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    int* img = malloc(sizeof(int)*n*n);
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                img[i*n + j] = 0;
            }
        }
    }

    int parallel_size = n/size;
    int* recv_buff = malloc(sizeof(int)*parallel_size*n);

    clock_t time;
    time = clock();

    MPI_Scatter(recv_buff, parallel_size*n, MPI_INT, img, parallel_size*n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < parallel_size; i++) {
        for (int j = 0; j < n; j++) {
            complex c;
            c.real = (j-xOffset-0.5*n)/(0.5*n);
            c.img = ((i+rank*parallel_size)-(0.5*n))/(0.5*n);
            recv_buff[i*n+j] = ms(c);
        }
    }

    MPI_Gather(recv_buff, parallel_size*n, MPI_INT, img, parallel_size*n, MPI_INT, 0, MPI_COMM_WORLD);

    time = clock() - time;
    double duration = ((double)time)/CLOCKS_PER_SEC;
    if (rank == 0) {
        printf("Duration: %f\n", duration);
        FILE* fp = fopen("imgStatic.pgm", "wb");
        fprintf(fp, "P2\n%d %d\n%d\n", n, n, maxIter);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fprintf(fp, "%d ", img[i*n+j]);
            }
            fprintf(fp, "\n");
        }
        fclose(fp);
    }

    MPI_Finalize();

    return 0;

}

int ms(complex c) {
    complex z;
    z.real = 0;
    z.img = 0;
    int k = 0;
    while ((z.real*z.real + z.img*z.img) < 4.0 && k < maxIter) {
        double tempReal = z.real*z.real - z.img*z.img + c.real;
        z.img = 2*z.real*z.img + c.img;
        z.real = tempReal;
        k++;
    }
    return k;
}
