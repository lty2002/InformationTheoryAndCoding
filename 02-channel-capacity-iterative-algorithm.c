#include<stdio.h>
#include<math.h>
#include<stdlib.h>

const double INIT_C = -1000.0;    //取初始迭代时的信道容量
const double EPSILON = 1e-5;

/**
 *
 * @param input_matrix  输入信源概率矩阵
 * @param x_size  输入信源个数
 * @param y_size  输入信宿个数
 * @param pa  输出最佳信源分布
 * @param k 输出迭代次数
 * @return 输出信道容量
 */
double getC(int x_size, int y_size, double input_matrix[x_size][y_size], double **pa, int *k) {
    int cnt = 0;
    double *p = malloc(sizeof(double) * x_size);
    double z[20];
    double F[20][20];
    double x, y, last;
    double C = INIT_C;

    for (int i = 0; i < x_size; i++) {
        p[i] = (double) (1.0 / (double) x_size);
    }
    do {
        cnt++;
        last = C;
        for (int j = 0; j < y_size; j++) {
            x = 0.0;
            for (int i = 0; i < x_size; i++) {
                x = x + (p[i]) * (input_matrix[i][j]);
            }
            if (x > 0) {
                for (int i = 0; i < x_size; i++)
                    F[i][j] = (p[i]) * (input_matrix[i][j]) / x;
            } else {
                for (int i = 0; i < x_size; i++) {
                    F[i][j] = 0.0;
                }
            }
        }
        y = 0.0;
        for (int i = 0; i < x_size; i++) {
            z[i] = 0.0;
            for (int j = 0; j < y_size; j++) {
                if (F[i][j] > 0)
                    z[i] = z[i] + (input_matrix[i][j] * (log(F[i][j]) / log(2.0)));
            }
            z[i] = (pow(2.0, z[i]));
            y = y + z[i];
        }
        for (int i = 0; i < x_size; i++) {
            p[i] = z[i] / y;
        }
        C = (log(y) / log(2.0));
    } while (fabs((C - last) / C) > EPSILON);

    *k = cnt;
    *pa = p;
    return C;
}

int main() {
    int k;
    double *pa;
    double in[5][3] = {
            {1,  0, 0},
            {1,  0, 0},
            {0,  1, 0},
            {0,  1, 0},
            {0,  0, 1},

    };
    double C = getC(5, 3, in, &pa, &k);

    printf("迭代次数为 = %d\n", k);//输出迭代次数
    printf("最佳信源分布为:\n");
    for (int i = 0; i < 2; i++) {
        printf("%.3lf ", pa[i]); //输出信源的最佳分布，保留3位小数
    }
    printf("\n");
    printf("信道容量 = %.3lf bit\n", C); //输出信道容量，保留3位小数
    free(pa);
    return 0;
}
