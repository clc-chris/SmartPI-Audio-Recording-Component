/* 
 * File:   config.h
 * Author: Chris
 *
 */
#include "setupFunctions.h"
#include "constants.h"


#include <locale.h>                     // Used for the config file reading
#include <config4cpp/Configuration.h>   // Used for the config file reading

#include <sys/statvfs.h>                // Used to check for the disk space
#include <iostream>                     // cout, endl, etc
#include <cstdlib>                      // exit() and system())
#include <stdio.h>                      // fopen and associated functiosn

using namespace std;
using namespace config4cpp;


/**
 * Function that neatens up the reporting and error control of this program
 * @param status The code to look at
 * @param step The description of the step that outputted the status code
 */
void parseFunctionReturn(int status, string step)
{
    if(status  < 0 )
    {
        cout << "ERROR: Error code " << status << " at step: " << step << endl;
        cout << "Program exiting" << endl;
        exit(status);
    }
    else if(status > 0)
    {
        cout << "WARNING: Warning code " << status << " at step: " << step << endl;
        cout << "Program continuing" << endl;
    }
}

/**
 * Reads a configuration file, and stores the information
 * http://www.config4star.org/config4star-getting-started-guide/overview-of-the-config4star-api.html#htoc23
 * @return Integer < 0 for error, and > 0 for warning. 0 for ok
 */
int parseConfiguration(Config *config, const char *configFile)
{
    // Scopes are used in the config file to separate blocks of entries
    const char* generalScope = "general";
    const char* generalRecordingScope = "generalRecording";
    const char* DMICScope = "DMICRecording";
    
    setlocale(LC_ALL, "");
    
    Configuration * cfg = Configuration::create();
    
    // Get individual config entries
    try 
    {
        cfg->parse(configFile);
         
        config->setPiID(cfg->lookupString(generalScope, "piID"));
        
        config->setBitrate(cfg->lookupInt(generalRecordingScope, "bitrate"));
        config->setAudioFilePath(cfg->lookupString(generalRecordingScope, "audioFilesPath"));
        config->setRecordingLength(cfg->lookupInt(generalRecordingScope, "recordLength"));      
        config->setFreeDiskSpaceForError(cfg->lookupInt(generalRecordingScope, "errorFree"));
        config->setFreeDiskSpaceForWarning(cfg->lookupInt(generalRecordingScope, "warningFree"));
        config->setRecordingMethod(cfg->lookupInt(generalRecordingScope, "recordingMethod"));
        config->setRecordingDelay(cfg->lookupInt(generalRecordingScope, "recordingDelay"));
        
        config->setDMICin2lDigVol(cfg->lookupInt(DMICScope, "IN2LDigitalVolume"));
        config->setDMICin2rDigVol(cfg->lookupInt(DMICScope, "IN2RDigitalVolume"));
        config->setDMICaifitx1Vol(cfg->lookupInt(DMICScope, "AIF1TX1volume"));
        config->setDMICaifitx2Vol(cfg->lookupInt(DMICScope, "AIFITX2volume"));
    }
    catch( const ConfigurationException & ex )
    {
        std::cout << ex.c_str() << std::endl;       
        return -1;
    }
    
    // Destroy the object
    cfg->destroy();
        
    return 0;
}

/**
 * Function that checks free disk space on the specificied folder path
 * http://www.systutorials.com/136585/how-to-get-available-filesystem-space-on-linux-a-c-function-and-a-cpp-example/
 * @return Integer < 0 for error, and > 0 for warning. 0 for ok
 */
int checkDiskSpace(string path, int freeSpaceForError, int freeSpaceForWarning)
{
    // Structure to hold free disk space
    struct statvfs stat;
    
    // Fill the above structure with the free space in the required path
    if(statvfs(path.c_str(), &stat) != 0)
    {
        return -2;
    }
    
    // Check if it is enough
    int megabytesFree = (stat.f_bsize * stat.f_bavail) / 1024 / 1024;
    
    if( megabytesFree < freeSpaceForError )
    {
        return -3;
    }
    else if 
        ((megabytesFree > freeSpaceForError ) 
        && 
        ( megabytesFree < freeSpaceForWarning))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * Function that checks if there is the recording device we are expecting
 * Not sure if this function is redundant or not...
 * http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c
 * @return Integer < 0 for error, and > 0 for warning. 0 for ok
 */
int checkRecordingDevice()
{
    FILE* pipe = popen("arecord -l", "r");
    if(!pipe)
    {
        return -4;
    }
    
    char buffer[256];
    string result = "";
    
    while (!feof(pipe))
    {
        if(fgets(buffer, 256, pipe) != NULL)
        {
            result += buffer;
        }
    }

    pclose(pipe);
        
    if( (result.find("sndrpiwsp")!= string::npos) && (result.find("wm5102") != string::npos) )
    {
        return 0;
    }
    else
    {
        return -5;
    }
}

/**
 * Set's up the recording device to use the specified method
 * 
 * This originally ran a script to do this work, but that was a security risk (as you
 * could just make a malicious script and replace it)
 * 
 * @param recordingMethod The method of recording
 * @return 
 */
int setupRecordingDevice(Config *config)
{
    switch( config->getRecordingMethod() )
    {
        case DMIC_RECORDING:
             system("amixer $1 -Dhw:sndrpiwsp cset name='IN2L Digital Volume' " + config->getDMICin2lDigVol());
             system("amixer $1 -Dhw:sndrpiwsp cset name='IN2R Digital Volume' " + config->getDMICin2rDigVol());
             system("amixer $1 -Dhw:sndrpiwsp cset name='LHPF1 Input 1' IN2L");
             system("amixer $1 -Dhw:sndrpiwsp cset name='LHPF2 Input 1' IN2R");
             system("amixer $1 -Dhw:sndrpiwsp cset name='LHPF1 Mode' High-pass");
             system("amixer $1 -Dhw:sndrpiwsp cset name='LHPF2 Mode' High-pass");
             system("amixer $1 -Dhw:sndrpiwsp cset name='LHPF1 Coefficients' 240,3");
             system("amixer $1 -Dhw:sndrpiwsp cset name='LHPF2 Coefficients' 240,3");
             system("amixer $1 -Dhw:sndrpiwsp cset name='AIF1TX1 Input 1' LHPF1");
             system("amixer $1 -Dhw:sndrpiwsp cset name='AIF1TX1 Input 1 Volume' " + config->getDMICaifitx1Vol());
             system("amixer $1 -Dhw:sndrpiwsp cset name='AIF1TX2 Input 1' LHPF2");
             system("amixer $1 -Dhw:sndrpiwsp cset name='AIF1TX2 Input 1 Volume' " + config-> getDMICaifitx2Vol());
             system("amixer $1 -Dhw:sndrpiwsp cset name='DMIC Switch' on");             
            break;
        default:
            return -1;
    }
    
    return 0;
}
