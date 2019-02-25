#include "NTP_Client.h"

using namespace srv;

NTP_Client::NTP_Client()
{
    ntpServerEP = udp::endpoint(asio::ip::address::from_string("88.147.254.235"), 123);

    n_io_service service;

    //udp\ntp client config
    uSocket = new udp::socket(service);

    try
    {
        uSocket->open(udp::v4());
        std::cout << "UDP Client started>\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Cannot start UDP client:\n"
                  << e.what() << '\n';
    }
}

NTP_Client &NTP_Client::Instance()
{
    static NTP_Client ntpc;
    return ntpc;
}

time_t NTP_Client::GetCurrentTimestamp()
{
    //Мог и shared_ptr создать, но контекста для него нет.
    ntp_packet *packet = new ntp_packet();

    uSocket->send_to(asio::buffer(packet, 48), ntpServerEP);

    udp::endpoint ntpAnswerEndPoint;
    uSocket->receive_from(asio::buffer(packet, 48), ntpAnswerEndPoint);

    time_t timestamp = (ntohl(packet->txTm_s) - NTP_TIMESTAMP_DELTA);

    delete packet;
    return timestamp; // Time-stamp seconds.
}