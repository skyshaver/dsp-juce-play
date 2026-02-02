
namespace dsp_jp {

	PluginEditor::PluginEditor(PluginProcessor& p) : AudioProcessorEditor(&p) {


		background.setImage(juce::ImageCache::getFromMemory(assets::Background_png, assets::Background_pngSize));

		logo.setImage(juce::ImageCache::getFromMemory(assets::Logo_png, assets::Logo_pngSize));


		// Make sure that before the constructor has finished, you've set the
		// editor's size to whatever you need it to be.
		setSize(540, 270);

		parameterOne.setSliderStyle(juce::Slider::LinearBarVertical);
		parameterOne.setRange(0.f, 100.f, 1.f);
		parameterOne.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
		parameterOne.setPopupDisplayEnabled(true, false, this);
		parameterOne.setTextValueSuffix(" ParamOne");
		parameterOne.setValue(1.f);
		addAndMakeVisible(&parameterOne);

		// listeners
		// parameterOne.addListener(this);

		/*addAndMakeVisible(background);
		addAndMakeVisible(logo);*/
	}

	void PluginEditor::resized() {
		const auto bounds = getLocalBounds();

		parameterOne.setBounds(40, 30, 20, getHeight() - 60);
		/*logo.setBounds({ 16, 16, 105, 24 });
		background.setBounds(bounds);*/


	}
	void dsp_jp::PluginEditor::paint(juce::Graphics& g) {
		// fill the whole window white
		g.fillAll(juce::Colours::white);
		// set the current drawing colour to black
		g.setColour(juce::Colours::black);
		// set the font size and draw text to the screen
		g.setFont(15.0f);
		g.drawFittedText("DSP Testing", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
	}

	//void dsp_jp::PluginEditor::sliderValueChanged(juce::Slider* slider) {
	//	// pluginProcessor.parameterOne = parameterOne.getValue();
	//}

}  // namespace dsp_jp
