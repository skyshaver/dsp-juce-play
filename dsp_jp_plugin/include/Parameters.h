#pragma once

namespace dsp_jp {
	struct Parameters {
		explicit Parameters(juce::AudioProcessor&);
		juce::AudioParameterFloat& parameterOne;
		// juce::AudioParameterBool& bypassed;
		// juce::AudioParameterChoice& waveform;

		JUCE_DECLARE_NON_COPYABLE(Parameters)
		JUCE_DECLARE_NON_MOVEABLE(Parameters)
	};
}  // namespace tremolo
