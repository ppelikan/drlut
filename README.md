# Dr. LUT - Lookup Table Generator

Online WASM version:
https://ppelikan.github.io/drlut

---

### To fix:
* triangle generation exceeds requested amplitude!
* provide save to file button implementation
* create default lut name from selected parameters
* research plot right click menu to remove some unneeded optioins
* add array size computation in bytes/kbytes/megabytes and widget to display it
* on plot the line equal to int max is not visible (outside plot area)
* output text horizontal scrolling missing
* on WASM version copy to clipboard uses ugly workaround

### Fixed:
* research 0x vs 0X in sprintf
* provide saturation and overflow selectable casting feature
* in hex mode, int8 and int16 are broken (overflow to 0xffffffff)
* zero values in hex mode do not have 0x prefix
* decrease amplitude slightly for non float types
* on WASM version - signed values are broken! no negative values are showed
* no negative numbers for period or array size !
* faster -+ buttons (ampl and offset)
* provide downcasting feature as function with selectable (value overflow vs value saturation)
* on WASM version copy to clipboard button does not work
* on WASM version big numbers overflowing does not work (eg. tan signal)
* plot throws exception on big array sizes!

---

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
