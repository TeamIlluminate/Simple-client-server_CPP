#include "UserConnection.h"
#include "Server.h"

using namespace srv;

UserConnection::UserConnection(socket_ptr c_socket) : client_socket(c_socket)
{
    std::thread handleThread(&UserConnection::Handler, this);
    handleThread.detach();

    // Строго регламинтировал порядок и количество байт сообщения(добавив команду).
    // 1 - Позволит фильтровать невалидные запросы.
    // 2 - telnet постоянно подкидывает два эскейп-символа, неприятно.
    // 3 - Позволяет биндинги функций-обработчиков делать
    handleFunctions.insert(std::pair<std::string, StringDelegate>("TIM", &UserConnection::TimeCommand));
    handleFunctions.insert(std::pair<std::string, StringDelegate>("EXI", &UserConnection::TerminateConnection));
}

void UserConnection::Handler()
{
    while (true)
    {
        size_t len = 0;
        TimePacket *t_packet = new TimePacket;
        try
        {
            len = client_socket->read_some(asio::buffer(t_packet, sizeof(TimePacket)));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error while reading data from netStream:\n"
                      << e.what() << '\n';
            break;
        }

        if (len > 0)
        {
            std::string cmd(t_packet->command, sizeof(t_packet->command));
            if (handleFunctions.find(cmd) != handleFunctions.end())
            {
                handleFunctions[cmd](*this, t_packet->data);
            } //Раньше здесь стоял else с отправкой извещения о неизвестной команде, но telnet так и норовит докинуть 2 байта, что нарушает эстетику.
            delete t_packet;
        }
        else
            break;
    }
    
    client_socket->close();
    Server::Instance().RemoveUser(shared_from_this());
}

/// HANDLERS

void UserConnection::Send(std::string message)
{
    try
    {
        write(*client_socket, asio::buffer(message, message.size()));
        write(*client_socket, asio::buffer("\n", 1));
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error while writing data in netStream"
                  << e.what() << '\n';
    }
}

void UserConnection::TimeCommand(std::string timezone_name)
{
    auto tz = Server::Instance().FindTimeZone(timezone_name);
    time_t ts = NTP_Client::Instance().GetCurrentTimestamp() + (tz.second * 3600); // seconds from 1 jan 1900 (NTP style) and additional UTC

    auto glTm = gmtime(&ts);
    std::string timeRepr = asctime(glTm);   //Convert Time-stamp to user-readable format
    timeRepr.erase(timeRepr.size() - 1, 1); //Remove '\n'
    std::string answTime = timeRepr + " " + tz.first;
    Send(answTime);
}

void UserConnection::TerminateConnection(std::string)
{
    //Достаточно сомнительное решение - отрубать сокет, чтобы блокирующая функция чтения из потока выбрасывала обрабатываемое исключение.
    //Но для устройства более адекватного решения нужно нагромождать и дальше функционалом, что еще больше выходит за рамки задания(но на git это решение будет добавлено позже).
    client_socket->shutdown(client_socket->shutdown_both);
}
