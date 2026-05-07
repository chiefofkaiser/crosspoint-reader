# CrossPoint BMP Viewer Navigation Enhancements

A custom enhancement branch for the [CrossPoint Reader Project](https://github.com/crosspoint-reader/crosspoint-reader) focused on improving BMP image viewing and navigation behavior on the XTEINK X4 / ESP32-C3 e-paper device.

This branch improves the usability of image browsing by allowing seamless navigation between BMP files and improving file browser behavior when exiting the viewer.

---

## Features Added

### Improved Back Navigation

Previously:
- Opening a BMP from the file browser and pressing **Back** returned the user to the CrossPoint main menu.

New behavior:
- Pressing **Back** now returns directly to the originating file browser folder.
- The previously viewed BMP file remains selected in the file list.

---

### Sequential BMP Navigation

While viewing a BMP image:

#### Next Image
- Side navigation buttons
- Front buttons mapped to "Next"
- PageForward actions

#### Previous Image
- Side navigation buttons
- Front buttons mapped to "Previous"
- PageBack actions

Supported input methods:
- Default front button layout
- Custom remapped front button layouts
- Side button swap settings

BMP files are automatically:
- detected from the current folder
- filtered by `.bmp` extension
- sorted alphabetically
- wrapped cyclically (last → first, first → last)

---

## Navigation Behavior

| Action | Result |
|---|---|
| Open BMP | Displays selected image |
| Back | Returns to same file in file browser |
| Next | Opens next BMP in folder |
| Previous | Opens previous BMP in folder |
| End of list | Wraps around |

---

## Technical Notes

### Device Target
- XTEINK X4
- ESP32-C3
- CrossPoint firmware

### Files Modified

```text
src/activities/util/BmpViewerActivity.cpp
src/activities/util/BmpViewerActivity.h
```

### Key Additions

- Folder-aware BMP indexing
- File list persistence
- Input remapping support
- Sequential image navigation
- Improved file browser return behavior

---

## Building

### Requirements
- VS Code
- PlatformIO
- Git
- ESP32-C3 toolchain

### Build

```bash
pio run
```

### Upload

```bash
pio run --target upload
```

---

## Branch

```text
feature/bmp-viewer-navigation
```

---

## Future Ideas

Potential future enhancements:

- Filename overlay
- Image index display (`3 / 12`)
- Slideshow mode
- Zoom / pan support
- Directory caching
- Last-viewed image persistence
- Additional image format support

---

## Credits

Built on top of the excellent [CrossPoint Reader Project](https://github.com/crosspoint-reader/crosspoint-reader) for the XTEINK X4 e-paper platform.
