#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_
#include <stdint.h>
#include <string>

// #include "hls_math.h"

using namespace std;
// file information
#define file_len 108480
#define sample_rate 48000
#define NEW_WAV_LEN 16000 // wav len after padding 0 at the end

// step 2 resample
#define SR 48000
#define NEW_SR 8000
#define SR_REP 6
#define NEW_SR_REP 1

#define LOW_PASS_FILTER_WIDTH 6
#define ROLLOFF 0.99
#define BASE_FREQ NEW_SR_REP *ROLLOFF
#define WIDTH 36 // LOW_PASS_FILTER_WIDTH * SR_REP / base_freq
#define IDX_LEN WIDTH * 2 + SR_REP
#define PAD_WAV_LEN IDX_LEN + file_len
#define W 128 //

// PARAMS for framing
#define WIN_LEN 480
#define HOP_LEN 160
#define FFT_WIN_LEN 512 // need to
#define SPEC_LEN 241
#define FRAME_COUNT 101 // frame_count

// PARAS FOR MEL SCALE
// #define SQUEEZED_SR 16000
#define N_MELS 40
#define MEL_SPEC_LEN 101 // frame_count
#define F_MIN 0
#define F_MAX 8000


// typedef ap_fixed<8,8> 


void trans_audio(float wav[file_len], float feature[N_MELS][MEL_SPEC_LEN]);

// int get_audio_length(char *filename);
// int get_audio_rate(char *filename);
int cal_gcd(int a, int b);
void hann_win(float window[WIN_LEN], int len);
float hz2mel(float freq);

void resample(float wav[file_len], float wav2_resampledp[NEW_WAV_LEN]);
void spectrogram(float wav[NEW_WAV_LEN], float wav3_in[FRAME_COUNT][SPEC_LEN]);
void mel_spectrogram(float spec[N_MELS][SPEC_LEN], float mel_spec[N_MELS][MEL_SPEC_LEN]);
void amplitude2db(float mel_spec[N_MELS][MEL_SPEC_LEN], float feature[N_MELS][MEL_SPEC_LEN]);

typedef struct tagWAVHEADER
{
    uint8_t ChunkID[4];
    uint32_t ChunkSize;
    uint8_t Format[4];
    uint8_t FmtChunkID[4];
    uint32_t FmtChunkSize;
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
    uint8_t DataChunkID[4];
    uint32_t DataChunkSize;
} WAVHEADER;

#endif // #ifndef _EXTRACTDATA_H_
