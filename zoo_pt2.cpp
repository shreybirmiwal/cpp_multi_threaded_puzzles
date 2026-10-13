// The Zoo has been facing financial diﬃculties because recent tax cuts did not allow adequate levels
//     of funding.In an eﬀort to cut expenses,
//     the zoo decided that visitors will be permitted to feed the
//         animals.To eat,
//     an animal has to come to the exhibit cage where it can receive some food from the
//         visitors,
//     do some tricks, and go back to the stable behind.There is only one cage left in the zoo, the rest were sold to raise some cash.The conditions are as follows :
// • Up to five herbivores can be in the exhibit cage.
// • No more than one carnivore can be in the exhibit cage.
// • Carnivores should not be allowed to starve, otherwise they become very aggressive.You are required to write an application to allow the animals to safely enter the exhibit cage and eat.Using a monitor, write the functions enter_herbivore(), exit_herbivore(), enter_carnivore() and exit_carnivore().

#include <mutex>
#include <condition_variable>

class HungryZoo
{
private:
    int herbivore_count_in_cage = 0;
    int carnivore_count_in_cage = 0;

    int waiting_carnivores = 0;

    std::mutex global_mutex;
    std::condition_variable carnivore_can_enter;
    std::condition_variable herbivore_can_enter;

public:
    void enter_herbivore()
    {
        std::unique_lock<std::mutex> gl(global_mutex);

        while (herbivore_count_in_cage >= 5 || waiting_carnivores != 0 || carnivore_count_in_cage != 0)
        {
            herbivore_can_enter.wait(gl);
        }
        herbivore_count_in_cage += 1;
    }
    void exit_herbivore()
    {
        std::unique_lock<std::mutex> gl(global_mutex);
        herbivore_count_in_cage -= 1;

        if (herbivore_count_in_cage == 0)
        {
            carnivore_can_enter.notify_one();
        }

        herbivore_can_enter.notify_one();
    }
    void enter_carnivore()
    {
        std::unique_lock<std::mutex> gl(global_mutex);
        waiting_carnivores += 1;
        while (carnivore_count_in_cage >= 1 || herbivore_count_in_cage != 0)
        {
            carnivore_can_enter.wait(gl);
        }
        waiting_carnivores -= 1;
        // 0 carnivores in cage, we have lock
        carnivore_count_in_cage += 1;
    }
    void exit_carnivore()
    {
        std::unique_lock<std::mutex> gl(global_mutex);
        carnivore_count_in_cage -= 1;
        carnivore_can_enter.notify_one();
        herbivore_can_enter.notify_all();
    }
};