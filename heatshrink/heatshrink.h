#pragma once

using namespace System;

namespace heatshrink {
	public ref class Encoder
	{
	public:
		/// <summary>
		/// Allocate a encoder with an expansion buffer size of 2 ^ windowBits, 
		/// and a lookahead size of 2 ^ lookaheadBits
		/// </summary>
		/// <param name="windowBits">Window size in bits</param>
		/// <param name="lookaheadBits">Lookahead size in bits</param>
		Encoder(uint8_t windowBits, uint8_t lookaheadBits);
		~Encoder();

		/// <summary>
		/// Reset the encoder
		/// </summary>
		void reset();

		/// <summary>
		/// Sink up to size bytes from buffer + offset into the encoder.
		/// inputSize is set to the number of bytes actually sunk (in case a buffer was filled)
		/// </summary>
		/// <param name="buffer">The buffer to be filled into the encoder</param>
		/// <param name="offset">The offset of the buffer to be filled</param>
		/// <param name="size">Number of bytes to be filled</param>
		/// <param name="inputSize">Number of bytes actually sunk</param>
		/// <returns>Refer to HSE_sink_res in original C version heatshrink</returns>
		HSE_sink_res sink(array<Byte>^ buffer, UInt64 offset, UInt64 size, UInt64% inputSize);

		/// <summary>
		/// Poll for output from the encoder, copying at most size bytes into buffer + offset
		/// (setting outputSize to the actual amount copied)
		/// </summary>
		/// <param name="buffer">The buffer to be filled from the encoder</param>
		/// <param name="offset">The offset of the buffer to be filled</param>
		/// <param name="size">Number of bytes to be filled</param>
		/// <param name="outputSize">Number of bytes actually polled</param>
		/// <returns>Refer to HSE_poll_res in original C version heatshrink</returns>
		HSE_poll_res poll(array<Byte>^ buffer, UInt64 offset, UInt64 size, UInt64% outputSize);
		
		/// <summary>
		/// Notify the encoder that the input stream is finished.
		/// If the return value is HSER_FINISH_MORE, there is still more output, so call poll() and repeat
		/// </summary>
		/// <returns>Refer to HSE_finish_res in original C version heatshrink</returns>
		HSE_finish_res finish();

	protected:
		!Encoder();

	private:
		bool m_isDisposed;
		heatshrink_encoder* m_encoder;
	};

	public ref class Decoder {
	public:
		/// <summary>
		/// Allocate a decoder with an input buffer of inputBufferSize bytes,
		/// an expansion buffer size of 2 ^ windowBits, and a lookahead
		/// size of 2 ^ lookaheadBits. (The window buffer and lookahead sizes
		/// must match the settings used when the data was compressed.)
		/// </summary>
		/// <param name="inputBufferSize">Input buffer size</param>
		/// <param name="windowBits">Window size in bits</param>
		/// <param name="lookaheadBits">Lookahead size in bits</param>
		Decoder(uint16_t inputBufferSize, uint8_t windowBits, uint8_t lookaheadBits);
		~Decoder();

		/// <summary>
		/// Reset the decoder
		/// </summary>
		void reset();

		/// <summary>
		/// Sink up to size bytes from buffer + offset into the decoder.
		/// inputSize is set to the number of bytes actually sunk (in case a buffer was filled)
		/// </summary>
		/// <param name="buffer">The buffer to be filled into the decoder</param>
		/// <param name="offset">The offset of the buffer to be filled</param>
		/// <param name="size">Number of bytes to be filled</param>
		/// <param name="inputSize">Number of bytes actually sunk</param>
		/// <returns>Refer to HSE_sink_res in original C version heatshrink</returns>
		HSD_sink_res sink(array<Byte>^ buffer, UInt64 offset, UInt64 size, UInt64% inputSize);

		/// <summary>
		/// Poll for output from the decoder, copying at most size bytes into buffer + offset
		/// (setting outputSize to the actual amount copied)
		/// </summary>
		/// <param name="buffer">The buffer to be filled from the decoder</param>
		/// <param name="offset">The offset of the buffer to be filled</param>
		/// <param name="size">Number of bytes to be filled</param>
		/// <param name="outputSize">Number of bytes actually polled</param>
		/// <returns>Refer to HSE_poll_res in original C version heatshrink</returns>
		HSD_poll_res poll(array<Byte>^ buffer, UInt64 offset, UInt64 size, UInt64% outputSize);

		/// <summary>
		/// Notify the decoder that the input stream is finished.
		/// If the return value is HSER_FINISH_MORE, there is still more output, so call poll() and repeat
		/// </summary>
		/// <returns>Refer to HSE_finish_res in original C version heatshrink</returns>
		HSD_finish_res finish();

	protected:
		!Decoder();

	private:
		bool m_isDisposed;
		heatshrink_decoder* m_decoder;
	};
}
