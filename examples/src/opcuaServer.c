/*
 ============================================================================
 Name        : opcuaServer.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : lala
 ============================================================================
 */
#define _XOPEN_SOURCE 1 //TODO HACK
#include <stdio.h>
#include <stdlib.h>

#include "networklayer.h"
#include "ua_application.h"

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#ifdef RASPI
	#include "raspberrypi_io.h"
#endif
#include <sys/ipc.h>
#include <sys/shm.h>
#define MAXMYMEM 4



UA_Int32 serverCallback(void * arg) {
	char *name = (char *) arg;
	printf("%s does whatever servers do\n",name);

	Namespace* ns0 = (Namespace*)UA_indexedList_find(appMockup.namespaces, 0)->payload;
	UA_Int32 retval;
	UA_Node const * node;
	UA_ExpandedNodeId serverStatusNodeId = NS0EXPANDEDNODEID(2256);
	retval = Namespace_get(ns0, &(serverStatusNodeId.nodeId),&node, UA_NULL);
	if(retval == UA_SUCCESS){
		((UA_ServerStatusDataType*)(((UA_VariableNode*)node)->value.data))->currentTime = UA_DateTime_now();
	}

	const UA_Node *foundNode1 = UA_NULL;

	Namespace_Entry_Lock *lock;
	//node which should be filled with data (float value)
	UA_NodeId tmpNodeId1;


	tmpNodeId1.encodingByte = UA_NODEIDTYPE_TWOBYTE;
	tmpNodeId1.identifier.numeric = 108;
	tmpNodeId1.namespace =  0;


	if(Namespace_get(ns0,&tmpNodeId1, &foundNode1,&lock) != UA_SUCCESS){
		return UA_ERROR;
	}



	*((float*)((UA_VariableNode *)foundNode1)->value.data) = *((float*)((UA_VariableNode *)foundNode1)->value.data) + 0.2f;




		return UA_SUCCESS;
}

int main(int argc, char** argv) {
	appMockup_init();
	NL_data* nl = NL_init(&NL_Description_TcpBinary,16664);

	struct timeval tv = {1, 0}; // 1 second
#ifdef RASPI
	initIO();
    pid_t i = fork();
    if (i == 0)
    {
    	Namespace* ns0 = (Namespace*)UA_indexedList_find(appMockup.namespaces, 0)->payload;
		const UA_Node *foundNode1 = UA_NULL;
		const UA_Node *foundNode2 = UA_NULL;
		const UA_Node *foundNode3 = UA_NULL;
		Namespace_Entry_Lock *lock;
		//node which should be filled with data (float value)
		UA_NodeId tmpNodeId1;
		UA_NodeId tmpNodeId2;
		UA_NodeId tmpNodeId3;

		tmpNodeId1.encodingByte = UA_NODEIDTYPE_TWOBYTE;
		tmpNodeId1.identifier.numeric = 108;
		tmpNodeId1.namespace =  0;

		tmpNodeId2.encodingByte = UA_NODEIDTYPE_TWOBYTE;
		tmpNodeId2.identifier.numeric = 109;
		tmpNodeId2.namespace =  0;

		tmpNodeId3.encodingByte = UA_NODEIDTYPE_TWOBYTE;
		tmpNodeId3.identifier.numeric = 110;
		tmpNodeId3.namespace =  0;
    	while(1){
			if(Namespace_get(ns0,&tmpNodeId1, &foundNode1,&lock) != UA_SUCCESS){
				_exit(1);
			}

			if(Namespace_get(ns0,&tmpNodeId2, &foundNode2,&lock) != UA_SUCCESS){
				_exit(1);
			}

			if(Namespace_get(ns0,&tmpNodeId3, &foundNode3,&lock) != UA_SUCCESS){
				_exit(1);
			}
			readTemp((float*)((UA_VariableNode *)foundNode1)->value.data);
			writePin(*((UA_Boolean*)((UA_VariableNode *)foundNode2)->value.data),0);
			writePin(*((UA_Boolean*)((UA_VariableNode *)foundNode3)->value.data),2);
			sleep(200);
    	}
    }
    else if (i > 0)
    {

        _exit(2);
    }
    else
    {
        perror("fork failed");
        _exit(3);
    }
#endif
	NL_msgLoop(nl, &tv, serverCallback,argv[0]);
}
