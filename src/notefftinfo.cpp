#include <algorithm>

#include "utils.h"
#include "notefftinfo.h"
#include "audiofft.h"

namespace SamplesToNotes
{

NoteFftInfo::NoteFftInfo(const AudioFft& audioFft, FftwReal a4Frequency)
    : _audioFft(audioFft)
    , _a4Frequency(a4Frequency)
    , _noteFrequencies(NoteLimit)
    , _noteFftBins(NoteLimit)
{
    for (int i = 0; i < NoteLimit; ++i)
    {
        _noteFrequencies[i] = FrequencyOfNote(i, _a4Frequency);
        _noteFftBins[i] = _audioFft.GetBinForFrequency(_noteFrequencies[i]);
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