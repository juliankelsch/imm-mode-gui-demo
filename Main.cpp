//
// Created by julia on 3/9/2021.
//

#include "platform/Platform.h"

int main(int argc, char** argv)
{
    application_settings AppSettings = {};
    AppSettings.WindowWidth = 1600;
    AppSettings.WindowHeight = 900;
    AppSettings.WindowTitle = "Procedural Game";
    AppSettings.OpenGLVersionMajor = 4;
    AppSettings.OpenGLVersionMinor = 3;
    AppSettings.UseMultisampling = false;
    AppSettings.Samples = 4;
    AppSettings.UseVSync = false;

    return PlatformRunApplication(&AppSettings);
}
