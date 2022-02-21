/**
 *  Dr.LUT - Lookup Table Generator
 *
 *  Copyright (c) 2021 by ppelikan
 *  github.com/ppelikan
 **/
#pragma once

#include <vector>
#include <string>
#include "wave_generator.h"
#include "multitype_array.h"

enum class CastMethod
{
    eOVERFLOW,
    eSATURATE
};

// this class provides separation between GUI and computation logic
// also, i'm terrible at picking proper names
class LutBuilder
{
private:
    const std::map<DataType, float> DefaultAmplitude{
        {DataType::eINT8, INT8_MAX},
        {DataType::eINT16, INT16_MAX},
        {DataType::eINT32, INT32_MAX},
        {DataType::eUINT8, INT8_MAX},
        {DataType::eUINT16, INT16_MAX},
        {DataType::eUINT32, INT32_MAX},
        {DataType::eFLOAT, 1.0},
        {DataType::eDOUBLE, 1.0}};

    const std::map<DataType, float> DefaultOffset{
        {DataType::eINT8, 0.0},
        {DataType::eINT16, 0.0},
        {DataType::eINT32, 0.0},
        {DataType::eUINT8, INT8_MAX},
        {DataType::eUINT16, INT16_MAX},
        {DataType::eUINT32, INT32_MAX},
        {DataType::eFLOAT, 0.0},
        {DataType::eDOUBLE, 0.0}};

    const std::vector<std::pair<std::string, std::string>> Presets{
        {"zeros", "0"},
        {"sin", "sin(2*pi*t/T)"},
        {"cos", "cos(2*pi*t/T)"},
        {"tan", "tan(pi*t/T)"},
        {"ctg", "1/tan(pi*t/T)"},
        {"sawtooth", "2*(t%T)/T-1"},
        {"sawtooth reversed", "1-2*(t%T)/T"},
        {"triangle", "4*abs(t/T-floor(t/T+1/2))-1"},
        {"white noise", "rand"},
        {"gauss", "exp(-((2*3*t/T-3)^2)/(2*0.8^2))"},
        {"hamming", "0.5-0.4*cos((2*pi*t)/(T-1))"},
        {"hann", "0.5*(1-cos((2*pi*t)/(T-1)))"},
        {"bartlett", "1-abs((t-(T-1)/2)/((T-1)/2))"},
        {"flat top", "0.215-0.416*cos(2*pi*t/T)+0.277*cos(4*pi*t/T)+\n0.083*cos(6*pi*t/T)+0.007*cos(8*pi*t/T)"},
        {"sigmoid", "1/(1+pow(e,-(t-(T/2))/(T/20)))"}};

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
    bool formulaError{false};

    LutBuilder();
    ~LutBuilder();
    void generate();
    double *peekWaveGetTable();
    void applyDefaults();
    std::string applyPreset();
    std::vector<char> getPresetList();
    void buildLut();
};
