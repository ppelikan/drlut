/**
 *  Dr.LUT - Lookup Table Generator
 * 
 *  Copyright (c) 2021 by ppelikan
 *  github.com/ppelikan
**/
#pragma once
#include <vector>
#include <string>

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

class MultitypeArray
{
private:
    std::vector<double> Array{std::vector<double>(1024, 0.0f)};
    void saturate(double min, double max);
    void cast_overflowing();
    void cast_saturating();
    std::string make_str(std::string format, double a);

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

