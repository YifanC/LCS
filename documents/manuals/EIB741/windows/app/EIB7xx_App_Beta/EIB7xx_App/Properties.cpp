
#include "StdAfx.h"
#include "Properties.h"

namespace EIB7xx_App
{

    #pragma warning(push)
    #pragma warning(disable: 4100)

    Object ^ PropertyEntryDescriptor::GetValue(Object ^ component)
    {
	    // Have the property bag raise an event to get the current value
	    // of the property.
	    PropertyEntryEventArgs ^ e = gcnew PropertyEntryEventArgs(item, nullptr);
        bag->OnGetValue(e);
        return e->Value;
    }

	void PropertyEntryDescriptor::SetValue(Object ^ component, Object ^ value)
	{
		// Have the property bag raise an event to set the current value
		// of the property.

		PropertyEntryEventArgs ^ e = gcnew PropertyEntryEventArgs(item, value);
		bag->OnSetValue(e);
	}

    #pragma warning(pop)

}
