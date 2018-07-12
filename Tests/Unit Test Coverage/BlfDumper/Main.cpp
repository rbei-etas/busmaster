/*
 * BLF files dumper
 * (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Author:      Pavel Boiko
 * Release:     1.0
 * Annotation:  This dumper is created just to demonstrate functionality of the BLF library.
 *              It also may be used for the library testing.
 * Revision History :
 *
 *     Date             Author               Detail description
 * ------------- --------------------- ------------------------------
 * May  15, 2014 Pavel Boiko           First version.
 * May  22, 2014 Andrey Oleynikov      Change output in according to original ETAS prototype format.
 */

#include <iostream>

#include "../../BlfLibrary/Src/IBlfLibrary.h"

void printVersionAndUsage()
{
    std::cout   << "BLF Dumper v0.1 (prototype). Copyright (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved." << std::endl
                << "Usage: BlfDumper [file.blf] [logfile.txt]" << std::endl
                ;
}

int main(int argc, char *argv[])
{
    // Check the arguments and if they are not exactly match our needs, show information about the program and stop it.
    if ((argc < 2) || (argc > 3))
    {
        printVersionAndUsage();
        return 0;
    }

    // Get command line parameters
    std::string sBlfFile = argv[1];
    std::string sLogFile;
    if (argc >= 3)
        sLogFile = argv[2];

    try
    {
        bool isOk = true;

        // Create BLF Library interface
        BLF::IBlfLibrary *pBlfLib = BLF::GetIBlfLibrary();
        isOk = (NULL != pBlfLib);
        if (!isOk)
        {
            std::cerr << "Error: Unable to get BLF Library interface." << std::endl;
            return 1;
        }

        // Enable logging mechanism (if need) to be able to deeply analyze how the library works
        if (!sLogFile.empty())
        {
            isOk = pBlfLib->EnableLogging(sLogFile);
            if (!isOk)
            {
                std::cerr << "Error: Unable to enable the logging mechanism." << std::endl;
                return 1;
            }
        }

        // Load BLF file
        isOk = pBlfLib->Load(argv[1]);
        if (!isOk)
        {
            std::cerr << "Error: Unable to load BLF file." << std::endl;
            return 1;
        }

        // Dump just loaded BLF file
        isOk = pBlfLib->Dump();
        if (!isOk)
        {
            std::cerr << "Error: Unable to dump BLF file." << std::endl;
            return 1;
        }
    }
    catch(const std::exception &e)
    {
        // Process critical error
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
