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
	const NoteFftInfo& GetNoteFftInfo() const;
	std::vector<NoteValue> GetNotesFromSamples(size_t resultSize);
	FftwReal* GetInputBuffer();
	size_t GetInputBufferSize() const;

private:
	size_t _fftWindowSize;
	FftwReal _samplingRate;
	FftwReal _a4Frequency;
	AudioFft _audioFft;
	NoteFftInfo _noteFftInfo;
	FftwComplex* _fftOutputBuffer;
};

}