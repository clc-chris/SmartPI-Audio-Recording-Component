/* 
 * File:   recording.h
 * Author: Chris Aeberhard
 *
 * This file stores functions related to the recording of files
 */
#include <sstream>      // Used to convert integers to strings
#include <string.h>     // String class
#include <time.h>       // Used for the name of the recorded file
#include <cstdlib>      // Used for system())

#include "config.h"      // Used for the config file


#ifndef RECORDING_H
#define	RECORDING_H

int record(Config *config)
{
    // Create and get the current time
    time_t now = time(0);
    // Put details into a structure
    tm *ltm = localtime(&now);
            
    std::ostringstream command;
 
    command << "arecord -Dhw:sndrpiwsp -r " << config->getBitrate() << 
            " -c 2 -d " << config->getRecordingLength() << " -f S32_LE " <<
            config->getAudioFilePath() << "/" << config->getPiID() << 
             "-" << (1900 + ltm->tm_year) << "-" << (1 + ltm->tm_mon) << 
            "-" << ltm->tm_mday << "-" << ltm->tm_hour << ltm->tm_min << ltm->tm_sec << ".WAVE";
    
    /*bitRateSS << config->getBitrate();
    durationSS << config->getRecordingLength();
    
    std::string command = "arecord -Dhw:sndrpiwsp -r " + bitRateSS.str() + 
            " -c 2 -d " + durationSS.str() + 
            " -f S32_LE " + config->getAudioFilePath() + "/" + config->getPiID() + 
            "-" + (1900 + ltm->tm_year) + "-" + (1 + ltm->tm_mon) + "-" + 
            ltm->tm_mday + "-" + ltm->tm_hour + ltm->tm_min + ltm->tm_sec + ".WAVE";*/
    
    
    return system(command.str().c_str());
}

#endif	/* RECORDING_H */

