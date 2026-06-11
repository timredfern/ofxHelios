# ofxHelios

openFrameworks addon for the [Helios Laser DAC](http://pages.bitlasers.com/helios/).

**v0.2.0** — Helios SDK v11, 16-bit output, IDN network DAC support.

## Features

- Background thread sends frames to the DAC without blocking your app
- `colourPolyline` extends `ofPolyline` with per-vertex colour
- Screen-space coordinates: draw with OF as normal, ofxHelios maps to the DAC
- Automatic point interpolation, corner dwell, and blanking
- USB and network (IDN protocol) DACs discovered automatically
- Optional `ofxHeliosGui` panel (requires ofxGui) for live parameter tweaking

## Requirements

- openFrameworks 0.12+
- macOS, Linux, or Windows (MSYS2)
- libusb is bundled for macOS; Linux/MSYS2 use system libusb via pkg-config

## Building the examples

```
cd example-fractalzoom
make
make RunRelease
```

Or open the project in the OF project generator and build from your IDE.

## Usage

```cpp
#include "ofxHelios.h"

ofxHelios laser;

void ofApp::setup() {
    laser.setup();            // scans USB + network, connects to first DAC
    laser.setPps(30000);
    laser.setIntensity(0.5);
}

void ofApp::draw() {
    colourPolyline line;
    line.addVertex(100, 100, ofColor::red);
    line.addVertex(500, 300, ofColor::green);
    line.addVertex(300, 500, ofColor::blue);
    line.addVertex(100, 100, ofColor::red);

    line.draw();              // preview on screen
    laser.draw(line);         // send to DAC
}
```

## Parameters

| Method | Default | Description |
|--------|---------|-------------|
| `setPps(int)` | 30000 | Points per second |
| `setIntensity(float)` | 1.0 | Master brightness (0.0-1.0) |
| `setSubdivide(int)` | 15 | Interpolation density (higher = more points between vertices) |
| `setBlankCount(int)` | 8 | Blank/dwell points at corners and between shapes |
| `setMaxAngle(float)` | 15.0 | Corner angle threshold (degrees) for inserting dwell points |
| `setOutputScale(float)` | 1.0 | Scale factor for laser output (0.0-1.0) |
| `setOutputCentre(glm::vec2)` | (0,0) | Offset the laser output centre |

## Examples

- **example-colourpolyline** — animated circles with per-vertex colour cycling
- **example-transforms** — rotating hexagons using ofMatrix4x4 transforms
- **example-fractalzoom** — infinite fractal pentagon zoom with ofxHeliosGui panel

## Known issues

- ofxGui crashes on Intel Macs running macOS Tahoe due to an OF tessellator bug. The core `ofxHelios` class is unaffected — only `ofxHeliosGui` triggers the crash. Use `ofxHelios` directly on affected machines.

## License

GPL v3 — see [LICENSE](LICENSE).
