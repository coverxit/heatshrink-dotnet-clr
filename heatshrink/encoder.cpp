#include "../heatshrink-c/heatshrink_encoder.h"
#include "../heatshrink-c/heatshrink_decoder.h"

#include "heatshrink.h"

heatshrink::Encoder::Encoder(uint8_t windowBits, uint8_t lookaheadBits) : m_isDisposed(false) {
	m_encoder = heatshrink_encoder_alloc(windowBits, lookaheadBits);
}

heatshrink::Encoder::~Encoder() {
	if (m_isDisposed) return;

	this->!Encoder();
	m_isDisposed = true;
}

heatshrink::Encoder::!Encoder() {
	heatshrink_encoder_free(m_encoder);
}

void heatshrink::Encoder::reset() {
	heatshrink_encoder_reset(m_encoder);
}

HSE_sink_res heatshrink::Encoder::sink(array<Byte>^ buffer, UInt64 offset, UInt64 size, UInt64% inputSize) {
	pin_ptr<Byte> pinnedBuffer = &buffer[0];
	uint8_t* bufferPtr = pinnedBuffer;

	size_t sunk;
	auto ret = heatshrink_encoder_sink(m_encoder, bufferPtr + offset, size, &sunk);
	inputSize = sunk;
	return ret;
}

HSE_poll_res heatshrink::Encoder::poll(array<Byte>^ buffer, UInt64 offset, UInt64 size, UInt64% outputSize) {
	pin_ptr<Byte> pinnedBuffer = &buffer[0];
	uint8_t* bufferPtr = pinnedBuffer;

	size_t polled;
	auto ret = heatshrink_encoder_poll(m_encoder, bufferPtr + offset, size, &polled);
	outputSize = polled;
	return ret;
}

HSE_finish_res heatshrink::Encoder::finish() {
	return heatshrink_encoder_finish(m_encoder);
}