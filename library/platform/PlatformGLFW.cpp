#include <stdlib.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Application.h"
#include "DebugLog.h"
#include "Platform.h"

static input_form InputForm = {};
static GLFWwindow* Window = nullptr;
static bool IsFullscreen = false;
static application_settings* AppSettings;

void PlatformExit()
{
    glfwSetWindowShouldClose(Window, GLFW_TRUE);
}

void PlatformToggleFullscreen()
{
    IsFullscreen = !IsFullscreen;
    const GLFWvidmode* Mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if(IsFullscreen)
    {
        glfwSetWindowMonitor(Window, glfwGetPrimaryMonitor(), 0, 0, Mode->width, Mode->height, Mode->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(Window, nullptr, (Mode->width - AppSettings->WindowWidth) / 2, (Mode->height - AppSettings->WindowHeight) / 2, AppSettings->WindowWidth, AppSettings->WindowHeight, 0);
    }
}

void* PlatformAllocateMemory(u32 NumberOfBytes)
{
    return malloc(NumberOfBytes);
}

void* PlatformAllocateMemoryZeroInitialized(u32 NumberOfBytes)
{
    return calloc(1, NumberOfBytes);
}

void* PlatformReallocateMemory(void* Memory, u32 NumberOfBytes)
{
    return realloc(Memory, NumberOfBytes);
}

void PlatformFreeMemory(void* Memory)
{
    free(Memory);
}

static void GLFWScrollCallback(GLFWwindow* Window, double X, double Y)
{
    InputForm.ScrollX = X;
    InputForm.ScrollY = Y;
}

static void GLFWCharCallback(GLFWwindow* window, unsigned int codepoint)
{

}

s32 PlatformRunApplication(application_settings* Settings)
{
    AppSettings = Settings;
    if(!glfwInit())
    {
        LogError("GLFW could not be initialized!\n");
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Settings->OpenGLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Settings->OpenGLVersionMinor);
    if(Settings->UseMultisampling)
        glfwWindowHint(GLFW_SAMPLES, Settings->Samples);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    if(DEBUG && Settings->OpenGLVersionMajor >= 4 && Settings->OpenGLVersionMinor >= 3)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    Window = glfwCreateWindow(Settings->WindowWidth, Settings->WindowHeight, Settings->WindowTitle, IsFullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

    if(!Window)
    {
        LogError("GLFW window could not be created!\n");
        return 1;
    }

    glfwMakeContextCurrent(Window);

    glfwSetScrollCallback(Window, GLFWScrollCallback);
    glfwSetCharCallback(Window, GLFWCharCallback);

    if(Settings->UseVSync)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }

    if(!gladLoadGL())
        return 1;

    while(!glfwWindowShouldClose(Window))
    {
        glfwPollEvents();

        glfwGetCursorPos(Window, &InputForm.MouseX, &InputForm.MouseY);
        glfwGetFramebufferSize(Window, &InputForm.FramebufferWidth, &InputForm.FramebufferHeight);
        InputForm.Time = glfwGetTime();

        InputForm.MouseButtonsPressed[MouseButtonLeft] = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        InputForm.MouseButtonsPressed[MouseButtonRight] = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
        InputForm.MouseButtonsPressed[MouseButtonMiddle] = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;

#define MAP2(Key, KeyGLFW) (InputForm.KeysPressed[Key] = glfwGetKey(Window, KeyGLFW) == GLFW_PRESS)
#define MAP(X) (InputForm.KeysPressed[Key##X] = glfwGetKey(Window, GLFW_KEY_##X) == GLFW_PRESS)

        MAP(A); MAP(B); MAP(C); MAP(D); MAP(E); MAP(F); MAP(G); MAP(H); MAP(I); MAP(J); MAP(K); MAP(L); MAP(M);
        MAP(N); MAP(O); MAP(P); MAP(Q); MAP(R); MAP(S); MAP(T); MAP(U); MAP(V); MAP(W); MAP(X); MAP(Y); MAP(Z);

        MAP(F1);MAP(F2);MAP(F3);MAP(F4);MAP(F5);MAP(F6);MAP(F7);MAP(F8);MAP(F9);MAP(F10);MAP(F11);MAP(F12);
        MAP(0);MAP(1);MAP(2);MAP(3);MAP(4);MAP(5);MAP(6);MAP(7);MAP(8);MAP(9);
        MAP2(KeySpace, GLFW_KEY_SPACE);
        MAP2(KeyTab, GLFW_KEY_TAB);
        MAP2(KeyEsc, GLFW_KEY_ESCAPE);
        MAP2(KeyEnter, GLFW_KEY_ENTER);
        MAP2(KeyBackspace, GLFW_KEY_BACKSPACE);

        ApplicationUpdateAndRender(&InputForm);

        glfwSwapBuffers(Window);
    }

    ApplicationShutdown();

    glfwDestroyWindow(Window);
    glfwTerminate();

    return 0;
}
