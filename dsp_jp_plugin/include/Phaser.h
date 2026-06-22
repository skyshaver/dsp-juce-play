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

#define SAMPLE_RATE (44100.f)  //sample rate
#define PI_FLOAT (3.14159f)

class Phaser {
public:
    Phaser()  //initialise to some useful defaults...
        : feedBack(.7f)
        , lfoPhase(0.f)
        , depth(1.f)
        , zm1(0.f)
    {
        Range(440.f, 1600.f);
        Rate(.5f);
    }

    void Range(float fMin, float fMax) { // Hz
        dmin = fMin / (SAMPLE_RATE / 2.f);
        dmax = fMax / (SAMPLE_RATE / 2.f);
    }

    void Rate(float rate) { // cps
        lfoInc = 2.f * PI_FLOAT * (rate / SAMPLE_RATE);
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
        if (lfoPhase >= PI_FLOAT * 2.f) {
            lfoPhase -= PI_FLOAT * 2.f;
        }

        //update filter coeffs
        //for (int i = 0; i < 6; i++) {
        //    allPassDelays[i].Delay(d);
        //}

        for (auto& e : allPassDelays) {
            e.Delay(d);
        }

        //calculate output
        float y = allPassDelays[0].Update(
            allPassDelays[1].Update(
                allPassDelays[2].Update(
                    allPassDelays[3].Update(
                        allPassDelays[4].Update(
                            allPassDelays[5].Update(inSamp + zm1 * feedBack))))));

        float test = 0.f;
        for (auto& e : allPassDelays | std::views::reverse) {

        }

        zm1 = y;

        return inSamp + y * depth;
    }
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

    float zm1;
};