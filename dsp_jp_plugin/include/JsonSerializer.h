#pragma once

namespace dsp_jp {
class JsonSerializer {
public:
  static void serialize(const Parameters&, juce::OutputStream&);

  /** @return Error message on failure; empty string otherwise.
   *           In case of error, no parameters are updated. */
  static juce::Result deserialize(juce::InputStream&, Parameters&);
};
}  // namespace dsp_jp
