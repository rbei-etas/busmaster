@echo off

if not exist out mkdir out
doxygen Doxyfiles\Application                  2> out\Application.log
doxygen Doxyfiles\BusEmulation                 2> out\BusEmulation.log
doxygen Doxyfiles\CAN_ETAS_BOA                 2> out\CAN_ETAS_BOA.log
doxygen Doxyfiles\CAN_ICS_neoVI                2> out\CAN_ICS_neoVI.log
doxygen Doxyfiles\CAN_Kvaser_CAN               2> out\CAN_Kvaser_CAN.log
doxygen Doxyfiles\CAN_MHS                      2> out\CAN_MHS.log
doxygen Doxyfiles\CAN_STUB                     2> out\CAN_STUB.log
doxygen Doxyfiles\CAN_Vector_XL                2> out\CAN_Vector_XL.log
doxygen Doxyfiles\CommonClass                  2> out\CommonClass.log
doxygen Doxyfiles\DataTypes                    2> out\DataTypes.log
doxygen Doxyfiles\DIL_Interface                2> out\DIL_Interface.log
doxygen Doxyfiles\DIL_J1939                    2> out\DIL_J1939.log
doxygen Doxyfiles\DmGraph                      2> out\DmGraph.log
doxygen Doxyfiles\Filter                       2> out\Filter.log
doxygen Doxyfiles\FrameProcessor               2> out\FrameProcessor.log
doxygen Doxyfiles\Include                      2> out\Include.log
doxygen Doxyfiles\MHS_CNFG                     2> out\MHS_CNFG.log
doxygen Doxyfiles\NodeSimEx                    2> out\NodeSimEx.log
doxygen Doxyfiles\ProjectConfiguration         2> out\ProjectConfiguration.log
doxygen Doxyfiles\PSDI_CAN                     2> out\PSDI_CAN.log
doxygen Doxyfiles\Replay                       2> out\Replay.log
doxygen Doxyfiles\SigGrphWnd                   2> out\SigGrphWnd.log
doxygen Doxyfiles\SignalDefiner                2> out\SignalDefiner.log
doxygen Doxyfiles\SignalWatch                  2> out\SignalWatch.log
doxygen Doxyfiles\TestSetupEditorGUI           2> out\TestSetupEditorGUI.log
doxygen Doxyfiles\TestSetupEditorLib           2> out\TestSetupEditorLib.log
doxygen Doxyfiles\TestSuiteExecutorGUI         2> out\TestSuiteExecutorGUI.log
doxygen Doxyfiles\TXWindow                     2> out\TXWindow.log
doxygen Doxyfiles\Utility                      2> out\Utility.log

if not exist "out\Format Converter" mkdir "out\Format Converter"
doxygen "Doxyfiles\Format Converter\AscLogConverter"      2> "out\Format Converter\AscLogConverter.log"
doxygen "Doxyfiles\Format Converter\CAPL2CConvereter"     2> "out\Format Converter\CAPL2CConverter.log"
doxygen "Doxyfiles\Format Converter\DBC2DBFConverter"     2> "out\Format Converter\DBC2DBFConverter.log"
doxygen "Doxyfiles\Format Converter\DBF2DBCConverter"     2> "out\Format Converter\DBF2DBCConverter.log"
doxygen "Doxyfiles\Format Converter\FormatConverterApp"   2> "out\Format Converter\FormatConverterApp.log"
doxygen "Doxyfiles\Format Converter\LogAscConverter"      2> "out\Format Converter\LogAscConverter.log"
