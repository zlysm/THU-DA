//
// Created by zlysm on 12/4/22.
//

//reference: https://blog.csdn.net/m0_66201040/article/details/123913349

#include <cstdio>
#include <cmath>

#define ITER_TIMES 1200

void Hessenberg(double **matrix, int dim);

void QR(double **matrix, double **Q, int dim);

void bubbleSort(double *array, int dim);

int main() {
    int dim;
    scanf("%d", &dim);
    auto **matrix = new double *[dim];
    auto **Q = new double *[dim];
    auto **prod = new double *[dim];
    auto *eigenValues = new double[dim];

    for (int i = 0; i < dim; ++i) {
        Q[i] = new double[dim];
        prod[i] = new double[dim];
        matrix[i] = new double[dim];
        for (int j = 0; j < dim; ++j) {
            Q[i][j] = j == i ? 1 : 0;
            prod[i][j] = 0;
            scanf("%lf", &matrix[i][j]);
        }
    }

    Hessenberg(matrix, dim);
    int times = ITER_TIMES;
    while (times--) {
        QR(matrix, Q, dim);
        for (int i = 0; i < dim; ++i)
            for (int j = 0; j < dim; ++j) {
                prod[i][j] = 0;
                for (int k = 0; k < dim; ++k)
                    prod[i][j] += matrix[i][k] * Q[k][j];
            }
        for (int i = 0; i < dim; ++i)
            for (int j = 0; j < dim; ++j) {
                matrix[i][j] = prod[i][j];
                Q[i][j] = j == i ? 1 : 0;
            }
    }

    for (int i = 0; i < dim; ++i)
        eigenValues[i] = matrix[i][i];
    bubbleSort(eigenValues, dim);
    for (int i = 0; i < dim; ++i)
        printf("%.2lf\n", eigenValues[i]);

    for (int i = 0; i < dim; ++i) {
        delete[] matrix[i];
        delete[] Q[i];
        delete[] prod[i];
    }
    delete[] matrix;
    delete[] Q;
    delete[] prod;
    delete[] eigenValues;
    return 0;
}

void Hessenberg(double **matrix, int dim) {
    for (int iter = 0; iter < dim - 2; ++iter) {
        //householder
        int length = dim - iter - 1;
        double norm = 0;
        auto *u = new double[length];  //length of u from dim - k - 1 to 1
        for (int i = 0; i < length; ++i)
            norm += matrix[iter + i + 1][iter] * matrix[iter + i + 1][iter];
        norm = sqrt(norm);

        u[0] = matrix[iter + 1][iter] + (matrix[iter + 1][iter] > 0 ? norm : -norm);
        for (int i = 1; i < length; ++i)
            u[i] = matrix[iter + i + 1][iter];

        norm = 0;
        for (int i = 0; i < length; ++i)
            norm += u[i] * u[i];
        norm = sqrt(norm);  //norm of u

        for (int i = 0; i < length; ++i)
            u[i] /= norm;

        //matrix multiply
        for (int i = 0; i < dim; ++i) {
            double sum = 0;
            for (int j = 0; j < length; ++j)  //u * matrix
                sum += u[j] * matrix[iter + j + 1][i];
            for (int j = 0; j < length; ++j)  //matrix - 2 * u * matrix
                matrix[iter + j + 1][i] -= 2 * sum * u[j];
        }

        for (int i = 0; i < dim; ++i) {
            double sum = 0;  //matrix * u
            for (int j = 0; j < length; ++j)
                sum += u[j] * matrix[i][iter + j + 1];
            for (int j = 0; j < length; ++j)  //matrix - 2 * matrix * u
                matrix[i][iter + j + 1] -= 2 * sum * u[j];
        }

        delete[] u;
    }
}

void QR(double **matrix, double **Q, int dim) {
    for (int i = 0; i < dim - 1; ++i) {
        double r = sqrt(matrix[i][i] * matrix[i][i] + matrix[i + 1][i] * matrix[i + 1][i]);
        double cos = matrix[i][i] / r;
        double sin = matrix[i + 1][i] / r;

        for (int j = 0; j < dim; ++j) {  //matrix = Q^T * matrix
            double temp = matrix[i][j];
            matrix[i][j] = cos * temp + sin * matrix[i + 1][j];
            matrix[i + 1][j] = -sin * temp + cos * matrix[i + 1][j];
        }

        if (i == 0) {  //init
            Q[i][i] = cos;
            Q[i][i + 1] = -sin;
            Q[i + 1][i] = sin;
            Q[i + 1][i + 1] = cos;
        } else  //update Q
            for (int j = 0; j < dim; ++j) {  //Q_i = G_i * Q_i-1
                double temp = Q[j][i];
                Q[j][i] = cos * temp + sin * Q[j][i + 1];
                Q[j][i + 1] = -sin * temp + cos * Q[j][i + 1];
            }
    }
}

void bubbleSort(double *array, int dim) {
    for (int i = 0; i < dim; ++i)
        for (int j = dim - 1; j > i; --j)
            if (array[j] < array[j - 1]) {
                double temp = array[j];
                array[j] = array[j - 1];
                array[j - 1] = temp;
            }
}