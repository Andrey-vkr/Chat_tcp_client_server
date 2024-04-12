#include "Server.h"


void Server::createSocket()
{
    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == -1)
    {
        std::cout << "Creation of Socket failed!" << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "Socket creation was successful.!" << std::endl;
    }
}

void Server::bindSocket()
{
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    bind_status = bind(sockert_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (bind_status == -1) 
    {
        std::cout << "Socket binding failed.!" << std::endl;
        exit(1);
    }
    else 
    {
        std::cout << "The socket binding was successful!" << std::endl;
    }
}

void Server::closeSocket()
{
    close(sockert_file_descriptor);
}

void Server::dataReceivingServer()
{
    connection_status = listen(sockert_file_descriptor, 5);

    if (connection_status == -1)
    {
        std::cout << "Socket is unable to listen for new connections.!" << std::endl;
        exit(1);
    }
    else 
    {
        std::cout << "Server is listening for new connection: " << std::endl;
    }

    length = sizeof(client);
    connection = accept(sockert_file_descriptor, (struct sockaddr*)&client, &length);

    if (connection == -1)
    {
        std::cout << "Server is unable to accept the data from client.!" << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "The client has joined!" << std::endl;
    }
}

char* Server::readData()
{
    bzero(message, MESSAGE_LENGTH);
    read(connection, message, sizeof(message));
    std::cout << "Data received from client: " << message << std::endl;

    return message;
}

void Server::writeData(std::string serverMessage)
{
    bzero(message, MESSAGE_LENGTH);
    ssize_t bytes = write(connection, strcpy(message, serverMessage.c_str()), sizeof(message));

    if (bytes >= 0)
    {
        std::cout << "Data successfully sent to the client.!" << message << std::endl;
    }
}
