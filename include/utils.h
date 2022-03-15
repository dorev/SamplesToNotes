#pragma once

#include "fftwutils.h"

namespace SamplesToNotes
{

FftwReal FrequencyOfNote(size_t note, FftwReal a4Frequency = 440);
FftwReal DifferencePercentage(FftwReal value1, FftwReal value2);
void GenerateSinWave(FftwReal* buffer, int bufferSize, FftwReal frequency, FftwReal samplingRate);

}