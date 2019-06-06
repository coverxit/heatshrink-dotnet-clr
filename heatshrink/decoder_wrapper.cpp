#include "../heatshrink-c/heatshrink_encoder.h"
#include "../heatshrink-c/heatshrink_decoder.h"

#include "heatshrink.h"

namespace heatshrink {
	HeatShrinkDecoder::HeatShrinkDecoder(uint16_t inputBufferSize, uint8_t windowBits, uint8_t lookaheadBits) : m_isDisposed(false) {
		m_decoder = heatshrink_decoder_alloc(inputBufferSize, windowBits, lookaheadBits);

		if (m_decoder == NULL) {
			throw gcnew System::ArgumentOutOfRangeException();
		}
	}

	HeatShrinkDecoder::~HeatShrinkDecoder() {
		if (m_isDisposed) return;

		this->!HeatShrinkDecoder();
		m_isDisposed = true;
	}

	HeatShrinkDecoder::!HeatShrinkDecoder() {
		if (m_decoder != nullptr) {
			heatshrink_decoder_free(m_decoder);
		}
	}

	void HeatShrinkDecoder::Reset() {
		heatshrink_decoder_reset(m_decoder);
	}

	DecoderSinkResult HeatShrinkDecoder::Sink(array<Byte>^ buffer, UInt64 offset, UInt64 size, UInt64% inputSize) {
		pin_ptr<Byte> pinnedBuffer = &buffer[0];
		uint8_t* bufferPtr = pinnedBuffer;

		size_t sunk;
		auto ret = heatshrink_decoder_sink(m_decoder, bufferPtr + offset, size, &sunk);
		inputSize = sunk;
		return static_cast<DecoderSinkResult>(ret);
	}

	DecoderPollResult HeatShrinkDecoder::Poll(array<Byte>^ buffer, UInt64 offset, UInt64 size, UInt64% outputSize) {
		pin_ptr<Byte> pinnedBuffer = &buffer[0];
		uint8_t* bufferPtr = pinnedBuffer;

		size_t polled;
		auto ret = heatshrink_decoder_poll(m_decoder, bufferPtr + offset, size, &polled);
		outputSize = polled;
		return static_cast<DecoderPollResult>(ret);
	}

	DecoderFinishResult HeatShrinkDecoder::Finish() {
		return static_cast<DecoderFinishResult>(heatshrink_decoder_finish(m_decoder));
	}
}
