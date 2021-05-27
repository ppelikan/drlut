/**
 *  Dr.LUT - Lookup Table Generator
 * 
 *  Copyright (c) 2019 by ppelikan
 *  github.com/ppelikan
**/
#include <memory>
#include <string>
#include <stdexcept>
#include <map>
#include <functional>
#include <algorithm>

#include "multitype_array.h"

template <typename... Args> // todo - replace with C++20's std::format in the future (not avaiable yet)
std::string string_format(const std::string &format, Args... args)
{
    size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1; // extra space for '\0'
    if (size <= 0)
        throw std::runtime_error("Error during formatting.");
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1); // we don't want the '\0' inside
}

void MultitypeArray::cast_overflowing()
{
    if ((type == DataType::eFLOAT) || (type == DataType::eDOUBLE))
        return;

    const std::map<DataType, std::function<void(void)>> dispatcher{
        {DataType::eINT8, [this]()
         {
             std::transform(Array.begin(), Array.end(), Array.begin(), [](double &c)
                            { return static_cast<int8_t>(c); });
         }},
        {DataType::eINT16, [this]()
         {
             std::transform(Array.begin(), Array.end(), Array.begin(), [](double &c)
                            { return static_cast<int16_t>(c); });
         }},
        {DataType::eINT32, [this]()
         { std::transform(Array.begin(), Array.end(), Array.begin(), [](double &c)
                          { return static_cast<int32_t>(c); }); }},
        {DataType::eUINT8, [this]()
         {
             std::transform(Array.begin(), Array.end(), Array.begin(), [](double &c)
                            { return static_cast<uint8_t>(c); });
         }},
        {DataType::eUINT16, [this]()
         {
             std::transform(Array.begin(), Array.end(), Array.begin(), [](double &c)
                            { return static_cast<uint16_t>(c); });
         }},
        {DataType::eUINT32, [this]()
         { std::transform(Array.begin(), Array.end(), Array.begin(), [](double &c)
                          { return static_cast<uint32_t>(c); }); }}};
    dispatcher.at(type)();
}

void MultitypeArray::saturate(double min, double max)
{
    std::transform(Array.begin(), Array.end(), Array.begin(), [=](double &c)
                   { return c > max ? max : (c < min ? min : c); });
}

void MultitypeArray::cast_saturating()
{
    if ((type == DataType::eFLOAT) || (type == DataType::eDOUBLE))
        return;
    const std::map<DataType, std::function<void(void)>> dispatcher{
        {DataType::eINT8, [this]()
         {
             saturate(INT8_MIN, INT8_MAX);
         }},
        {DataType::eINT16, [this]()
         {
             saturate(INT16_MIN, INT16_MAX);
         }},
        {DataType::eINT32, [this]()
         {
             saturate(INT32_MIN, INT32_MAX);
         }},
        {DataType::eUINT8, [this]()
         {
             saturate(0, UINT8_MAX);
         }},
        {DataType::eUINT16, [this]()
         {
             saturate(0, UINT16_MAX);
         }},
        {DataType::eUINT32, [this]()
         {
             saturate(0, UINT32_MAX);
         }}};
    dispatcher.at(type)();
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

std::string MultitypeArray::make_str(std::string format, double a)
{
    const std::map<DataType, std::function<std::string()>> disp{
        {DataType::eINT8, [format, a]()
         { return string_format(format, static_cast<int32_t>(a)); }},
        {DataType::eINT16, [format, a]()
         { return string_format(format, static_cast<int32_t>(a)); }},
        {DataType::eINT32, [format, a]()
         { return string_format(format, static_cast<int32_t>(a)); }},
        {DataType::eUINT8, [format, a]()
         { return string_format(format, static_cast<int32_t>(a)); }},
        {DataType::eUINT16, [format, a]()
         { return string_format(format, static_cast<int32_t>(a)); }},
        {DataType::eUINT32, [format, a]()
         { return string_format(format, static_cast<uint32_t>(a)); }},
        {DataType::eFLOAT, [format, a]()
         { return string_format(format, a); }},
        {DataType::eDOUBLE, [format, a]()
         { return string_format(format, a); }}};
    return disp.at(type)();
}

std::string MultitypeArray::to_string(size_t columns, std::string separator)
{
    struct DataDict
    {
        const std::string decFormat;
        const std::string hexFormat;
        const uint16_t decLen;
        const uint16_t hexLen;
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
    uint16_t len;
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

    std::string result{""};
    uint16_t pos = -1;
    if ((type == DataType::eFLOAT) || (type == DataType::eDOUBLE))
    {
        for (auto a : Array)
        {
            if (++pos >= len)
            {
                pos = 0;
                result += "\n";
            }
            result += string_format(format, a) + separator;
        }
        if (base == NumericBase::eHEX)
        {
            std::replace(result.begin(), result.end(), 'X', 'x');
            std::replace(result.begin(), result.end(), 'P', 'p');
        }
    }
    else if (type == DataType::eUINT32) // ugly! but it works fast,
        for (auto a : Array)
        {
            if (++pos >= len)
            {
                pos = 0;
                result += "\n";
            }
            result += string_format(format, static_cast<uint32_t>(a)) + separator;
        }
    else
        for (auto a : Array)
        {
            if (++pos >= len)
            {
                pos = 0;
                result += "\n";
            }
            result += string_format(format, static_cast<int32_t>(a)) + separator;
        }

    result.erase(result.size() - separator.size());
    return result;
}
