///
/// \file       Properties.h
///
/// \copyright  (C) 2002 Tony Allowatt
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \authors    Tony Allowatt \n
///             Dr. Thomas Rohn
///
/// $Id: Properties.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: Properties.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      Implementation of the ICustomTypeDescriptor interface.
///
/// This classes are used to customize the behavior of a property grid control.
/// By help of this classes the properties shown at a property grid can be modified at runtime.
/// So it will be possible to show dependencies between various properties.
///

#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Text::RegularExpressions;

namespace EIB7xx_App
{
	#pragma region "PropertyEntry class definition"
    // property index list
    typedef array<Object ^>     IndexList;
    // attribute list
    typedef array<Attribute ^>  AttributeList;

    /// <summary>
	/// Represents a single property in a PropertyEntry.
	/// </summary>
	public ref class PropertyEntry
	{
    private:
            String ^        name;
            IndexList ^     indices;
            String ^        type;
            String ^        category;
            String ^        displayName;
            bool            readOnly;
            String ^        description;
            Object ^        defaultValue;
            String ^        editor;
            String ^        typeConverter;
            
            AttributeList ^ attributes;

    public:
		/// <summary>
		/// Initializes a new instance of the PropertyEntry class.
		/// </summary>
		/// <param name="name">The name of the property displayed in the property grid.</param>
		/// <param name="type">A Type that represents the type of the property.</param>
		/// <param name="category">The category under which the property is displayed in the
		/// property grid.</param>
		/// <param name="displayName">The display name under which the property is displayed in the
		/// property grid.</param>
		/// <param name="description">A String ^ that is displayed in the help area of the
		/// property grid.</param>
		/// <param name="defaultValue">The default value of the property, or null if there is
		/// no default value.</param>
		PropertyEntry(String ^ name, Type ^ type,
                      String ^ category, String ^ displayName,
                      String ^ description, Object ^ defaultValue)
        {
            this->name          = name;
            this->indices       = nullptr;
            this->type          = type->AssemblyQualifiedName;
            this->category      = category;
            this->displayName   = displayName;
            this->readOnly      = false;
            this->description   = description;
            this->defaultValue  = defaultValue;
            this->editor        = nullptr;
            this->typeConverter = nullptr;
            this->attributes    = nullptr;
        }

		/// <summary>
		/// Initializes a new instance of the PropertyEntry class.
		/// </summary>
		/// <param name="name">The name of the property displayed in the property grid.</param>
		/// <param name="type">A Type that represents the type of the property.</param>
		/// <param name="category">The category under which the property is displayed in the
		/// property grid.</param>
		/// <param name="displayName">The display name under which the property is displayed in the
		/// property grid.</param>
		/// <param name="description">A String ^ that is displayed in the help area of the
		/// property grid.</param>
		/// <param name="defaultValue">The default value of the property, or null if there is
		/// no default value.</param>
		/// <param name="typeConverter">The Type that represents the type of the type
		/// converter for this property.  This type must derive from TypeConverter.</param>
		PropertyEntry(String ^ name, Type ^ type,
                      String ^ category, String ^ displayName,
                      String ^ description, Object ^ defaultValue,
                      Type ^ typeConverter)
        {
            this->name          = name;
            this->indices       = nullptr;
            this->type          = type->AssemblyQualifiedName;
            this->category      = category;
            this->displayName   = displayName;
            this->readOnly      = false;
            this->description   = description;
            this->defaultValue  = defaultValue;
            this->editor        = nullptr;
            this->typeConverter = typeConverter->AssemblyQualifiedName;
            this->attributes    = nullptr;
        }

		/// <summary>
		/// Initializes a new instance of the PropertyEntry class.
		/// </summary>
		/// <param name="name">The name of the property displayed in the property grid.</param>
		/// <param name="indices">The index list of the property displayed in the property grid.</param>
		/// <param name="type">A Type that represents the type of the property.</param>
		/// <param name="category">The category under which the property is displayed in the
		/// property grid.</param>
		/// <param name="displayName">The display name under which the property is displayed in the
		/// property grid.</param>
		/// <param name="description">A String ^ that is displayed in the help area of the
		/// property grid.</param>
		/// <param name="typeConverter">The Type that represents the type of the type
		/// converter for this property.  This type must derive from TypeConverter.</param>
		PropertyEntry(String ^ name, IndexList ^ indices, Type ^ type,
                      String ^ category, String ^ displayName,
                      String ^ description,
                      Type ^ typeConverter)
        {
            this->name          = name;
            this->indices       = indices;
            this->type          = type->AssemblyQualifiedName;
            this->category      = category;
            this->displayName   = displayName;
            this->readOnly      = false;
            this->description   = description;
            this->defaultValue  = nullptr;
            this->editor        = nullptr;
            this->typeConverter = typeConverter->AssemblyQualifiedName;
            this->attributes    = nullptr;
        }

		/// <summary>
		/// Initializes a new instance of the PropertyEntry class.
		/// </summary>
		/// <param name="name">The name of the property displayed in the property grid.</param>
		/// <param name="indices">The index list of the property displayed in the property grid.</param>
		/// <param name="type">A Type that represents the type of the property.</param>
		/// <param name="category">The category under which the property is displayed in the
		/// property grid.</param>
		/// <param name="displayName">The display name under which the property is displayed in the
		/// property grid.</param>
		/// <param name="description">A String ^ that is displayed in the help area of the
		/// property grid.</param>
		/// <param name="defaultValue">The default value of the property, or null if there is
		/// no default value.</param>
		/// <param name="typeConverter">The Type that represents the type of the type
		/// converter for this property.  This type must derive from TypeConverter.</param>
		PropertyEntry(String ^ name, IndexList ^ indices, Type ^ type,
                      String ^ category, String ^ displayName,
                      String ^ description, Object ^ defaultValue,
                      Type ^ typeConverter)
        {
            this->name          = name;
            this->indices       = indices;
            this->type          = type->AssemblyQualifiedName;
            this->category      = category;
            this->displayName   = displayName;
            this->readOnly      = false;
            this->description   = description;
            this->defaultValue  = defaultValue;
            this->editor        = nullptr;
            this->typeConverter = typeConverter->AssemblyQualifiedName;
            this->attributes    = nullptr;
        }

		/// <summary>
		/// Initializes a new instance of the PropertyEntry class.
		/// </summary>
		/// <param name="name">The name of the property displayed in the property grid.</param>
		/// <param name="type">A Type that represents the type of the property.</param>
		/// <param name="readOnly">The read only attribute of the property.</param>
		/// <param name="category">The category under which the property is displayed in the
		/// property grid.</param>
		/// <param name="displayName">The display name under which the property is displayed in the
		/// property grid.</param>
		/// <param name="description">A String ^ that is displayed in the help area of the
		/// property grid.</param>
		PropertyEntry(String ^ name, Type ^ type,
                      bool readOnly,
                      String ^ category, String ^ displayName,
                      String ^ description)
        {
            this->name          = name;
            this->indices       = nullptr;
            this->type          = type->AssemblyQualifiedName;
            this->category      = category;
            this->displayName   = displayName;
            this->readOnly      = readOnly;
            this->description   = description;
            this->defaultValue  = nullptr;
            this->editor        = nullptr;
            this->typeConverter = nullptr;
            this->attributes    = nullptr;
        }

		/// <summary>
		/// Initializes a new instance of the PropertyEntry class.
		/// </summary>
		/// <param name="name">The name of the property displayed in the property grid.</param>
		/// <param name="type">A Type that represents the type of the property.</param>
		/// <param name="readOnly">The read only attribute of the property.</param>
		/// <param name="category">The category under which the property is displayed in the
		/// property grid.</param>
		/// <param name="displayName">The display name under which the property is displayed in the
		/// property grid.</param>
		/// <param name="description">A String ^ that is displayed in the help area of the
		/// property grid.</param>
		/// <param name="typeConverter">The Type that represents the type of the type
		/// converter for this property.  This type must derive from TypeConverter.</param>
		PropertyEntry(String ^ name, Type ^ type,
                      bool readOnly,
                      String ^ category, String ^ displayName,
                      String ^ description,
                      Type ^ typeConverter)
        {
            this->name          = name;
            this->indices       = nullptr;
            this->type          = type->AssemblyQualifiedName;
            this->category      = category;
            this->displayName   = displayName;
            this->readOnly      = readOnly;
            this->description   = description;
            this->defaultValue  = nullptr;
            this->editor        = nullptr;
            this->typeConverter = typeConverter->AssemblyQualifiedName;
            this->attributes    = nullptr;
        }

		/// <summary>
		/// Initializes a new instance of the PropertyEntry class.
		/// </summary>
		/// <param name="name">The name of the property displayed in the property grid.</param>
		/// <param name="type">A Type that represents the type of the property.</param>
		/// <param name="readOnly">The read only attribute of the property.</param>
		/// <param name="category">The category under which the property is displayed in the
		/// property grid.</param>
		/// <param name="displayName">The display name under which the property is displayed in the
		/// property grid.</param>
		/// <param name="description">A String ^ that is displayed in the help area of the
		/// property grid.</param>
		/// <param name="defaultValue">The default value of the property, or null if there is
		/// no default value.</param>
		/// <param name="typeConverter">The Type that represents the type of the type
		/// converter for this property.  This type must derive from TypeConverter.</param>
		PropertyEntry(String ^ name, Type ^ type,
                      bool readOnly,
                      String ^ category, String ^ displayName,
                      String ^ description, Object ^ defaultValue,
                      Type ^ typeConverter)
        {
            this->name          = name;
            this->indices       = nullptr;
            this->type          = type->AssemblyQualifiedName;
            this->category      = category;
            this->displayName   = displayName;
            this->readOnly      = readOnly;
            this->description   = description;
            this->defaultValue  = defaultValue;
            this->editor        = nullptr;
            this->typeConverter = typeConverter->AssemblyQualifiedName;
            this->attributes    = nullptr;
        }

		/// <summary>
		/// Gets or sets a collection of additional Attributes for this property.  This can
		/// be used to specify attributes beyond those supported intrinsically by the
		/// PropertyEntry class, such as ReadOnly and Browsable.
		/// </summary>
		property AttributeList ^ Attributes
		{
			AttributeList ^ get() { return attributes; }
			void set(AttributeList ^ value) { attributes = (AttributeList ^)value; }
		}

		/// <summary>
		/// Gets or sets the name of this property.
		/// </summary>
		property String ^ Name
		{
			String ^ get() { return name; }
			void set(String ^ value) { name = String::Copy(value); }
		}

		/// <summary>
		/// Gets or sets the index list of this property.
		/// </summary>
		property IndexList ^ Indices
		{
			IndexList ^ get() { return indices; }
			void set(IndexList ^ value) { indices = value; }
		}

		/// <summary>
		/// Gets or sets the fully qualfied name of the type of this
		/// property.
		/// </summary>
		property String ^ TypeName
		{
			String ^ get() { return type; }
			void set(String ^ value) { type = String::Copy(value); }
		}

		/// <summary>
		/// Gets or sets the category name of this property.
		/// </summary>
		property String ^ Category
		{
			String ^ get() { return category; }
			void set(String ^ value) { category = String::Copy(value); }
		}

		/// <summary>
		/// Gets or sets the display name of this property.
		/// </summary>
		property String ^ DisplayName
		{
			String ^ get() { return displayName; }
			void set(String ^ value) { displayName = String::Copy(value); }
		}

		/// <summary>
		/// Gets or sets the read only attribute of this property.
		/// </summary>
		property bool ReadOnly
		{
			bool get() { return readOnly; }
			void set(bool value) { readOnly = value; }
		}

		/// <summary>
		/// Gets or sets the help text description of this property.
		/// </summary>
		property String ^ Description
		{
			String ^ get() { return description; }
			void set(String ^ value) { description = String::Copy(value); }
		}

		/// <summary>
		/// Gets or sets the default value of this property.
		/// </summary>
		property Object ^ DefaultValue
		{
			Object ^ get() { return defaultValue; }
			void set(Object ^ value) { defaultValue = value; }
		}

		/// <summary>
		/// Gets or sets the fully qualified name of the editor type for
		/// this property.
		/// </summary>
		property String ^ EditorTypeName
		{
			String ^ get() { return editor; }
			void set(String ^ value) { editor = String::Copy(value); }
		}

        /// <summary>
		/// Gets or sets the fully qualified name of the type converter
		/// type for this property.
		/// </summary>
		property String ^ ConverterTypeName
		{
			String ^ get() { return typeConverter; }
			void set(String ^ value) { typeConverter = String::Copy(value); }
		}

        /// <summary>
		/// Gets or sets the type converter type for this property.
		/// </summary>
		property Type ^ ConverterType
		{
			Type ^ get() { return typeConverter->GetType(); }
			void set(Type ^ value) { typeConverter = value->AssemblyQualifiedName; }
		}
    };
	#pragma endregion

	#pragma region "PropertyEntryEventArgs class definition"
    /// <summary>
	/// Provides data for the GetValue and SetValue events of the PropertyClass class.
	/// </summary>
	public ref class PropertyEntryEventArgs : public EventArgs
	{
    private:
        PropertyEntry ^ pProperty;
		Object ^ value;

    public:
		/// <summary>
		/// Initializes a new instance of the PropertyEntryEventArgs class.
		/// </summary>
		/// <param name="pProperty">The PropertyEntry that represents the property whose
		/// value is being requested or set.</param>
		/// <param name="value">The current value of the property.</param>
        PropertyEntryEventArgs(PropertyEntry ^ pProperty, Object ^ value)
		{
			this->pProperty = pProperty;
			this->value = value;
		}

		/// <summary>
		/// Gets the PropertyEntry that represents the property whose value is being
		/// requested or set.
		/// </summary>
		property PropertyEntry ^ Property
		{
			PropertyEntry ^ get() { return pProperty; }
		}

		/// <summary>
		/// Gets or sets the current value of the property.
		/// </summary>
		property Object ^ Value
		{
			Object ^ get() { return value; }
			void set(Object ^ value) { this->value = value; }
		}
    };
	#pragma endregion

    /// <summary>
	/// Represents the method that will handle the GetValue and SetValue events of the
	/// PropertyClass class.
	/// </summary>
	public delegate void PropertyEntryEventHandler(Object ^ sender, PropertyEntryEventArgs ^ e);

    #pragma region "PropertyEntryCollection class definition"
    /// <summary>
    /// Encapsulates a collection of PropertyEntry objects.
    /// </summary>
    [Serializable]
    public ref class PropertyEntryCollection : public Collections::IList
    {
    private:
        ArrayList ^ innerArray;

    public:
        /// <summary>
        /// Initializes a new instance of the PropertyEntryCollection class.
        /// </summary>
        PropertyEntryCollection()
        {
	        innerArray = gcnew ArrayList();
        }

        /// <summary>
        /// Initializes a new instance of the PropertyEntryCollection class
        /// with a flat copy of an other PropertyEntryCollection class.
        /// </summary>
        PropertyEntryCollection(PropertyEntryCollection ^ source)
        {
	        innerArray = (ArrayList ^)source->innerArray->Clone();
        }

        /// <summary>
        /// Gets the number of elements in the PropertyEntryCollection.
        /// </summary>
        /// <value>
        /// The number of elements contained in the PropertyEntryCollection.
        /// </value>
        property int Count
        {
	        virtual int get() { return innerArray->Count; } 
        }

        /// <summary>
        /// Gets a value indicating whether the PropertyEntryCollection has a fixed size.
        /// </summary>
        /// <value>
        /// true if the PropertyEntryCollection has a fixed size; otherwise, false.
        /// </value>
        property bool IsFixedSize
        {
	        virtual bool get() { return false; }
        }
		
        /// <summary>
        /// Gets a value indicating whether the PropertyEntryCollection is read-only.
        /// </summary>
        property bool IsReadOnly
        {
	        virtual bool get() { return false; }
        }

        /// <summary>
        /// Gets a value indicating whether access to the collection is synchronized (thread-safe).
        /// </summary>
        /// <value>
        /// true if access to the PropertyEntryCollection is synchronized (thread-safe); otherwise, false.
        /// </value>
        property bool IsSynchronized
        {
	        virtual bool get() { return false; }
        }

        /// <summary>
        /// Gets an object that can be used to synchronize access to the collection.
        /// </summary>
        /// <value>
        /// An object that can be used to synchronize access to the collection.
        /// </value>
        property Object ^ SyncRoot
        {
            virtual Object ^ get() = Collections::ICollection::SyncRoot::get
            { return nullptr; }
        }

        /// <summary>
        /// Gets or sets the element at the specified index.
        /// In C#, this property is the indexer for the PropertyEntryCollection class.
        /// </summary>
        /// <param name="index">The zero-based index of the element to get or set.</param>
        /// <value>
        /// The element at the specified index.
        /// </value>
        property Object ^ default[int]
        {
	        virtual Object ^ get(int index) { return innerArray[index]; }
	        virtual void set(int index, Object ^ value) { innerArray[index] = value; }
        }

        /// <summary>
        /// Adds a PropertyEntry to the end of the PropertyEntryCollection.
        /// </summary>
        /// <param name="value">The PropertyEntry to be added to the end of the PropertyEntryCollection.</param>
        /// <returns>The PropertyEntryCollection index at which the value has been added.</returns>
        virtual int Add(Object ^ value)
        {
	        int index = innerArray->Add(value);

	        return index;
        }

        /// <summary>
        /// Adds the elements of an array of PropertyEntry objects to the end of the PropertyEntryCollection.
        /// </summary>
        /// <param name="pArray">The PropertyEntry array whose elements should be added to the end of the
        /// PropertyEntryCollection.</param>
        virtual void AddRange(Collections::ICollection ^ pArray)
        {
	        innerArray->AddRange(pArray);
        }

        /// <summary>
        /// Removes all elements from the PropertySpecCollection.
        /// </summary>
        virtual void Clear()
        {
	        innerArray->Clear();
        }

        /// <summary>
        /// Determines whether a PropertySpec is in the PropertySpecCollection.
        /// </summary>
        /// <param name="item">The PropertySpec to locate in the PropertySpecCollection. The element to locate
        /// can be a null reference (Nothing in Visual Basic).</param>
        /// <returns>true if item is found in the PropertySpecCollection; otherwise, false.</returns>
        virtual bool Contains(Object ^ item)
        {
	        return innerArray->Contains(item);
        }

        /// <summary>
        /// Determines whether a PropertySpec with the specified name is in the PropertyEntryCollection.
        /// </summary>
        /// <param name="name">The name of the PropertyEntry to locate in the PropertyEntryCollection.</param>
        /// <returns>true if item is found in the PropertyEntryCollection; otherwise, false.</returns>
        virtual bool Contains(String ^ name)
        {
	        for each(PropertyEntry ^ spec in innerArray)
		        if (spec->Name == name)
			        return true;

	        return false;
        }

        /// <summary>
        /// Copies the entire PropertyEntryCollection to a compatible one-dimensional Array, starting at the
        /// beginning of the target array.
        /// </summary>
        /// <param name="pArray">The one-dimensional Array that is the destination of the elements copied
        /// from PropertyEntryCollection. The Array must have zero-based indexing.</param>
        virtual void CopyTo(Array ^ pArray)
        {
	        innerArray->CopyTo(pArray);
        }

        /// <summary>
        /// Copies the PropertyEntryCollection or a portion of it to a one-dimensional array.
        /// </summary>
        /// <param name="pArray">The one-dimensional Array that is the destination of the elements copied
        /// from the collection.</param>
        /// <param name="index">The zero-based index in array at which copying begins.</param>
        virtual void CopyTo(Array ^ pArray, int index)
        {
	        innerArray->CopyTo(pArray, index);
        }

        /// <summary>
        /// Returns an enumerator that can iterate through the PropertyEntryCollection.
        /// </summary>
        /// <returns>An IEnumerator for the entire PropertyEntryCollection.</returns>
        virtual Collections::IEnumerator ^ GetEnumerator()
        {
	        return innerArray->GetEnumerator();
        }

        /// <summary>
        /// Searches for the specified PropertyEntry and returns the zero-based index of the first
        /// occurrence within the entire PropertyEntryCollection.
        /// </summary>
        /// <param name="value">The PropertyEntry to locate in the PropertyEntryCollection.</param>
        /// <returns>The zero-based index of the first occurrence of value within the entire PropertyEntryCollection,
        /// if found; otherwise, -1.</returns>
        virtual int IndexOf(Object ^ value)
        {
	        return innerArray->IndexOf(value);
        }

        /// <summary>
        /// Searches for the PropertyEntry with the specified name and returns the zero-based index of
        /// the first occurrence within the entire PropertyEntryCollection.
        /// </summary>
        /// <param name="name">The name of the PropertyEntry to locate in the PropertyEntryCollection.</param>
        /// <returns>The zero-based index of the first occurrence of value within the entire PropertyEntryCollection,
        /// if found; otherwise, -1.</returns>
        virtual int IndexOf(String ^ name)
        {
	        int i = 0;

	        for each(PropertyEntry ^ spec in innerArray)
	        {
		        if (spec->Name == name)
			        return i;

		        i ++;
	        }

	        return -1;
        }

        /// <summary>
        /// Inserts a PropertyEntry object into the PropertyEntryCollection at the specified index.
        /// </summary>
        /// <param name="index">The zero-based index at which value should be inserted.</param>
        /// <param name="value">The PropertyEntry to insert.</param>
        virtual void Insert(int index, Object ^ value)
        {
	        innerArray->Insert(index, value);
        }

        /// <summary>
        /// Removes the first occurrence of a specific object from the PropertyEntryCollection.
        /// </summary>
        /// <param name="value">The PropertyEntry to remove from the PropertyEntryCollection.</param>
        virtual void Remove(Object ^ value)
        {
	        innerArray->Remove(value);
        }

        /// <summary>
        /// Removes the property with the specified name from the PropertyEntryCollection.
        /// </summary>
        /// <param name="name">The name of the PropertyEntry to remove from the PropertyEntryCollection.</param>
        virtual void Remove(String ^ name)
        {
	        int index = IndexOf(name);
	        RemoveAt(index);
        }

        /// <summary>
        /// Removes the object at the specified index of the PropertyEntryCollection.
        /// </summary>
        /// <param name="index">The zero-based index of the element to remove.</param>
        virtual void RemoveAt(int index)
        {
	        innerArray->RemoveAt(index);
        }

        /// <summary>
        /// Copies the elements of the PropertyEntryCollection to a new PropertyEntry array.
        /// </summary>
        /// <returns>A PropertyEntry array containing copies of the elements of the PropertyEntryCollection.</returns>
        virtual Array ^ ToArray()
        {
            return innerArray->ToArray(PropertyEntry::typeid);
        }
		
        /// <summary>
        /// Searches for the PropertyEntry with the specified name and returns the PropertyEntry of
        /// the first occurrence within the entire PropertyEntryCollection.
        /// </summary>
        /// <param name="name">The name of the PropertyEntry to locate in the PropertyEntryCollection.</param>
        /// <returns>The PropertyEntry of the first occurrence of value within the entire PropertyEntryCollection,
        /// if found; otherwise, null.</returns>
        virtual Object ^ ItemOf(String ^ name)
        {
	        int i = 0;

	        for each(PropertyEntry ^ spec in innerArray)
	        {
		        if (spec->Name == name)
			        return spec;

		        i ++;
	        }

	        return nullptr;
        }
    };
    #pragma endregion

    ref class PropertyClass;

    #pragma region "PropertyEntryDescriptor class definition"
    public ref class PropertyEntryDescriptor : public PropertyDescriptor
    {
    private:
        PropertyClass ^ bag;
	    PropertyEntry ^ item;

    public:
        PropertyEntryDescriptor(PropertyEntry ^ item, PropertyClass ^ bag,
                                String ^ name, AttributeList ^  attrs) :
		    PropertyDescriptor(name, attrs)
	    {
		    this->bag = bag;
		    this->item = item;
	    }

	    property Type ^ ComponentType
	    {
		    virtual Type ^ get() override { return item->GetType(); }
	    }

	    property bool IsReadOnly
	    {
            virtual bool get() override { return (Attributes->Matches(ReadOnlyAttribute::Yes)); }
	    }

	    property Type ^ PropertyType
	    {
            virtual Type ^ get() override { return Type::GetType(item->TypeName); }
	    }

	    virtual bool CanResetValue(Object ^ component) override
	    {
		    if (item->DefaultValue == nullptr)
			    return false;
		    else
			    return !this->GetValue(component)->Equals(item->DefaultValue);
	    }

	    virtual Object ^ GetValue(Object ^ component) override;

	    virtual void ResetValue(Object ^ component) override
	    {
		    SetValue(component, item->DefaultValue);
	    }

	    virtual void SetValue(Object ^ component, Object ^ value) override;

	    virtual bool ShouldSerializeValue(Object ^ component) override
	    {
		    Object ^ val = this->GetValue(component);

		    if (item->DefaultValue == nullptr && val == nullptr)
			    return false;
		    else
			    return !val->Equals(item->DefaultValue);
	    }
    };
    #pragma endregion

	#pragma region "PropertyClass class definition"
    /// <summary>
	/// Represents a collection of custom properties that can be selected into a
	/// PropertyGrid to provide functionality beyond that of the simple reflection
	/// normally used to query an object's properties.
	/// </summary>
	public ref class PropertyClass : public ICustomTypeDescriptor
	{
    private:
        String ^ defaultProperty;
		PropertyEntryCollection ^ properties;

    public:
		/// <summary>
		/// Initializes a new instance of the PropertyClass class.
		/// </summary>
        PropertyClass()
		{
			defaultProperty = nullptr;
			properties = gcnew PropertyEntryCollection();
		}

		/// <summary>
		/// Gets or sets the name of the default property in the collection.
		/// </summary>
		property String ^ DefaultProperty
		{
			String ^ get() { return defaultProperty; }
			void set(String ^ value) { defaultProperty = String::Copy(value); }
		}

		/// <summary>
		/// Gets the collection of properties contained within this PropertyClass.
		/// </summary>
		property PropertyEntryCollection ^ Properties
		{
			PropertyEntryCollection ^ get() { return properties; }
			void set(PropertyEntryCollection ^ value) { properties = value; }
		}

		/// <summary>
		/// Occurs when a PropertyGrid requests the value of a property.
		/// </summary>
		event PropertyEntryEventHandler ^ GetValue;

		/// <summary>
		/// Occurs when the user changes the value of a property in a PropertyGrid.
		/// </summary>
		event PropertyEntryEventHandler ^ SetValue;

		/// <summary>
		/// Raises the GetValue event.
		/// </summary>
		/// <param name="e">A PropertyEntryEventArgs that contains the event data.</param>
        virtual void OnGetValue(PropertyEntryEventArgs ^ e)
		{
			GetValue(this, e);
		}

		/// <summary>
		/// Raises the SetValue event.
		/// </summary>
		/// <param name="e">A PropertyEntryEventArgs that contains the event data.</param>
		virtual void OnSetValue(PropertyEntryEventArgs ^ e)
		{
			SetValue(this, e);
		}

		#pragma region "ICustomTypeDescriptor explicit interface definitions"
		// Most of the functions required by the ICustomTypeDescriptor are
		// merely pssed on to the default TypeDescriptor for this type,
		// which will do something appropriate.  The exceptions are noted
		// below.
        virtual AttributeCollection ^ GetAttributes() = ICustomTypeDescriptor::GetAttributes
		{
            return TypeDescriptor::GetAttributes(this, true);
		}

		virtual String ^ GetClassName() = ICustomTypeDescriptor::GetClassName
		{
            return TypeDescriptor::GetClassName(this, true);
		}

		virtual String ^ GetComponentName() = ICustomTypeDescriptor::GetComponentName
		{
            return TypeDescriptor::GetComponentName(this, true);
		}

		virtual TypeConverter ^ GetConverter() = ICustomTypeDescriptor::GetConverter
		{
            return TypeDescriptor::GetConverter(this, true);
		}

		virtual EventDescriptor ^ GetDefaultEvent() = ICustomTypeDescriptor::GetDefaultEvent
		{
            return TypeDescriptor::GetDefaultEvent(this, true);
		}

		virtual PropertyDescriptor ^ GetDefaultProperty() = ICustomTypeDescriptor::GetDefaultProperty
		{
			// This function searches the property list for the property
			// with the same name as the DefaultProperty specified, and
			// returns a property descriptor for it.  If no property is
			// found that matches DefaultProperty, a null reference is
			// returned instead.

			PropertyEntry ^ propertySpec = nullptr;
			if (defaultProperty != nullptr)
			{
				int index = properties->IndexOf(defaultProperty);
				propertySpec = (PropertyEntry ^)properties[index];
			}

			if (propertySpec != nullptr)
				return gcnew PropertyEntryDescriptor(propertySpec, this, propertySpec->Name, nullptr);
			else
				return nullptr;
		}

		virtual Object ^ GetEditor(Type ^ editorBaseType) = ICustomTypeDescriptor::GetEditor
		{
            return TypeDescriptor::GetEditor(this, editorBaseType, true);
		}

		virtual EventDescriptorCollection ^ GetEvents() = ICustomTypeDescriptor::GetEvents
		{
            return TypeDescriptor::GetEvents(this, true);
		}

		virtual EventDescriptorCollection ^ GetEvents(AttributeList ^ attributes) = ICustomTypeDescriptor::GetEvents
		{
            return TypeDescriptor::GetEvents(this, attributes, true);
		}

		virtual PropertyDescriptorCollection ^ GetProperties() = ICustomTypeDescriptor::GetProperties
		{
            return ((ICustomTypeDescriptor ^)this)->GetProperties(gcnew AttributeList(0));
		}

        #pragma warning(push)
        #pragma warning(disable: 4100)

		virtual PropertyDescriptorCollection ^ GetProperties(AttributeList ^ attributes) = ICustomTypeDescriptor::GetProperties
		{
			// Rather than passing this function on to the default TypeDescriptor,
			// which would return the actual properties of PropertyClass, I construct
			// a list here that contains property descriptors for the elements of the
			// Properties list in the bag.

			ArrayList ^ props = gcnew ArrayList();

			for each(PropertyEntry ^ pProperty in properties)
			{
				ArrayList ^ attrs = gcnew ArrayList();

				// If a category, display name, read only, description, editor, or type converter
				// are specified in the PropertyEntry, create attributes to define that relationship.
				if (pProperty->Category != nullptr)
					attrs->Add(gcnew CategoryAttribute(pProperty->Category));

				if (pProperty->DisplayName != nullptr)
					attrs->Add(gcnew DisplayNameAttribute(pProperty->DisplayName));

				if (pProperty->ReadOnly == true)
                    attrs->Add(gcnew ReadOnlyAttribute(pProperty->ReadOnly));

				if (pProperty->Description != nullptr)
					attrs->Add(gcnew DescriptionAttribute(pProperty->Description));

				if (pProperty->EditorTypeName != nullptr)
                    attrs->Add(gcnew EditorAttribute(pProperty->EditorTypeName, UITypeEditor::typeid));

				if (pProperty->ConverterTypeName != nullptr)
					attrs->Add(gcnew TypeConverterAttribute(pProperty->ConverterTypeName));

				// Additionally, append the custom attributes associated with the
				// PropertyEntry, if any.
				if (pProperty->Attributes != nullptr)
					attrs->AddRange(pProperty->Attributes);

                AttributeList ^ attrArray = (AttributeList ^)attrs->ToArray(Attribute::typeid);

				// Create a new property descriptor for the property item, and add
				// it to the list.
				PropertyEntryDescriptor ^ pd = gcnew PropertyEntryDescriptor(pProperty, this,
                                                                           pProperty->Name, attrArray);
				props->Add(pd);
			}

			// Convert the list of PropertyDescriptors to a collection that the
			// ICustomTypeDescriptor can use, and return it.
			array<PropertyDescriptor ^> ^ propArray =
                                (array<PropertyDescriptor ^> ^)props->ToArray(PropertyDescriptor::typeid);
			return gcnew PropertyDescriptorCollection(propArray);
		}

		virtual Object ^ GetPropertyOwner(PropertyDescriptor ^ pd) = ICustomTypeDescriptor::GetPropertyOwner
		{
			return this;
		}

        #pragma warning(pop)

        #pragma endregion
    };
	#pragma endregion

	#pragma region "PropertyTable class definition"
	/// <summary>
	/// An extension of PropertyClass that manages a table of property values, in
	/// addition to firing events when property values are requested or set.
	/// </summary>
	public ref class PropertyTable : public PropertyClass
	{
    private:
        Hashtable ^ propValues;

    public:
		/// <summary>
		/// Initializes a new instance of the PropertyTable class.
		/// </summary>
        PropertyTable()
		{
			propValues = gcnew Hashtable();
		}

		/// <summary>
		/// Gets or sets the value of the property with the specified name.
		/// <p>In C#, this property is the indexer of the PropertyTable class.</p>
		/// </summary>
		property Object ^ default[String ^]
		{
			Object ^ get(String ^ key) { return propValues[key]; }
			void set(String ^ key, Object ^ value) { propValues[key] = value; }
		}

		/// <summary>
		/// This member overrides PropertyClass.OnGetValue.
		/// </summary>
		virtual void OnGetValue(PropertyEntryEventArgs ^ e) override
		{
			e->Value = propValues[e->Property->Name];
            PropertyClass::OnGetValue(e);
		}

		/// <summary>
		/// This member overrides PropertyClass.OnSetValue.
		/// </summary>
		virtual void OnSetValue(PropertyEntryEventArgs ^ e) override
		{
			propValues[e->Property->Name] = e->Value;
			PropertyClass::OnSetValue(e);
		}
	};
	#pragma endregion
}
