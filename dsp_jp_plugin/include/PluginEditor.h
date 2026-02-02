#pragma once

namespace dsp_jp {
	class PluginEditor : public juce::AudioProcessorEditor {
	public:
		explicit PluginEditor(PluginProcessor&);

		void resized() override;
		void paint(juce::Graphics&) override;

	private:

		// PluginProcessor& pluginProcessor;
		// void sliderValueChanged(juce::Slider* slider) override;
		juce::Slider parameterOne;

		juce::ImageComponent background;
		juce::ImageComponent logo;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
	};
}  // namespace dsp_jp
