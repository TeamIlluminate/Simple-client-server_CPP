#include "Server.h"

int main(int, char**) {
    return srv::Server::Instance().Listen();
}
