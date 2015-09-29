/* 
 * File:   setupFunctions.h
 * Author: Chris Aeberhard
 *
 */

#ifndef SETUPFUNCTIONS_H
#define	SETUPFUNCTIONS_H

#include "config.h"

void parseFunctionReturn(int status, std::string function);
int parseConfiguration(Config *config, const char *configFile);
int checkDiskSpace(std::string path, int freeSpaceForError, int freeSpaceForWarning);
int checkRecordingDevice();
int setupRecordingDevice(Config *config);

#endif	/* SETUPFUNCTIONS_H */

