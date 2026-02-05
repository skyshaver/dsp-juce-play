#pragma once

namespace dsp_jp {
	class DspJp {
	public:
		void prepare(double sampleRate, int expectedMaxFramesPerBlock, int numInputBuses) {
			juce::ignoreUnused(sampleRate, expectedMaxFramesPerBlock);
			a0.resize(numInputBuses, parameterOne);
			a1.resize(numInputBuses, parameterOne - 1.f);
			z1.resize(numInputBuses, 0.f);
		}

		void setParameterOne(float pOne) {
			parameterOne = pOne;
			for (auto& e : a1)
				e = pOne;
		}

		float applyHpf(float inputSample, int channelNum) {
			float xn = inputSample;
			float xn_1 = z1[channelNum];
			float yn = a0[channelNum] * xn + a1[channelNum] * xn_1;
			z1[channelNum] = xn;
			return yn;
		}

		/*
		* _ for subscript, ^ for superscript
		* 0.0 <= a_1 <= 0.49, a_0 = a_1 - 1.0
		* hpf difference equation
		* y(n) = a_0x(n) + a_1x(n - 1) for each channel
		* a1 is the slider value, a0 is the slider value - 1.0 (a1 - 1.0)
		* yn = ao_channel * input_sample + a1_channel * input_sample_-1
		*/

		void process(juce::AudioBuffer<float>& buffer) noexcept {

			// for each frame
			for (const auto frameIndex : std::views::iota(0, buffer.getNumSamples())) {
				// for each channel sample in the frame
				for (const auto channelIndex : std::views::iota(0, buffer.getNumChannels())) {

					const auto inputSample = buffer.getSample(channelIndex, frameIndex);
					// const auto outputSample = inputSample * parameterOne;
					const auto outputSample = applyHpf(inputSample, channelIndex);
					buffer.setSample(channelIndex, frameIndex, outputSample);
				}
			}
		}

		void reset() noexcept {}

	private:
		float parameterOne{ 0.01f };
		std::vector<float> a0;
		std::vector<float> a1;
		std::vector<float> z1;

	};
}  // namespace dsp_jp
