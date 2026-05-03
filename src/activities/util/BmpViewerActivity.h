#pragma once

#include <string>

#include "../Activity.h"

class BmpViewerActivity : public Activity {
 public:
  BmpViewerActivity(GfxRenderer& renderer, MappedInputManager& mappedInput, std::string path);

  void onEnter() override;
  void onExit() override;
  void loop() override;

 private:
  std::string filePath;
};