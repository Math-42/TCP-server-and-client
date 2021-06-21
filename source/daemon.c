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

int* readConfigFile(char* configurationFileName, void (* configHandler)()) {
	syslog(LOG_INFO, "Lendo arquivo de configuração");

	FILE *configurationFile = fopen(configurationFileName, "r");

	if (configurationFile == NULL) { //testa se o arquivo foi aberto
        syslog(LOG_ERR, "Não foi possivel abrir o arquivo de configurações");
        return NULL;
    }

	char readChar;
	int lineCounter = 1;
	while(fread(&readChar,sizeof(char),1,configurationFile) != 0){//conta quantas linhas existem
		lineCounter += (readChar == '\n') ? 1:0;
	}

	fseek(configurationFile,0,0);

	int* configs = (int*)malloc(lineCounter*sizeof(int));
	int i = 0;

	while(lineCounter--){
		fscanf(configurationFile, "%*s %*c %d",&configs[i++]);
	}

	configHandler(configs);

	free(configs);
	syslog(LOG_INFO, "Configurações lidas com sucesso");
	fclose(configurationFile);

	return configs;
}

void setupLogFile(daemonStruct* currDaemonInfos){
	currDaemonInfos->logStreamFile = fopen(currDaemonInfos->logFileName,"w+");

	if(currDaemonInfos->logStreamFile != NULL){
		currDaemonInfos->logStreamFile = stdout;
		return;
	}

	syslog(LOG_ERR, "Erro ao abrir arquivo de log");
}

void finishDaemonRoutine(daemonStruct* currDaemonInfos){
	syslog(LOG_INFO, "Finalizando daemon");
	if(currDaemonInfos->logStreamFile != NULL) fclose(currDaemonInfos->logStreamFile);
	closelog();
}