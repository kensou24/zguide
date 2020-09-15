//
//  Task sink in C++
//  Binds PULL socket to tcp://localhost:5558
//  Collects results from workers via that socket
//

#include <time.h>
#include <zmq.hpp>

#if (defined(WIN32))
#include <zhelpers.hpp>
#else
#include <unistd.h>
#endif

#include <iostream>

int main(int argc, char* argv[])
{
    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t receiver(context, ZMQ_PULL);
    receiver.bind("tcp://*:5558");

    //  Wait for start of batch
    zmq::message_t message;
    receiver.recv(&message);

    //  Start our clock now
    int64_t startTime = s_clock();

    //  Process 100 confirmations
    int task_nbr;
    int total_msec = 0; //  Total calculated cost in msecs
    for (task_nbr = 0; task_nbr < 100; task_nbr++) {

        receiver.recv(&message);
        if (task_nbr % 10 == 0)
            std::cout << ":" << std::flush;
        else
            std::cout << "." << std::flush;
    }
    //  Calculate and report duration of batch
    int64_t endtime = s_clock();

    total_msec = endtime - startTime;
    std::cout << "\nTotal elapsed time: " << total_msec << " msec\n"
              << std::endl;
    return 0;
}
