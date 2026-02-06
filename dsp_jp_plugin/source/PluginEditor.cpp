
namespace dsp_jp {

	PluginEditor::PluginEditor(PluginProcessor& p) : AudioProcessorEditor(&p), 
			parameterOneAttachment{p.getParameterRefs().parameterOne, parameterOneSlider},
			parameterTwoAttachment{ p.getParameterRefs().parameterTwo, parameterTwoSlider } {


		background.setImage(juce::ImageCache::getFromMemory(assets::Background_png, assets::Background_pngSize));

		logo.setImage(juce::ImageCache::getFromMemory(assets::Logo_png, assets::Logo_pngSize));


		// Make sure that before the constructor has finished, you've set the
		// editor's size to whatever you need it to be.
		setSize(540, 270);

		// don't need to manually add range as parameterAttachment sets range/interval
		// parameterOneSlider.setRange(0.01f, 1.f, 0.01f);
		// parameterOneSlider.setRange(p.getParameterRefs().parameterOne.range.start, p.getParameterRefs().parameterOne.range.end, p.getParameterRefs().parameterOne.range.interval);
		parameterOneSlider.setSliderStyle(juce::Slider::LinearBarVertical);
		parameterOneSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
		parameterOneSlider.setPopupDisplayEnabled(true, false, this);
		parameterOneSlider.setTextValueSuffix(" Coeff ao");
		parameterOneSlider.setValue(1.f);
		addAndMakeVisible(&parameterOneSlider);
		
		parameterTwoSlider.setSliderStyle(juce::Slider::LinearBarVertical);
		parameterTwoSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
		parameterTwoSlider.setPopupDisplayEnabled(true, false, this);
		parameterTwoSlider.setTextValueSuffix(" Coeff a1");
		parameterTwoSlider.setValue(0.f);
		addAndMakeVisible(&parameterTwoSlider);

		// listeners
		// parameterOne.addListener(this);

		/*addAndMakeVisible(background);
		addAndMakeVisible(logo);*/
	}

	void PluginEditor::resized() {
		const auto bounds = getLocalBounds();

		parameterOneSlider.setBounds(40, 30, 20, getHeight() - 60);
		parameterTwoSlider.setBounds(80, 30, 20, getHeight() - 60);
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
