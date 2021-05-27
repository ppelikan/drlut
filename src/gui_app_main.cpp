/**
 *  Dr.LUT - Lookup Table Generator
 * 
 *  Copyright (c) 2019 by ppelikan
 *  github.com/ppelikan
**/
#include <ctime>
#include <map>
#include <utility>
#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"
#include "lut_builder.h"
#include "gui_app_main.h"

#ifdef WASM_BUILD
#include <emscripten.h>
EM_JS(void, js_copy_to_clipboard, (const char *str),
      {
          var copyText = document.getElementById("clipboard_text_aera");
          copyText.value = UTF8ToString(str);
          copyText.select();
          copyText.setSelectionRange(0, 99999); /* For mobile devices */
          document.execCommand("copy");
      });
#endif

std::map<DataType, float> DefaultAmplitude{
    {DataType::eINT8, INT8_MAX},
    {DataType::eINT16, INT16_MAX},
    {DataType::eINT32, INT32_MAX},
    {DataType::eUINT8, INT8_MAX},
    {DataType::eUINT16, INT16_MAX},
    {DataType::eUINT32, INT32_MAX},
    {DataType::eFLOAT, 1.0},
    {DataType::eDOUBLE, 1.0}};

std::map<DataType, float> DefaultOffset{
    {DataType::eINT8, 0.0},
    {DataType::eINT16, 0.0},
    {DataType::eINT32, 0.0},
    {DataType::eUINT8, INT8_MAX},
    {DataType::eUINT16, INT16_MAX},
    {DataType::eUINT32, INT32_MAX},
    {DataType::eFLOAT, 0.0},
    {DataType::eDOUBLE, 0.0}};

// static const float plotPadding{5.0};
// std::map<DataType, std::pair<float, float>> DefaultPlot{
//     {DataType::eINT8, {INT8_MIN - plotPadding, INT8_MAX + plotPadding}},
//     {DataType::eINT16, {INT16_MIN - plotPadding, INT16_MAX + plotPadding}},
//     {DataType::eINT32, {INT32_MIN - plotPadding * 500.0, INT32_MAX + plotPadding * 500.0}},
//     {DataType::eUINT8, {-plotPadding, UINT8_MAX + plotPadding}},
//     {DataType::eUINT16, {-plotPadding, UINT16_MAX + plotPadding}},
//     {DataType::eUINT32, {-plotPadding * 500.0, UINT32_MAX + plotPadding * 500.0}},
//     {DataType::eFLOAT, {-1.1, 1.1}},
//     {DataType::eDOUBLE, {-1.1, 1.1}}};

static const float spacingY{6.0f};
static const uint32_t step{1};
static const uint32_t stepFast{10};
// bool plotAutoFit{true};
// std::pair<float, float> plotScale{DefaultPlot[Builder.Table.type]};

LutBuilder Builder;

using namespace ImGui;

void generate()
{
    Builder.generate();
    // plotAutoFit = true;
    // ImPlot::SetNextPlotLimits(-1.0, Builder.arraySize + 1.0, DefaultPlot[Builder.Table.type].first, DefaultPlot[Builder.Table.type].second,ImGuiCond_Always );
}

void initGUI_main()
{
    // Builder.generate();;
    generate();
}

void loopGUI_main()
{
    static bool keepOnePeriod = true;
    static double numberOfPeriods = 1.0;

    const ImGuiViewport *viewport = GetMainViewport();
    SetNextWindowPos(viewport->Pos);
    SetNextWindowSize(viewport->Size);

    Begin("Dr. LUT", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

    BeginTable("tab1", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable);
    TableNextColumn();
    BeginChild("child1", ImVec2(0, 0), false);

    if (CollapsingHeader("Waveform properties", ImGuiTreeNodeFlags_DefaultOpen))
    {
        Dummy(ImVec2(0, spacingY));
        if (Combo("Waveform type", reinterpret_cast<int *>(&Builder.selectedWaveType), "Zeros\0sin\0cos\0tan\0ctg\0Sawtooth\0Sawtooth reversed\0Triangle\0White noise\0\0"))
            generate();
        if (ImGui::InputScalar("Samples per period", ImGuiDataType_U32, &Builder.samplesPerPeriod, &step, &stepFast, "%u"))
        {
            if (keepOnePeriod)
                Builder.arraySize = Builder.samplesPerPeriod;
            generate();
        }
        if (Checkbox("Generate one period", &keepOnePeriod))
        {
            if (keepOnePeriod)
                Builder.arraySize = Builder.samplesPerPeriod;
            generate();
        }

        if (!keepOnePeriod)
        {
            numberOfPeriods = (float)Builder.arraySize / (float)Builder.samplesPerPeriod;
            SameLine();
            Text(" |  Number of periods: %f", numberOfPeriods);
        }

        if (InputDouble("Amplitude##double", &Builder.amplitudeDouble, 1.0f, 10.0f, "%.8f"))
            generate();
        if (InputDouble("Offset##double", &Builder.offsetDouble, 1.0f, 10.0f, "%.8f"))
            generate();
        Dummy(ImVec2(0, spacingY));
    }
    if (CollapsingHeader("Array properties", ImGuiTreeNodeFlags_DefaultOpen))
    {
        Dummy(ImVec2(0, spacingY));
        if (Combo("Data type", reinterpret_cast<int *>(&Builder.Table.type), "int8\0int16\0int32\0uint8\0uint16\0uint32\0float\0\0"))
        {
            Builder.amplitudeDouble = DefaultAmplitude[Builder.Table.type];
            Builder.offsetDouble = DefaultOffset[Builder.Table.type];
            generate();
            // plotAutoFit = false;
            // ImPlot::SetNextPlotLimits(-1.0, Builder.arraySize + 1.0, DefaultPlot[Builder.Table.type].first, DefaultPlot[Builder.Table.type].second, ImGuiCond_Always);
        }
        if (RadioButton("overflow", reinterpret_cast<int *>(&Builder.castMethod), 0))
            generate();
        SameLine();
        if (RadioButton("saturate", reinterpret_cast<int *>(&Builder.castMethod), 1))
            generate();
        SameLine();
        ImVec2 textpos = GetContentRegionAvail();
        Dummy(ImVec2(textpos.x * 0.6666 - 68.0, 0.0f));
        SameLine();
        Text("Range behaviour");
        if (ImGui::InputScalar("Array size", ImGuiDataType_U32, &Builder.arraySize, &step, &stepFast, "%u"))
        {
            if (keepOnePeriod)
                Builder.samplesPerPeriod = Builder.arraySize;
            generate();
        }
        RadioButton("dec", reinterpret_cast<int *>(&Builder.Table.base), 0);
        SameLine();
        RadioButton("hex", reinterpret_cast<int *>(&Builder.Table.base), 1);
        SameLine();
        textpos = GetContentRegionAvail();
        Dummy(ImVec2(textpos.x * 0.6666 - 45.0, 0.0f));
        SameLine();
        Text("Numeric base");
        Dummy(ImVec2(0, spacingY));
    }

    if (CollapsingHeader("Plot preview", ImGuiTreeNodeFlags_DefaultOpen))
    {
        Dummy(ImVec2(0, spacingY / 2));
        ImVec2 plotDimensions = GetContentRegionAvail();
        if (plotDimensions.y < 50.0f)
            plotDimensions.y = 50.0f;

        // ImPlot::SetNextPlotLimits(-1.0, Builder.arraySize + 1.0, DefaultPlot[Builder.Table.type].first, DefaultPlot[Builder.Table.type].second, ImGuiCond_Always);
        ImPlotAxisFlags plotflag = ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_AutoFit;
        // if (plotAutoFit)
        //     plotflag |= ImPlotAxisFlags_AutoFit;
        ImPlotFlags plotflag2 = ImPlotFlags_NoTitle | ImPlotFlags_NoLegend;
        if (ImPlot::BeginPlot("##plot", "", "", plotDimensions, plotflag2, plotflag, plotflag))
        {
            // ImPlot::PlotLine("##plot_line", Builder.peekWaveGetTable(), Builder.arraySize);
            ImPlot::PlotStairs("##plot_line", Builder.peekWaveGetTable(), Builder.arraySize);
            ImPlot::EndPlot();
        }
    }

    EndChild();
    TableNextColumn();
    BeginChild("child2", ImVec2(0, 0), false);

    if (Button("Generate"))
    {
        Builder.buildLut();
    }
    SameLine();

    static std::time_t popupStart;
    if (Button("Copy all to clipboard"))
    {
#ifdef WASM_BUILD
        js_copy_to_clipboard(Builder.OutputText.c_str());
#else
        SetClipboardText(Builder.OutputText.c_str());
#endif
        OpenPopup("Done");
        popupStart = std::time(NULL);
    }
    if (BeginPopup("Done"))
    {
        Text(" Done! ");
        if (std::time(NULL) - popupStart > 1)
            CloseCurrentPopup();
        EndPopup();
    }

    SameLine();
    Button("Save as file");
    Spacing();
    Separator();

    InputTextMultiline("output_text", &Builder.OutputText, GetContentRegionAvail());

    EndChild();
    Dummy(ImVec2(0, spacingY));
    EndTable();
    End();
}
