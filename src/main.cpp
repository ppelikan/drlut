/**
 *  Dr.LUT - Lookup Table Generator
 *
 *  Copyright (c) 2021 by ppelikan
 *  github.com/ppelikan
 **/
#include <cstdio>
#include <SDL.h>

#ifdef WASM_BUILD
#include <emscripten.h>
#include <SDL_opengles2.h>
#else
#include <GLES2/gl2.h>
#endif

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

#include "gui_app_main.h"

#ifdef WASM_BUILD
EM_JS(void, js_copy_to_clipboard, (const char *str),
      {
          var contentText = document.getElementById("clipboard_text_aera");
          contentText.value = UTF8ToString(str);
          contentText.select();
          contentText.setSelectionRange(0, 99999); // For mobile devices
          document.execCommand("copy");
      });

EM_ASYNC_JS(char *, js_paste_from_clipboard, (), {
        var str = "";
        try
        {str = await navigator.clipboard.readText();}
        catch (err)
        {window.alert("Unable to paste from clipboard!\nCheck if page address starts with 'https'!\nOr use different browser.");}
        const size = lengthBytesUTF8(str) + 1;
        const rtn = _malloc(size);
        stringToUTF8(str, rtn, size);
        return rtn;
});
#endif

SDL_Window *window = NULL;
SDL_GLContext gl_context = NULL;

#ifndef WASM_BUILD
static bool Finish = false;
#endif

static void main_loop(void *);

int main(int, char **)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        #ifndef WASM_BUILD
        printf("Error: %s\n", SDL_GetError());
        #endif
        return -1;
    }

    const char *glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

#ifdef WASM_BUILD
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
#endif

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("Dr LUT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, window_flags);
    gl_context = SDL_GL_CreateContext(window);

    if (!gl_context)
    {
        #ifndef WASM_BUILD
        fprintf(stderr, "Failed to initialize SDL GL context!\n");
        #endif
        return 1;
    }

    SDL_GL_MakeCurrent(window, gl_context);

    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImPlot::GetStyle().PlotPadding = ImVec2(1, 1);
    ImPlot::GetStyle().LabelPadding = ImVec2(0, 0);
    ImPlot::GetStyle().LegendPadding = ImVec2(0, 0);
    ImPlot::GetStyle().FitPadding = ImVec2(0.05, 0.05);
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    initGUI_main();
#ifdef WASM_BUILD
    io.SetClipboardTextFn = [](void *, const char *text)
    {
        js_copy_to_clipboard(text);
    };
    io.GetClipboardTextFn = [](void *)
    {
        return (const char *)js_paste_from_clipboard();
    };

    // This function call won't return, and will engage in an infinite loop, processing events from the browser, and dispatching them.
    emscripten_set_main_loop_arg(main_loop, NULL, 0, true);
#else
    while (!Finish)
    {
        main_loop(NULL);
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    ImPlot::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
#endif
}

static void main_loop(void *arg)
{
    ImGuiIO &io = ImGui::GetIO(); // todo check if use of io here and in main() isn't redundant
    IM_UNUSED(arg);               // We can pass this argument as the second parameter of emscripten_set_main_loop_arg(), but we don't use that.

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
#ifndef WASM_BUILD
        if (event.type == SDL_QUIT)
            Finish = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            Finish = true;
#endif
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    static ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.0f);

    loopGUI_main();

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}
