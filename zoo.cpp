// You have been hired by the government of Fasaconia to move the animals from their
//     current preserve to a zoo across the river because developers want to build a giant
//         shopping mall on the site of the preserve.It has been observed that if 10 bison and one lion
//             are placed in a boat,
//     then the trip proceeds safely.If, however, the number of bison is between one and nine, then the lion will eat at least one of them.On the other hand, if the number of bison exceeds 10, then they will gang up on the lion and rid the animal kingdom of one of its mighty carnivores.The animals arrive one at a time in an arbitrary order.There are 1, 000 bison and 100 lions.A sailor is available to ferry a decrepit boat back and forth,
//     but due to budgetary constraints, the solution must be limited to 100 round trips.The boat cannot take more than 11 animals at a time.Write monitor code that allows the animals to board the boat safely.The monitor should implement the functions BisonArrives(),
//     LionArrives(), and Sailor(), together with all necessary state and condition variables.
#include <iostream>
#include <mutex>
#include <thread>

class Zoo
{

public:
    std::mutex global_mutex;
    std::condition_variable boat_needs_lion;
    std::condition_variable boat_needs_bison;

    std::condition_variable conditions_met_to_sail;

    int boat_lion_count;
    int boat_bison_count;

    void LionArrives()
    {
        std::unique_lock<std::mutex> cur_mutex(global_mutex);

        while (boat_lion_count >= 1 || boat_bison_count != 10)
        {
            boat_needs_lion.wait(cur_mutex);
        }
        // boat_lion_count is = 0 and we have 10 bison, and we have global lock
        boat_lion_count += 1;

        conditions_met_to_sail.notify_one();
    }
    void BisonArrives()
    {
        std::unique_lock<std::mutex> cur_mutex(global_mutex);

        while (boat_bison_count >= 10)
        {
            boat_needs_bison.wait(cur_mutex);
        }

        boat_bison_count += 1;

        if (boat_bison_count == 10)
        {
            boat_needs_lion.notify_one();
        }
    }

    void Sailor()
    {
        std::unique_lock<std::mutex> cur_mutex(global_mutex);

        while (boat_bison_count != 10 || boat_lion_count != 1)
        {
            conditions_met_to_sail.wait(cur_mutex);
        }

        std::cout << "sail!" << std::endl;
        boat_bison_count = 0;
        boat_lion_count = 0;

        boat_needs_bison.notify_all();
    }
};

int main() {
    // Zoo main()
};