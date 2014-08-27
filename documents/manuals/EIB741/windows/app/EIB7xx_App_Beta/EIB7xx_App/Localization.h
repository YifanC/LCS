#pragma once

#include "Defines.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Resources;
using namespace System::Reflection;

namespace EIB7xx_App
{

public ref class LocalCategoryAttribute : public CategoryAttribute
{
public:
    LocalCategoryAttribute(String ^ categoryKey) : CategoryAttribute(categoryKey)
    {
    }

protected:
    virtual String ^ GetLocalizedString(String ^ categoryKey) override
    {
        // get the resource set for the current locale.
        //
        // create resource manger for localized strings
        ResourceManager ^ resManager = GUI_Support::GetResManager();

        return resManager->GetString(categoryKey);

/*
        String ^ categoryName = null;

        // walk up the cultures until we find one with
        // this key as a resource as our category name
        // if we reach the invariant culture, quit.
        //
        for (CultureInfo culture = CultureInfo.CurrentCulture;
             categoryName == null && culture != CultureInfo.InvariantCulture;
             culture = culture.Parent)
        {
            categoryName = (string)resourceManager.GetObject(categoryKey, culture);
        }   
        return categoryName;
*/
    }
};

}