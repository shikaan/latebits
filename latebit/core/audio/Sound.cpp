#include "Sound.h"

#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_stdinc.h>

#include "latebit/core/audio/AudioManager.h"
#include "latebit/sid/parser/TuneParser.h"
#include "latebit/utils/Logger.h"

using namespace std;
using namespace sid;

namespace lb {
Sound::Sound() = default;

Sound::~Sound() {
  this->sound.reset();
  this->sound = nullptr;
}

auto Sound::play(bool loop) -> void { AM.playSound(this->sound, loop); }
auto Sound::pause() -> void { AM.pauseSound(this->sound); }
auto Sound::stop() -> void { AM.stopSound(this->sound); }

auto Sound::getLabel() const -> string { return this->label; }
auto Sound::setLabel(string l) -> void { this->label = l; }

auto Sound::loadSound(string filename) -> int {
  if (this->sound != nullptr) {
    this->sound.reset();
    this->sound = nullptr;
  }

  this->sound = TuneParser::fromFile(filename, &SOUND_PARSER_OPTIONS);

  if (this->sound == nullptr) {
    Log.error("Sound::loadSound(): Unable to load sound.");
    return -1;
  }

  return 0;
}

}  // namespace lb
