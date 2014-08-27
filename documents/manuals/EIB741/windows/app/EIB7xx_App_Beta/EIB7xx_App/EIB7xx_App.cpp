// EIB7xx_App.cpp: Hauptprojektdatei.

#include "stdafx.h"
#include "FormMain.h"

using namespace System;

using namespace EIB7xx_App;

#pragma warning(push)
#pragma warning(disable: 4100)

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
    // Aktivieren visueller Effekte von Windows XP, bevor Steuerelemente erstellt werden
    Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

    // possible command line arguments
    // no               - use windows language
    //                  - start standard application
    // /l:default       - use default language = en-US
    // /l:en-US         - use default language = en-US
    // /l:de-DE         - use german language
    // /app:crankshaft  - start crankshaft application
    // /pwd:diadur      - set password diadur
    // get command line arguments
    String ^ setLanguage    = L"";
    String ^ argLanguage    = L"/l:";
    String ^ setApplication = L"";
    String ^ argApplication = L"/app:";
    String ^ setPassword    = L"";
    String ^ argPassword    = L"/pwd:";
    for each(String ^ arg in args)
    {
        if (arg->StartsWith(argLanguage) == true)
        {
            setLanguage = arg->Replace(argLanguage, L"");
        }
        else if (arg->StartsWith(argApplication) == true)
        {
            setApplication = arg->Replace(argApplication, L"");
        }
        else if (arg->StartsWith(argPassword) == true)
        {
            setPassword = arg->Replace(argPassword, L"");
        }
    }

	// Hauptfenster erstellen und ausführen
	Application::Run(gcnew FormMain(setLanguage, setApplication, setPassword));
	return 0;
}

#pragma warning(pop)
