#include <stdio.h>

#include <stdint.h>
#include <string>
using namespace std;
void read_audio(float *);

char* filename = (char*)"sys.wav";
int16_t gt_first128[128] = { -7,  -1,  -2,  -9,  -9, -12, -14, -12, -18, -19, -12,  -2,   7,   9,
         10,   9,  -2,  -3,  -4,   1,   2,   6,   4,   5,  11,  24,  26,  21,
         24,  22,  15,  11,  13,  18,  11,  11,   9,  12,   2,  -3,  -3,  -1,
          4,   1,  -3,  -4,  -8, -10,  -7,  -7,  -7,  -8,  -7,   0,  -1,  -4,
         -1,   7,   2,  -1,   3,   5,  -3,  -5,  -2,  -2,  -6,  -5,   6,  -2,
         -7,  -8,   4,   4,  -3,   2,   5,   3,   0,   0,   5,   3,  -6,  -7,
         -3,   2,  -4,  -6,   1,   5,   7,   3,   9,  15,  11,   0,  -3,  -3,
         -5,   0,   7,  13,   8,   4,   4,   4,   3,   1,   1,   4,   0,   0,
         -1,  -5,  -9, -16, -13, -13, -10,  -9,  -7,  -9, -19, -30, -26, -22,
        -27, -33};


int16_t gt_middle128[128] = {15,  23,  19,  11,  18,  14,   7,   2,   5,  10,   6,   5,   1,   0,
          6,   7,  12,  15,  10,   7,   5,  12,  14,  18,   8,   5,   2,   4,
         -3, -10, -13, -12,  -5,   1,   1,   3,   4,   7,   8,   7,   7,   9,
          8,   3,  -1,  -2,  -5,   2,   2,   7,   7,   2,   6,   4,   2,  -2,
          0,   8,  13,  13,  12,  17,  20,  32,  34,  29,  16,   7,   6,   8,
         -9, -16,  -3,  11,   0,  -9,   5,  21,  13,   0,   4,   3,  -1,   1,
         16,  21,   2, -12, -12, -11, -10,  -8,  -8,  -5, -10, -23, -35, -37,
        -37, -40, -32, -21, -16, -24, -29, -16, -16, -16, -19, -19, -18, -12,
        -12,  -8,  -5,  -6,   1,   7,   7,   8,   2,   4,  10,  15,  19,  22,
         30,  37};

int16_t gt_last128[128] = {-14, -17, -21, -23, -23, -19, -21, -18, -16, -12,  -9, -12, -15,  -8,
        -10,  -7,  -9,  -1,   4,   5,  10,  11,  15,  19,  17,  22,  28,  27,
         18,  15,   8,   2,   0,   6,   9,   3,   7,   3,  -1,  -4,  -6,  -6,
        -12,  -8, -12, -11,  -9, -11, -15, -12, -18, -16, -13, -15, -15, -12,
         -6, -10,  -3,   2,   0,   4,   4,   2,   0,   0,  -4,   0,  -1,   0,
          8,   6,  -2,  -7, -14, -13, -17, -22, -25, -24, -29, -28, -31, -33,
        -36, -36, -33, -23, -21, -26, -30, -26, -18, -14,  -8,   0,   8,  11,
         18,  28,  33,  26,  19,  15,  10,  14,  14,  19,  20,  18,  20,  18,
          9,  11,  11,   8,   9,  -1,  -5, -15, -27, -37, -45, -50, -60, -54,
        -51, -52};



