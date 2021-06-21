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

typedef struct daemonStruct{
	char *configFileName;
	char *logFileName;
	char *daemonName;
	FILE *logStreamFile;
} daemonStruct;

/**
 * Rotina de inicialização da deamon, abre o log, cria o arquivo de log e define como os sinais serão tratados
 * @param currDaemonInfos struct contendo as informações da daemon
 * @param signalHandler função que lidará com os sinais recebidos pela daemon
 * @param configHandler função que receberá as configurações lidas pela daemon
 */
void initDaemon(daemonStruct* currDaemonInfos, void (* signalHandler)(), void (configHandler)(int *));

/**
 * Lê o arquivo de configurações, gera um array com os valores lidos e chama a função passada
 * @param configurationFileName nome do arquivo onde as configurações estão salvas
 * @param configHandler função que receberá as configurações lidas
 */
void readConfigFile(char* configurationFileName, void (* configHandler)());

/**
 * Inicializa o arquivo de log
 * @param currDaemonInfos struct contendo as informações da daemon
 */
void setupLogFile(daemonStruct* currDaemonInfos);

/**
 * Rotina de finalização da daemon, fecha o arquivo de log e o log
 * TODO: adicionar função de finalização
 * @param currDaemonInfos struct contendo as informações da daemon
 */
void finishDaemonRoutine(daemonStruct* currDaemonInfos);

#endif