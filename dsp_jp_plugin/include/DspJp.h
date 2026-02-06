#pragma once

namespace dsp_jp {
	class DspJp {
	public:
		void prepare(double sampleRate, int expectedMaxFramesPerBlock, int numChannels) {
			juce::ignoreUnused(sampleRate, expectedMaxFramesPerBlock);
			a0.resize(numChannels, parameterOne);
			a1.resize(numChannels, parameterOne - 1.f);
			z1.resize(numChannels, 0.f);
		}

		void setParameterOne(float pOne) {
			parameterOne = pOne;
			// omitting this loop makes a lpf??
			for (auto& e : a0)
				e = pOne;
			for (auto& e : a1)
				e = pOne - 1.f;
		}

		float applyHpf(float inputSample, int channelIndex) {
			float yn = a0[channelIndex] * inputSample + a1[channelIndex] * z1[channelIndex];
			z1[channelIndex] = inputSample;
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


		/*
		* a feed forward filter will be a linear phase filter if its coefficients are symmetrical aobut their center
		* In this case (1, 1) is symmetrical, (.5, .5) is symmetrical, so is (-.25, 0, -.25)

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
		std::vector<float> a0;		// coeff (slider pos)
		std::vector<float> a1;		// coeff - 1.f
		std::vector<float> z1;		// last sample (n - 1)

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