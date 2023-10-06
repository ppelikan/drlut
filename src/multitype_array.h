/**
 *  Dr.LUT - Lookup Table Generator
 * 
 *  Copyright (c) 2021 by ppelikan
 *  github.com/ppelikan
**/
#pragma once
#include <vector>
#include <map>
#include <string>
#include <cstdint>

enum class DataType
{
    eINT8,
    eINT16,
    eINT32,
    eUINT8,
    eUINT16,
    eUINT32,
    eFLOAT,
    eDOUBLE
};

enum class NumericBase
{
    eDEC,
    eHEX
};

// table of floats that simulates being table of selected type
// usage:
// 1. resize() or not
// 2. input float data using: c_array() or vector_double()
// 3. cast to selected data type using: cast()
// 4. observe data using: c_array(), vector_double() or to_string()
class MultitypeArray
{
private:
    const std::map<DataType, std::pair<double, double>> ranges{
        {DataType::eINT8, {INT8_MIN, INT8_MAX}},
        {DataType::eINT16, {INT16_MIN, INT16_MAX}},
        {DataType::eINT32, {INT32_MIN, INT32_MAX}},
        {DataType::eUINT8, {0.0, UINT8_MAX}},
        {DataType::eUINT16, {0.0, UINT16_MAX}},
        {DataType::eUINT32, {0.0, UINT32_MAX}}};
    
    std::vector<double> Array{std::vector<double>(1024, 0.0f)};
    void cast_overflowing();
    void cast_saturating();

public:
    DataType type{DataType::eINT16};
    NumericBase base{NumericBase::eDEC};

    MultitypeArray();
    ~MultitypeArray();
    size_t size();
    double *c_array();
    std::vector<double> &vector_double();
    void resize(size_t size);
    void cast(bool overflow = true);
    std::string to_string(size_t columns = 52, std::string separator = ",");
};
