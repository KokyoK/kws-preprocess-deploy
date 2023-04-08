

#include "transform.h"
#include <stdio.h>
#include <math.h>
#include "fft.h"
#include <iostream>

using namespace std;

// int FileSet = 0;    // fseek
// int FileEnd = 0;    //
// int FileLength = 0; //
// short InputData[W]; //

// WAVHEADER FileHeader; //
void trans_audio(float wav1[file_len], float feature[N_MELS][MEL_SPEC_LEN])
{

    printf("file len: %d\n", file_len);
    printf("sample rate: %d\n", sample_rate);

    // ------------------------ step 1: read audio ---------------------------

    int flags[3] = {0, 0, 0};
    /*
        for (int i = 0; i < 128; i++)
        {
            if (wavform[i] != gt_first128[i])
            {
                printf("test 1 fail! \n");
                flags[0] = 1;
                break;
            }

            if (wavform[i + 10000] != gt_middle128[i])
            {
                printf("test 2 fail! \n");
                flags[1] = 1;
                break;
            }

            // printf("wavform[file_len-1-i] = %d  %d\n",wavform[file_len-1-i],gt_last128[127-i]);
            if (wavform[file_len - 1 - i] != gt_last128[127 - i])
            {
                // if(wavform[i+108416] !=  gt_last128[i]){
                printf("test 3 fail! \n");
                flags[2] = 1;
                // break;
                // break;
            }

            // printf("main for: %d\n", *(r_wavform + i));
        }

        for (int i = 0; i < 3; i++)
        {
            if (flags[i] == 0){
                printf("test %d pass! \n",i+1);
            }
        }
    */
    // printf("test pass! \n");

    // -------------------------step 2: resample ------------------------------
    // int NEW_WAV_LEN = (int)(file_len*new_sr/sample_rate);

    float wav_resampled[NEW_WAV_LEN];
    // int new_sr=8000;
    resample(wav1, wav_resampled);

    // -------------------------step 3:  padding, framing, window, FFT ------------------------------

    // int frame_count = (int)(NEW_WAV_LEN + HOP_LEN) / HOP_LEN;
    int fft_rep = (int)(WIN_LEN / 2 + 1);
    float wav_spec[FRAME_COUNT][SPEC_LEN];

    spectrogram(wav_resampled, wav_spec);
    // for (int i=0; i<1; i++){
    //     for (int j=0; j<fft_rep; j++){
    //         cout << i << ", " << j << "  "<< wav_spec[i][j] << endl;
    //     }
    // }

    // ------------------------- step 4:  Mel scale  ------------------------------
    float mel_spec[N_MELS][MEL_SPEC_LEN];
    mel_spectrogram(wav_spec, mel_spec);
    // for (int i = 0; i < N_MELS; i++) //矩阵乘法
    // {
    //     for (int k = 0; k < MEL_SPEC_LEN; k++)
    //     {
    //         // cout << i << " " << k << "  " << mel_spec[i][k] << endl;
    //     }
    // }

    // ------------------------- step 5:  Amplitude 2 DB  ------------------------------
    // float feature[N_MELS][MEL_SPEC_LEN];
    amplitude2db(mel_spec, feature);
    // for (int i = 0; i < N_MELS; i++) //矩阵乘法
    // {
    //     for (int k = 0; k < MEL_SPEC_LEN; k++)
    //     {
    //         cout << i << " " << k << "  " << feature[i][k] << endl;
    //     }
    // }
}

int cal_gcd(int a, int b)
{
    int temp, i;
    if (a < b)
    {
        temp = a;
        a = b;
        b = temp;
    }
    for (i = b; i > 0; i--)
        if (a % i == 0 && b % i == 0)
        {
            return i;
        }

    return 1;
}

void hann_win(float window[WIN_LEN])
{
    for (int i = 0; i < WIN_LEN; i++)
    {
        window[i] = (float)(0.5 - 0.5 * cos((float)((2 * M_PI * i) / (WIN_LEN - 1))));
        // cout <<"win " <<i<< "  "<< window[i] <<endl;
        //  cout << window[i] <<endl;
        // printf("%.16f,\n", window[i]);
    }
}

float hz2mel(float freq)
{
    return (float)2595.0 * log10(1.0 + (float)(freq / 700.0));
}
float mel2hz(float mel)
{
    return (float)700 * (pow(10, (float)mel / 2595.0) - 1.0);
}

//-----------------------------------------------------------------------------------

// step 2: resample
void resample(float wav[file_len], float wav2_resampled[NEW_WAV_LEN])
{

    // int gcd = cal_gcd(SR_REP, NEW_SR_REP);
    // SR_REP = (int)SR_REP / gcd;
    // NEW_SR_REP = (int)NEW_SR_REP / gcd;
    //  float base_freq = min(SR_REP, NEW_SR_REP);
    // base_freq *= ROLLOFF;
    //  int width = ceil(LOW_PASS_FILTER_WIDTH * SR_REP / base_freq);
    // int idx_len = width * 2 + SR_REP;
    int idx[IDX_LEN];
    double kernels[NEW_SR_REP][IDX_LEN];
    for (int j = 0; j < IDX_LEN; j++)
    {
        idx[j] = j - WIDTH;
    }

    // printf("%d\n",IDX_LEN);
    float scale = BASE_FREQ / SR_REP;
    for (int i = 0; i < NEW_SR_REP; i++)
    {
        double t[IDX_LEN], window[IDX_LEN];
        for (int j = 0; j < IDX_LEN; j++)
        {
            t[j] = (double)((double)-i / NEW_SR_REP + (double)idx[j] / SR_REP) * BASE_FREQ;
            if (t[j] < -LOW_PASS_FILTER_WIDTH)
            {
                t[j] = -LOW_PASS_FILTER_WIDTH;
            }
            else if (t[j] > LOW_PASS_FILTER_WIDTH)
            {
                t[j] = LOW_PASS_FILTER_WIDTH;
            }
            window[j] = pow(cos(t[j] * M_PI / LOW_PASS_FILTER_WIDTH / 2), 2);
            t[j] *= M_PI;
            // printf("%8f\n",window[j]);
            if (t[j] == 0)
            {
                t[j] = 1;
            }
            else
            {
                t[j] = sin(t[j]) / t[j];
            }
            t[j] *= window[j];
            kernels[i][j] = t[j] * scale;

            // printf("%.18lf\n",kernels[i][j]);
        }
    }
    // --------------------end compute kernel, WIDTH ----------------------------------
    // padding 0
    // int pad_wav_len = IDX_LEN + file_len;
    float pad_wav[PAD_WAV_LEN];
    for (int i = 0; i < PAD_WAV_LEN; i++)
    {
        if (i < WIDTH)
        {
            pad_wav[i] = 0;
        }
        else if (i >= file_len + WIDTH)
        {
            pad_wav[i] = 0;
        }
        else
        {
            pad_wav[i] = wav[i - WIDTH];
        }
    }
    // conv1d kernels kernels. stride = SR_REP
    // int resampled_len = (PAD_WAV_LEN - (IDX_LEN - SR_REP)) / SR_REP;
    // float resampled[resampled_len];
    float resampled[NEW_WAV_LEN];
    for (int i = 0; i < NEW_SR_REP; i++)
    { // for each kernel
        for (int j = 0; j < NEW_WAV_LEN; j++)
        { // for each target value
            float cur_val = 0.0;
            float t,w;
            for (int k = 0; k < IDX_LEN; k++)
            {
                w = (pad_wav[j * SR_REP + k]);
                t =  kernels[i][k] * w;
                cur_val += t;
            }
            resampled[j] = cur_val;
            // cout << resampled[j] << endl;
        }
    }
    // clip the audio before target len;
    for (int i = 0; i < NEW_WAV_LEN; i++)
    {
        wav2_resampled[i] = resampled[i];
        // cout << wav2_resampled << endl;

        // cout <<i<<"    "<< wav2_resampled[i] << endl;
    }
}

// step 3: padding, framing, window, FFT
void spectrogram(float wav[NEW_WAV_LEN], float wav3_in[FRAME_COUNT][SPEC_LEN])
{
    int frame_count = (int)(NEW_WAV_LEN + HOP_LEN) / HOP_LEN;
    int fft_rep = (int)(WIN_LEN / 2 + 1);
    float pad_wav[NEW_WAV_LEN + WIN_LEN];
    // reflect padding
    for (int i = 0; i < (WIN_LEN + NEW_WAV_LEN); i++)
    {
        if (i < (WIN_LEN / 2))
        {
            pad_wav[i] = wav[(WIN_LEN / 2) - i];
        }
        else if (i >= NEW_WAV_LEN + (WIN_LEN / 2))
        {
            pad_wav[i] = wav[NEW_WAV_LEN * 2 + (WIN_LEN / 2) - i - 2];
        }
        else
        {
            pad_wav[i] = wav[i - (WIN_LEN / 2)];
        }
        //    cout << i <<" " <<  pad_wav[i] << endl;
    }

    // framing
    // int fft_len = WIN_LEN / 2 + 1;
    // float spec[frame_count][SPEC_LEN];
    for (int i = 0; i < FRAME_COUNT; i++)
    // for (int i = 0; i < 1; i++)
    {
        float audio_frame[WIN_LEN];
        float window[WIN_LEN];

        hann_win(window);
        for (int j = 0; j < WIN_LEN; j++)
        {
            audio_frame[j] = pad_wav[i * HOP_LEN + j];

            audio_frame[j] = audio_frame[j] * window[j];
            //  cout << j <<" " << audio_frame[j] << endl;
        }

        // int new_win_size = 1;
        // while (WIN_LEN > new_win_size)
        // {
        //     new_win_size *= 2;
        // }

        float x[FFT_WIN_LEN * 2];

        for (int j = 0; j < 2 * FFT_WIN_LEN; j++)
        {
            if (j < WIN_LEN)
            {
                x[j] = audio_frame[j];
            }
            else
            {
                x[j] = 0;
            }
            //  cout << j <<" " << x[j] << endl;
        }

        // need to discard padding

        float out[2 * FFT_WIN_LEN];
        float pow_out[SPEC_LEN];
        FFT(x, out);

        // stft.abs.pow(2)
        for (int j = 0; j < SPEC_LEN; j++)
        {
            // pow_out[j] = sqrt(out[2 * j] * out[2 * j] + out[2 * j + 1] * out[2 * j + 1]);
            // cout << j <<" " << pow_out[j] << endl;
            // pow_out[j] = pow(pow_out[j], 2);
            pow_out[j] = out[2 * j] * out[2 * j] + out[2 * j + 1] * out[2 * j + 1];
            // cout << j <<" " << pow_out[j] << endl;
            wav3_in[i][j] = pow_out[j];
        }
    }
}

// step 4: apply mel scale
void mel_spectrogram(float spec[N_MELS][SPEC_LEN], float mel_spec[N_MELS][MEL_SPEC_LEN])
{
    float all_freqs[SPEC_LEN];
    float step = (float)2 * NEW_SR / 2 / (SPEC_LEN - 1); // fake sr = real sr/2, squeeze 2 s into 1s
    for (int i = 0; i < SPEC_LEN; i++)
    {
        all_freqs[i] = i * step;
        //  cout << "all_frqs "<< i << "  " <<all_freqs[i] << endl;
    }
    // hz to mel
    float m_min = hz2mel(F_MIN);
    float m_max = hz2mel(F_MAX);

    float m_pts[N_MELS + 2];
    float f_pts[N_MELS + 2];
    step = (float)(m_max - m_min) / (N_MELS + 1);
    for (int i = 0; i < N_MELS + 2; i++)
    {
        m_pts[i] = i * step + m_min;
        f_pts[i] = mel2hz(m_pts[i]);
        //    cout << "f_pts "<< i << "  " <<f_pts[i] << endl;
    }
    float f_diff[N_MELS + 1];
    for (int i = 0; i < N_MELS + 1; i++)
    {
        f_diff[i] = f_pts[i + 1] - f_pts[i];
        // cout << "f_diff "<< i << "  "<< f_diff[i] << endl;
    }

    float slopes[SPEC_LEN][N_MELS + 2];

    for (int i = 0; i < SPEC_LEN; i++)
    {
        for (int j = 0; j < N_MELS + 2; j++)
        {
            slopes[i][j] = f_pts[j] - all_freqs[i];
            // cout <<"i: "<< i <<"  j: "<<j <<"= " << slopes[i][j] << endl;
        }
    }
    float down_slopes[SPEC_LEN][N_MELS];
    float up_slopes[SPEC_LEN][N_MELS];
    float fb[SPEC_LEN][N_MELS];
    for (int i = 0; i < SPEC_LEN; i++)
    {
        for (int j = 0; j < N_MELS; j++)
        {
            down_slopes[i][j] = (float)-1 * slopes[i][j] / f_diff[j];
            up_slopes[i][j] = (float)slopes[i][j + 2] / f_diff[j + 1];
            fb[i][j] = max((float)0.0, min(down_slopes[i][j], up_slopes[i][j]));
            // cout <<"i: "<< i <<"  j: "<<j <<" = " << fb[i][j] << endl;
        }
    }

    // transpose
    // float spec_T[MEL_SPEC_LEN][SPEC_LEN]; // 241 101
    // for (int i = 0; i < SPEC_LEN; i++)
    // {
    //     for (int j = 0; j < MEL_SPEC_LEN; j++)
    //     {
    //         spec_T[i][j] = spec[j][i];
    //     }
    // }

    // spec_T * fb ([101,241] * [241, 40])
    float mel_spec_T[MEL_SPEC_LEN][N_MELS];
    for (int i = 0; i < MEL_SPEC_LEN; i++) // 矩阵乘法
    {
        for (int k = 0; k < N_MELS; k++)
        {
            mel_spec_T[i][k] = 0;
            for (int j = 0; j < SPEC_LEN; j++)
            {
                mel_spec_T[i][k] += spec[i][j] * fb[j][k];
            }
        }
    }
    // transpose
    for (int i = 0; i < MEL_SPEC_LEN; i++)
    {
        for (int k = 0; k < N_MELS; k++)
        {
            mel_spec[k][i] = mel_spec_T[i][k];
            // cout << i << " " << k << "  " << mel_spec[i][k] << endl;
        }
    }
}

// step 5: amplitude 2 DB
void amplitude2db(float mel_spec[N_MELS][MEL_SPEC_LEN], float feature[N_MELS][MEL_SPEC_LEN])
{
    for (int i = 0; i < N_MELS; i++) // 矩阵乘法
    {
        for (int j = 0; j < MEL_SPEC_LEN; j++)
        {
            if (mel_spec[i][j] < 1e-10)
            {
                mel_spec[i][j] = 1e-10;
            }
            else if (mel_spec[i][j] > 80)
            {
                mel_spec[i][j] = 80;
            }
            mel_spec[i][j] = (float)log10(mel_spec[i][j]) * 10.0;
            feature[i][j] = mel_spec[i][j];
            // cout << i << "," << j << "  " << mel_spec[i][j] << endl;
        }
    }
}


