/**
 *  Dr.LUT - Lookup Table Generator
 * 
 *  Copyright (c) 2021 by ppelikan
 *  github.com/ppelikan
**/
#pragma once

#include <map>
#include <functional>
#include <vector>
#include <random>

enum class WaveformType
{
    eZEROS,
    eSIN,
    eCOS,
    eTAN,
    eCTG,
    eSAWTOOTH,
    eSAWTOOTHR,
    eTRIANGLE,
    eNOISE
};

struct GeneratorConfig
{
    std::vector<double> &SampleArray;
    WaveformType waveType;
    size_t samplesPerPeriod;
};

class WaveGenerator
{
private:
    static std::mt19937 *Gen;
    static const std::map<WaveformType, std::function<void(GeneratorConfig)>> dispatcher;

    static void generateZeros(GeneratorConfig cfg);
    static void generateSin(GeneratorConfig cfg);
    static void generateCos(GeneratorConfig cfg);
    static void generateTan(GeneratorConfig cfg);
    static void generateCtg(GeneratorConfig cfg);
    static void generateSawtooth(GeneratorConfig cfg);
    static void generateSawtoothRev(GeneratorConfig cfg);
    static void generateTriangle(GeneratorConfig cfg);
    static void generateNoise(GeneratorConfig cfg);

public:
    static void init();
    static void generate(GeneratorConfig config);
};
