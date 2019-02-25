/*
    Регламент количества и порядка байт общения между клиентом и сервером, так же синонимы для работы с booost::aiso .
*/

#include <iostream>
#include <asio.hpp>
#pragma once

typedef std::shared_ptr<asio::ip::tcp::socket> socket_ptr;
typedef asio::io_service n_io_service; //net input\output service
typedef asio::ip::tcp tcp;
typedef asio::ip::udp udp;

struct TimePacket
{
    // int8_t flags; // Управляющий байт(8 флагов, при необходимости можно проконтроллировать начало\конец транзакции, перессылку при обрыве и т.д.) //

    // Команда сообщения(является символьным обозначением функции-обработчика)
    char command[3];
    // Данные(аргументы). Использовать можно как угодно.
    char data[3];
};

//Definition of NTP\UDP\IP packet
struct ntp_packet
{

    uint8_t li_vn_mode = 0x1b; // Eight bits. li, vn, and mode.
                               // li.   Two bits.   Leap indicator.
                               // vn.   Three bits. Version number of the protocol.
                               // mode. Three bits. Client will pick mode 3 for client.

    uint8_t stratum;   // Eight bits. Stratum level of the local clock.
    uint8_t poll;      // Eight bits. Maximum interval between successive messages.
    uint8_t precision; // Eight bits. Precision of the local clock.

    uint32_t rootDelay;      // 32 bits. Total round trip delay time.
    uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock source.
    uint32_t refId;          // 32 bits. Reference clock identifier.

    uint32_t refTm_s; // 32 bits. Reference time-stamp seconds.
    uint32_t refTm_f; // 32 bits. Reference time-stamp fraction of a second.

    uint32_t origTm_s; // 32 bits. Originate time-stamp seconds.
    uint32_t origTm_f; // 32 bits. Originate time-stamp fraction of a second.

    uint32_t rxTm_s; // 32 bits. Received time-stamp seconds.
    uint32_t rxTm_f; // 32 bits. Received time-stamp fraction of a second.

    uint32_t txTm_s; // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
    uint32_t txTm_f; // 32 bits. Transmit time-stamp fraction of a second.
};