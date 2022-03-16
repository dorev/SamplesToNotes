#include "noteextractor.h"

namespace SamplesToNotes
{

NoteExtractor::NoteExtractor(FftwReal samplingRate, FftwReal a4Frequency)
	: _samplingRate(samplingRate)
	, _a4Frequency(a4Frequency)
	, _audioFft(_samplingRate)
	, _noteFftInfo(AudioFft::N, _samplingRate, _a4Frequency)
	, _fftOutputBuffer(_audioFft.GetOutputBufferPointer())
{
}

size_t NoteExtractor::GetFftWindowSize() const
{
	return AudioFft::N;
}

std::vector<NoteValue> NoteExtractor::GetNotesFromSamples(FftwReal* sampleBuffer, size_t bufferSize, size_t resultSize)
{
	if (bufferSize < AudioFft::N)
		throw "bufferSize should be greater or equal to the FFT window";

	_audioFft.Execute(sampleBuffer);
		
	// Scan note bins from the FFT output buffer
	std::vector<NoteValue> results(resultSize, {0, 0});
	const std::vector<size_t>& noteBins = _noteFftInfo.GetNoteFftBins();
	for (size_t note = 0; note < NoteFftInfo::NoteLimit; ++note)
	{
		// Compare with each saved result
		for (size_t i = 0; i < resultSize; ++i)
		{
			FftwReal value = _fftOutputBuffer[noteBins[note]][0];
			NoteValue& result = results[i];
			if (value > result.value)
			{
				// Shift right lower values
				for (size_t j = resultSize - 1; j > i; --j)
					results[j] = results[j - 1];

				// Insert higher result
				result.note = note;
				result.value = value;
				break;
			}
		}
	}
	return results;
}

}