/* -*-mode:c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef LINK_QUEUE_HH
#define LINK_QUEUE_HH

#include <queue>
#include <cstdint>
#include <string>
#include <fstream>
#include <memory>

#include "file_descriptor.hh"
#include "binned_livegraph.hh"

class LinkQueue
{
private:
    const static unsigned int PACKET_SIZE = 1504; /* default max TUN payload size */

    struct QueuedPacket
    {
        int bytes_to_transmit;
        std::string contents;
        uint64_t arrival_time;

        QueuedPacket( const std::string & s_contents, const uint64_t s_arrival_time );
    };

    unsigned int next_delivery_;
    std::vector< uint64_t > schedule_;
    uint64_t base_timestamp_;

    std::queue< QueuedPacket > packet_queue_;

    std::unique_ptr<std::ofstream> log_;
    std::unique_ptr<BinnedLiveGraph> graph_;

    bool repeat_;

    uint64_t next_delivery_time( void ) const;

    void use_a_delivery_opportunity( void );

    void discard_wasted_opportunities( const uint64_t now );

public:
    LinkQueue( const std::string & link_name, const std::string & filename, const std::string & logfile, const bool repeat, const bool graph );

    void read_packet( const std::string & contents );

    void write_packets( FileDescriptor & fd );

    unsigned int wait_time( void );

    bool pending_output( void );
};

#endif /* LINK_QUEUE_HH */
