del libWrapper_CAN.a
g++ -DNO_INCLUDE_TEMPSTRUCT -c  Trigonometric.cpp BMCANDefines.cpp BMUtility.cpp StringFunctions.cpp FileOperations.cpp   OtherCaplFunc.cpp CANCAPLWrapper.cpp Wrapper_Common.cpp 
ar -rcs libWrapper_CAN.a Trigonometric.o StringFunctions.o BMCANDefines.o BMUtility.o OtherCaplFunc.o FileOperations.o  CANCAPLWrapper.o  Wrapper_Common.o
del Trigonometric.o StringFunctions.o BMCANDefines.o BMUtility.o OtherCaplFunc.o FileOperations.o  CANCAPLWrapper.o  Wrapper_Common.o
copy /Y libWrapper_CAN.a ..\OBJ\
