/**
 *  Dr.LUT - Lookup Table Generator
 *
 *  Copyright (c) 2021 by ppelikan
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
          var contentText = document.getElementById("clipboard_text_aera");
          contentText.value = UTF8ToString(str);
          contentText.select();
          contentText.setSelectionRange(0, 99999); // For mobile devices
          document.execCommand("copy");
      });

EM_JS(void, js_save_as_file, (const char *str),
      {
          var downloadableLink = document.createElement('a');
          downloadableLink.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(UTF8ToString(str)));
          downloadableLink.download = "lut.h";
          document.body.appendChild(downloadableLink);
          downloadableLink.click();
          document.body.removeChild(downloadableLink);
      });
#else
#include <fstream>
#include "ImGuiFileDialog.h"
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

static const float spacingY{6.0f};
static const uint32_t step{1};
static const uint32_t stepFast{10};
static const int sizeMax{100000};

LutBuilder Builder;

using namespace ImGui;

#ifndef WASM_BUILD
void saveToFile(std::string name)
{
    std::ofstream outfile;
    outfile.open(name, std::ios_base::app);
    outfile << Builder.OutputText;
    outfile.close();
}
#endif

void generate()
{
    if (Builder.arraySize > sizeMax)
        Builder.arraySize = sizeMax;
    Builder.generate();
}

void initGUI_main()
{
    generate();
}

void popup(bool render_now = false)
{
    static std::time_t popupStart;
    static bool trigger;
    // simple "state machine" to show popup just for one second
    if (!render_now)
    {
        trigger = true;
        popupStart = std::time(NULL);
        return;
    }
    if (trigger)
    {
        OpenPopup("Done");
        trigger = false;
    }
    if (BeginPopup("Done"))
    {
        Text(" Done! ");
        if (std::time(NULL) - popupStart > 1)
            CloseCurrentPopup();
        EndPopup();
    }
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
        if (Combo("Waveform type", reinterpret_cast<int *>(&Builder.selectedWaveType), "zeros\0sin\0cos\0tan\0ctg\0sawtooth\0sawtooth reversed\0triangle\0white noise\0gauss\0\0"))
            generate();
        if (ImGui::InputScalar("Samples per period", ImGuiDataType_U32, &Builder.samplesPerPeriod, &step, &stepFast, "%u"))
        {
            if (Builder.samplesPerPeriod > sizeMax)
                keepOnePeriod = false;
            if (keepOnePeriod)
                Builder.arraySize = Builder.samplesPerPeriod;
            generate();
        }
        if (Checkbox("Generate one period", &keepOnePeriod))
        {
            if (keepOnePeriod)
            {
                if (Builder.samplesPerPeriod > sizeMax)
                    Builder.samplesPerPeriod = sizeMax;
                Builder.arraySize = Builder.samplesPerPeriod;
            }
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
            if (Builder.arraySize > sizeMax)
                Builder.arraySize = sizeMax;
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

        ImPlotAxisFlags plotflag = ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_AutoFit;
        ImPlotFlags plotflag2 = ImPlotFlags_NoTitle | ImPlotFlags_NoLegend;
        if (ImPlot::BeginPlot("##plot", "", "", plotDimensions, plotflag2, plotflag, plotflag))
        {
            if (Builder.arraySize <= 4000) // todo - drawing more datapoints is currently not supported
                ImPlot::SetNextMarkerStyle(ImPlotMarker_Square, 2.0f);
            if (Builder.arraySize <= 5121) // todo - drawing more datapoints is currently not supported
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

    if (Button("Copy all to clipboard"))
    {
#ifdef WASM_BUILD
        js_copy_to_clipboard(Builder.OutputText.c_str());
#else
        SetClipboardText(Builder.OutputText.c_str());
#endif
        popup();
    }
    SameLine();
    if (Button("Save as file"))
    {
#ifdef WASM_BUILD
        js_save_as_file(Builder.OutputText.c_str());
        popup();
#else
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File Name", ".h,.hpp,.c,.cpp,.txt,.", ".h");
#endif
    }

    Spacing();
    Separator();

    InputTextMultiline("output_text", &Builder.OutputText, GetContentRegionAvail());

    EndChild();
    Dummy(ImVec2(0, spacingY));
    EndTable();
    End();
#ifndef WASM_BUILD
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, ImVec2((float)WINDOW_W / 1.5, (float)WINDOW_H / 2.0), ImVec2((float)WINDOW_W, (float)WINDOW_H)))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            saveToFile(ImGuiFileDialog::Instance()->GetFilePathName());
            popup();
        }
        ImGuiFileDialog::Instance()->Close();
    }
#endif
    popup(true);
}