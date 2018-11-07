#include <pthread.h>
#include <capture.h>
#include <sys/socket.h>
#include <syslog.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//Connection handler
void *conn_handler(void *);
struct thread_args {
		int conn;
		int socket_desc;
		int client_socket;
};
/*
 * Connection_handler method that handles every independent client
 * 
 */
void *conn_handler(void *arg)
{
	syslog(LOG_INFO,"INSIDE THREAD!");
	struct sockaddr_in client;
	struct thread_args *args = arg;

	int conn = args->conn;
	int socket_desc = args->socket_desc;
	int client_socket = args->client_socket;


	media_frame  *frame;
    void     *data;
    size_t   img_size;
    media_stream *stream;

    int i = 0;
    char *resolution = capture_get_resolutions_list(0);
    char *res;
    syslog(LOG_INFO,"Resolutions %s\n",resolution);
    res = strtok(resolution,",");
    
    syslog(LOG_INFO,"Highest resolution = %s",res);
    stream = capture_open_stream(IMAGE_JPEG, res);
    size_t sizes[10];
    void* images[10];

    for (i = 0; i < 10; ++i)
    {

    	frame     = capture_get_frame(stream);
		images[i] = capture_frame_data(frame);
    	sizes[i]  = capture_frame_size(frame);

    	syslog(LOG_INFO,"img_size= %zu",sizes[i]);
    	//sprintf(&size, "%zu",img_size);
    	//syslog(LOG_INFO,"%s",size);
    	//sizes[i] = size;

    	/*memset(data,0,sizeof(data));
    	memset(size,0,sizeof(size));
    	memset(data,0,sizeof(data));*/
    	capture_frame_free(frame);

    }
    
    syslog(LOG_INFO,"After for");
	client_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&conn);
	
	syslog(LOG_INFO,"Sending");
	write(client_socket,"TEST!\n",strlen("TEST!\n"));

	free(args);
}

int main(){

	//int socket_desc;
	//int conn;
	int *new_socket;
	struct sockaddr_in server;
	struct thread_args args;
    //Create socket
	args.socket_desc = socket(AF_INET , SOCK_STREAM , 0);

	if (args.socket_desc == -1) syslog(LOG_INFO,"FAILED TO CREATE SOCKET!");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8080 );

    //Bind
	if( bind(args.socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
        //Log error!
		syslog(LOG_INFO,"Bind ERROR");
		return 1;
	}

	listen(args.socket_desc , 3);

	args.conn = sizeof(struct sockaddr_in);

    //Todo http request --> start taking images and create a thread
	//while(1){

		//if(http_req){
			pthread_t client_thread;
        	new_socket = malloc(sizeof *new_socket);
        	*new_socket = args.client_socket;
			if( pthread_create( &client_thread , NULL ,conn_handler , (void*)&args) < 0)
			{
				syslog(LOG_INFO,"Failed to create msg thread!!!");
				return 1;
			}
			pthread_join(client_thread,NULL);
		//}
	//}
}
