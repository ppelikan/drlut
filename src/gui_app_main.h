/**
 *  Dr.LUT - Lookup Table Generator
 * 
 *  Copyright (c) 2019 by ppelikan
 *  github.com/ppelikan
**/
#pragma once

void initGUI_main();
void loopGUI_main();

/*

to fix:

triangle generation exceeds requested amplitude!
plot throws exception on big array sizes
research float vs double difference and fix code or remove selection from gui etc.
on WASM version big numbers overflowing does not work (eg tan signal)

output text horizontal scrolling missing
create default lut name from selected parameters
research plot right click menu to remove some unneeded optioins
add array size computation in bytes/kbytes/megabytes and widget to display it
on plot the line for int max not visible (outside plot area)
on WASM version copy to clipboard uses ugly workaround

done:
research 0x vs 0X in sprintf
provide saturation and overflow sselectable casting feature
in hex mode, int8 and int16 are broken (overflow to 0xffffffff)
zero values in hex mode do not have 0x prefix
decrease amplitude slightly for non float types
on WASM version - signed values are broken! no negative values are showed
no negative numbers for period or array size !
faster -+ buttons (ampl and offset)
provide downcasting feature as function with selectable (value overflow vs value saturation)
on WASM version copy to clipboard button does not work

multitype number:
przechowuje wartość jako double
posiada przeciążenia operatorów matematycznych itp, by zachowywać się jak zwykły double
można go rzutować na zwykłego double oraz posiada rozmiar jednego double

a może lepiej zrobić
multitype array dziedziczący po std::vector ?


potrzebujemy:
na pojedyńczych elementach:
operacje jak na zwykłym double

na całej tablicy elementów:
castowanie wszystkich wartości wg wybranego typu z jednoczesnym zachowaniem wyników castowania do wersji double
możliwość castowania pojedynczej wartości wg wybranego typu i następnie konwersji tego do stringa





waveform generator ---->  postprocessor  ---------> type caster -------> text generaotor
waveform type             amplitude                 data type            dec hex oct
samples per period        offset
array size

klasy:
lut generator
//odpowiada za wykonanie całego procesu
//przechowuje ustawienia

bazowa z metodami wirtualnym: generator base
pochodne: sine generator, triangle generator itd...


// Dummy(ImVec2(0, spacingY / 2));
// if (InputInt("Samples per period", reinterpret_cast<int *>(&Builder.samplesPerPeriod)))
 // if (Builder.selectedDataType == DataType::eFLOAT || Builder.selectedDataType == DataType::eDOUBLE)
     // else
        //     InputInt("Amplitude##int", (int *)&Builder.amplitudeInt); //todo fix this cast!

        // if (Builder.selectedDataType == DataType::eFLOAT || Builder.selectedDataType == DataType::eDOUBLE)
   // else
        //     InputInt("Offset##int", (int *)&Builder.offsetInt); //todo fix this cast!
  // if (InputInt("Array size", &Builder.arraySize))
                             
// struct Number
// {
//     union
//     {
//         double vFlt;
//         int32_t vInt;
//     } val;
//     bool isFlt = true;
//     double getFlt()
//     {
//         if (isFlt)
//             return val.vFlt;
//         else
//             return (double)val.vInt;
//     }
// };

    // BeginChild("child2", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    // ImVec2 textDimensions = GetContentRegionAvail();
    // textDimensions.x = 1000;

   // EndChild();
    // InputTextMultiline("output", output_text, IM_ARRAYSIZE(output_text), ImVec2(-FLT_MIN, -FLT_MIN));


*/
