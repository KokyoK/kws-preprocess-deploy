#include <stdio.h>
#include <iostream>
#include "transform.cpp"
#include "tb_transform.h"
// #include "tc_resnet8.h"
#include <string>
#include <math.h>

// #include "paras.h"

using namespace std;

WAVHEADER FileHeader;
int FileSet = 0;    // fseek
int FileEnd = 0;    //
int FileLength = 0; //
short InputData[W]; //

// int get_audio_length(char *filename)
// {
//     FILE *Ifp;
//     Ifp = fopen(filename, "rb");
//     fseek(Ifp, 0, SEEK_END);
//     FileEnd = ftell(Ifp);
//     // printf("total file len: %d bytes \n", FileEnd);
//     rewind(Ifp);
//     FileLength = FileEnd / 2;
//     return FileLength - 22;
// }
// int get_audio_rate(char *filename)
// {
//     FILE *Ifp;
//     Ifp = fopen(filename, "rb");
//     fread(&FileHeader, 1, sizeof(WAVHEADER), Ifp);
//     return FileHeader.SampleRate;
// }

// void showWavHead(WAVHEADER Header)
// {
//     printf("ChunkID: %c%c%c%c\n", Header.ChunkID[0], Header.ChunkID[1], Header.ChunkID[2], Header.ChunkID[3]);
//     printf("ChunkSize: %u\n", Header.ChunkSize);
//     printf("Format: %c%c%c%c\n", Header.Format[0], Header.Format[1], Header.Format[2], Header.Format[3]);
//     printf("FmtChunkID: %c%c%c%c\n", Header.FmtChunkID[0], Header.FmtChunkID[1], Header.FmtChunkID[2], Header.FmtChunkID[3]);
//     printf("FmtChunkSize: %u\n", Header.FmtChunkSize);
//     printf("AudioFormat: %d\n", Header.AudioFormat);
//     printf("NumChannels: %d\n", Header.NumChannels);
//     printf("SampleRate: %u\n", Header.SampleRate);
//     printf("ByteRate: %u\n", Header.ByteRate);
//     printf("BlockAlign: %d\n", Header.BlockAlign);
//     printf("BitsPerSample: %d\n", Header.BitsPerSample);
//     printf("DataChunkID: %c%c%c%c\n", Header.DataChunkID[0], Header.DataChunkID[1], Header.DataChunkID[2], Header.DataChunkID[3]);
//     printf("DataChunkSize: %u\n", Header.DataChunkSize);
// }
// step 1: read audio
void read_audio(float *wav)
{
    FILE *Ifp, *txt;

    Ifp = fopen("sys.wav", "rb");

    fread(&FileHeader, 1, sizeof(WAVHEADER), Ifp);
    // showWavHead(FileHeader)

    FileLength = file_len;
    // int file_len = FileLength;
    int j = 0;
    int16_t wavform[file_len];
    while (FileLength >= W)
    {
        fread(InputData, sizeof(short), W, Ifp);
        // printf("init: %d\n", *(wavform + j));
        for (int i = 0; i < W; i++)
        {

            wavform[j] = InputData[i];
            // printf("for: %d\n", *(wavform + j));
            j += 1;
            // printf("i = %d\n", i);
        }
        FileLength -= W;
    }

    fread(InputData, sizeof(short), FileLength, Ifp);
    for (int i = 0; i < FileLength; i++)
    {
        wavform[j] = InputData[i];
        j += 1;
    }

    for (int i = 0; i < file_len; i++)
    {
        wav[i] = (float)((float)wavform[i]) / pow(2, 15);
        // cout << wav[i] << endl;
    }
    // return wav;
}

int main()
{
    printf("================================  program start ============================ \n");
    char *filename = (char *)"sys.wav";
    // int file_len = get_audio_length(filename);
    // int sample_rate = get_audio_rate(filename);

    float wavform[file_len];
    read_audio(wavform);
    
    float feature[N_MELS*MEL_SPEC_LEN];

    trans_audio(wavform, feature);
    for (int i = 0; i < N_MELS; i++) // 矩阵乘法
    {
        for (int k = 0; k < MEL_SPEC_LEN; k++)
        {
            cout << i << " " << k << "  " << feature[i][k] << endl;
        }
    }
    // ADT feat[40][101];
    // for (int i = 0; i < N_MELS; i++)
    // {
    //     for (int j = 0;j <MEL_SPEC_LEN; j++)
    //     {
    //         feat[i][j]= (ADT)feature[i][j];
    //         // cout <<"finish feat extract." <<i << " " << j << "  " << feat[i][j] << endl;
    //     }
    // }
    // ADT prob[NUM_CLASS];
    // tcresnet_flow(feat,prob);
    
    // ADT prob[NUM_CLASS];
    // tc_resnet_full(wavform, prob);
	// for (int i = 0; i < 10; i++){

	// 	cout << i << "  " << prob[i] << endl;
	// }
    // CONV0_DWs

    printf("================================  program end ============================== \n");
    return 0;
}
