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
#include <time.h>


//Connection handler
void *conn_handler(void *);
void *send_img(void *socket);
void send_response(int * client);
size_t sizes[10];
void* images[10];

/*
 * Connection_handler method that handles every independent client
 * 
 */
void *conn_handler(void *arg)
{
    syslog(LOG_INFO,"INSIDE THREAD!");

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
}

void *send_img(void *client)
{
    int i = 0;
    char *msg;
    syslog(LOG_INFO,"sendimg");
    for ( i = 0; i < 10; ++i)
    {
        //sprintf(msg,"%zu\n",sizes[i]);
        //syslog(LOG_INFO,"%s\n",msg);
        write(client,"30000",strlen("30000"));
        write(client,"\n",strlen("\n"));
        syslog(LOG_INFO,"sending image");
        write(client,images[i],sizeof(images[i]));
	}
    close(client);
    free(client);
}

void send_response(int * client){
    write(client, "HTTP/1.1 200 OK\n", strlen("HTTP/1.1 200 OK\n"));
    
    write(client, "Content-length: 146\n", strlen("Content-length: 146\n"));
    
    write(client, "Content-Type: text/html\n\n", strlen("Content-Type: text/html\n\n"));
    
    write(client, "<html><body><H1>FALL WAS DETECTED, SAVING IMAGES</H1></body></html>", strlen("<html><body><H1>FALL WAS DETECTED, SAVING IMAGES</H1></body></html>"));
}

int main(){

    int conn, socket_desc, client_socket;
    int *new_socket;
    struct sockaddr_in server, client;
    char req[1000];

    //Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);

	if (socket_desc == -1) syslog(LOG_INFO,"FAILED TO CREATE SOCKET!");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8080 );

    //Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
        //Log error!
        syslog(LOG_INFO,"Bind ERROR");
        return 1;
	}

    listen(socket_desc , 4);

    conn = sizeof(struct sockaddr_in);

    while(client_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&conn)){
		
        recv(client_socket,req,1000,0);
        syslog(LOG_INFO,"got: %s",req);
		
        if(strstr(req,"/getReady")){
			
            send_response(client_socket);
            close(client_socket);
            pthread_t client_thread;
            
            if( pthread_create( &client_thread , NULL ,conn_handler , NULL) < 0)
            {
                syslog(LOG_INFO,"Failed to create msg thread!!!");
                return 1;
            }
        }
        else if(strstr(req,"/send")){
            syslog(LOG_INFO,"elseif");
            pthread_t client_thread;
            new_socket = malloc(sizeof *new_socket);
            *new_socket = client_socket;
            if( pthread_create( &client_thread , NULL ,send_img, (void*)new_socket) < 0)
            {
                syslog(LOG_INFO,"Failed to create msg thread!!!");
                return 1;
            }
        }
    }
}
