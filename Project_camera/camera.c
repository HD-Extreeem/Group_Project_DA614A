#include <pthread.h>
#include <capture.h>
#include <netinet/in.h>
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
void *send_img(void *);
void send_response(int * client);
char * append(char * str1, char * str2);
char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length);

//Variable for storing the img data
char *imgs = "";

//Table used for encoding the image to a base64 (ASCII)
static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
//Mod table used for encoding
static int mod_table[] = {0, 2, 1};



/*
 * Connection_handler method that handles every independent client
 * This method captures the image for sending upon getting a
 * request from the Android application
 *
 */

void *conn_handler(void *arg)
{

    media_frame   *frame;
    char          msg[100];
    void          *data;
    size_t        img_size;
    media_stream  *stream;
    char          *res,*str;
    
    size_t        output_length = 0;
    int           i             = 0;
    char          *resolution   = capture_get_resolutions_list(0);
    
    imgs = "";
    //syslog(LOG_INFO,"Resolutions available %s\n",resolution);
    
    //Substring the first highest resolution
    res = strtok(resolution,",");

    //Build the parameter string used for the stream
    sprintf(msg, "fps=3&%s",res);
    syslog(LOG_INFO,"Highest resolution = %s",res);

    //Open the stream to start capturing the images
    stream = capture_open_stream(IMAGE_JPEG, res);
    
    //Loop 10 times to capture the image and store it to a char *
    for (i = 0; i < 10; ++i)
    {
        frame     = capture_get_frame(stream);
        data      = capture_frame_data(frame);
        img_size  = capture_frame_size(frame);
        str       = base64_encode((unsigned char*)data, img_size, &output_length);
        imgs      = append(imgs,str);
        free(str);
        capture_frame_free(frame);
    }
    
    syslog(LOG_INFO,"DONE SAVING IMAGES!");
    return 0;
}


/**
 * This is method handles the Android camera communication
 * Upon receiving a request from the Android application /send
 * We start parsing the images saved and sending them to the Android device
 *
 */
void *send_img(void *client)
{
    char *img = "";
    
    int  socket = *(int*)client;
    int  i      = 0;
    img         = strtok(imgs, ",");

    //Loop over the images saved and send them to the Android client
    for ( i = 0; i < 10; ++i)
    {   

        write(socket,img , strlen(img));
        
        //Get next image in img
        img = strtok(NULL,",");

	}
    
    //Free all resources and close socket connection
    imgs = "";
    syslog(LOG_INFO,"Imgs sent to Android!");
    //shutdown(socket,SHUT_RDWR);
    //shutdown(client,SHUT_RDWR);
    //close(client);
    //free(client);
    return 0;
}


/*---------------------------------------------------------------*/
/*Method used for responding on the http request from the ESP8266*/
/*---------------------------------------------------------------*/

void send_response(int * client){
    write(client, "HTTP/1.1 200 OK\n", strlen("HTTP/1.1 200 OK\n"));
    
    write(client, "Content-length: 146\n", strlen("Content-length: 146\n"));
    
    write(client, "Content-Type: text/html\n\n", strlen("Content-Type: text/html\n\n"));
    
    write(client, "<html><body><H1>FALL WAS DETECTED, SAVING IMAGES</H1></body></html>", strlen("<html><body><H1>FALL WAS DETECTED, SAVING IMAGES</H1></body></html>"));
}

char * append(char * str1, char * str2)
{
    char * res = "";
    asprintf(&res, "%s,%s\n", str1, str2);
    return res;
}

int main(){

    int                 conn, socket_desc, client_socket;
    int                 *new_socket;
    struct sockaddr_in  server, client;
    char                req[1000];
    int                 options = 1;

    //Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);

	if (socket_desc == -1) syslog(LOG_INFO,"FAILED TO CREATE SOCKET!");

	server.sin_family       = AF_INET;
	server.sin_addr.s_addr  = INADDR_ANY;
	server.sin_port         = htons( 8080 );

	if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR,(char*) &options,sizeof(options))<0){
		syslog(LOG_INFO,"setsockopt FAILED!");
		close(socket_desc);
		return 1;
	}

    //Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
        //Log error!
        syslog(LOG_INFO,"Bind ERROR");
        close(socket_desc);
        return 1;
	}

    listen(socket_desc , 4);

    conn = sizeof(struct sockaddr_in);

    //Loop forever and listen to incomming clients to connect
    while((client_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&conn))>0){
		
        recv(client_socket,req,1000,0);
		
        if(strstr(req,"/getReady")){
			
            send_response(client_socket);
            close(client_socket);
            pthread_t get_img_thread;
            
            if( pthread_create( &get_img_thread , NULL ,conn_handler , NULL) < 0)
            {
                syslog(LOG_INFO,"Failed to create msg thread!!!");
                return 1;
            }
        }
        else if(strstr(req,"/send")){
            if(strlen(imgs)>1){
                syslog(LOG_INFO,"Sending images");
                pthread_t send_img_thread;
            
                //new_socket  = malloc(sizeof *new_socket);
                //*new_socket = client_socket;
            
                if( pthread_create( &send_img_thread , NULL ,send_img, (void*)&client_socket) < 0)
                {
                    syslog(LOG_INFO,"Failed to create msg thread!!!");
                    return 1;
                }
            }
           
        }
        memset(&req[0], 0, sizeof(req));
    }
}




/*---------------------------------------------*/
/*---------------------------------------------*/
/*A part of Base64 encode library from base64.c*/
/*Own no rights to this code!				   */
/*---------------------------------------------*/
/*---------------------------------------------*/

char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {

  *output_length = 4 * ((input_length + 2) / 3);

  char *encoded_data = malloc(*output_length);
  if (encoded_data == NULL) return NULL;
  int i,j;
  for (i = 0, j = 0; i < input_length;) {

    uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
    uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
    uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

    uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

    encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
    encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
    encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
    encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
  }

  for (i = 0; i < mod_table[input_length % 3]; i++)
    encoded_data[*output_length - 1 - i] = '=';

  
  return encoded_data;
}
