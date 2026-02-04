#pragma once

namespace dsp_jp {
class DspJp {
public:
  void prepare(double sampleRate, int expectedMaxFramesPerBlock) {
    juce::ignoreUnused(sampleRate, expectedMaxFramesPerBlock);
  }

  void setParameterOne(float pOne) {
      
      // translate from dB to range [0.0, 1.0]
      parameterOne = std::pow(10.f, pOne / 20.f);
      
      // DBG(parameterOne);
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
