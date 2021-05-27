# drlut



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
source 'path_to_your_emscripten_installation/emsdk/emsdk_env.sh'
make web
make serve
```
