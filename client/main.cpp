#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <thread>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

const int  SERVER_PORT = 7878;
const int  BUFF_SIZE   = 526;
const char *SERVER_IP  = "127.0.0.1";

using namespace std;

void handler(const int conn, const string& data)
{
    while(1)
    {
       // char sendBuff[BUFF_SIZE] = "HTTP/1.0 200 OK\r\n"
       //         "Client: sendClient for test/0.1.0\r\n"
       //         "Content-Type: text/plain\r\n\r\n"
       //         "{\"jsonrpc\":\"1.0\",\"id\":\"1\",\"method\":\"listunspent\", "
       //         "\"wallet_type\" : \"2\",\"params\":[\"33CLiih6wUvE7j2wE4e2UdAkFxwFY4u6uu\"]}";

        send(conn, data.c_str(), data.size() , 0);
        cout << "Send to server >>>>>>>>> :" << data << endl;

        char receiveBuf[1024] = {0};
        //接收数据
        int num = recv(conn, receiveBuf, 101, 0);
        if(num > 1)
        {
        	cout << "Recv from server <<<<<<< :" << receiveBuf << endl;
            return;
        }
    }
}

void clientConnect(const string& src)
{
    int sockfd = -1;
    sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( -1 == sockfd )
    {
        perror( "sock created" );
        exit( -1 );
    }

    struct sockaddr_in server;
    memset( &server, 0, sizeof( struct sockaddr_in ) );
    server.sin_family      = AF_INET;
    server.sin_port        = htons(SERVER_PORT) ;
    server.sin_addr.s_addr = inet_addr(SERVER_IP) ;

    int res = -1;
    res = connect( sockfd, (struct sockaddr*)&server, sizeof( sockaddr ) );
    if( -1 == res )
    {
        perror( "Sock connect" );
        exit( -1 );
    }

    handler(sockfd, src);

    close(sockfd);

    return;
}

int main(int argc, char* arv[])
{
	cout << "---(NOTE: If you want to exit client ,"
		"please input q or Q or exit)---" << endl;
	string data;

    while(1)
    {
		cout << "Please input the data:";
		cin >> data;

		if("q" == data or "Q" == data or
				"exit" == data)
		{
			cout << "Exit the client!" << endl;
			return 0;
		}

        thread one(clientConnect, data);
//        one.detach();
        one.join();
    }

    return 0;
}

