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

//todo make this class static
class WaveGenerator
{
private:
    static std::mt19937 *Gen;

    void generateZeros(GeneratorConfig cfg);
    void generateSin(GeneratorConfig cfg);
    void generateCos(GeneratorConfig cfg);
    void generateTan(GeneratorConfig cfg);
    void generateCtg(GeneratorConfig cfg);
    void generateSawtooth(GeneratorConfig cfg);
    void generateSawtoothRev(GeneratorConfig cfg);
    void generateTriangle(GeneratorConfig cfg);
    void generateNoise(GeneratorConfig cfg);

    const std::map<WaveformType, std::function<void(GeneratorConfig)>> dispatcher{
        {WaveformType::eZEROS, [&](GeneratorConfig cfg)
         { generateZeros(cfg); }},
        {WaveformType::eSIN, [&](GeneratorConfig cfg)
         { generateSin(cfg); }},
        {WaveformType::eCOS, [&](GeneratorConfig cfg)
         { generateCos(cfg); }},
        {WaveformType::eTAN, [&](GeneratorConfig cfg)
         { generateTan(cfg); }},
        {WaveformType::eCTG, [&](GeneratorConfig cfg)
         { generateCtg(cfg); }},
        {WaveformType::eSAWTOOTH, [&](GeneratorConfig cfg)
         { generateSawtooth(cfg); }},
        {WaveformType::eSAWTOOTHR, [&](GeneratorConfig cfg)
         { generateSawtoothRev(cfg); }},
        {WaveformType::eTRIANGLE, [&](GeneratorConfig cfg)
         { generateTriangle(cfg); }},
        {WaveformType::eNOISE, [&](GeneratorConfig cfg)
         { generateNoise(cfg); }}};

public:
    WaveGenerator();
    ~WaveGenerator();
    void generate(GeneratorConfig config);
};
