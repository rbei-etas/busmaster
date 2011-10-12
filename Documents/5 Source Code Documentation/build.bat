@echo off

mkdir out
doxygen Doxyfiles/Application                  2> out/Application/doxygen-log.txt
doxygen Doxyfiles/BusEmulation                 2> out/BusEmulation/doxygen-log.txt
doxygen Doxyfiles/CAN_ETAS_BOA                 2> out/CAN_ETAS_BOA/doxygen-log.txt
doxygen Doxyfiles/CAN_ICS_neoVI                2> out/CAN_ICS_neoVI/doxygen-log.txt
doxygen Doxyfiles/CAN_Kvaser_CAN               2> out/CAN_Kvaser_CAN/doxygen-log.txt
doxygen Doxyfiles/CAN_STUB                     2> out/CAN_STUB/doxygen-log.txt
doxygen Doxyfiles/CAN_Vector_XL                2> out/CAN_Vector_XL/doxygen-log.txt
doxygen Doxyfiles/CommonClass                  2> out/CommonClass/doxygen-log.txt
doxygen Doxyfiles/ConfigDialogsDIL             2> out/ConfigDialogsDIL/doxygen-log.txt
doxygen Doxyfiles/DIL_Interface                2> out/DIL_Interface/doxygen-log.txt
doxygen Doxyfiles/DataTypes                    2> out/DataTypes/doxygen-log.txt
doxygen Doxyfiles/Filter                       2> out/Filter/doxygen-log.txt
doxygen Doxyfiles/FrameProcessor               2> out/FrameProcessor/doxygen-log.txt
doxygen Doxyfiles/Include                      2> out/Include/doxygen-log.txt
doxygen Doxyfiles/NodeSimEx                    2> out/NodeSimEx/doxygen-log.txt
doxygen Doxyfiles/PSDI_CAN                     2> out/PSDI_CAN/doxygen-log.txt
doxygen Doxyfiles/ProjectConfiguration         2> out/ProjectConfiguration/doxygen-log.txt
doxygen Doxyfiles/Replay                       2> out/Replay/doxygen-log.txt
doxygen Doxyfiles/SignalWatch                  2> out/SignalWatch/doxygen-log.txt
doxygen Doxyfiles/TXWindow                     2> out/TXWindow/doxygen-log.txt
doxygen Doxyfiles/TestSetupEditorGUI           2> out/TestSetupEditorGUI/doxygen-log.txt
doxygen Doxyfiles/TestSetupEditorLib           2> out/TestSetupEditorLib/doxygen-log.txt
doxygen Doxyfiles/TestSuiteExecutorGUI         2> out/TestSuiteExecutorGUI/doxygen-log.txt
doxygen Doxyfiles/Utility                      2> out/Utility/doxygen-log.txt

mkdir out\TOOLS_EXTERNAL
doxygen Doxyfiles/TOOLS_EXTERNAL/CAPL_2_C      2> out/TOOLS_EXTERNAL/CAPL_2_C/doxygen-log.txt
doxygen Doxyfiles/TOOLS_EXTERNAL/EXT_DBC_2_DBF 2> out/TOOLS_EXTERNAL/EXT_DBC_2_DBF/doxygen-log.txt
doxygen Doxyfiles/TOOLS_EXTERNAL/EXT_DBF_2_DBC 2> out/TOOLS_EXTERNAL/EXT_DBF_2_DBC/doxygen-log.txt
