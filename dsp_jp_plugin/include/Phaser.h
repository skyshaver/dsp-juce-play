/*
    class: Phaser
    implemented by: Ross Bencina <rossb@kagi.com>
    date: 24/8/98

    Phaser is a six stage phase shifter, intended to reproduce the
    sound of a traditional analogue phaser effect.
    This implementation uses six first order all-pass filters in
    series, with delay time modulated by a sinusoidal.

    This implementation was created to be clear, not efficient.
    Obvious modifications include using a table lookup for the lfo,
    not updating the filter delay times every sample, and not
    tuning all of the filters to the same delay time.

    Thanks to:
    The nice folks on the music-dsp mailing list, including...
    Chris Towsend and Marc Lindahl

    ...and Scott Lehman's Phase Shifting page at harmony central:
    http://www.harmony-central.com/Effects/Articles/Phase_Shifting/

*/

#pragma once

namespace dsp_jp {

    class Phaser {
    
    public:
        Phaser()  //initialise to some useful defaults...
            : feedBack(.7f)
            , lfoPhase(0.f)
            , depth(.5f)
            , zm1(0.f)
        {
           
        }

        void Prepare(double sr, int expectedMaxFramesPerBlock) {
            juce::ignoreUnused(expectedMaxFramesPerBlock);
            sampleRate = static_cast<float>(sr);
            // need sampleRate to be set to set these defaults
            Range(440.f, 1600.f);
            Rate(.5f);
        }

        void Range(float fMin, float fMax) { // Hz
            dmin = fMin / (sampleRate / 2.f);
            dmax = fMax / (sampleRate / 2.f);
        }

        void Rate(float rate) { // cps
            lfoInc = 2.f * juce::MathConstants<float>::pi * (rate / sampleRate); 
        }

        void Feedback(float fb) { // 0 -> < 1.
            feedBack = fb;
        }

        void Depth(float d) {  // 0 -> 1.
            depth = d;
        }

        float Update(float inSamp) {
            //calculate and update phaser sweep lfo...
            float d = dmin + (dmax - dmin) * ((sin(lfoPhase) + 1.f) / 2.f);
            lfoPhase += lfoInc;
            if (lfoPhase >= juce::MathConstants<float>::pi * 2.f) {
                lfoPhase -= juce::MathConstants<float>::pi * 2.f;
            }

            //update filter coeffs
            //for (int i = 0; i < 6; i++) {
            //    allPassDelays[i].Delay(d);
            //}

            for (auto& e : allPassDelays) {
                e.Delay(d);
            }

            //calculate output
            //float y = allPassDelays[0].Update(
            //    allPassDelays[1].Update(
            //        allPassDelays[2].Update(
            //            allPassDelays[3].Update(
            //                allPassDelays[4].Update(
            //                    allPassDelays[5].Update(inSamp + zm1 * feedBack))))));

            // updated for readibility, needs testing see https://godbolt.org/z/P54ve5o51 for live test
            auto y = std::accumulate(std::next(allPassDelays.rbegin()), allPassDelays.rend(),
                allPassDelays.back().Update(inSamp + zm1 * feedBack), [](auto acc, auto& e) { return e.Update(acc); });
            zm1 = y;

            return inSamp + y * depth;
        }

        void process(juce::AudioBuffer<float>& buffer) noexcept {

            // for each frame
            for (const auto frameIndex : std::views::iota(0, buffer.getNumSamples())) {
                // for each channel sample in the frame
                for (const auto channelIndex :
                    std::views::iota(0, buffer.getNumChannels())) {
                    const auto inputSample = buffer.getSample(channelIndex, frameIndex);
                    // DBG("output:" << Update(inputSample) << " input: " << inputSample);
                    const auto outputSample = Update(inputSample);
                    buffer.setSample(channelIndex, frameIndex, outputSample);
                }
            }
        }

        void reset() noexcept {}


    private:
        class AllPassDelay {
        public:
            AllPassDelay()
                : a1(0.f)
                , zm1(0.f)
            {
            }

            void Delay(float delay) { // sample delay time
                a1 = (1.f - delay) / (1.f + delay);
            }

            float Update(float inSamp) {
                float y = inSamp * -a1 + zm1;
                zm1 = y * a1 + inSamp;

                return y;
            }
        private:
            float a1, zm1;
        };

        // AllPassDelay allPassDelays[6];
        std::array<AllPassDelay, 6> allPassDelays;

        float dmin, dmax; // range
        float feedBack; 
        float lfoPhase;
        float lfoInc;
        float depth;
        float sampleRate;

        float zm1;
    };

} // namespace dsp_jp