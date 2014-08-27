#pragma once

using namespace System;
using namespace System::Drawing;

namespace EIB7xx_App
{
    #define RESOURCE_FILE               L"EIB7xx_App.WinFormStrings"

    #define ZERO_DOUBLE                 (1e-9)

    /// <summary>
    /// This enum lists all possible GUI commands.
    /// </summary>
    enum class GuiCommand : UInt32
    {
        ResetEib,
        UpdateEib,
        SaveRecordingData
    };

    /// <summary>
    /// This enum lists all possible application project states.
    /// </summary>
    enum class ProjectState : UInt32
    {
        NoProject,
        ProjectSet,
        ProjectChanged
    };

    /// <summary>
    /// This enum lists all possible recording measurement states.
    /// </summary>
    enum class RecordingState : UInt32
    {
        Init,
        Empty,
        Measurement,
        Display
    };

    /// <summary>
    /// This enum lists all possible recording measurement state trigers.
    /// </summary>
    enum class RecordingTrigger : UInt32
    {
        Init,
        Start,
        Stop,
        Save,
        TabPage
    };

    #define JH_COLOR_GREEN_STD          Color::FromArgb((255 << 24) + (176 << 16) + (203 << 8) + ( 36 << 0))
    #define JH_COLOR_GRAY_LIGHT         Color::FromArgb((255 << 24) + (239 << 16) + (239 << 8) + (239 << 0))
    #define JH_COLOR_WHITE              Color::White
    #define JH_COLOR_BLACK              Color::Black
    #define JH_COLOR_RED                Color::Red

    #define REPORT_COLOR_TEXT           Color::Black
    #define REPORT_COLOR_WARNING        Color::Blue
    #define REPORT_COLOR_ERROR          Color::Red
    #define REPORT_COLOR_DEBUG          Color::Green

    /// <summary>
    /// This enum lists all possible report styles.
    /// </summary>
    enum class ReportStyle : UInt32
    {
        Information,
        Warning,
        Error,
        Debug
    };

}
