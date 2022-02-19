/**
 *  Dr.LUT - Lookup Table Generator
 *
 *  Copyright (c) 2021 by ppelikan
 *  github.com/ppelikan
 **/
#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <string.h>
#include "wave_generator.h"
#include "multitype_array.h"

enum class CastMethod
{
    eOVERFLOW,
    eSATURATE
};

// thic class provides separation between GUI and computation logic
// also, i'm terrible at picking proper names
class LutBuilder
{
private:
    void executePostprocessing();
    void executeTypeConversion();

public:
    MultitypeArray Table;
    uint32_t samplesPerPeriod{1024};
    int32_t arraySize{1024};
    CastMethod castMethod{CastMethod::eSATURATE};
    double amplitudeDouble{INT16_MAX};
    double offsetDouble{0.0};
    int selectedPreset{1};
    std::string OutputText{""};
    std::string formulaText{""};
    bool formulaError = false;

    std::vector<std::pair<std::string, std::string>> Presets{
        {"zeros", "0"},
        {"sin", "sin(t*2*pi/T)"},
        {"cos", "cos(t*2*pi/T)"},
        {"tan", "tan(t*pi/T)"},
        {"ctg", "1/tan(t*pi/T)"},
        {"sawtooth", "(t%T)/T"},
        {"sawtooth reversed", "1-(t%T)/T"},
        {"triangle", "2*abs(t/T-floor(t/T+1/2))"},
        {"white noise", "rand"},
        {"gauss", "exp(-((2*3*t/T-3)^2)/(2*0.8^2))"},
        {"hamming", "0.5-0.4*cos((2*pi*t)/(T-1))"},
        {"hann", "0.5*(1-cos((2*pi*t)/(T-1)))"},
        {"bartlett", "1-abs((t-(T-1)/2)/((T-1)/2))"},
        {"flat top", "0.215-0.416*cos(2*pi*t/T)+0.277*cos(4*pi*t/T)+\n0.083*cos(6*pi*t/T)+0.007*cos(8*pi*t/T)"},
        {"sigmoid", "1/(1+pow(e,-(t-(T/2))/(T/20)))"}};

    LutBuilder();
    ~LutBuilder();
    void generate();
    double *peekWaveGetTable();
    void buildLut();
};
