
namespace dsp_jp {
	namespace {
		auto& addParameterToProcessor(juce::AudioProcessor& processor, auto parameter) {
			auto& result = *parameter;
			processor.addParameter(parameter.release());
			return result;
		}
	}

	juce::AudioParameterFloat& createParameterOneParameter(
		juce::AudioProcessor& processor) {
		constexpr auto versionHint = 1;
		return addParameterToProcessor(
			processor,
			std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ "parameterOne.value", versionHint }, "HPF Coeff",
				juce::NormalisableRange<float>{0.0f, 0.49f, 0.01f},
				0.25f,
				juce::AudioParameterFloatAttributes{}.withLabel("inc(1)")));
	
	}
	juce::AudioParameterFloat& createParameterTwoParameter(
		juce::AudioProcessor& processor) {
		constexpr auto versionHint = 1;
		return addParameterToProcessor(
			processor,
			std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ "parameterTwo.value", versionHint }, "Parameter Two Value",
				juce::NormalisableRange<float>{0.01f, 1.f, 0.01f},
				1.f,
				juce::AudioParameterFloatAttributes{}.withLabel("inc(1)")));
	}

	Parameters::Parameters(juce::AudioProcessor& processor) : parameterOne { createParameterOneParameter(processor) }, parameterTwo{ createParameterTwoParameter(processor) } {
		// juce::ignoreUnused(processor);
	}
}  // namespace dsp_jp
