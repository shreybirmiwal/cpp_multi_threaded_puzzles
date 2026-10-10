#include <iostream>
#include <mutex>
#include <thread>

int main()
{

    return 0;
}

class family
{

public:
    std::mutex global_mutex;
    int fish_basket_capacity = 5;
    int pot_capacity = 3;

    int cur_fish_in_basket = 0;
    int cur_fish_in_pot = 0;

    std::condition_variable empty_basket; // father
    std::condition_variable can_cook;     // mother
    std::condition_variable kids_can_eat;

    void father_bring_fish(int amount_fish)
    {
        std::unique_lock<std::mutex> cur_lock(global_mutex);

        while (cur_fish_in_basket != 0)
        {
            empty_basket.wait(cur_lock);
        }

        // the cur_fish_in_basket == 0, and we have lock
        cur_fish_in_basket += amount_fish;

        // lets signal to the chef
        can_cook.notify_one();
    }

    void mother_cook()
    {
        std::unique_lock<std::mutex> cur_lock(global_mutex);

        while (cur_fish_in_basket == 0 || cur_fish_in_pot == pot_capacity)
        {
            can_cook.wait(cur_lock);
        }

        // fish in basket, and we arent at capacity, and we have lock
        cur_fish_in_pot += 1;
        cur_fish_in_basket -= 1;

        if (cur_fish_in_basket == 0)
        {
            empty_basket.notify_one();
        }
        if (cur_fish_in_pot == 1)
        {
            kids_can_eat.notify_all();
        }
    }

    void kid_eat()
    {
        std::unique_lock<std::mutex> cur_lock(global_mutex);

        while (cur_fish_in_pot == 0)
        {
            kids_can_eat.wait(cur_lock);
        }

        // fish in pot and we have lock
        cur_fish_in_pot -= 1;
        if (cur_fish_in_pot == pot_capacity - 1)
        {
            can_cook.notify_one();
        }

        if (cur_fish_in_pot == 0)
        {
            empty_basket.notify_one();
        }
    }
};