/* 
 * File:   main.cpp
 * Author: Chris Aeberhard
 *
 * Program reads a configuration file, runs some checks, and then begins recording
 * audio from a local card
 */


#include <unistd.h>             // Used for the fork calls
#include <time.h>               // Used to compare time

#include "recording.h"          // Function(s) that do the recording
#include "config.h"             // Class to store configuration details
#include "setupFunctions.h"     // Function(s) that do all the setup steps and checks
#include "constants.h"          // Constants used in the program

using namespace std;

void setup();


// A config file holds all our settings for the program
Config config;
Config *ptrConfig = &config;

/**
 * Our entry point for the program
 * @return 0 for ok, negative for error!
 */
int main(int argc, char** argv) 
{          
    setup();

    // Perpetual loop
    while(1)
    {
        // Time handling variables
        time_t start;
        time_t now;
        double seconds;
        
        // Make a child process
        int pid = fork();
            
        // If child process
        if(pid == 0)
        {            
            record(ptrConfig);
            
            // We've recorded, so break out of here!
            break;
        }
        // Parent process
        else
        {
            time(&start);
            time(&now);
                        
            while(difftime(now, start) < (config.getRecordingLength() + (config.getRecordingDelay() / 1000)))
            {
                time(&now);
            }
        }
    }

    return 0;
}

/**
 * Does all the initial configuration and checks
 */
void setup()
{
    // Get the settings from the configuration file
    parseFunctionReturn(parseConfiguration(ptrConfig, conf), "Parse configuration file");
    
    // Make sure we have enough disk space (relevant diskspace is in the configuration file)
    parseFunctionReturn(checkDiskSpace(config.getAudioFilePath(), 
                                        config.getFreeDiskSpaceForError(),
                                        config.getFreeDiskSpaceForWarning()),
                        "Check audio file folder disk space");  
    
    // Make sure we can detect the recording device
    parseFunctionReturn(checkRecordingDevice(), "Check recording device");
    
    // Run some setup commands (based on the method of recording set in the configuration file)
    parseFunctionReturn(setupRecordingDevice(ptrConfig), "Setting up recording device");   
}