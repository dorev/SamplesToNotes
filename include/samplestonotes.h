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

class SamplesToNotes
{
public:
	ONLY_EXPLICIT_CONSTRUCTOR(SamplesToNotes);
	SamplesToNotes(size_t fftWindowSize, FftwReal samplingRate, FftwReal a4Frequency = 440);
	size_t GetFftWindowSize() const;
	std::vector<NoteValue> GetNotesFromSamples(FftwReal* sampleBuffer, size_t bufferSize, size_t resultSize);

private:
	size_t _fftWindowSize;
	FftwReal _samplingRate;
	FftwReal _a4Frequency;
	AudioFft _audioFft;
	NoteFftInfo _noteFftInfo;
	FftwComplex* _fftOutputBuffer;
};

}