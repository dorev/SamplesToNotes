#pragma once

#include  <vector>
#include "fftwutils.h"

namespace SamplesToNotes
{

class NoteFftInfo
{
public:
    static const size_t NoteLimit = 128;

    NoteFftInfo(size_t N, FftwReal samplingRate, FftwReal a4Frequency = 440);
    const std::vector<size_t>& GetNoteFftBins() const;
    const std::vector<FftwReal>& GetNoteFrequencies() const;
    bool CheckNoteBinsAreUnique() const;

private:
    FftwReal _a4Frequency;
    FftwReal _binRange;
    std::vector<FftwReal> _noteFrequencies;
    std::vector<size_t> _noteFftBins;
};

}