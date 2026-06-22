#pragma once

namespace dsp_jp {
class DspJp {
public:
  void prepare(double sampleRate, int expectedMaxFramesPerBlock) {
    juce::ignoreUnused(sampleRate, expectedMaxFramesPerBlock);
  }

  void setParameterOne(float pOne) {
      parameterOne = pOne;
  }

  void process(juce::AudioBuffer<float>& buffer) noexcept {
  
      // for each frame
    for (const auto frameIndex : std::views::iota(0, buffer.getNumSamples())) {      
      // for each channel sample in the frame
      for (const auto channelIndex :
           std::views::iota(0, buffer.getNumChannels())) {      
        const auto inputSample = buffer.getSample(channelIndex, frameIndex);      
        const auto outputSample = inputSample * parameterOne;      
        buffer.setSample(channelIndex, frameIndex, outputSample);
      }
    }
  }

  void reset() noexcept {}

private:
    float parameterOne{ 0.01f };
  
};
}  // namespace dsp_jp


/*
* Basic DSP test signals
* DC/Step (0Hz signal), 
*   - series like (0, 0, 0 , 1, ~) where there is one step from 0 to 1 and 1 repeats forever
*   - useful to tell the transient attack of the filter and give the 0Hz(DC) response
* Nyquist 
*   - series like (-1, 1)
*   - indenpendent of the systems actual sample rate
*   - minimum of two samples, 
* 1/2 Nyquist
*   - series like (-1, 0, 1 , 0)
*   - minimum of four samples
* 1/4 Nyquist
*   - series like (0.0, 0.707, 1, 0.707, 0, -0.707, -1, -0.707)
*   - minimum 8 samples
* Impulse
*   - the value 1 in an infinitely long string of 0
*/