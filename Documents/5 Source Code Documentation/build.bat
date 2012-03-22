@echo off

if not exist out mkdir out
doxygen Doxyfiles\Application
doxygen Doxyfiles\BusEmulation
doxygen Doxyfiles\CAN_ETAS_BOA
doxygen Doxyfiles\CAN_ICS_neoVI
doxygen Doxyfiles\CAN_Kvaser_CAN
doxygen Doxyfiles\CAN_MHS
doxygen Doxyfiles\CAN_STUB
doxygen Doxyfiles\CAN_Vector_XL
doxygen Doxyfiles\CommonClass
doxygen Doxyfiles\DataTypes
doxygen Doxyfiles\DIL_Interface
doxygen Doxyfiles\DIL_J1939
doxygen Doxyfiles\DmGraph
doxygen Doxyfiles\Filter
doxygen Doxyfiles\FrameProcessor
doxygen Doxyfiles\Include
doxygen Doxyfiles\MHS_CNFG
doxygen Doxyfiles\NodeSimEx
doxygen Doxyfiles\ProjectConfiguration
doxygen Doxyfiles\PSDI_CAN
doxygen Doxyfiles\Replay
doxygen Doxyfiles\SigGrphWnd
doxygen Doxyfiles\SignalDefiner
doxygen Doxyfiles\SignalWatch
doxygen Doxyfiles\TestSetupEditorGUI
doxygen Doxyfiles\TestSetupEditorLib
doxygen Doxyfiles\TestSuiteExecutorGUI
doxygen Doxyfiles\TXWindow
doxygen Doxyfiles\Utility

if not exist "out\Format Converter" mkdir "out\Format Converter"
doxygen "Doxyfiles\Format Converter\AscLogConverter"
doxygen "Doxyfiles\Format Converter\CAPL2CConvereter"
doxygen "Doxyfiles\Format Converter\DBC2DBFConverter"
doxygen "Doxyfiles\Format Converter\DBF2DBCConverter"
doxygen "Doxyfiles\Format Converter\FormatConverterApp"
doxygen "Doxyfiles\Format Converter\LogAscConverter"
