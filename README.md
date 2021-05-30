# Dr. LUT - Lookup Table Generator

Online tool written in C/C++ that generates waveforms and exports them as arrays.

Online version:
https://ppelikan.github.io/drlut


## Features:
* avaiable waveforms:
  * zeros, sin, cos, tan, ctan, sawtooth, triangle, white noise, gauss
* avaiable output array formats:
  * signed: int8, int16, int32, float (double)
  * unsigned: uint8, uint16, uint32
* configurable amplitude and y-axis offset
* selectable range behaviour:
  * int overflow or int saturation
* selectable array output format:
  * hexadecimal or decimal
* copy to clipboard option


---
---


```
To fix:
* triangle generation sometimes exceeds requested amplitude
* provide save to file button implementation
* add array size computation in bytes/kbytes/megabytes and widget to display it
* add typical DSP window functions: Hamming, Hann, Blackman, Flat-top, Blackman-Harris etc.
* output text - horizontal scrolling missing
* plot disappears for more than 5121 datapoints (ugly workaround)
* on WASM version copy to clipboard uses ugly workaround
```

## Building

### Portable version (use this for debuging):

Tool installation:
```
sudo apt-get install gcc make
sudo apt-get install libglfw3-dev libgles2-mesa-dev
git clone https://github.com/ocornut/imgui.git
git clone https://github.com/epezent/implot.git
```

Building:
```
cd drlut
make portable
./build/portale/app_executable
```

### Web WASM Emscripten version (for release deployment):

Tool installation:
```
sudo apt-get install python3
sudo apt-get install cmake
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
