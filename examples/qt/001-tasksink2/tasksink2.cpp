//
//  Task sink in C++ - design 2
//  Adds pub-sub flow to send kill signal to workers
//

#include "zhelpers.hpp"

int main(int argc, char* argv[])
{
    zmq::context_t context(1);

    //  Socket to receive messages on
    zmq::socket_t receiver(context, ZMQ_PULL);
    receiver.bind("tcp://*:5558");

    //  Socket for worker control
    zmq::socket_t controller(context, ZMQ_PUB);
    controller.bind("tcp://*:5559");

    //  Wait for start of batch
    s_recv(receiver);

    //  Start our clock now
    int64_t startTime = s_clock();

    //  Process 100 confirmations
    int task_nbr;
    for (task_nbr = 0; task_nbr < 100; task_nbr++) {
        s_recv(receiver);

        if (task_nbr % 10 == 0)
            std::cout << ":";
        else
            std::cout << ".";
    }
    //  Calculate and report duration of batch
    int64_t endtime = s_clock();

    int64_t total_msec = endtime - startTime;
    std::cout << "\nTotal elapsed time: " << total_msec
              << " msec\n"
              << std::endl;

    //  Send kill signal to workers
    std::string killStr = "KILL";
    s_send(controller, killStr);

    //  Finished
    Sleep(1); //  Give 0MQ time to deliver
    return 0;
}
