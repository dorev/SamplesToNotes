#include "samplestonotes.h"
#include <iostream>

namespace SamplesToNotes
{

SamplesToNotes::SamplesToNotes(size_t fftWindowSize, FftwReal samplingRate, FftwReal a4Frequency)
	: _fftWindowSize(fftWindowSize)
	, _samplingRate(samplingRate)
	, _a4Frequency(a4Frequency)
	, _audioFft(_fftWindowSize, _samplingRate)
	, _noteFftInfo(_audioFft, _a4Frequency)
	, _fftOutputBuffer(_audioFft.GetOutputBuffer())
{
}

const NoteFftInfo& SamplesToNotes::GetNoteFftInfo() const
{
	return _noteFftInfo;
}

FftwReal* SamplesToNotes::GetInputBuffer()
{
	return _audioFft.GetInputBuffer();
}

size_t SamplesToNotes::GetInputBufferSize() const
{
	return _audioFft.GetInputBufferSize();
}

std::vector<NoteValue> SamplesToNotes::GetNotesFromSamples(size_t resultSize)
{
	_audioFft.Execute();
	auto topBins = _audioFft.GetTopBins(resultSize);

	// Scan note bins from the FFT output buffer
	std::vector<NoteValue> results(resultSize, {0, 0});
	const std::vector<size_t>& noteBins = _noteFftInfo.GetNoteFftBins();
	for (size_t note = 0; note < NoteFftInfo::NoteLimit; ++note)
	{
		// Compare with each saved result
		for (size_t i = 0; i < resultSize; ++i)
		{
			size_t noteBin = noteBins[note];
			FftwReal value = _fftOutputBuffer[noteBin][0];
			if (value > results[i].value)
			{
				// Shift right lower values
				if (resultSize > 1)
				{
					for (size_t j = resultSize - 1; j > i; --j)
						results[j] = results[j - 1];
				}

				// Insert higher result
				results[i].note = note;
				results[i].value = value;
				break;
			}
		}
	}
	return results;
}

}