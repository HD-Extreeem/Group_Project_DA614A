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

//Struct for thread parameters to send when creating a thread
struct thread_args {
    int conn;
    int socket_desc;
};

/*
 * Connection_handler method that handles every independent client
 *
 */
void *conn_handler(void *arg)
{
    struct sockaddr_in client;
    struct thread_args *args = arg;
    
    int conn = args->conn;
    int socket_desc = args->socket_desc;
    int client_socket;
    //int client_socket = args->client_socket;
    
    media_frame   *frame;
    //void          *data;
    //size_t        img_size;
    media_stream  *stream;
    
    int i = 0;
    char *resolution = capture_get_resolutions_list(0);
    char *res;
    
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
        
        //memset(data,0,sizeof(data));
        memset(size,0,sizeof(size));
        capture_frame_free(frame);
        
    }
    
    client_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&conn);
    
    //Todo add code for sending to the android device
    
    capture_close_stream(stream);
    close(client_socket);
    //free(args);
    //free(client_socket);
}

int main(){
    int *new_socket;
    struct sockaddr_in server;
    struct thread_args args;
    
    //Create socket
    args.socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    
    if (args.socket_desc == -1) syslog(LOG_INFO,"FAILED TO CREATE SOCKET!");
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8080 );
    
    //Binds the connection to port, so the client can connect to it through sockets
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
    if( pthread_create( &client_thread , NULL ,conn_handler , (void*)&args) < 0)
    {
        syslog(LOG_INFO,"Failed to create msg thread!!!");
        return 1;
    }
    pthread_join(client_thread,NULL);
    //}
    //}
    return 0;
}
