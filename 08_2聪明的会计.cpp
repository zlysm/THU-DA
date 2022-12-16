//
// Created by zlysm on 12/16/22.
//

//Reference: https://zhuanlan.zhihu.com/p/135259438
//Be aware of the precision of double!!!!!

#include <cstdio>
#include <ccomplex>

//rotation factor
#define W(N, k, n) (std::exp(std::complex<double>(0, -k * 2 * M_PI * n / N)))

typedef enum {
    FFT_FORWARD = 1,
    FFT_BACKWARD = -1
} FFT_DIRECTION;

void binReverse(std::complex<double> *X, int N);

void FFT(std::complex<double> *X, int N, FFT_DIRECTION dir);

int nearestPowerOfTwo(int n) {
    n -= 1;
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    n |= (n >> 16);
    return n + 1;
}

int main() {
    int N, M;
    scanf("%d %d", &N, &M);
    const int size = nearestPowerOfTwo(N);
    std::complex<double> Y[size];
    std::complex<double> H[size];
    std::complex<double> X[size];

    int index = 0;
    for (int i = 0; i < N + M - 1; ++i) {
        double temp;
        scanf("%lf", &temp);
        if (i < size)
            Y[i] = std::complex<double>(temp, 0);
        else {
            double oldReal = Y[index].real();
            Y[index++] = std::complex<double>(oldReal + temp, 0);
        }
    }

    for (int i = 0; i < M; ++i) {
        double temp;
        scanf("%lf", &temp);
        H[i] = std::complex<double>(temp, 0);
    }

    FFT(Y, size, FFT_FORWARD);
    FFT(H, size, FFT_FORWARD);

    for (int i = 0; i < size; ++i)
        X[i] = Y[i] / (H[i] * std::complex<double>(size, 0));

    FFT(X, size, FFT_BACKWARD);

    bool legal = true;
    for (int i = 0; i < N; ++i) {
        double temp = X[i].real() * 10000 + 1e-6;
        temp -= (int) temp;
        temp = temp * 10;
        if (fabs(temp) > 1) {
            legal = false;
            break;
        }
    }

    if (legal)
        for (int i = 0; i < N; ++i)
            printf("%.4lf ", X[i].real());
    else
        printf("%d", N);

    return 0;
}

void binReverse(std::complex<double> *X, int N) {
    for (int i = 0; i < N; ++i) {
        int target = 0;
        double halfSize = log2(N) / 2 + 0.5;
        for (int j = 0; j < (int) halfSize; ++j) {
            int m = 1;
            int n = N / 2;
            m <<= j;
            n >>= j;
            if (i & n) target |= m;
            if (i & m) target |= n;
        }

        if (i < target) {
            std::complex<double> temp = X[target];
            X[target] = X[i];
            X[i] = temp;
        }
    }
}

void FFT(std::complex<double> *X, int N, FFT_DIRECTION dir) {
    binReverse(X, N);
    int power = (int) log2(N);
    for (int i = 1; i <= power; ++i) {
        int gap = 1 << (i - 1);
        for (int j = 0; j < gap; ++j) {
            int step = 1 << (power - i);
            int p = j * step;
            for (int k = 0; k < step; ++k) {  //蝶形运算
                int index = j + k * gap * 2;
                std::complex<double> temp = X[index];
                X[index] += X[index + gap] * W(N, p, dir);
                X[index + gap] = temp - X[index + gap] * W(N, p, dir);
            }
        }
    }
}