#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n 1000
#define maxIter n
#define xOffset 200

typedef struct complex {
    double real;
    double img;
} complex;

int ms(complex c);

int main(int argc, char** argv) {

    int img[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            img[i][j] = 0;
        }
    }

    clock_t time;
    time = clock();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            complex c;
            c.real = (j-xOffset-0.5*n)/(0.5*n);
            c.img = (i-0.5*n)/(0.5*n);
            img[i][j] = ms(c);
        }
    }

    time = clock() - time;
    double duration = ((double)time)/CLOCKS_PER_SEC;
    printf("Duration: %f\n", duration);

    FILE* fp = fopen("img.pgm", "wb");
    fprintf(fp, "P2\n%d %d\n%d\n", n, n, maxIter);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(fp, "%d ", img[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);

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