#include <fstream>

#include "gtest/gtest.h"
#include "fftwutils.h"
#include "utils.h"

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


TEST_F(SamplesToNotesTests, Csv440)
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

} // namespace MyCode