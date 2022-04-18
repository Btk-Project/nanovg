#include <SDL2/SDL.h>
#include <GLES3/gl3.h>


#define NANOVG_GLES3_IMPLEMENTATION
#define FONS_USE_FREETYPE

#include "./src/nanovg.c"
#include "./src/nanovg_gl.h"

static SDL_GLContext glcontext;
static NVGcontext *nvg;
void draw(){
    nvgBeginPath(nvg);
    nvgMoveTo(nvg, 0, 0);
    nvgLineTo(nvg, 100, 100);
    nvgLineTo(nvg, 0, 100);
    nvgClosePath(nvg);
    nvgStroke(nvg);

    nvgBeginPath(nvg);
    nvgFontSize(nvg, 12);
    nvgTextAlign(nvg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgFillColor(nvg, nvgRGBA(0, 0, 0,  255));
    nvgText(nvg,100,100,"奥LEXER萨HELLO啊LLVM世界GCC阿克",nullptr);
    nvgFill(nvg);
}

int main(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    glcontext = SDL_GL_CreateContext(window);
    nvg = nvgCreateGLES3(NVG_ANTIALIAS | NVG_DEBUG);

    int id = nvgCreateFont(nvg, "sans", "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc");

    if(id == FONS_INVALID){
        printf("Failed to load font\n");
        return 1;
    }

    SDL_Event event;
    while(SDL_WaitEvent(&event)){
        if(event.type == SDL_QUIT){
            break;
        }
        if(event.type == SDL_WINDOWEVENT){
            if(event.window.event == SDL_WINDOWEVENT_EXPOSED){
                glClearColor(1,1,1,1);
                glClearStencil(0);
                glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                nvgBeginFrame(nvg, 640, 480, 1.0f);
                nvgFontFaceId(nvg, id);
                draw();
                nvgEndFrame(nvg);
                
                SDL_GL_SwapWindow(window);
            }
        }
    }

    nvgDeleteGLES3(nvg);

    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}