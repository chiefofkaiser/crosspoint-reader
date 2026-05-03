#pragma once

#include <string>
#include <vector>

#include "../Activity.h"

class BmpViewerActivity : public Activity {
 public:
  BmpViewerActivity(GfxRenderer& renderer, MappedInputManager& mappedInput, std::string path);

  void onEnter() override;
  void onExit() override;
  void loop() override;

 private:
  std::string filePath;
  std::string folderPath;
  std::vector<std::string> bmpFiles;
  int currentIndex = 0;

  void renderCurrentImage();
  void loadBmpFilesInFolder();
  void goToNextImage();
  void goToPreviousImage();

  static std::string getFolderPath(const std::string& path);
  static bool isBmpFile(const std::string& filename);
};