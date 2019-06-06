#include "../heatshrink-c/heatshrink_encoder.h"
#include "../heatshrink-c/heatshrink_decoder.h"

#include "heatshrink.h"

namespace heatshrink {
	HeatShrinkEncoder::HeatShrinkEncoder(uint8_t windowBits, uint8_t lookaheadBits) : m_isDisposed(false) {
		m_encoder = heatshrink_encoder_alloc(windowBits, lookaheadBits);

		if (m_encoder == NULL) {
			throw gcnew System::ArgumentOutOfRangeException();
		}
	}

	HeatShrinkEncoder::~HeatShrinkEncoder() {
		if (m_isDisposed) return;

		this->!HeatShrinkEncoder();
		m_isDisposed = true;
	}

	HeatShrinkEncoder::!HeatShrinkEncoder() {
		if (m_encoder != nullptr) {
			heatshrink_encoder_free(m_encoder);
		}
	}

	void HeatShrinkEncoder::Reset() {
		heatshrink_encoder_reset(m_encoder);
	}

	EncoderSinkResult HeatShrinkEncoder::Sink(array<Byte>^ buffer, UInt64 offset, UInt64 size, UInt64% inputSize) {
		pin_ptr<Byte> pinnedBuffer = &buffer[0];
		uint8_t* bufferPtr = pinnedBuffer;

		size_t sunk;
		auto ret = heatshrink_encoder_sink(m_encoder, bufferPtr + offset, size, &sunk);
		inputSize = sunk;
		return static_cast<EncoderSinkResult>(ret);
	}

	EncoderPollResult HeatShrinkEncoder::Poll(array<Byte>^ buffer, UInt64 offset, UInt64 size, UInt64% outputSize) {
		pin_ptr<Byte> pinnedBuffer = &buffer[0];
		uint8_t* bufferPtr = pinnedBuffer;

		size_t polled;
		auto ret = heatshrink_encoder_poll(m_encoder, bufferPtr + offset, size, &polled);
		outputSize = polled;
		return static_cast<EncoderPollResult>(ret);
	}

	EncoderFinishResult HeatShrinkEncoder::Finish() {
		return static_cast<EncoderFinishResult>(heatshrink_encoder_finish(m_encoder));
	}
}
