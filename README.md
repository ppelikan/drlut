# Dr LUT - Lookup Table Generator [<img alt="Dr LUT icon" src="favicon.ico" align="right"/>](https://ppelikan.github.io/drlut)

Online tool written in C/C++ that generates waveforms and exports them as arrays.

Online version:
https://ppelikan.github.io/drlut


## Features:
* live editing of waveform formula (C/C++ compatible syntax)
* available waveform presets:
  * zeros, sin, cos, tan, ctan, sawtooth, triangle, white noise, gauss, hamming, hann, bartlett, flat top, sigmoid
* available output array formats:
  * signed: int8, int16, int32, float (double)
  * unsigned: uint8, uint16, uint32
* configurable amplitude and y-axis offset
* selectable int range behaviour:
  * overflow or saturation
* selectable array output format:
  * hexadecimal or decimal


---
---


```
todo:
* add array size computation in bytes/kbytes/megabytes and widget to display it
* "warning: 'BeginPlot' is deprecated"
* fix broken comments for formulas containing `\n` characters (buildLut() function)
* NaN and INF warning should be shown if detected (before type casting)
* fix clipboard (Ctrl+C and Ctrl+V) on web target
* add include guards to generated headers
```

## Building

### Portable version (use this for debuging):

Tool installation:
```
sudo apt-get install gcc make libglfw3-dev libgles2-mesa-dev
git clone https://github.com/ocornut/imgui.git
git clone https://github.com/epezent/implot.git
```

Building:
```
cd drlut
make portable
./build/portable/app_executable
```

### Web WASM Emscripten version (for release deployment):

Tool installation:
```
sudo apt-get install cmake python3
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
git pull
./emsdk install latest
./emsdk activate latest
```

Building:
```
cd drlut
source 'path_to_your_emscripten_installation/emsdk/emsdk_env.sh'
make web
make serve
```
