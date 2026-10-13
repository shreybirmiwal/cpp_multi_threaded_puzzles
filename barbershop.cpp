// A barbershop consists of a waiting room with n chairs and a separate room containing the
//     barber’s chair.If there are no customers to serve,
//     the barber goes to sleep.If a customer
//         enters the barbershop while the barber is busy and all the chairs in the waiting room are
//             occupied,
//     the customer leaves.If the barber is busy but a waiting - room chair is available,
//     the customer sits in one of the available chairs.If the barber is asleep, the customer wakes the barber.Using semaphores, write a program that coordinates the barber and the customers.

#include <iostream>
#include <semaphore>

constexpr int max_waiting_seats = 4;

class barbershop

{
public:
    std::counting_semaphore<> waiting_spots{max_waiting_seats};
    std::counting_semaphore<> cur_queue{0};
    std::counting_semaphore barber_ready{0};

    void new_customer()
    {
        int res = waiting_spots.try_acquire(); // decrement waiting spots if possible
        if (!res)
        {
            return;
        }

        cur_queue.release();    // incrememnet queue
        barber_ready.acquire(); // wait until barber ready
    }

    void barbe_cut_hair()
    {
        while (true)
        {
            cur_queue.acquire();     // take them off the queue. block if nobody availble
            waiting_spots.release(); // waiting spot freed up
            barber_ready.release();
        }
    }
};