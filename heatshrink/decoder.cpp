#include "../heatshrink-c/heatshrink_encoder.h"
#include "../heatshrink-c/heatshrink_decoder.h"

#include "heatshrink.h"

heatshrink::Decoder::Decoder(uint16_t inputBufferSize, uint8_t windowBits, uint8_t lookaheadBits) : m_isDisposed(false) {
	m_decoder = heatshrink_decoder_alloc(inputBufferSize, windowBits, lookaheadBits);
}

heatshrink::Decoder::~Decoder() {
	if (m_isDisposed) return;

	this->!Decoder();
	m_isDisposed = true;
}

heatshrink::Decoder::!Decoder() {
	heatshrink_decoder_free(m_decoder);
}

void heatshrink::Decoder::reset() {
	heatshrink_decoder_reset(m_decoder);
}

HSD_sink_res heatshrink::Decoder::sink(array<Byte>^ buffer, UInt64 offset, UInt64 size, UInt64% inputSize) {
	pin_ptr<Byte> pinnedBuffer = &buffer[0];
	uint8_t* bufferPtr = pinnedBuffer;

	size_t sunk;
	auto ret = heatshrink_decoder_sink(m_decoder, bufferPtr + offset, size, &sunk);
	inputSize = sunk;
	return ret;
}

HSD_poll_res heatshrink::Decoder::poll(array<Byte>^ buffer, UInt64 offset, UInt64 size, UInt64% outputSize) {
	pin_ptr<Byte> pinnedBuffer = &buffer[0];
	uint8_t* bufferPtr = pinnedBuffer;

	size_t polled;
	auto ret = heatshrink_decoder_poll(m_decoder, bufferPtr + offset, size, &polled);
	outputSize = polled;
	return ret;
}

HSD_finish_res heatshrink::Decoder::finish() {
	return heatshrink_decoder_finish(m_decoder);
}