/**
 *  Dr.LUT - Lookup Table Generator
 * 
 *  Copyright (c) 2021 by ppelikan
 *  github.com/ppelikan
**/
#include <algorithm>
#include <string>
#include "lut_builder.h"

void LutBuilder::executePosprocessing()
{
    std::vector<double> &arr = Table.vector_double();
    double mux = amplitudeDouble - 0.0000000001; // temporary solution to unwanted overflows
    double off = offsetDouble;
    std::transform(arr.begin(), arr.end(), arr.begin(), [mux](double &c)
                   { return c * mux; });
    std::transform(arr.begin(), arr.end(), arr.begin(), [off](double &c)
                   { return c + off; });
}

void LutBuilder::executeTypeConversion()
{
    if (castMethod == CastMethod::eSATURATE)
        Table.cast(false);
    else
        Table.cast(true);
}

LutBuilder::LutBuilder()
{
}

LutBuilder::~LutBuilder()
{
}

void LutBuilder::generate()
{
    if (arraySize < 1)
        arraySize = 1;
    if (samplesPerPeriod < 1)
        samplesPerPeriod = 1;
    GeneratorConfig cfg{Table.vector_double(), selectedWaveType, samplesPerPeriod};
    Table.resize(arraySize);
    Generator.generate(cfg);
    executePosprocessing();
    executeTypeConversion();
}

double *LutBuilder::peekWaveGetTable()
{
    return Table.c_array();
}

void LutBuilder::buildLut()
{
    generate();

    const std::map<DataType, std::string> typeName{
        {DataType::eINT8, "int8_t"},
        {DataType::eINT16, "int16_t"},
        {DataType::eINT32, "int32_t"},
        {DataType::eUINT8, "uint8_t"},
        {DataType::eUINT16, "uint16_t"},
        {DataType::eUINT32, "uint32_t"},
        {DataType::eFLOAT, "float_t"},
        {DataType::eDOUBLE, "double_t"}};

    OutputText.clear(); //todo create default lut name from selected parameters
    OutputText = "const " + typeName.at(Table.type) + " Lut [" + std::to_string(Table.size()) + "] = { \n";
    OutputText += Table.to_string();
    OutputText += " }; \n\n";
}
