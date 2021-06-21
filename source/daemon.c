#include "daemon.h"

void initDaemon(daemonStruct* currDaemonInfos, void (* signalHandler)(int), void (configHandler)(int *)){


	openlog(currDaemonInfos->daemonName, LOG_PID|LOG_CONS, LOG_DAEMON);
	setupLogFile(currDaemonInfos);

    syslog(LOG_INFO, "Iniciando deamon: %s", currDaemonInfos->daemonName);

	//fazer um for talvez? já que os sinais são apenas inteiros
    signal(SIGINT, signalHandler);
    signal(SIGHUP, signalHandler);

	readConfigFile(currDaemonInfos->configFileName, configHandler);

}
