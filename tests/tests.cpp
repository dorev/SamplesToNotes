#include <iostream>

#define ANSI_TXT_BLK "\033[0;30m"
#define ANSI_TXT_RED "\033[0;31m"
#define ANSI_TXT_GRN "\033[0;32m"
#define ANSI_TXT_YLW "\033[0;33m"
#define ANSI_TXT_BLU "\033[0;34m"
#define ANSI_TXT_MGT "\033[0;35m"
#define ANSI_TXT_CYA "\033[0;36m"
#define ANSI_TXT_WHI "\033[0;37m"
#define ANSI_TXT_DFT "\033[0;0m"    // Console default
#define GTEST_BOX "[     cout ] "
#define COUT_GTEST std::cout << ANSI_TXT_GRN << GTEST_BOX << ANSI_TXT_DFT
#define COUT std::cout
#define ENDL ANSI_TXT_DFT << '\n'

#include <fstream>

#include "gtest/gtest.h"
#include "fftwutils.h"
#include "defines.h"
#include "utils.h"
#include "noteextractor.h"

namespace SamplesToNotes
{

class SamplesToNotesTests : public ::testing::Test
{
protected:

    SamplesToNotesTests()
    {
    }

    ~SamplesToNotesTests() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};


TEST_F(SamplesToNotesTests, DISABLED_Csv440)
{
    const int N = 16384;
    FftwReal samplingRate = 44100;
    FftwReal a4Frequency = 440;
    FftwReal in[N] = { 0 };
    std::ofstream file;

    // Input
    GenerateSinWave(in, N, a4Frequency, samplingRate);
    HammingWindow(in, N);
    file.open("tmp/input.csv", std::ios_base::out | std::ios_base::trunc);
    for (int i = 0; i < N; ++i)
        file << i * (1.0 / samplingRate) << ',' << in[i] << '\n';
    file.close();

    // Output
    FftwComplex out[N / 2 + 1] = { 0 };
    FftwPlan plan = FftwDftPlan(N, in, out, FFTW_ESTIMATE);
    FftwExecute(plan);
    //ScaleToDecibel(out, N/2 + 1);
    FftwReal binMidFreqs[N] = { 0 };
    FftwReal binHzRange = samplingRate / N;
    for (int i = 0; i < N; ++i)
        binMidFreqs[i] = (i * binHzRange) + (binHzRange / 2);
    file.open("tmp/output.csv", std::ios_base::out | std::ios_base::trunc);
    for (int i = 0; i < (N / 2 + 1); ++i)
        file << binMidFreqs[i] << ',' << out[i][0] << '\n';
    file.close();

    // Print top 4 values
    for (int i = 0; i < 4; ++i)
    {
        FftwReal maxValue = 0;
        int maxBin = 0;
        for (int bin = 0; bin < (N / 2 + 1); ++bin)
        {
            if (out[bin][0] > maxValue)
            {
                maxValue = out[bin][0];
                maxBin = bin;
            }
        }
        out[maxBin][0] = 0; // remove value for next pass
        FftwReal binMidFreq = (maxBin * binHzRange) + (binHzRange / 2);
        std::cout << (i + 1) << " -> Freq: " << binMidFreq << "Hz - Value: " << maxValue << "\n";
    }
    FftwDestroyPlan(plan);
}

const std::vector<FftwReal> standardSampleRates{ 44100, 48000, 88200, 96000 };
const std::vector<size_t> standardFftWindowSizes{ 1024, 4096, 8192, 16384, 32768, 65536 };

TEST_F(SamplesToNotesTests, FftAllSingleNotePrecision)
{
    for (const size_t fftWindowSize : standardFftWindowSizes)
    {
        for (const FftwReal samplingRate : standardSampleRates)
        {
            COUT_GTEST << "========================================================================================================================" << ENDL;
            COUT_GTEST << "FFT SIZE: " << fftWindowSize << "\tSAMPLING RATE : " << samplingRate << ENDL;
            COUT_GTEST << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << ENDL;

            AudioFft fft(fftWindowSize, samplingRate);
            NoteFftInfo noteInfo(fftWindowSize, samplingRate);
            size_t note = 0;

            for (const FftwReal noteFrequency : noteInfo.GetNoteFrequencies())
            {
                COUT_GTEST << "Note " << note << " (" << NoteNames[note % 12] << (note / 12) << ")" << "\tNote frequency: ";
                COUT << noteFrequency;

                // Run FFT
                FftwReal* inputBuffer = fft.GetInputBufferPointer();
                FftwComplex* outputBuffer = fft.GetOutputBufferPointer();
                GenerateSinWave(inputBuffer, fftWindowSize, noteFrequency, samplingRate);
                fft.Execute();
                FftwReal fftFrequency = fft.GetTopBin().frequency;
                COUT << "\tFFT frequency: " << std::setprecision(5) << fftFrequency;

                // Get difference between the desired note frequency and the frequency detected by FFT
                FftwReal difference = DifferencePercentage(noteFrequency, fftFrequency);
                COUT << "\tDifference: " << std::setprecision(5) << difference << "%";

                // FFT frequency between the note and the next (except for last note)
                FftwReal deltaSign = fftFrequency < noteFrequency ? FftwReal(-1) : FftwReal(1);
                FftwReal semitonePrecision(0);
                if (deltaSign > 0 && note < (NoteFftInfo::NoteLimit - 1))
                {
                    FftwReal nextNoteFrequency = noteInfo.GetNoteFrequencies()[note + 1];
                    semitonePrecision = (nextNoteFrequency - fftFrequency) / (nextNoteFrequency - noteFrequency) * FftwReal(100);
                }
                // FFT frequency between the note and the previous  (except for first note)
                else if (deltaSign <= 0 && note > 0)
                {
                    FftwReal previousNoteFrequency = noteInfo.GetNoteFrequencies()[note - 1];
                    semitonePrecision = (fftFrequency - previousNoteFrequency) / (noteFrequency - previousNoteFrequency) * FftwReal(100);
                }
                else
                {
                    COUT << "\tUnable to calculate semitone precision" << ENDL;
                    ++note;
                    continue;
                }

                auto precisionColor = ANSI_TXT_DFT;
                if (semitonePrecision > 99)
                {
                    precisionColor = ANSI_TXT_CYA;
                }
                else if (semitonePrecision > 90)
                {
                    precisionColor = ANSI_TXT_GRN;
                }
                else if (semitonePrecision > 80)
                {
                    precisionColor = ANSI_TXT_YLW;
                }
                else
                {
                    precisionColor = ANSI_TXT_RED;
                }

                COUT << precisionColor << "  Semitone precision: " << std::setprecision(5) << semitonePrecision << "%" << ENDL;
                ++note;
            }
            COUT_GTEST << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << ENDL;
            COUT_GTEST << "FFT SIZE: " << fftWindowSize << "\tSAMPLING RATE : " << samplingRate << ENDL;
            COUT_GTEST << "========================================================================================================================" << ENDL;
        }
    }
}

TEST_F(SamplesToNotesTests, FftMultipleNote)
{
}




} // namespace MyCode