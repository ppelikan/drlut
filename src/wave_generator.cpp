/**
 *  Dr.LUT - Lookup Table Generator
 * 
 *  Copyright (c) 2019 by ppelikan
 *  github.com/ppelikan
**/
#include <algorithm>
#include <cmath>
#include "wave_generator.h"

std::mt19937 *WaveGenerator::Gen;

WaveGenerator::WaveGenerator()
{
    if (Gen != nullptr)
        return;
    std::random_device rd;        // obtain a random number from hardware
    Gen = new std::mt19937(rd()); // seed the generator
}

WaveGenerator::~WaveGenerator()
{
}

void WaveGenerator::generateZeros(GeneratorConfig cfg)
{
    std::fill(cfg.SampleArray.begin(), cfg.SampleArray.end(), 0.0f);
}

void WaveGenerator::generateSin(GeneratorConfig cfg)
{
    for (size_t i = 0; i < cfg.SampleArray.size(); ++i)
    {
        cfg.SampleArray[i] = sin((double)i * 2.0 * M_PI / (double)cfg.samplesPerPeriod);
    }
}

void WaveGenerator::generateCos(GeneratorConfig cfg)
{
    for (size_t i = 0; i < cfg.SampleArray.size(); ++i)
    {
        cfg.SampleArray[i] = cos((double)i * 2.0 * M_PI / (double)cfg.samplesPerPeriod);
    }
}

void WaveGenerator::generateTan(GeneratorConfig cfg)
{
    for (size_t i = 0; i < cfg.SampleArray.size(); ++i)
    {
        cfg.SampleArray[i] = tan((double)i * M_PI / (double)cfg.samplesPerPeriod);
    }
}

void WaveGenerator::generateCtg(GeneratorConfig cfg)
{
    for (size_t i = 0; i < cfg.SampleArray.size(); ++i)
    {
        cfg.SampleArray[i] = tan(M_PI / 2.0 - (double)i * M_PI / (double)cfg.samplesPerPeriod);
    }
}

void WaveGenerator::generateSawtooth(GeneratorConfig cfg)
{
    uint32_t pos{0};
    for (size_t i = 0; i < cfg.SampleArray.size(); ++i)
    {
        cfg.SampleArray[i] = ((double)pos / ((double)cfg.samplesPerPeriod) * 2.0) - 1.0;
        pos++;
        pos = pos % cfg.samplesPerPeriod;
    }
}

void WaveGenerator::generateSawtoothRev(GeneratorConfig cfg)
{
    uint32_t pos{0};
    for (size_t i = 0; i < cfg.SampleArray.size(); ++i)
    {
        cfg.SampleArray[i] = -((double)pos / ((double)cfg.samplesPerPeriod) * 2.0) + 1.0;
        pos++;
        pos = pos % cfg.samplesPerPeriod;
    }
}

void WaveGenerator::generateTriangle(GeneratorConfig cfg)
{
    uint32_t pos{0};
    for (size_t i = 0; i < cfg.SampleArray.size(); ++i)
    {
        cfg.SampleArray[i] = -std::abs(((double)pos / ((double)cfg.samplesPerPeriod) * 2.0) - 1.0) * 2.0 + 1.0;
        pos++;
        pos = pos % cfg.samplesPerPeriod;
    }
}

void WaveGenerator::generateNoise(GeneratorConfig cfg)
{
    std::uniform_real_distribution<double> distr(-1.0, 1.0);
    std::generate(cfg.SampleArray.begin(), cfg.SampleArray.end(), [&]()
                  { return distr(*Gen); });
}

void WaveGenerator::generate(GeneratorConfig config)
{
    dispatcher.at(config.waveType)(config);
}
