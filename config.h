/* 
 * File:   config.h
 * Author: Chris Aeberhard
 *
 * This class records and gives the configuration of the program, based on user preferences
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include <string>
#include "constants.h"

using namespace std;
        
class Config
{
    private:
        std::string piID;                    // The ID of the PI (in the Smart PI network))
        int bitRate;                        // The audio recording rate
        std::string audioFilePath;          // The path where audio files are stored
        int recordingLength;                // The amount of time in seconds to record per file
        int recordingDelay;                 // The amount of time in seconds to wait before recording the next file
        int freeDiskSpaceForError;          // Min free space needed to error (i.e. not enough))
        int freeDiskSpaceForWarning;        // Min free space needed for a warning (i.e. just enough))

        // Recording method
        int recordingMethod;
        
        // Volume details for input from the DMIC
        int DMICin2lDigVol;
        int DMICin2rDigVol;
        int DMICaif1tx1Vol;
        int DMICaif1tx2Vol;
        
    public:
        // Setters
        void setPiID(std::string id) { piID = id; }
        void setBitrate(int bitrate) { bitRate = bitrate;}
        void setAudioFilePath(std::string path) { audioFilePath = path; }
        void setRecordingLength(int length) { recordingLength = length; }    
        void setRecordingDelay(int delay) { recordingDelay = delay; }
        void setFreeDiskSpaceForError(int space) { freeDiskSpaceForError = space;}
        void setFreeDiskSpaceForWarning(int space) { freeDiskSpaceForWarning = space;}
        
        
        /**
         * There is only the option for DMIC recording at the moment. More 
         * recording methods can be added later (e.g. line in, or digital in). 
         * To do so just add OR clauses in the if statement. 
         * This setter gives a default option in case an invalid option is given
         * @param method The method requested
         */
        void setRecordingMethod(int method) 
        { 
            if( method == DMIC_RECORDING )
            {
                recordingMethod = method;
            }
            else
            {
                recordingMethod = DMIC_RECORDING;
            }
        
        }
        
        void setDMICin2lDigVol(int vol) { DMICin2lDigVol = vol;}
        void setDMICin2rDigVol(int vol) { DMICin2rDigVol = vol;}
        void setDMICaifitx1Vol(int vol) { DMICaif1tx1Vol = vol;}
        void setDMICaifitx2Vol(int vol) { DMICaif1tx2Vol = vol;}
        
        
        // Getters
        std::string getPiID() { return piID;}
        int getBitrate() { return bitRate;}
        std::string getAudioFilePath() { return audioFilePath;}
        int getRecordingLength() { return recordingLength;}
        int getRecordingDelay() { return recordingDelay;}
        int getFreeDiskSpaceForError() { return freeDiskSpaceForError;}
        int getFreeDiskSpaceForWarning() { return freeDiskSpaceForWarning;}
    
        int getRecordingMethod() { return recordingMethod;}
        int getDMICin2lDigVol() { return DMICin2lDigVol;}
        int getDMICin2rDigVol() { return DMICin2rDigVol;}
        int getDMICaifitx1Vol() { return DMICaif1tx1Vol;}
        int getDMICaifitx2Vol() { return DMICaif1tx2Vol;}
};


#endif	/* CONFIG_H */

