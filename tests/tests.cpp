
#include <fstream>

#include "gtest/gtest.h"
#include "testutils.h"
#include "fftwutils.h"
#include "defines.h"
#include "utils.h"
#include "samplestonotes.h"

namespace SamplesToNotes
{

// =================================================================================================
// 
// =================================================================================================

class FftSpeedTest : public testing::TestWithParam<std::tuple<FftwReal, size_t>>
{
};

const std::vector<FftwReal> standardSampleRates{ 44100, 48000, 88200, 96000 };
const std::vector<size_t> standardFftWindowSizes{ 4096, 8192, 16384, 32768, 65536 };

INSTANTIATE_TEST_SUITE_P(SamplingRateAndFftWindowPermutationsSuite,
    FftSpeedTest,
    ::testing::Combine(
        ::testing::ValuesIn(standardSampleRates),
        ::testing::ValuesIn(standardFftWindowSizes)));

TEST_P(FftSpeedTest, FftAllSingleNotes)
{
    const bool verbose = false;
    Stopwatch stopwatch;
    FftwReal samplingRate = std::get<0>(GetParam());
    size_t fftWindowSize = std::get<1>(GetParam());
    Milliseconds windowDuration = static_cast<double>(fftWindowSize) / static_cast<double>(samplingRate) * 1000.0;
    SamplesToNotes s2n(fftWindowSize, samplingRate);
    const NoteFftInfo& noteInfo = s2n.GetNoteFftInfo();

    COUT_GTEST << "FFT SIZE: " << fftWindowSize << " - SAMPLING RATE : " << samplingRate << " Hz - WINDOW DURATION : " << windowDuration << " ms" ENDL;
    size_t successfulDetections = 0;
    std::vector<size_t> missedDetections;
    for (size_t note = 0; note < noteInfo.NoteLimit; ++note)
    {
        // Run FFT
        FftwReal noteFrequency = noteInfo.GetNoteFrequencies()[note];
        GenerateSinWave(s2n.GetInputBuffer(), fftWindowSize, noteFrequency, samplingRate);
        stopwatch.Start();
        std::vector<NoteValue> result = s2n.GetNotesFromSamples(3);
        stopwatch.Lap();
        if (result.size() > 0 && result[0].note == note)
            ++successfulDetections;
        else
            missedDetections.push_back(note);
    }
    COUT_GTEST << "AVG SAMPLESTONOTES EXECUTION: " << stopwatch.GetLapsAverage() << " ms - SUCCESSFUL DETECTION: " << static_cast<double>(successfulDetections) / static_cast<double>(noteInfo.NoteLimit) * 100.0 << " %" << ENDL;
    if (missedDetections.size() > 0)
    {
        COUT_GTEST << "Missed notes: ";
        for (const auto& note : missedDetections)
            COUT << note << " (" << NoteNames[note % 12] << (note / 12) << ")  ";
        COUT << ENDL;
    }
}


} // namespace SamplesToNotes