#pragma once

#include  <vector>
#include "fftwutils.h"
#include "defines.h"

namespace SamplesToNotes
{

class AudioFft;

class NoteFftInfo
{
public:
    static const size_t NoteLimit = 116;
   
public:
    ONLY_EXPLICIT_CONSTRUCTOR(NoteFftInfo);
    NoteFftInfo(const AudioFft& audioFft, FftwReal a4Frequency = 440);
    const std::vector<size_t>& GetNoteFftBins() const;
    const std::vector<FftwReal>& GetNoteFrequencies() const;
    bool CheckNoteBinsAreUnique() const;

private:
    const AudioFft& _audioFft;
    FftwReal _a4Frequency;
    size_t _outputBinCount;
    FftwReal _binRange;
    std::vector<FftwReal> _noteFrequencies;
    std::vector<size_t> _noteFftBins;
};

}