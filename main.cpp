#include "tasks.h"

#include <cstdint>
#include <exception>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>

int main(int argc, char* argv[])
{
    if (argc > 3)
    {
        std::cerr << "Использование: lab2 [файл стёкол] [seed]\n";
        return 1;
    }

    try
    {
        const std::filesystem::path glassesPath = argc > 1 ? argv[1] : "glasses.txt";
        std::mt19937::result_type seed = 0;

        if (argc > 2)
        {
            std::istringstream input(argv[2]);
            unsigned long long parsedSeed = 0;
            input >> std::ws;

            if (input.peek() == '-' || !(input >> parsedSeed))
            {
                throw std::invalid_argument("Seed должен быть целым числом от 0 до 4294967295.");
            }

            input >> std::ws;

            if (!input.eof() || parsedSeed > std::numeric_limits<std::uint32_t>::max())
            {
                throw std::invalid_argument("Seed должен быть целым числом от 0 до 4294967295.");
            }

            seed = static_cast<std::mt19937::result_type>(parsedSeed);
        }
        else
        {
            std::random_device randomDevice;
            seed = randomDevice();
        }

        std::mt19937 generator(seed);
        std::cout << std::setprecision(10) << "Seed: " << seed << '\n';
        RunArrayTask();
        RunVectorTask(generator);
        RunListTask();
        RunMapTask(glassesPath);
        RunDequeAndSetTask(generator);
    }
    catch (const std::exception& error)
    {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
