///
/// \file       FormInfo.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: FormInfo.h,v 1.3 2011/12/16 13:37:56 rohn Exp $
/// $Name:  $
/// $Revision: 1.3 $
/// $State: Exp $
/// $Log: FormInfo.h,v $
/// Revision 1.3  2011/12/16 13:37:56  rohn
/// Version 0.9.0.1 (siehe History.txt)
///
/// Revision 1.2  2011/09/20 12:24:35  rohn
/// Version 0.3.0.2 (siehe History.txt)
///
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      Info window class.
///
/// This file will be changed by the windows forms designer.
/// Be very carefully in changing anything in this file!
///

#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace EIB7xx_App {

	/// <summary>
	/// Zusammenfassung für FormInfo
	///
	/// Warnung: Wenn Sie den Namen dieser Klasse ändern, müssen Sie auch
	///          die Ressourcendateiname-Eigenschaft für das Tool zur Kompilierung verwalteter Ressourcen ändern,
	///          das allen RESX-Dateien zugewiesen ist, von denen diese Klasse abhängt.
	///          Anderenfalls können die Designer nicht korrekt mit den lokalisierten Ressourcen
	///          arbeiten, die diesem Formular zugewiesen sind.
	/// </summary>
	public ref class FormInfo : public System::Windows::Forms::Form
	{
	public:
		FormInfo()
		{
			InitializeComponent();

            this->labelVersion->Text = L"Version: 0.9.0.1 (Beta)";
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~FormInfo()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::Button^  buttonOK;
    private: System::Windows::Forms::Label^  labelApplication;
    private: System::Windows::Forms::Label^  labelVersion;
    private: System::Windows::Forms::Label^  labelCopyright;
    private: System::Windows::Forms::PictureBox^  pictureBoxDiadur;

    protected: 

    protected: 

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent()
		{
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(FormInfo::typeid));
            this->buttonOK = (gcnew System::Windows::Forms::Button());
            this->labelApplication = (gcnew System::Windows::Forms::Label());
            this->labelVersion = (gcnew System::Windows::Forms::Label());
            this->labelCopyright = (gcnew System::Windows::Forms::Label());
            this->pictureBoxDiadur = (gcnew System::Windows::Forms::PictureBox());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxDiadur))->BeginInit();
            this->SuspendLayout();
            // 
            // buttonOK
            // 
            this->buttonOK->BackColor = System::Drawing::SystemColors::Control;
            this->buttonOK->Location = System::Drawing::Point(321, 141);
            this->buttonOK->Margin = System::Windows::Forms::Padding(4);
            this->buttonOK->Name = L"buttonOK";
            this->buttonOK->Size = System::Drawing::Size(100, 30);
            this->buttonOK->TabIndex = 0;
            this->buttonOK->Text = L"OK";
            this->buttonOK->UseVisualStyleBackColor = false;
            this->buttonOK->Click += gcnew System::EventHandler(this, &FormInfo::buttonOK_Click);
            // 
            // labelApplication
            // 
            this->labelApplication->AutoSize = true;
            this->labelApplication->Location = System::Drawing::Point(207, 31);
            this->labelApplication->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
            this->labelApplication->Name = L"labelApplication";
            this->labelApplication->Size = System::Drawing::Size(131, 17);
            this->labelApplication->TabIndex = 1;
            this->labelApplication->Text = L"EIB 7xx Application";
            // 
            // labelVersion
            // 
            this->labelVersion->AutoSize = true;
            this->labelVersion->Location = System::Drawing::Point(207, 77);
            this->labelVersion->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
            this->labelVersion->Name = L"labelVersion";
            this->labelVersion->Size = System::Drawing::Size(109, 17);
            this->labelVersion->TabIndex = 3;
            this->labelVersion->Text = L"Version: 0.1.0.1";
            // 
            // labelCopyright
            // 
            this->labelCopyright->AutoSize = true;
            this->labelCopyright->Location = System::Drawing::Point(207, 98);
            this->labelCopyright->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
            this->labelCopyright->Name = L"labelCopyright";
            this->labelCopyright->Size = System::Drawing::Size(379, 17);
            this->labelCopyright->TabIndex = 4;
            this->labelCopyright->Text = L"Copyright (C) 2011 DR. JOHANNES HEIDENHAIN GmbH";
            // 
            // pictureBoxDiadur
            // 
            this->pictureBoxDiadur->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBoxDiadur.Image")));
            this->pictureBoxDiadur->InitialImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBoxDiadur.InitialImage")));
            this->pictureBoxDiadur->Location = System::Drawing::Point(18, 18);
            this->pictureBoxDiadur->Margin = System::Windows::Forms::Padding(0);
            this->pictureBoxDiadur->Name = L"pictureBoxDiadur";
            this->pictureBoxDiadur->Size = System::Drawing::Size(167, 167);
            this->pictureBoxDiadur->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
            this->pictureBoxDiadur->TabIndex = 5;
            this->pictureBoxDiadur->TabStop = false;
            // 
            // FormInfo
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 17);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::Color::White;
            this->ClientSize = System::Drawing::Size(604, 200);
            this->Controls->Add(this->pictureBoxDiadur);
            this->Controls->Add(this->labelCopyright);
            this->Controls->Add(this->labelVersion);
            this->Controls->Add(this->labelApplication);
            this->Controls->Add(this->buttonOK);
            this->Font = (gcnew System::Drawing::Font(L"Arial", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->Margin = System::Windows::Forms::Padding(4);
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"FormInfo";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
            this->Text = L"Info";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxDiadur))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private:
        
        #pragma warning(push)
        #pragma warning(disable: 4100)
		/// <summary>
		/// Event handler for the OK button click event.
		/// </summary>
        System::Void buttonOK_Click(System::Object^  sender, System::EventArgs^  e)
        {
            this->Close();
        }
        #pragma warning(pop)
    };
}
