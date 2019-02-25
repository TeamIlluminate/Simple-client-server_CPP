#include "Server.h"

using namespace srv;

Server &Server::Instance()
{
    static Server serv;
    return serv;
}

Server::Server()
{
    timezones.insert(std::pair<std::string, time_t>("BST", 1)); //BST - Btitish Summer Time timezone ?
    timezones.insert(std::pair<std::string, time_t>("IRK", 8));
}

int Server::Listen()
{
    n_io_service service; //boost io-service

    //tcp listener config
    tcp::endpoint listenEP(asio::ip::tcp::v4(), 25565);
    tcp::acceptor listener(service, listenEP);

    std::cout << "<Server started>\n";

    while (true)
    {
        socket_ptr sock(new tcp::socket(service));
        listener.accept(*sock);
        std::cout << "Client connected>\n";
        user_connections.push_back(std::make_shared<U_Conn>(sock));
    }
}

void Server::RemoveUser(std::shared_ptr<U_Conn> u_conn)
{
    user_connections.remove(u_conn);
}

std::pair<std::string, int> Server::FindTimeZone(std::string timezone)
{
    if(timezones.find(timezone) != timezones.end())
    {
        return *timezones.find(timezone);
    }
    else
    {
        return std::pair<std::string, int>("UTC_0", 0);
    }
    
}