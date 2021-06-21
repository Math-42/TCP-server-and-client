#ifndef DAEMON_H
#define DAEMON_H

#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>


/**
 * Transforma o programa em uma daemon
 */
void initDaemon(daemonStruct* currDaemonInfos, void (* signalHandler)(), void (configHandler)(int *));

/**
 * 
 * 
 */
int* readConfigFile(char* configurationFileName, void (* configHandler)());


#endif