using System;
using System.Linq;

using Microsoft.VisualStudio.TestTools.UnitTesting;

using heatshrink;

namespace heatshrink_unittest
{
    [TestClass]
    public class HeatShrinkUnitTest
    {
        private const int MinWindowBits = 4;
        private const int MaxWindowBits = 15;

        private const int MinLookaheadBits = 3;

        [TestMethod]
        public void EncoderAllocShouldRejectInvalidArguments()
        {
            void tester(byte windowBits, byte lookaheadBits)
            {
                try
                {
                    new HeatShrinkEncoder(windowBits, lookaheadBits);
                    Assert.Fail();
                }
                catch (ArgumentOutOfRangeException)
                {
                    // Excpected
                }
            }

            tester(MinWindowBits - 1, 8);
            tester(MaxWindowBits + 1, 8);
            tester(8, MinLookaheadBits - 1);
            tester(8, 9);
        }

        [TestMethod]
        public void EncoderSinkShouldAcceptInputWhenItWillFit()
        {
            var encoder = new HeatShrinkEncoder(8, 7);

            var input = Enumerable.Repeat('*', 256).Select(c => (byte) c).ToArray();
            ulong bytesCopied = 0;

            Assert.AreEqual(EncoderSinkResult.Ok, encoder.Sink(input, 0, 256, ref bytesCopied));
            Assert.AreEqual(256UL, bytesCopied);
        }

        [TestMethod]
        public void EncoderSinkShouldAcceptPartialInputWhenSomeWillFit()
        {
            var encoder = new HeatShrinkEncoder(8, 7);

            var input = Enumerable.Repeat('*', 512).Select(c => (byte)c).ToArray();
            ulong bytesCopied = 0;

            Assert.AreEqual(EncoderSinkResult.Ok, encoder.Sink(input, 0, 512, ref bytesCopied));
            Assert.AreEqual(256UL, bytesCopied);
        }

        [TestMethod]
        public void EncoderPollShouldIndicateWhenNoInputIsProvided()
        {
            var encoder = new HeatShrinkEncoder(8, 7);

            var output = new byte[512];
            ulong outputSize = 0;

            Assert.AreEqual(EncoderPollResult.Empty, encoder.Poll(output, 0, 512, ref outputSize));
        }

        [TestMethod]
        public void EncoderShouldEmitDataWithoutRepetitionsAsLiteralSequence()
        {
            var encoder = new HeatShrinkEncoder(8, 7);

            var input = new byte[5];
            var output = new byte[1024];
            var expected = new byte[] { 0x80, 0x40, 0x60, 0x50, 0x38, 0x20 };
            ulong copied = 0;

            for (int i = 0; i < 5; ++i) input[i] = (byte) i;

            Assert.AreEqual(EncoderSinkResult.Ok, encoder.Sink(input, 0, 5, ref copied));
            Assert.AreEqual(5UL, copied);

            // Should get no output yet, since encoder doesn't know input is complete. 
            copied = 0;
            var pres = encoder.Poll(output, 0, 1024, ref copied);
            Assert.AreEqual(EncoderPollResult.Empty, pres);
            Assert.AreEqual(0UL, copied);

            // Mark input stream as done, to force small input to be processed.
            var fres = encoder.Finish();
            Assert.AreEqual(EncoderFinishResult.More, fres);

            pres = encoder.Poll(output, 0, 1024, ref copied);
            Assert.AreEqual(EncoderPollResult.Empty, pres);

            for (int i = 0; i < expected.Length; ++i)
                Assert.AreEqual(expected[i], output[i]);

            Assert.AreEqual(EncoderFinishResult.Done, encoder.Finish());
        }
    }
}
