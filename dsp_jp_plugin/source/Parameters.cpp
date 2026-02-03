
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
				juce::ParameterID{ "parameterOne.value", versionHint }, "Parameter One Value",
				juce::NormalisableRange<float>{0.01f, 1.f, 0.01f},
				1.f,
				juce::AudioParameterFloatAttributes{}.withLabel("inc(1)")));
	}
	Parameters::Parameters(juce::AudioProcessor& processor) : parameterOne { createParameterOneParameter(processor) } {
		// juce::ignoreUnused(processor);
	}
}  // namespace dsp_jp
