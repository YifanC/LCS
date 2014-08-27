<?xml version='1.0'?>
<Project Type="Project" LVVersion="8008005">
	<Item Name="Mein Computer" Type="My Computer">
		<Property Name="CCSymbols" Type="Str">OS,Win;CPU,x86;</Property>
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">Mein Computer/VI-Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">Mein Computer/VI-Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="AuxAxis" Type="Folder">
			<Item Name="AuxAxis.vi" Type="VI" URL="AuxAxis/AuxAxis.vi"/>
		</Item>
		<Item Name="EIB7Info" Type="Folder">
			<Item Name="subvis" Type="Folder">
				<Item Name="IPAddressFormat.vi" Type="VI" URL="EIB7Info/subvis/IPAddressFormat.vi"/>
				<Item Name="IPAddressToUInt.vi" Type="VI" URL="EIB7Info/subvis/IPAddressToUInt.vi"/>
			</Item>
			<Item Name="GetConnInfo.vi" Type="VI" URL="EIB7Info/GetConnInfo.vi"/>
			<Item Name="GetNetworkSettings.vi" Type="VI" URL="EIB7Info/GetNetworkSettings.vi"/>
			<Item Name="SetNetworkSettings.vi" Type="VI" URL="EIB7Info/SetNetworkSettings.vi"/>
		</Item>
		<Item Name="Polling" Type="Folder">
			<Item Name="PollEncoderData.vi" Type="VI" URL="Polling/PollEncoderData.vi"/>
			<Item Name="PollIncrEnDatPositions.vi" Type="VI" URL="Polling/PollIncrEnDatPositions.vi"/>
			<Item Name="PollPositions.vi" Type="VI" URL="Polling/PollPositions.vi"/>
		</Item>
		<Item Name="Recording" Type="Folder">
			<Item Name="Recording.vi" Type="VI" URL="Recording/Recording.vi"/>
		</Item>
		<Item Name="SoftRealtime" Type="Folder">
			<Item Name="SoftRealtime.vi" Type="VI" URL="SoftRealtime/SoftRealtime.vi"/>
			<Item Name="SoftRealtime_EnDat.vi" Type="VI" URL="SoftRealtime/SoftRealtime_EnDat.vi"/>
		</Item>
		<Item Name="Streaming" Type="Folder">
			<Item Name="Streaming.vi" Type="VI" URL="Streaming/Streaming.vi"/>
		</Item>
		<Item Name="Trigger" Type="Folder">
			<Item Name="ExtTriggerOutput.vi" Type="VI" URL="Trigger/ExtTriggerOutput.vi"/>
			<Item Name="IntervalCounter.vi" Type="VI" URL="Trigger/IntervalCounter.vi"/>
			<Item Name="PulsCounter.vi" Type="VI" URL="Trigger/PulsCounter.vi"/>
			<Item Name="RI_Trigger.vi" Type="VI" URL="Trigger/RI_Trigger.vi"/>
		</Item>
		<Item Name="Abhängigkeiten" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
				<Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
				<Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
				<Item Name="Simple Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Simple Error Handler.vi"/>
				<Item Name="DialogType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogType.ctl"/>
				<Item Name="General Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler.vi"/>
				<Item Name="DialogTypeEnum.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogTypeEnum.ctl"/>
				<Item Name="General Error Handler CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler CORE.vi"/>
				<Item Name="Check Special Tags.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Check Special Tags.vi"/>
				<Item Name="TagReturnType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/TagReturnType.ctl"/>
				<Item Name="Set String Value.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set String Value.vi"/>
				<Item Name="GetRTHostConnectedProp.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetRTHostConnectedProp.vi"/>
				<Item Name="Error Code Database.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Code Database.vi"/>
				<Item Name="Format Message String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Format Message String.vi"/>
				<Item Name="Find Tag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find Tag.vi"/>
				<Item Name="Search and Replace Pattern.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Search and Replace Pattern.vi"/>
				<Item Name="Set Bold Text.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set Bold Text.vi"/>
				<Item Name="Details Display Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Details Display Dialog.vi"/>
				<Item Name="ErrWarn.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/ErrWarn.ctl"/>
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="eventvkey.ctl" Type="VI" URL="/&lt;vilib&gt;/event_ctls.llb/eventvkey.ctl"/>
				<Item Name="Not Found Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Not Found Dialog.vi"/>
				<Item Name="Three Button Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog.vi"/>
				<Item Name="Three Button Dialog CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog CORE.vi"/>
				<Item Name="Longest Line Length in Pixels.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Longest Line Length in Pixels.vi"/>
				<Item Name="Convert property node font to graphics font.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Convert property node font to graphics font.vi"/>
				<Item Name="Get Text Rect.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Get Text Rect.vi"/>
				<Item Name="BuildHelpPath.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/BuildHelpPath.vi"/>
				<Item Name="GetHelpDir.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetHelpDir.vi"/>
				<Item Name="Get String Text Bounds.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Get String Text Bounds.vi"/>
				<Item Name="LVBoundsTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVBoundsTypeDef.ctl"/>
			</Item>
			<Item Name="eib7.lvlib" Type="Library" URL="../eib7.lvlib">
				<Item Name="Menus" Type="Folder">
					<Item Name="eib7_top.mnu" Type="Document" URL="../eib7_top.mnu"/>
					<Item Name="eib7_device.mnu" Type="Document" URL="../eib7_device.mnu"/>
					<Item Name="eib7_io.mnu" Type="Document" URL="../eib7_io.mnu"/>
					<Item Name="eib7_axis.mnu" Type="Document" URL="../eib7_axis.mnu"/>
					<Item Name="eib7_driver.mnu" Type="Document" URL="../eib7_driver.mnu"/>
					<Item Name="eib7_auxaxis.mnu" Type="Document" URL="../eib7_auxaxis.mnu"/>
				</Item>
				<Item Name="SubVIs" Type="Folder">
					<Item Name="Error Converter (ErrCode or Status).vi" Type="VI" URL="../subvi/Error Converter (ErrCode or Status).vi"/>
					<Item Name="EIB7GetDataFieldPtr.vi" Type="VI" URL="../subvi/EIB7GetDataFieldPtr.vi"/>
					<Item Name="EIB7GetDataFieldPtrRaw.vi" Type="VI" URL="../subvi/EIB7GetDataFieldPtrRaw.vi"/>
				</Item>
				<Item Name="Typedefs" Type="Folder">
					<Item Name="EIB7_AxisRefnum.ctl" Type="VI" URL="../Typedefs/EIB7_AxisRefnum.ctl"/>
					<Item Name="EIB7_AxisTriggerSrc.ctl" Type="VI" URL="../Typedefs/EIB7_AxisTriggerSrc.ctl"/>
					<Item Name="EIB7_Bandwidth.ctl" Type="VI" URL="../Typedefs/EIB7_Bandwidth.ctl"/>
					<Item Name="EIB7_BootMode.ctl" Type="VI" URL="../Typedefs/EIB7_BootMode.ctl"/>
					<Item Name="EIB7_ConnectionStatus.ctl" Type="VI" URL="../Typedefs/EIB7_ConnectionStatus.ctl"/>
					<Item Name="EIB7_DataPacketSection.ctl" Type="VI" URL="../Typedefs/EIB7_DataPacketSection.ctl"/>
					<Item Name="EIB7_DataRegion.ctl" Type="VI" URL="../Typedefs/EIB7_DataRegion.ctl"/>
					<Item Name="EIB7_EIBRefnum.ctl" Type="VI" URL="../Typedefs/EIB7_EIBRefnum.ctl"/>
					<Item Name="EIB7_EncoderSettings.ctl" Type="VI" URL="../Typedefs/EIB7_EncoderSettings.ctl"/>
					<Item Name="EIB7_EncoderType.ctl" Type="VI" URL="../Typedefs/EIB7_EncoderType.ctl"/>
					<Item Name="EIB7_EnDatCalcTime.ctl" Type="VI" URL="../Typedefs/EIB7_EnDatCalcTime.ctl"/>
					<Item Name="EIB7_EnDatRecoveryTime.ctl" Type="VI" URL="../Typedefs/EIB7_EnDatRecoveryTime.ctl"/>
					<Item Name="EIB7_Firmware.ctl" Type="VI" URL="../Typedefs/EIB7_Firmware.ctl"/>
					<Item Name="EIB7_InterfaceClock.ctl" Type="VI" URL="../Typedefs/EIB7_InterfaceClock.ctl"/>
					<Item Name="EIB7_InterfaceType.ctl" Type="VI" URL="../Typedefs/EIB7_InterfaceType.ctl"/>
					<Item Name="EIB7_IOMode.ctl" Type="VI" URL="../Typedefs/EIB7_IOMode.ctl"/>
					<Item Name="EIB7_IORefnum.ctl" Type="VI" URL="../Typedefs/EIB7_IORefnum.ctl"/>
					<Item Name="EIB7_OperatingMode.ctl" Type="VI" URL="../Typedefs/EIB7_OperatingMode.ctl"/>
					<Item Name="EIB7_OutputTriggerSrc.ctl" Type="VI" URL="../Typedefs/EIB7_OutputTriggerSrc.ctl"/>
					<Item Name="EIB7_PositionDataField.ctl" Type="VI" URL="../Typedefs/EIB7_PositionDataField.ctl"/>
					<Item Name="EIB7_PulsCounterStart.ctl" Type="VI" URL="../Typedefs/EIB7_PulsCounterStart.ctl"/>
					<Item Name="EIB7_PulsCounterTrigger.ctl" Type="VI" URL="../Typedefs/EIB7_PulsCounterTrigger.ctl"/>
					<Item Name="EIB7_ReferencePosition.ctl" Type="VI" URL="../Typedefs/EIB7_ReferencePosition.ctl"/>
					<Item Name="EIB7_Refmarks.ctl" Type="VI" URL="../Typedefs/EIB7_Refmarks.ctl"/>
					<Item Name="EIB7_RITriggerEdge.ctl" Type="VI" URL="../Typedefs/EIB7_RITriggerEdge.ctl"/>
					<Item Name="EIB7_SoftwareTrigger.ctl" Type="VI" URL="../Typedefs/EIB7_SoftwareTrigger.ctl"/>
					<Item Name="EIB7_TriggerSource.ctl" Type="VI" URL="../Typedefs/EIB7_TriggerSource.ctl"/>
					<Item Name="EIB7_UpdateStatus.ctl" Type="VI" URL="../Typedefs/EIB7_UpdateStatus.ctl"/>
					<Item Name="EIB7_InterpolationFactor.ctl" Type="VI" URL="../Typedefs/EIB7_InterpolationFactor.ctl"/>
					<Item Name="EIB7_EdgeResolution.ctl" Type="VI" URL="../Typedefs/EIB7_EdgeResolution.ctl"/>
					<Item Name="EIB7_IntervalCounterMode.ctl" Type="VI" URL="../Typedefs/EIB7_IntervalCounterMode.ctl"/>
					<Item Name="EIB7_IntervalCounterStart.ctl" Type="VI" URL="../Typedefs/EIB7_IntervalCounterStart.ctl"/>
				</Item>
				<Item Name="eib7.dll" Type="Document" URL="../eib7.dll"/>
				<Item Name="EIB7AuxAxisTriggerSource.vi" Type="VI" URL="../VIs/EIB7AuxAxisTriggerSource.vi"/>
				<Item Name="EIB7AuxClearCounter.vi" Type="VI" URL="../VIs/EIB7AuxClearCounter.vi"/>
				<Item Name="EIB7AuxClearLostTriggerError.vi" Type="VI" URL="../VIs/EIB7AuxClearLostTriggerError.vi"/>
				<Item Name="EIB7AuxClearRefStatus.vi" Type="VI" URL="../VIs/EIB7AuxClearRefStatus.vi"/>
				<Item Name="EIB7AuxClearSignalErrors.vi" Type="VI" URL="../VIs/EIB7AuxClearSignalErrors.vi"/>
				<Item Name="EIB7AuxGetEncoderData.vi" Type="VI" URL="../VIs/EIB7AuxGetEncoderData.vi"/>
				<Item Name="EIB7AuxGetPosition.vi" Type="VI" URL="../VIs/EIB7AuxGetPosition.vi"/>
				<Item Name="EIB7AuxGetRefActive.vi" Type="VI" URL="../VIs/EIB7AuxGetRefActive.vi"/>
				<Item Name="EIB7AuxSetRITriggerEdge.vi" Type="VI" URL="../VIs/EIB7AuxSetRITriggerEdge.vi"/>
				<Item Name="EIB7AuxSetTimestamp.vi" Type="VI" URL="../VIs/EIB7AuxSetTimestamp.vi"/>
				<Item Name="EIB7AuxStartRef.vi" Type="VI" URL="../VIs/EIB7AuxStartRef.vi"/>
				<Item Name="EIB7AuxStopRef.vi" Type="VI" URL="../VIs/EIB7AuxStopRef.vi"/>
				<Item Name="EIB7AxisTriggerSource.vi" Type="VI" URL="../VIs/EIB7AxisTriggerSource.vi"/>
				<Item Name="EIB7ClearCounter.vi" Type="VI" URL="../VIs/EIB7ClearCounter.vi"/>
				<Item Name="EIB7ClearEncoderErrors.vi" Type="VI" URL="../VIs/EIB7ClearEncoderErrors.vi"/>
				<Item Name="EIB7ClearEnDatErrorMsg.vi" Type="VI" URL="../VIs/EIB7ClearEnDatErrorMsg.vi"/>
				<Item Name="EIB7ClearFIFO.vi" Type="VI" URL="../VIs/EIB7ClearFIFO.vi"/>
				<Item Name="EIB7ClearLostTriggerError.vi" Type="VI" URL="../VIs/EIB7ClearLostTriggerError.vi"/>
				<Item Name="EIB7ClearPowerSupplyError.vi" Type="VI" URL="../VIs/EIB7ClearPowerSupplyError.vi"/>
				<Item Name="EIB7ClearRefLatched.vi" Type="VI" URL="../VIs/EIB7ClearRefLatched.vi"/>
				<Item Name="EIB7ClearRefStatus.vi" Type="VI" URL="../VIs/EIB7ClearRefStatus.vi"/>
				<Item Name="EIB7Close.vi" Type="VI" URL="../VIs/EIB7Close.vi"/>
				<Item Name="EIB7ConfigDataPacket.vi" Type="VI" URL="../VIs/EIB7ConfigDataPacket.vi"/>
				<Item Name="EIB7ConfigPulsCounter.vi" Type="VI" URL="../VIs/EIB7ConfigPulsCounter.vi"/>
				<Item Name="EIB7ConnectionInfo.vi" Type="VI" URL="../VIs/EIB7ConnectionInfo.vi"/>
				<Item Name="EIB7EnableIncrementalTermination.vi" Type="VI" URL="../VIs/EIB7EnableIncrementalTermination.vi"/>
				<Item Name="EIB7EnDat21GetPosition.vi" Type="VI" URL="../VIs/EIB7EnDat21GetPosition.vi"/>
				<Item Name="EIB7EnDat21ReadMem.vi" Type="VI" URL="../VIs/EIB7EnDat21ReadMem.vi"/>
				<Item Name="EIB7EnDat21ReadTestValue.vi" Type="VI" URL="../VIs/EIB7EnDat21ReadTestValue.vi"/>
				<Item Name="EIB7EnDat21ResetEncoder.vi" Type="VI" URL="../VIs/EIB7EnDat21ResetEncoder.vi"/>
				<Item Name="EIB7EnDat21SelectMemRange.vi" Type="VI" URL="../VIs/EIB7EnDat21SelectMemRange.vi"/>
				<Item Name="EIB7EnDat21WriteMem.vi" Type="VI" URL="../VIs/EIB7EnDat21WriteMem.vi"/>
				<Item Name="EIB7EnDat21WriteTestCommand.vi" Type="VI" URL="../VIs/EIB7EnDat21WriteTestCommand.vi"/>
				<Item Name="EIB7EnDat22ErrorReset.vi" Type="VI" URL="../VIs/EIB7EnDat22ErrorReset.vi"/>
				<Item Name="EIB7EnDat22GetPosition.vi" Type="VI" URL="../VIs/EIB7EnDat22GetPosition.vi"/>
				<Item Name="EIB7EnDat22ReadMem.vi" Type="VI" URL="../VIs/EIB7EnDat22ReadMem.vi"/>
				<Item Name="EIB7EnDat22SelectMemRange.vi" Type="VI" URL="../VIs/EIB7EnDat22SelectMemRange.vi"/>
				<Item Name="EIB7EnDat22SetAddInfo.vi" Type="VI" URL="../VIs/EIB7EnDat22SetAddInfo.vi"/>
				<Item Name="EIB7EnDat22SetAddInfoCycle.vi" Type="VI" URL="../VIs/EIB7EnDat22SetAddInfoCycle.vi"/>
				<Item Name="EIB7EnDat22WriteMem.vi" Type="VI" URL="../VIs/EIB7EnDat22WriteMem.vi"/>
				<Item Name="EIB7EnDat22WriteTestCommand.vi" Type="VI" URL="../VIs/EIB7EnDat22WriteTestCommand.vi"/>
				<Item Name="EIB7FIFOEntryCount.vi" Type="VI" URL="../VIs/EIB7FIFOEntryCount.vi"/>
				<Item Name="EIB7GetAxis.vi" Type="VI" URL="../VIs/EIB7GetAxis.vi"/>
				<Item Name="EIB7GetBootMode.vi" Type="VI" URL="../VIs/EIB7GetBootMode.vi"/>
				<Item Name="EIB7GetConnectionStatus.vi" Type="VI" URL="../VIs/EIB7GetConnectionStatus.vi"/>
				<Item Name="EIB7GetDataFieldAnalog.vi" Type="VI" URL="../VIs/EIB7GetDataFieldAnalog.vi"/>
				<Item Name="EIB7GetDataFieldEnDat_AI1.vi" Type="VI" URL="../VIs/EIB7GetDataFieldEnDat_AI1.vi"/>
				<Item Name="EIB7GetDataFieldEnDat_AI2.vi" Type="VI" URL="../VIs/EIB7GetDataFieldEnDat_AI2.vi"/>
				<Item Name="EIB7GetDataFieldPOS.vi" Type="VI" URL="../VIs/EIB7GetDataFieldPOS.vi"/>
				<Item Name="EIB7GetDataFieldRaw.vi" Type="VI" URL="../VIs/EIB7GetDataFieldRaw.vi"/>
				<Item Name="EIB7GetDataFieldREF.vi" Type="VI" URL="../VIs/EIB7GetDataFieldREF.vi"/>
				<Item Name="EIB7GetDataFieldREFCoded.vi" Type="VI" URL="../VIs/EIB7GetDataFieldREFCoded.vi"/>
				<Item Name="EIB7GetDataFieldStatusWord.vi" Type="VI" URL="../VIs/EIB7GetDataFieldStatusWord.vi"/>
				<Item Name="EIB7GetDataFieldTimestamp.vi" Type="VI" URL="../VIs/EIB7GetDataFieldTimestamp.vi"/>
				<Item Name="EIB7GetDataFieldTriggerCounter.vi" Type="VI" URL="../VIs/EIB7GetDataFieldTriggerCounter.vi"/>
				<Item Name="EIB7GetDataFieldU16.vi" Type="VI" URL="../VIs/EIB7GetDataFieldU16.vi"/>
				<Item Name="EIB7GetDataFieldU32.vi" Type="VI" URL="../VIs/EIB7GetDataFieldU32.vi"/>
				<Item Name="EIB7GetDriverID.vi" Type="VI" URL="../VIs/EIB7GetDriverID.vi"/>
				<Item Name="EIB7GetEncoderData.vi" Type="VI" URL="../VIs/EIB7GetEncoderData.vi"/>
				<Item Name="EIB7GetErrorInfo.vi" Type="VI" URL="../VIs/EIB7GetErrorInfo.vi"/>
				<Item Name="EIB7GetFIFOSize.vi" Type="VI" URL="../VIs/EIB7GetFIFOSize.vi"/>
				<Item Name="EIB7GetHostIP.vi" Type="VI" URL="../VIs/EIB7GetHostIP.vi"/>
				<Item Name="EIB7GetHostname.vi" Type="VI" URL="../VIs/EIB7GetHostname.vi"/>
				<Item Name="EIB7GetIdentNumber.vi" Type="VI" URL="../VIs/EIB7GetIdentNumber.vi"/>
				<Item Name="EIB7GetInputConfig.vi" Type="VI" URL="../VIs/EIB7GetInputConfig.vi"/>
				<Item Name="EIB7GetIO.vi" Type="VI" URL="../VIs/EIB7GetIO.vi"/>
				<Item Name="EIB7GetMAC.vi" Type="VI" URL="../VIs/EIB7GetMAC.vi"/>
				<Item Name="EIB7GetNetwork.vi" Type="VI" URL="../VIs/EIB7GetNetwork.vi"/>
				<Item Name="EIB7GetNumberOfOpenConnections.vi" Type="VI" URL="../VIs/EIB7GetNumberOfOpenConnections.vi"/>
				<Item Name="EIB7GetOutputConfig.vi" Type="VI" URL="../VIs/EIB7GetOutputConfig.vi"/>
				<Item Name="EIB7GetPosition.vi" Type="VI" URL="../VIs/EIB7GetPosition.vi"/>
				<Item Name="EIB7GetPowerSupplyStatus.vi" Type="VI" URL="../VIs/EIB7GetPowerSupplyStatus.vi"/>
				<Item Name="EIB7GetRecordingMemSize.vi" Type="VI" URL="../VIs/EIB7GetRecordingMemSize.vi"/>
				<Item Name="EIB7GetRecordingStatus.vi" Type="VI" URL="../VIs/EIB7GetRecordingStatus.vi"/>
				<Item Name="EIB7GetRefActive.vi" Type="VI" URL="../VIs/EIB7GetRefActive.vi"/>
				<Item Name="EIB7GetSerialNumber.vi" Type="VI" URL="../VIs/EIB7GetSerialNumber.vi"/>
				<Item Name="EIB7GetStreamingStatus.vi" Type="VI" URL="../VIs/EIB7GetStreamingStatus.vi"/>
				<Item Name="EIB7GetTimerTriggerTicks.vi" Type="VI" URL="../VIs/EIB7GetTimerTriggerTicks.vi"/>
				<Item Name="EIB7GetTimestampTicks.vi" Type="VI" URL="../VIs/EIB7GetTimestampTicks.vi"/>
				<Item Name="EIB7GetTriggerDelayTicks.vi" Type="VI" URL="../VIs/EIB7GetTriggerDelayTicks.vi"/>
				<Item Name="EIB7GetVersion.vi" Type="VI" URL="../VIs/EIB7GetVersion.vi"/>
				<Item Name="EIB7GlobalTriggerEnable.vi" Type="VI" URL="../VIs/EIB7GlobalTriggerEnable.vi"/>
				<Item Name="EIB7Identify.vi" Type="VI" URL="../VIs/EIB7Identify.vi"/>
				<Item Name="EIB7IncrPosToDouble.vi" Type="VI" URL="../VIs/EIB7IncrPosToDouble.vi"/>
				<Item Name="EIB7InitAxis.vi" Type="VI" URL="../VIs/EIB7InitAxis.vi"/>
				<Item Name="EIB7InitInput.vi" Type="VI" URL="../VIs/EIB7InitInput.vi"/>
				<Item Name="EIB7InitOutput.vi" Type="VI" URL="../VIs/EIB7InitOutput.vi"/>
				<Item Name="EIB7MasterTriggerSource.vi" Type="VI" URL="../VIs/EIB7MasterTriggerSource.vi"/>
				<Item Name="EIB7Open.vi" Type="VI" URL="../VIs/EIB7Open.vi"/>
				<Item Name="EIB7OutputTriggerSource.vi" Type="VI" URL="../VIs/EIB7OutputTriggerSource.vi"/>
				<Item Name="EIB7ReadEnDatIncrPos.vi" Type="VI" URL="../VIs/EIB7ReadEnDatIncrPos.vi"/>
				<Item Name="EIB7ReadFIFOData.vi" Type="VI" URL="../VIs/EIB7ReadFIFOData.vi"/>
				<Item Name="EIB7ReadFIFODataRaw.vi" Type="VI" URL="../VIs/EIB7ReadFIFODataRaw.vi"/>
				<Item Name="EIB7ReadIO.vi" Type="VI" URL="../VIs/EIB7ReadIO.vi"/>
				<Item Name="EIB7Reset.vi" Type="VI" URL="../VIs/EIB7Reset.vi"/>
				<Item Name="EIB7ResetTimestamp.vi" Type="VI" URL="../VIs/EIB7ResetTimestamp.vi"/>
				<Item Name="EIB7ResetTriggerCounter.vi" Type="VI" URL="../VIs/EIB7ResetTriggerCounter.vi"/>
				<Item Name="EIB7SelectMode.vi" Type="VI" URL="../VIs/EIB7SelectMode.vi"/>
				<Item Name="EIB7SetFIFOSize.vi" Type="VI" URL="../VIs/EIB7SetFIFOSize.vi"/>
				<Item Name="EIB7SetHostname.vi" Type="VI" URL="../VIs/EIB7SetHostname.vi"/>
				<Item Name="EIB7SetIntervalCounterInterpolation.vi" Type="VI" URL="../VIs/EIB7SetIntervalCounterInterpolation.vi"/>
				<Item Name="EIB7SetIntervalCounterTrigger.vi" Type="VI" URL="../VIs/EIB7SetIntervalCounterTrigger.vi"/>
				<Item Name="EIB7SetNetwork.vi" Type="VI" URL="../VIs/EIB7SetNetwork.vi"/>
				<Item Name="EIB7SetPowerSupply.vi" Type="VI" URL="../VIs/EIB7SetPowerSupply.vi"/>
				<Item Name="EIB7SetRefCheck.vi" Type="VI" URL="../VIs/EIB7SetRefCheck.vi"/>
				<Item Name="EIB7SetRITriggeredge.vi" Type="VI" URL="../VIs/EIB7SetRITriggeredge.vi"/>
				<Item Name="EIB7SetTimeout.vi" Type="VI" URL="../VIs/EIB7SetTimeout.vi"/>
				<Item Name="EIB7SetTimerTriggerPeriod.vi" Type="VI" URL="../VIs/EIB7SetTimerTriggerPeriod.vi"/>
				<Item Name="EIB7SetTimestamp.vi" Type="VI" URL="../VIs/EIB7SetTimestamp.vi"/>
				<Item Name="EIB7SetTimestampPeriod.vi" Type="VI" URL="../VIs/EIB7SetTimestampPeriod.vi"/>
				<Item Name="EIB7SetTriggerInputDelay.vi" Type="VI" URL="../VIs/EIB7SetTriggerInputDelay.vi"/>
				<Item Name="EIB7SizeOfFIFOEntry.vi" Type="VI" URL="../VIs/EIB7SizeOfFIFOEntry.vi"/>
				<Item Name="EIB7SizeOfFIFOEntryRaw.vi" Type="VI" URL="../VIs/EIB7SizeOfFIFOEntryRaw.vi"/>
				<Item Name="EIB7SoftwareTrigger.vi" Type="VI" URL="../VIs/EIB7SoftwareTrigger.vi"/>
				<Item Name="EIB7StartRef.vi" Type="VI" URL="../VIs/EIB7StartRef.vi"/>
				<Item Name="EIB7StopRef.vi" Type="VI" URL="../VIs/EIB7StopRef.vi"/>
				<Item Name="EIB7TerminateConnection.vi" Type="VI" URL="../VIs/EIB7TerminateConnection.vi"/>
				<Item Name="EIB7TransferRecordingData.vi" Type="VI" URL="../VIs/EIB7TransferRecordingData.vi"/>
				<Item Name="EIB7UpdateState.vi" Type="VI" URL="../VIs/EIB7UpdateState.vi"/>
				<Item Name="EIB7WriteIO.vi" Type="VI" URL="../VIs/EIB7WriteIO.vi"/>
			</Item>
			<Item Name="eib7.dll" Type="Document" URL="../eib7.dll"/>
		</Item>
		<Item Name="Build-Spezifikationen" Type="Build"/>
	</Item>
</Project>
