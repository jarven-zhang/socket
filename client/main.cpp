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

const int  SERVER_PORT = 4286;
const int  BUFF_SIZE   = 526;
//const char *SERVER_IP  = "127.0.0.1";
const char *SERVER_IP  = "121.4.82.218";
const int RECEIVE_BUFF_SIZE = 526;

using namespace std;

int sendMsg(const int conn)
{
    char receiveBuf[RECEIVE_BUFF_SIZE] = {0};
	cout << "---(NOTE: If you want to exit client ,"
		"please input q or Q or exit)---" << endl;

	string data("");
    while(1)
    {
       // char sendBuff[BUFF_SIZE] = "HTTP/1.0 200 OK\r\n"
       //         "Client: sendClient for test/0.1.0\r\n"
       //         "Content-Type: text/plain\r\n\r\n"
       //         "{\"jsonrpc\":\"1.0\",\"id\":\"1\",\"method\":\"listunspent\", "
       //         "\"wallet_type\" : \"2\",\"params\":[\"33CLiih6wUvE7j2wE4e2UdAkFxwFY4u6uu\"]}";

		cout << "Please input the data to port " << SERVER_PORT << ", data:";
		cin >> data;

		if("q" == data or 
			"Q" == data or
			"exit" == data)
		{
			cout << "Exit the client!" << endl;
			close(conn);
			return 0;
		}

        send(conn, data.c_str(), data.size() , 0);
        //cout << "Send to server >>>>>>>>> :" << data << endl;

        //接收数据
		memset(receiveBuf, 0, RECEIVE_BUFF_SIZE);
        int num = recv(conn, receiveBuf, RECEIVE_BUFF_SIZE, 0);
        if(num > 1)
        {
        	cout << "Recv from server <<<<<<< :" << receiveBuf << endl;
        }
    }

	return 0;
}

int connectToServer(int & sockfd, const char* server_ip, const int server_port)
{
    sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( -1 == sockfd )
    {
        perror( "sock created" );
        exit( -1 );
    }

    struct sockaddr_in server;
    memset( &server, 0, sizeof( struct sockaddr_in ) );
    server.sin_family      = AF_INET;
    //server.sin_port        = htons(SERVER_PORT) ;
    //server.sin_addr.s_addr = inet_addr(SERVER_IP) ;
    server.sin_port        = htons(server_port) ;
    server.sin_addr.s_addr = inet_addr(server_ip) ;

    int res = -1;
    res = connect( sockfd, (struct sockaddr*)&server, sizeof( sockaddr ) );
    if( -1 == res )
    {
        perror( "Sock connect" );
    	close(sockfd);
        exit( -1 );
    }

    return 0;
}

int main(int argc, char* argv[])
{
	if (2 > argc)
	{
        cout << "Please input right args!" << endl;
        exit( -1 );
	}

    int sockfd = -1;
	connectToServer(sockfd, argv[1], atoi(argv[2]));
    sendMsg(sockfd);

    close(sockfd);

#if 0
    thread one(connectToServer, data);
	one.join();
    while(1)
    {
		sleep(1);
    }
#endif
    return 0;
}

