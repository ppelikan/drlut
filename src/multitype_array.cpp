/**
 *  Dr.LUT - Lookup Table Generator
 *
 *  Copyright (c) 2021 by ppelikan
 *  github.com/ppelikan
 **/
#include <memory>
#include <string>
#include <stdexcept>
#include <map>
#include <cmath>
#include <functional>
#include <algorithm>

#include "multitype_array.h"

template <typename... Args> // todo - replace with C++20's std::format in the future (not available yet)
std::string string_format(const std::string &format, Args... args)
{
    size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1; // extra space for '\0'
    if (size <= 0)
        throw std::runtime_error("Error during formatting.");
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1); // we don't want the '\0' inside
}

double wrap_range(double v, double min, double max)
{
    auto wmax = [](double v, double max)
    {
        return fmod(max + fmod(v, max), max);
    };
    return min + wmax(v - min, max - min);
}

void MultitypeArray::cast_overflowing()
{
    if ((type == DataType::eFLOAT) || (type == DataType::eDOUBLE))
        return;
    double min = ranges.at(type).first;
    double max = ranges.at(type).second;
    std::transform(Array.begin(), Array.end(), Array.begin(), [=](double &c)
                   { return round(wrap_range(c, min, max)); });
}

void MultitypeArray::cast_saturating()
{
    if ((type == DataType::eFLOAT) || (type == DataType::eDOUBLE))
        return;
    double min = ranges.at(type).first;
    double max = ranges.at(type).second;
    std::transform(Array.begin(), Array.end(), Array.begin(), [=](double &c)
                   {
                       double v = round(c);
                       return v > max ? max : (v < min ? min : v); });
}

MultitypeArray::MultitypeArray()
{
}

MultitypeArray::~MultitypeArray()
{
}

size_t MultitypeArray::size()
{
    return Array.size();
}

double *MultitypeArray::c_array()
{
    return &Array[0];
}

std::vector<double> &MultitypeArray::vector_double()
{
    return Array;
}

void MultitypeArray::resize(size_t size)
{
    Array.resize(size);
}

void MultitypeArray::cast(bool overflow)
{
    if (overflow)
        cast_overflowing();
    else
        cast_saturating();
}

std::string MultitypeArray::to_string(size_t columns, std::string separator)
{
    struct DataDict
    {
        const std::string decFormat;
        const std::string hexFormat;
        const int decLen;
        const int hexLen;
    };
    const std::map<DataType, DataDict> DataTypeDict{
        {DataType::eINT8, {"% 4i", "0x%02hhX", 4, 4}},
        {DataType::eINT16, {"% 6i", "0x%04hX", 6, 6}},
        {DataType::eINT32, {"% 11i", "0x%08X", 11, 10}},
        {DataType::eUINT8, {"%3u", "0x%02hhX", 3, 4}},
        {DataType::eUINT16, {"%5u", "0x%04hX", 5, 6}},
        {DataType::eUINT32, {"%10u", "0x%08X", 10, 10}},
        {DataType::eFLOAT, {"% #.10f", "%021A", 10, 21}},
        {DataType::eDOUBLE, {"% #.10f", "%021A", 10, 21}}};

    std::string format;
    int len;
    if (base == NumericBase::eDEC)
    {
        format = DataTypeDict.at(type).decFormat;
        len = columns / (DataTypeDict.at(type).decLen + separator.size());
    }
    else if (base == NumericBase::eHEX)
    {
        format = DataTypeDict.at(type).hexFormat;
        len = columns / (DataTypeDict.at(type).hexLen + separator.size());
    }

    std::function<std::string(std::string, double)> formater;

    if ((type == DataType::eFLOAT) || (type == DataType::eDOUBLE))
        formater = [](const std::string &format, double a)
        { return string_format(format, a); };
    else if (type == DataType::eUINT32)
        formater = [](const std::string &format, double a)
        { return string_format(format, static_cast<uint32_t>(a)); };
    else
        formater = [](const std::string &format, double a)
        { return string_format(format, static_cast<int32_t>(a)); };

    std::string result{""};
    int pos = -1;
    for (auto a : Array)
    {
        if (++pos >= len)
        {
            pos = 0;
            result += "\n";
        }
        result += formater(format, a) + separator;
    }
    if (base == NumericBase::eHEX)
    {
        std::replace(result.begin(), result.end(), 'X', 'x');
        std::replace(result.begin(), result.end(), 'P', 'p');
    }
    result.erase(result.size() - separator.size());
    return result;
}
