#include <thread>
#include <functional>
#include "NTP_Client.h"
#pragma once

namespace srv
{

class UserConnection : public std::enable_shared_from_this<UserConnection>
{

  typedef std::function<void(UserConnection&, std::string)> StringDelegate;

  public:
    UserConnection(socket_ptr c_socket);

  private:
    socket_ptr client_socket;
    std::map<std::string, StringDelegate> handleFunctions;

    void Handler();

    void TimeCommand(std::string);

    void Send(std::string message);
    void TerminateConnection(std::string);
};

typedef UserConnection U_Conn;

} // namespace srv