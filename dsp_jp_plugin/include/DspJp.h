#pragma once

namespace dsp_jp {
	class DspJp {
	public:
		void prepare(double sampleRate, int expectedMaxFramesPerBlock, int numChannels) {
			juce::ignoreUnused(sampleRate, expectedMaxFramesPerBlock);
			a0.resize(numChannels, parameterOne);
			a1.resize(numChannels, parameterTwo);
			z1.resize(numChannels, 0.f);
		}

		void setParameterOne(float pOne) {
			parameterOne = pOne;			
			for (auto& e : a0)
				e = pOne;
			
		}

		void setParameterTwo(float pTwo) {
			parameterTwo = pTwo;
			for (auto& e : a1)
				e = pTwo;
		}

		/*
		* Feed Forward
		* a feed forward filter will be a linear phase filter if its coefficients are symmetrical aobut their center
		* In this case (1, 1) is symmetrical, (.5, .5) is symmetrical, so is (-.25, 0, -.25)
		* z is the previous input sample in this case
		*/
		float applyFeedForwardFilter(float inputSample, int channelIndex) {
			float yn = a0[channelIndex] * inputSample + a1[channelIndex] * z1[channelIndex];
			z1[channelIndex] = inputSample;
			return yn;
		}

		/* 
		* Feed Back
		* y(n) = a_0x(n) - b_1y(n-1)
		* z is the previous output sample
		* b1 is a1, no need to relabel coeff sliders
		*/
		float applyFeedBackFilter(float inputSample, int channelIndex) {
			float yn = a0[channelIndex] * inputSample - a1[channelIndex] * z1[channelIndex];
			z1[channelIndex] = yn;
			return yn;
		}

		/*
		* _ for subscript, ^ for superscript
		* 0.0 <= a_1 <= 0.49, a_0 = a_1 - 1.0
		* feed forward difference equation
		* y(n) = a_0x(n) + a_1x(n - 1) for each channel
		*/


		
		void process(juce::AudioBuffer<float>& buffer) noexcept {

			// for each frame
			for (const auto frameIndex : std::views::iota(0, buffer.getNumSamples())) {
				// for each channel sample in the frame
				for (const auto channelIndex : std::views::iota(0, buffer.getNumChannels())) {

					const auto inputSample = buffer.getSample(channelIndex, frameIndex);
					// const auto outputSample = inputSample * parameterOne;
					const auto outputSample = applyFeedBackFilter(inputSample, channelIndex);
					buffer.setSample(channelIndex, frameIndex, outputSample);
				}
			}
		}

		void reset() noexcept {}

	private:
		float parameterOne{ 1.f };
		float parameterTwo{ 0.f };
		std::vector<float> a0;		// coeff (slider pos)
		std::vector<float> a1;		// coeff - 1.f
		std::vector<float> b1;		// 
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