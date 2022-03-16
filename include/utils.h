#pragma once

#include "fftwutils.h"

namespace SamplesToNotes
{

FftwReal FrequencyOfNote(size_t note, FftwReal a4Frequency = 440);
FftwReal DifferencePercentage(FftwReal value1, FftwReal value2);
void GenerateSinWave(FftwReal* buffer, size_t bufferSize, FftwReal frequency, FftwReal samplingRate);

static const char* NoteNames[12] =
{
	"C",
	"C#",
	"D",
	"D#",
	"E",
	"F",
	"F#",
	"G",
	"G#",
	"A",
	"A#",
	"B"
};


}