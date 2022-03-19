
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
// FFT PRECISION TEST
// =================================================================================================

class FftPrecisionTest : public testing::TestWithParam<std::tuple<FftwReal, size_t>>
{
};

const std::vector<FftwReal> standardSampleRates{ 44100, 48000, 88200, 96000 };
const std::vector<size_t> standardFftWindowSizes{ 4096, 8192, 16384, 32768, 65536 };

INSTANTIATE_TEST_SUITE_P(SamplingRateAndFftWindowPermutationsSuite,
    FftPrecisionTest,
    ::testing::Combine(
        ::testing::ValuesIn(standardSampleRates),
        ::testing::ValuesIn(standardFftWindowSizes)));

TEST_P(FftPrecisionTest, FftAllSingleNotePrecision)
{
    const bool verbose = false;
    FftwReal samplingRate = std::get<0>(GetParam());
    size_t fftWindowSize = std::get<1>(GetParam());
    Stopwatch stopwatch;

    Milliseconds windowDuration = static_cast<double>(fftWindowSize) / static_cast<double>(samplingRate) * 1000.0;
    if (verbose)
    {
        COUT_GTEST << "=================================================================================================" << ENDL;
        COUT_GTEST << "FFT SIZE: " << fftWindowSize << "\tSAMPLING RATE : " << samplingRate << " Hz\tWINDOW DURATION : " << windowDuration << " ms" ENDL;
        COUT_GTEST << "=================================================================================================" << ENDL;
    }
    else
    {
        COUT_GTEST <<  "FFT SIZE: " << fftWindowSize << "\tSAMPLING RATE : " << samplingRate << " Hz\tWINDOW DURATION : " << windowDuration << " ms" ENDL;
    }

    AudioFft fft(fftWindowSize, samplingRate);
    NoteFftInfo noteInfo(fft);
    size_t note = 0;

    struct Result
    {
        size_t note;
        FftwReal noteFrequency;
        FftwReal fftFrequency;
        FftwReal difference;
        FftwReal semitonePrecision;
        Milliseconds fftExecutionTime;
    };
    std::vector<Result> results;
    results.reserve(NoteFftInfo::NoteLimit);

    // Execute FFT for all notes with generated sin wave input
    for (const FftwReal noteFrequency : noteInfo.GetNoteFrequencies())
    {
        if (verbose)
        {
            COUT_GTEST << "Note " << note << " (" << NoteNames[note % 12] << (note / 12) << ")" << "\tNote frequency: ";
            COUT << noteFrequency;
        }

        // Run FFT
        FftwReal* inputBuffer = fft.GetInputBuffer();
        FftwComplex* outputBuffer = fft.GetOutputBuffer();
        GenerateSinWave(inputBuffer, fftWindowSize, noteFrequency, samplingRate);
        stopwatch.Start();
        fft.Execute();
        Milliseconds fftExecutionTime = stopwatch.Lap();
        FftwReal fftFrequency = fft.GetTopBin().frequency.center;
        if(verbose)
            COUT << "\tFFT frequency: " << std::setprecision(5) << fftFrequency;

        // Get difference between the desired note frequency and the frequency detected by FFT
        FftwReal difference = DifferencePercentage(noteFrequency, fftFrequency);
        if (verbose)
            COUT << "\tDifference: " << std::setprecision(5) << difference << "%";

        FftwReal deltaSign = fftFrequency < noteFrequency ? FftwReal(-1) : FftwReal(1);
        FftwReal semitonePrecision(0);
        // FFT frequency between the note and the next (except for last note)
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
            if (verbose)
                COUT << "\tUnable to calculate semitone precision" << ENDL;
            ++note;
            continue;
        }

        auto precisionColor = ANSI_TXT_DFT;
        if (semitonePrecision >= 99)
            precisionColor = ANSI_TXT_CYA;
        else if (semitonePrecision >= 90)
            precisionColor = ANSI_TXT_GRN;
        else if (semitonePrecision >= 80)
            precisionColor = ANSI_TXT_YLW;
        else
            precisionColor = ANSI_TXT_RED;
        if (verbose)
            COUT << precisionColor << "  Semitone precision: " << std::setprecision(5) << semitonePrecision << "%" << ENDL;

        // Store results
        results.push_back({ note, noteFrequency, fftFrequency, difference, semitonePrecision, fftExecutionTime });
        ++note;
    }

    size_t highestNoteDetected = results.size();
    size_t lowestNoteDetected = 0;
    for (size_t i = (NoteFftInfo::NoteLimit / 2); i < results.size(); ++i)
    {
        if (results[i].semitonePrecision < 80)
        {
            highestNoteDetected = i - 1;
            break;
        }
    }
    for (size_t i = (NoteFftInfo::NoteLimit / 2); i >= 0; --i)
    {
        if (results[i].semitonePrecision < 80)
        {
            lowestNoteDetected = i + 1;
            break;
        }
    }

    COUT_GTEST << "NOTE RANGE:\t"
        << lowestNoteDetected << " (" << NoteNames[lowestNoteDetected % 12] << (lowestNoteDetected / 12) << ") - "
        << highestNoteDetected << " (" << NoteNames[highestNoteDetected % 12] << (highestNoteDetected / 12) << ")\t"
        << "COVERAGE: " << FftwReal(highestNoteDetected - lowestNoteDetected) / FftwReal(NoteFftInfo::NoteLimit) * FftwReal(100) << " %\t" 
        << "AVG FFT TIME: " << stopwatch.GetLapsAverage() << " ms" << ENDL;
}

} // namespace SamplesToNotes