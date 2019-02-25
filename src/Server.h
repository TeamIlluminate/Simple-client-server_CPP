#pragma once
#include <list>
#include <string>
#include <thread>
#include <map>
#include "Packets.h"
#include "UserConnection.h"

namespace srv
{

class Server
{

public:

  static Server &Instance();
  int Listen();
  void RemoveUser(std::shared_ptr<U_Conn> u_connection);
  std::pair<std::string, int> FindTimeZone(std::string timezone);
private:

  std::list<std::shared_ptr<U_Conn>> user_connections;
  std::map<std::string, int> timezones; //DB of TimeZones

  Server();
  ~Server(){};
  Server(const Server &) = delete;
  Server operator=(const Server &) = delete;
};

} // namespace srv