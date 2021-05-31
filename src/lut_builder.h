/**
 *  Dr.LUT - Lookup Table Generator
 * 
 *  Copyright (c) 2021 by ppelikan
 *  github.com/ppelikan
**/
#pragma once

#include <cstdint>
#include <string>
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
    WaveformType selectedWaveType{WaveformType::eSIN};
    std::string OutputText{""};

    LutBuilder();
    ~LutBuilder();
    void generate();
    double *peekWaveGetTable();
    void buildLut();
};
