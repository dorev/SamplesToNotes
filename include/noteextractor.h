#pragma once

#include <vector>
#include "fftwutils.h"
#include "notefftinfo.h"
#include "audiofft.h"

namespace SamplesToNotes
{

struct NoteValue
{
	size_t note;
	FftwReal value;
};

class NoteExtractor
{
public:
	NoteExtractor(FftwReal samplingRate, FftwReal a4Frequency = 440);
	size_t GetFftWindowSize() const;
	std::vector<NoteValue> GetNotesFromSamples(FftwReal* sampleBuffer, size_t bufferSize, size_t resultSize);

private:
	FftwReal _samplingRate;
	FftwReal _a4Frequency;
	AudioFft _audioFft;
	NoteFftInfo _noteFftInfo;
	FftwComplex* _fftOutputBuffer;
};

}