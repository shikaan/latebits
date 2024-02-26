#include "SpriteParser.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include "Colors.h"
#include "Logger.h"
#include "Sprite.h"

using namespace std;

namespace lb {
auto SpriteParser::getLine(ifstream* file) -> string {
  string line;
  getline(*file, line);
  line.erase(
    remove_if(line.begin(), line.end(), [](char c) { return c == '\r'; }),
    line.end());
  return line;
}

auto SpriteParser::parseSprite(string filename, string label) -> Sprite {
  ifstream file(filename);

  if (!file.is_open()) {
    Log.error("SpriteParser::parseSprite(): Could not open file", filename);
    return {};
  }

  // Order of these lines matters! Do not change!
  int frames = stoi(getLine(&file));
  int width = stoi(getLine(&file));
  int height = stoi(getLine(&file));
  int slowdown = stoi(getLine(&file));

  Sprite sprite(label, width, height, slowdown, frames);

  for (int i = 0; i < frames; i++) {
    if (!file.good()) {
      Log.error("SpriteParser::parseSprite(): Unexpected end of file at frame",
                i);
      return {};
    }

    vector<Color> content;
    content.reserve(width * height);

    for (int j = 0; j < height; j++) {
      auto line = getLine(&file);
      if (line.size() != width) {
        Log.error("SpriteParser::parseSprite(): Invalid line length", i,
                  "for frame", j, "expected", width, "got", line.length());
        return {};
      }

      for (char c : line) content.push_back(fromHex(c));
    }

    sprite.addFrame(Frame(width, height, content));
  }

  return sprite;
}

}  // namespace lb
