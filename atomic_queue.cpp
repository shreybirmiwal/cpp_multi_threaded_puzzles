// The children in the city are very excited about feeding the animals.
// In the morning, large numbers of children were in front of the zoo wanting to enter.
// However, it is not logistically possible to let everyone enter at the same time, as this would scare the animals.
// Therefore, the zoo manager asked the children to stand in a queue.Write the functions Child_enter and Admit_one.
// A child calls the first function to obtain permission to enter the zoo, while the manager calls the second function to admit the next child.
// You only have access to atomic variables.

#include <atomic>

class atomic_queue
{

    std::atomic<int> cur_ticket_num = 0;
    std::atomic<int> next_ticket = 0;

public:
    void Child_enter()
    {
        int my_ticket = next_ticket++;
        while (cur_ticket_num != my_ticket)
        {
            ;
        }
        // enter!
    }

    void Admit_one()
    {
        cur_ticket_num++;
    }
};