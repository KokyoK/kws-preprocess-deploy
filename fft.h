
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;
#include <fftw3.h>
#define N 480
#define FFT_WIN_LEN 512 // need to
#define SPEC_LEN 241
// const float PI = 3.1416;

inline void swap(float &a, float &b)
{
    float t;
    t = a;
    a = b;
    b = t;
}

void bitrp(float xreal[N], float ximag[N])
{
    // 位反转置换 Bit-reversal Permutation
    int i, j, a, b, p;

    for (i = 1, p = 0; i < N; i *= 2)
    {
        p++;
    }
    for (i = 0; i < N; i++)
    {
        a = i;
        b = 0;
        for (j = 0; j < p; j++)
        {
            b = (b << 1) + (a & 1); // b = b * 2 + a % 2;
            a >>= 1;                // a = a / 2;
        }
        if (b > i)
        {
            swap(xreal[i], xreal[b]);
            swap(ximag[i], ximag[b]);
        }
    }
}

void FFT(float x[SPEC_LEN], float out[2 * FFT_WIN_LEN])
{
    float xreal[N];
    float ximag[N];

    for (int j = 0; j < 2 * N; j++)
    {
        if (j < N)
        {
            xreal[j] = x[j];
        }
        else
        {
            ximag[j - N] = x[j];
        }

        //    cout << "j:"<<j<<xreal[j]<<eNdl;
    }
    // 快速傅立叶变换，将复数 x 变换后仍保存在 x 中，xreal, ximag 分别是 x 的实部和虚部
    float wreal[N / 2], wimag[N / 2], treal, timag, ureal, uimag, arg;
    int m, k, j, t, index1, index2;

    bitrp(xreal, ximag);

    // 计算 1 的前 n / 2 个 n 次方根的共轭复数 W'j = wreal [j] + i * wimag [j] , j = 0, 1, ... , n / 2 - 1
    arg = -2 * M_PI / N;
    treal = cos(arg);
    timag = sin(arg);
    wreal[0] = 1.0;
    wimag[0] = 0.0;
    for (j = 1; j < N / 2; j++)
    {
        wreal[j] = wreal[j - 1] * treal - wimag[j - 1] * timag;
        wimag[j] = wreal[j - 1] * timag + wimag[j - 1] * treal;
    }

    for (m = 2; m <= N; m *= 2)
    {
        for (k = 0; k < N; k += m)
        {
            for (j = 0; j < m / 2; j++)
            {
                index1 = k + j;
                index2 = index1 + m / 2;
                t = N * j / m; // 旋转因子 w 的实部在 wreal [] 中的下标为 t
                treal = wreal[t] * xreal[index2] - wimag[t] * ximag[index2];
                timag = wreal[t] * ximag[index2] + wimag[t] * xreal[index2];
                ureal = xreal[index1];
                uimag = ximag[index1];
                xreal[index1] = ureal + treal;
                ximag[index1] = uimag + timag;
                xreal[index2] = ureal - treal;
                ximag[index2] = uimag - timag;
            }
        }
    }

    for (int i = 0; i < N / 2 + 1; i++)
    {
        out[2 * i] = xreal[i];
        out[2 * i + 1] = ximag[i];
        cout << "real " << i << "  " << xreal[i] << endl;
        cout << "imag " << i << "  " << ximag[i] << endl;
    }
}

void DFT(float x[SPEC_LEN], float out[2 * FFT_WIN_LEN])
{
    float xreal[N];
    float ximag[N];

    for (int i = 0; i < N/2+1; i++)
    {
        xreal[i] = 0;
        ximag[i] = 0;
        for (int n = 0; n < N; n++)
        {
            double angle = -2 * M_PI * i * n / N;
            xreal[i] += x[n] * cos(angle);
            ximag[i] += x[n] * sin(angle);
        }
    }

    for (int i = 0; i < N / 2 + 1; i++)
    {
        out[2 * i] = xreal[i];
        out[2 * i + 1] = ximag[i];
        // cout << "real " << i << "  " << xreal[i] << endl;
        // cout << "imag " << i << "  " << ximag[i] << endl;
    }
}
// #include <stdio.h>
// #include <stdlib.h>
// #include <fftw3.h>



void FFTW(float x[SPEC_LEN], float result[N+2])
{
    fftw_complex *in, *out;
    fftw_plan p;
    int i;

    // 分配输入输出数组内存
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    // 设置输入信号
    for (i = 0; i < N; i++) {
        in[i][0] = x[i];
        in[i][1] = 0;
    }

    // 创建 DFT 计算计划
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    // 执行 DFT 计算
    fftw_execute(p);

    // 打印结果
    for (i = 0; i < N; i++) {
        // printf("out[%d] = %f + %fi\n", i, out[i][0], out[i][1]);
        result[2*i] = out[i][0];
        result[2*i+1] = out[i][1];
        
    }

    // 释放内存
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    // return 0;
}

