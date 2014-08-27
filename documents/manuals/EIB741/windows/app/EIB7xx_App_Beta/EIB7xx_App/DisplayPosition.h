///
/// \file       DisplayPosition.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: DisplayPosition.h,v 1.2 2011/09/20 12:24:36 rohn Exp $
/// $Name:  $
/// $Revision: 1.2 $
/// $State: Exp $
/// $Log: DisplayPosition.h,v $
/// Revision 1.2  2011/09/20 12:24:36  rohn
/// Version 0.3.0.2 (siehe History.txt)
///
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB axis position data display class.
///
/// This class is used for the display of the EIB axis position
/// data via the position data panel.
///

#pragma once

#include "FormMain.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Resources;
using namespace System::Reflection;
using namespace System::Windows::Forms;

namespace EIB7xx_App
{
 	#pragma region "DisplayLed class definition"
    // special definitions
    #define LED_CTRL_COLOR_TRANS    UInt32(0)
    #define LED_CTRL_COLOR_GREEN    UInt32(1)
    #define LED_CTRL_COLOR_RED      UInt32(2)
    #define LED_CTRL_COLOR_YELLOW   UInt32(3)
    #define LED_CTRL_BLINK_ON_MS    Int32(700)
    #define LED_CTRL_BLINK_OFF_MS   Int32(300)
    #define LED_CTRL_TRIGGER_ON_MS  Int32(3000)

    /// <summary>
    /// This class is used for the display of a LED control.
    /// </summary>
    public ref class DisplayLed
    {
    private:
        ImageList ^         imageListLed;       ///< the image list for LED picture controls
        PictureBox ^        pictureBoxLed;      ///< the LED picture box

        bool                isOn;               ///< the LED on state
        bool                isBlinking;         ///< the LED blink state
        Int32               blinkPeriodMs;      ///< the LED blink period in ms
        UInt32              onColor;            ///< the LED on color index
        UInt32              offColor;           ///< the LED off color index

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="pictureBoxLed">The status LED picture box.</param>
        DisplayLed(GUI_Support ^  guiSupport,
                   PictureBox ^   pictureBoxLed)
        {
            this->imageListLed  = guiSupport->ImageListLed;
            this->pictureBoxLed = pictureBoxLed;

            pictureBoxLed->Image = imageListLed->Images[LED_CTRL_COLOR_TRANS];
            isBlinking           = false;
        }

        /// <summary>
        /// Set LED on.
        /// </summary>
		/// <param name="onColor">The LED on color index.</param>
        void SetOn(UInt32 onColor)
        {
            pictureBoxLed->Image = imageListLed->Images[onColor];
            isBlinking           = false;
        }

        /// <summary>
        /// Set LED blinking.
        /// </summary>
		/// <param name="onColor">The LED on color index.</param>
        void SetBlinking(UInt32 onColor)
        {
            SetBlinking(onColor, LED_CTRL_COLOR_TRANS);
        }

        /// <summary>
        /// Set LED blinking.
        /// </summary>
		/// <param name="onColor">The LED on color index.</param>
		/// <param name="offColor">The LED off color index.</param>
        void SetBlinking(UInt32 onColor, UInt32 offColor)
        {
            this->onColor  = onColor;
            this->offColor = offColor;
            if (isBlinking == true)
            {
                return;
            }
            pictureBoxLed->Image = imageListLed->Images[onColor];
            isOn                 = true;
            isBlinking           = true;
            blinkPeriodMs        = LED_CTRL_BLINK_ON_MS;
        }

        /// <summary>
        /// LED blink clock.
        /// </summary>
        void BlinkClock()
        {
            if (isBlinking == false)
            {
                return;
            }
            blinkPeriodMs -= EIB_DISPLAY_TIMER_PERIOD;
            if (blinkPeriodMs <= 0)
            {
                if (isOn == true)
                {
                    pictureBoxLed->Image = imageListLed->Images[offColor];
                    isOn                 = false;
                    blinkPeriodMs        = LED_CTRL_BLINK_OFF_MS;
                }
                else
                {
                    pictureBoxLed->Image = imageListLed->Images[onColor];
                    isOn                 = true;
                    blinkPeriodMs        = LED_CTRL_BLINK_ON_MS;
                }
            }
        }
    };
	#pragma endregion

 	#pragma region "DisplayPosition class definition"
    /// <summary>
    /// This class is used for the display of the EIB axis position
    /// data via the position data panel.
    /// </summary>
    public ref class DisplayPosition : public DisplayBase
    {
    private:
        ImageList ^         imageListLed;       ///< the image list for LED picture controls
        TextBox ^           textBoxPos;         ///< the position text box
        Label ^             labelUnit;          ///< the unit label
        Button ^            buttonRef;          ///< the start REF button
        Button ^            buttonClear;        ///< the clear button

        DisplayLed ^        ledCtrlStatus;      ///< the status LED
        DisplayLed ^        ledCtrlRef;         ///< the reference LED
        DisplayLed ^        ledCtrlTrigger;     ///< the trigger LED

        UInt32              triggerOnCount;     ///< trigger on read data count
        DateTime            triggerOnTime;      ///< trigger on time
        TimeSpan            triggerOnPeriod;    ///< trigger on period

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="eibTreeList">The EIB tree list.</param>
		/// <param name="textBoxPos">The position text box.</param>
		/// <param name="labelUnit">The unit label.</param>
		/// <param name="pictureBoxStatus">The status LED picture box.</param>
		/// <param name="pictureBoxRef">The reference LED picture box.</param>
		/// <param name="pictureBoxTrigger">The trigger LED picture box.</param>
		/// <param name="buttonRef">The the start REF button.</param>
		/// <param name="buttonClear">The the clear button.</param>
		/// <param name="displayAxis">The axis to display index.</param>
        DisplayPosition(GUI_Support ^  guiSupport,
                        EIB_TreeList ^ eibTreeList,
                        TextBox ^      textBoxPos,
                        Label ^        labelUnit,
                        PictureBox ^   pictureBoxStatus,
                        PictureBox ^   pictureBoxRef,
                        PictureBox ^   pictureBoxTrigger,
                        Button ^       buttonRef,
                        Button ^       buttonClear,
                        UInt32         displayAxis)
            : DisplayBase(guiSupport, eibTreeList, displayAxis)
        {
            this->imageListLed      = guiSupport->ImageListLed;
            this->textBoxPos        = textBoxPos;
            this->labelUnit         = labelUnit;
            this->buttonRef         = buttonRef;
            this->buttonClear       = buttonClear;

            triggerOnPeriod = TimeSpan(0, 0, 0, 0, LED_CTRL_TRIGGER_ON_MS);

            ledCtrlStatus  = gcnew DisplayLed(guiSupport, pictureBoxStatus);
            ledCtrlRef     = gcnew DisplayLed(guiSupport, pictureBoxRef);
            ledCtrlTrigger = gcnew DisplayLed(guiSupport, pictureBoxTrigger);

            // clear position display
            ClearDisplay();
        }

        /// <summary>
        /// Clear the position display.
        /// </summary>
        void ClearDisplay()
        {
            triggerOnCount   = 0;
            triggerOnTime    = DateTime::Now;
            textBoxPos->Text = L"";
            labelUnit->Text  = L"";
            ledCtrlStatus->SetOn(LED_CTRL_COLOR_TRANS);
            ledCtrlRef->SetOn(LED_CTRL_COLOR_TRANS);
            ledCtrlTrigger->SetOn(LED_CTRL_COLOR_TRANS);
            // disable start ref and clear buttons
            buttonRef->Enabled   = false;
            buttonClear->Enabled = false;
        }

        /// <summary>
        /// Manage the blinking controls.
        /// </summary>
        void BlinkDisplay()
        {
            ledCtrlStatus->BlinkClock();
            ledCtrlRef->BlinkClock();
            ledCtrlTrigger->BlinkClock();
        }

        /// <summary>
        /// Refresh the position display.
        /// </summary>
		/// <param name="parameter">The EIB parameters.</param>
		/// <param name="displayMode">The display mode flags.</param>
        void RefreshDisplay(EIB_Param ^ parameter, UInt32 displayMode)
        {
            // initialize flags
            bool isInit      = ((displayMode & DISPLAY_MODE_INIT_OK)   != 0);
            bool isRunning   = ((displayMode & DISPLAY_MODE_RUNNING)   != 0);
            bool isPolling   = ((displayMode & DISPLAY_MODE_POLLING)   != 0);
            bool isRecording = ((displayMode & DISPLAY_MODE_RECORDING) != 0);
            
            bool isAbsolute  = false;
            
            bool enableRef       = false;
            bool enableClear     = false;
            bool displayStatus   = false;
            bool displayRef      = false;
            bool displayTrigger  = false;
            bool displayPosition = false;
            // get data packet data from axis index
            GetDataPacketData(parameter);
            // check if axis is enabled
            if (isEnabled == true)
            {
                // enable start ref and clear buttons
                if (isInit == true)
                {
                    if (isIncremental == true)
                    {
                        buttonRef->Enabled = true;
                        enableRef          = true;
                    }
                    if (   (isRunning == false)
                        || (isPolling == true))
                    {
                        buttonClear->Enabled = true;
                        enableClear          = true;
                    }
                }
                // display trigger LED
                if (isRunning == true)
                {
                    if (warningFlags->WarningLostTriggerCount == false)
                    {
                        if (displayBuffer->ReadDataCount != triggerOnCount)
                        {
                            // trigger received
                            ledCtrlTrigger->SetOn(LED_CTRL_COLOR_GREEN);
                            triggerOnCount = displayBuffer->ReadDataCount;
                            triggerOnTime  = DateTime::Now;
                        }
                        else if (   (displayBuffer->ReadDataCount == 0)
                                 || ((DateTime::Now).Subtract(triggerOnTime) > triggerOnPeriod))
                        {
                            // waiting for trigger
                            ledCtrlTrigger->SetBlinking(LED_CTRL_COLOR_YELLOW);
                        }
                    }
                    else
                    {
                        // trigger lost
                        ledCtrlTrigger->SetBlinking(LED_CTRL_COLOR_RED);
                    }
                    displayTrigger = true;
                }
                // display status LED
                if (isRunning == true)
                {
                    if (   (   (dataPacketData->StatusWordUsed == true)
                            && (   ((  dataPacketData->StatusWordValue
                                     & (UInt16)DataPacketStatus::PositionValid) == 0)
                                || (   (isIncremental == true)
                                    && ((  dataPacketData->StatusWordValue
                                         & (UInt16)DataPacketStatus::ErrorStatusInc) != 0))
                                || (   (isIncremental == false)
                                    && ((  dataPacketData->StatusWordValue
                                         & (UInt16)DataPacketStatus::ErrorStatusEnDat) != 0))))
                        || (   (   dataPacketData->EnDatAI1Used == true)
                            && ((  dataPacketData->EnDatAI1StatusValue[0]
                                 & (UInt16)DataPacketStatus::ErrorStatusAI) != 0))
                        || (   (   dataPacketData->EnDatAI2Used == true)
                            && ((  dataPacketData->EnDatAI2StatusValue[0]
                                 & (UInt16)DataPacketStatus::ErrorStatusAI) != 0)))
                    {
                        // signal error state
                        ledCtrlStatus->SetBlinking(LED_CTRL_COLOR_RED);
                    }
                    else if (   (dataPacketData->StatusWordUsed == true)
                             || (dataPacketData->EnDatAI1Used   == true)
                             || (dataPacketData->EnDatAI2Used   == true))
                    {
                        // signal status ok
                        ledCtrlStatus->SetOn(LED_CTRL_COLOR_GREEN);
                    }
                    displayStatus = true;
                }
                // display position data
                if (   (isRunning      == true)
                    || (showLastValues == true)
                    || (isRecording    == true))
                {
                    // check if position data valid
                    if (dataPacketData->PositionValues->Valid == true)
                    {
                        // check for invalid position
                        if ((  dataPacketData->StatusWordValue
                             & (UInt16)DataPacketStatus::PositionValid) != 0)
                        {
                            textBoxPos->ForeColor = JH_COLOR_BLACK;
                        }
                        else
                        {
                            textBoxPos->ForeColor = JH_COLOR_GRAY_LIGHT;
                        }
                        // set absolute flag
                        isAbsolute = dataPacketData->PositionValues->Absolute;
                        // display position data
                        String ^ valueFormat =
                            String::Format(L"F{0}",
                                           UInt32(eibTreeList->Common->Display->DisplayPrecision));
                        if (   (dataPacketData->PositionValues->Multiturn == false)
                            || (guiSupport->GuiConfig->GuiParam->ShowMultiturn == false))
                        {
                            textBoxPos->Text =
                                    dataPacketData->PositionValues->Position.ToString(valueFormat);
                        }
                        else
                        {
                            textBoxPos->Text =
                                      dataPacketData->PositionValues->Revolution.ToString()
                                    + L" : "
                                    + dataPacketData->PositionValues->Position.ToString(valueFormat);
                        }
                        labelUnit->Text = dataPacketData->PositionValues->Unit;
                        displayPosition = true;
                    }
                }
                // display ref LED
                if (isInit == true)
                {
                    if (eibTreeList->SelectedEIB->AxisCheckRef(axisIndex, isAuxAxis) == true)
                    {
                        // waiting for reference position
                        ledCtrlRef->SetBlinking(LED_CTRL_COLOR_YELLOW);
                        displayRef = true;
                    }
                    else if (isAbsolute == true)
                    {
                        // reference position is valid
                        ledCtrlRef->SetOn(LED_CTRL_COLOR_GREEN);
                        displayRef = true;
                    }
                }
            }
            // disable / clear unused controls
            if (enableRef == false)
            {
                buttonRef->Enabled = false;
            }
            if (enableClear == false)
            {
                buttonClear->Enabled = false;
            }
            if (displayStatus == false)
            {
                ledCtrlStatus->SetOn(LED_CTRL_COLOR_TRANS);
            }
            if (displayRef == false)
            {
                ledCtrlRef->SetOn(LED_CTRL_COLOR_TRANS);
            }
            if (displayTrigger == false)
            {
                ledCtrlTrigger->SetOn(LED_CTRL_COLOR_TRANS);
            }
            if (displayPosition == false)
            {
                textBoxPos->Text = L"";
                labelUnit->Text  = L"";
            }
        }
    };
	#pragma endregion
}
