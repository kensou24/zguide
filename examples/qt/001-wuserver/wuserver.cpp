//
//  Weather update server in C++
//  Binds PUB socket to tcp://*:5556
//  Publishes random weather updates
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <zmq.hpp>

#if (defined(WIN32))
#include <zhelpers.hpp>
#endif

#define within(num) (int)((float)num * random() / (RAND_MAX + 1.0))

int main()
{

    //  Prepare our context and publisher
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5556");
    //publisher.bind("tcp://*:5557"); // Not usable on Windows.

    //  Initialize random number generator
    srandom((unsigned)time(NULL));
    while (1) {

        int zipcode, temperature, relhumidity;

        //  Get values that will fool the boss
        zipcode = 10001; //within(100000);
        temperature = within(215) - 80;
        relhumidity = within(50) + 10;

        //  Send message to all subscribers
        zmq::message_t message(20);
        snprintf((char*)message.data(), 20,
            "%05d %d %d", zipcode, temperature, relhumidity);

        //std::cout << "publish message " << message.data() << std::endl;

        publisher.send(message);
    }
    return 0;
}
