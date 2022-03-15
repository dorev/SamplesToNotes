#include <algorithm>

#include "utils.h"
#include "notefftinfo.h"

namespace SamplesToNotes
{

NoteFftInfo::NoteFftInfo(size_t fftWindowSize, FftwReal samplingRate, FftwReal a4Frequency)
    : _a4Frequency(a4Frequency)
    , _binRange(samplingRate / fftWindowSize)
    , _noteFrequencies(NoteLimit)
    , _noteFftBins(NoteLimit)
{
    // Get frequency for each note with A4 standard
    for (int i = 0; i < NoteLimit; ++i)
        _noteFrequencies[i] = FrequencyOfNote(i, _a4Frequency);

    // Get the bin containing each note
    size_t note = 0;
    for (size_t bin = 0; bin < fftWindowSize; ++bin)
    {
        FftwReal binMaxFrequency = FFTW_REAL(bin + 1) * _binRange;
        for (; note < NoteLimit; ++note)
        {
            FftwReal noteFrequency = _noteFrequencies[note];
            if (noteFrequency < binMaxFrequency)
                _noteFftBins[note] = bin;
            else
            {
                _noteFftBins[note] = bin + 1;
                break;
            }
        }
    }
}

const std::vector<size_t>& NoteFftInfo::GetNoteFftBins() const
{
    return _noteFftBins;
}

const std::vector<FftwReal>& NoteFftInfo::GetNoteFrequencies() const
{
    return _noteFrequencies;
}

bool NoteFftInfo::CheckNoteBinsAreUnique() const
{
    // Check if two notes are in the same bin at least once
    return std::adjacent_find(_noteFftBins.begin(), _noteFftBins.end()) == _noteFftBins.end();
}

}