@echo off

if not exist out mkdir out
doxygen Doxyfiles\Application                  2> out\Application.log
doxygen Doxyfiles\BusEmulation                 2> out\BusEmulation.log
doxygen Doxyfiles\CAN_ETAS_BOA                 2> out\CAN_ETAS_BOA.log
doxygen Doxyfiles\CAN_ICS_neoVI                2> out\CAN_ICS_neoVI.log
doxygen Doxyfiles\CAN_Kvaser_CAN               2> out\CAN_Kvaser_CAN.log
doxygen Doxyfiles\CAN_STUB                     2> out\CAN_STUB.log
doxygen Doxyfiles\CAN_Vector_XL                2> out\CAN_Vector_XL.log
doxygen Doxyfiles\CommonClass                  2> out\CommonClass.log
doxygen Doxyfiles\ConfigDialogsDIL             2> out\ConfigDialogsDIL.log
doxygen Doxyfiles\DIL_Interface                2> out\DIL_Interface.log
doxygen Doxyfiles\DataTypes                    2> out\DataTypes.log
doxygen Doxyfiles\Filter                       2> out\Filter.log
doxygen Doxyfiles\FrameProcessor               2> out\FrameProcessor.log
doxygen Doxyfiles\Include                      2> out\Include.log
doxygen Doxyfiles\NodeSimEx                    2> out\NodeSimEx.log
doxygen Doxyfiles\PSDI_CAN                     2> out\PSDI_CAN.log
doxygen Doxyfiles\ProjectConfiguration         2> out\ProjectConfiguration.log
doxygen Doxyfiles\Replay                       2> out\Replay.log
doxygen Doxyfiles\SignalWatch                  2> out\SignalWatch.log
doxygen Doxyfiles\TXWindow                     2> out\TXWindow.log
doxygen Doxyfiles\TestSetupEditorGUI           2> out\TestSetupEditorGUI.log
doxygen Doxyfiles\TestSetupEditorLib           2> out\TestSetupEditorLib.log
doxygen Doxyfiles\TestSuiteExecutorGUI         2> out\TestSuiteExecutorGUI.log
doxygen Doxyfiles\Utility                      2> out\Utility.log

if not exist out\TOOLS_EXTERNAL mkdir out\TOOLS_EXTERNAL
doxygen Doxyfiles\TOOLS_EXTERNAL\CAPL_2_C      2> out\TOOLS_EXTERNAL\CAPL_2_C.log
doxygen Doxyfiles\TOOLS_EXTERNAL\DBC_2_DBF     2> out\TOOLS_EXTERNAL\DBC_2_DBF.log
doxygen Doxyfiles\TOOLS_EXTERNAL\DBF_2_DBC     2> out\TOOLS_EXTERNAL\DBF_2_DBC.log
