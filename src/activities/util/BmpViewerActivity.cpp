#include "BmpViewerActivity.h"

#include <algorithm>
#include <cctype>

#include <Bitmap.h>
#include <GfxRenderer.h>
#include <HalStorage.h>
#include <I18n.h>

#include "components/UITheme.h"
#include "fontIds.h"

BmpViewerActivity::BmpViewerActivity(GfxRenderer& renderer, MappedInputManager& mappedInput, std::string path)
    : Activity("BmpViewer", renderer, mappedInput), filePath(std::move(path)) {}

void BmpViewerActivity::onEnter() {
  Activity::onEnter();

  folderPath = getFolderPath(filePath);
  loadBmpFilesInFolder();
  renderCurrentImage();
}

void BmpViewerActivity::renderCurrentImage() {
  FsFile file;

  const auto pageWidth = renderer.getScreenWidth();
  const auto pageHeight = renderer.getScreenHeight();
  Rect popupRect = GUI.drawPopup(renderer, tr(STR_LOADING_POPUP));
  GUI.fillPopupProgress(renderer, popupRect, 20);

  if (Storage.openFileForRead("BMP", filePath, file)) {
    Bitmap bitmap(file, true);

    if (bitmap.parseHeaders() == BmpReaderError::Ok) {
      int x, y;

      if (bitmap.getWidth() > pageWidth || bitmap.getHeight() > pageHeight) {
        float ratio = static_cast<float>(bitmap.getWidth()) / static_cast<float>(bitmap.getHeight());
        const float screenRatio = static_cast<float>(pageWidth) / static_cast<float>(pageHeight);

        if (ratio > screenRatio) {
          x = 0;
          y = std::round((static_cast<float>(pageHeight) - static_cast<float>(pageWidth) / ratio) / 2);
        } else {
          x = std::round((static_cast<float>(pageWidth) - static_cast<float>(pageHeight) * ratio) / 2);
          y = 0;
        }
      } else {
        x = (pageWidth - bitmap.getWidth()) / 2;
        y = (pageHeight - bitmap.getHeight()) / 2;
      }

      const auto labels = mappedInput.mapLabels(tr(STR_BACK), "", "Prev", "Next");
      GUI.fillPopupProgress(renderer, popupRect, 50);

      renderer.clearScreen();
      renderer.drawBitmap(bitmap, x, y, pageWidth, pageHeight, 0, 0);

      GUI.drawButtonHints(renderer, labels.btn1, labels.btn2, labels.btn3, labels.btn4);
      renderer.displayBuffer(HalDisplay::HALF_REFRESH);

    } else {
      renderer.clearScreen();
      renderer.drawCenteredText(UI_10_FONT_ID, pageHeight / 2, "Invalid BMP File");
      const auto labels = mappedInput.mapLabels(tr(STR_BACK), "", "", "");
      GUI.drawButtonHints(renderer, labels.btn1, labels.btn2, labels.btn3, labels.btn4);
      renderer.displayBuffer(HalDisplay::HALF_REFRESH);
    }

    file.close();
  } else {
    renderer.clearScreen();
    renderer.drawCenteredText(UI_10_FONT_ID, pageHeight / 2, "Could not open file");
    const auto labels = mappedInput.mapLabels(tr(STR_BACK), "", "", "");
    GUI.drawButtonHints(renderer, labels.btn1, labels.btn2, labels.btn3, labels.btn4);
    renderer.displayBuffer(HalDisplay::HALF_REFRESH);
  }
}

void BmpViewerActivity::onExit() {
  Activity::onExit();
  renderer.clearScreen();
  renderer.displayBuffer(HalDisplay::HALF_REFRESH);
}

void BmpViewerActivity::loop() {
  Activity::loop();

  if (mappedInput.wasReleased(MappedInputManager::Button::Back)) {
    activityManager.goToFileBrowser(filePath);
    return;
  }

  if (mappedInput.wasReleased(MappedInputManager::Button::Up) ||
    mappedInput.wasReleased(MappedInputManager::Button::PageBack)) {
  goToPreviousImage();
  return;
}

if (mappedInput.wasReleased(MappedInputManager::Button::Down) ||
    mappedInput.wasReleased(MappedInputManager::Button::PageForward)) {
  goToNextImage();
  return;
  }
}

void BmpViewerActivity::loadBmpFilesInFolder() {
  bmpFiles.clear();
  currentIndex = 0;

  FsFile root;

  if (!Storage.openFileForRead("BMPDIR", folderPath, root)) {
    bmpFiles.push_back(filePath);
    return;
  }

  for (auto file = root.openNextFile(); file; file = root.openNextFile()) {
    if (!file.isDirectory()) {
      char name[256];
      file.getName(name, sizeof(name));

      std::string filename = name;

      if (isBmpFile(filename)) {
        std::string fullPath = folderPath;

        if (!fullPath.empty() && fullPath.back() != '/') {
          fullPath += "/";
        }

        fullPath += filename;
        bmpFiles.push_back(fullPath);
      }
    }

    file.close();
  }

  root.close();

  std::sort(bmpFiles.begin(), bmpFiles.end());

  for (size_t i = 0; i < bmpFiles.size(); i++) {
    if (bmpFiles[i] == filePath) {
      currentIndex = static_cast<int>(i);
      break;
    }
  }

  if (bmpFiles.empty()) {
    bmpFiles.push_back(filePath);
    currentIndex = 0;
  }
}

void BmpViewerActivity::goToNextImage() {
  if (bmpFiles.empty()) {
    return;
  }

  currentIndex++;

  if (currentIndex >= static_cast<int>(bmpFiles.size())) {
    currentIndex = 0;
  }

  filePath = bmpFiles[currentIndex];
  renderCurrentImage();
}

void BmpViewerActivity::goToPreviousImage() {
  if (bmpFiles.empty()) {
    return;
  }

  currentIndex--;

  if (currentIndex < 0) {
    currentIndex = static_cast<int>(bmpFiles.size()) - 1;
  }

  filePath = bmpFiles[currentIndex];
  renderCurrentImage();
}

std::string BmpViewerActivity::getFolderPath(const std::string& path) {
  const size_t slash = path.find_last_of('/');

  if (slash == std::string::npos) {
    return "";
  }

  return path.substr(0, slash);
}

bool BmpViewerActivity::isBmpFile(const std::string& filename) {
  if (filename.length() < 4) {
    return false;
  }

  std::string lower = filename;

  std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) {
    return static_cast<char>(std::tolower(c));
  });

  return lower.substr(lower.length() - 4) == ".bmp";
}