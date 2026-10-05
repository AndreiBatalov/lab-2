#include "tasks.h"

#include <algorithm>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
template <class Container>
void PrintValues(const Container& values)
{
    bool isFirst = true;

    for (const auto& value : values)
    {
        if (!isFirst)
        {
            std::cout << ' ';
        }

        std::cout << value;
        isFirst = false;
    }

    if (isFirst)
    {
        std::cout << "(пусто)";
    }

    std::cout << '\n';
}

int RandomValue(std::mt19937& generator)
{
    std::uniform_real_distribution<double> distribution(0.0, 5.0);
    return static_cast<int>(std::round(distribution(generator)));
}

bool ReadNumber(double& value)
{
    std::string line;

    while (true)
    {
        std::cout << "Введите число: ";

        if (!std::getline(std::cin, line))
        {
            return false;
        }

        std::istringstream input(line);
        double number = 0.0;

        if (input >> number)
        {
            input >> std::ws;

            if (input.eof() && std::isfinite(number))
            {
                value = number;
                return true;
            }
        }

        std::cout << "Ошибка: введите одно конечное число.\n";
    }
}

bool ContinueInput()
{
    std::string line;

    while (true)
    {
        std::cout << "Продолжить ввод? 1 — да, 0 — нет: ";

        if (!std::getline(std::cin, line))
        {
            return false;
        }

        std::istringstream input(line);
        int choice = -1;

        if (input >> choice)
        {
            input >> std::ws;

            if (input.eof() && (choice == 0 || choice == 1))
            {
                return choice == 1;
            }
        }

        std::cout << "Ошибка: выберите 0 или 1.\n";
    }
}

std::map<std::string, double> ReadGlasses(const std::filesystem::path& path)
{
    std::ifstream input(path);

    if (!input)
    {
        throw std::runtime_error("Не удалось открыть файл стёкол: " + path.string());
    }

    std::map<std::string, double> glasses;
    std::string line;
    std::size_t lineNumber = 0;

    while (std::getline(input, line))
    {
        ++lineNumber;
        std::istringstream row(line);
        row >> std::ws;

        if (row.eof())
        {
            continue;
        }

        std::string name;
        double refractiveIndex = 0.0;

        if (!(row >> name >> refractiveIndex))
        {
            throw std::runtime_error("Некорректные данные в строке " + std::to_string(lineNumber));
        }

        row >> std::ws;

        if (!row.eof() || !std::isfinite(refractiveIndex) || refractiveIndex <= 0.0)
        {
            throw std::runtime_error("Некорректный показатель преломления в строке "
                                     + std::to_string(lineNumber));
        }

        if (!glasses.emplace(name, refractiveIndex).second)
        {
            throw std::runtime_error("Повторяющееся название стекла: " + name);
        }
    }

    if (input.bad())
    {
        throw std::runtime_error("Ошибка чтения файла стёкол.");
    }

    if (glasses.empty())
    {
        throw std::runtime_error("Файл стёкол пуст.");
    }

    return glasses;
}
}

void RunArrayTask()
{
    std::cout << "\nДинамический массив double\n";
    constexpr int size = 10;
    double* values = new double[size];
    double sum = 0.0;

    for (int i = 0; i < size; ++i)
    {
        values[i] = static_cast<double>(i) * 10.0;
        sum += values[i];
        std::cout << values[i] << (i + 1 == size ? '\n' : ' ');
    }

    std::cout << "Среднее арифметическое: " << sum / size << '\n';
    delete[] values;
}

void RunVectorTask(std::mt19937& generator)
{
    std::cout << "\nВектор int\n";
    std::vector<int> values(15);

    for (auto& value : values)
    {
        value = RandomValue(generator);
    }

    std::cout << "Исходный вектор: ";
    PrintValues(values);
    std::cout << "Максимальный элемент: "
              << *std::max_element(values.begin(), values.end()) << '\n';
    std::cout << "Сумма элементов: "
              << std::accumulate(values.begin(), values.end(), 0) << '\n';

    const auto withoutZeros = std::remove(values.begin(), values.end(), 0);
    values.resize(static_cast<std::size_t>(std::distance(values.begin(), withoutZeros)));
    std::cout << "После удаления нулей: ";
    PrintValues(values);

    std::sort(values.begin(), values.end());
    std::cout << "После сортировки: ";
    PrintValues(values);

    const auto withoutDuplicates = std::unique(values.begin(), values.end());
    values.resize(static_cast<std::size_t>(std::distance(values.begin(), withoutDuplicates)));
    std::cout << "После удаления повторов: ";
    PrintValues(values);
}

void RunListTask()
{
    std::cout << "\nСписок double\n";
    std::list<double> values;
    double value = 0.0;

    while (ReadNumber(value))
    {
        values.push_back(value);

        if (!ContinueInput())
        {
            break;
        }
    }

    std::cout << "Содержимое списка: ";
    PrintValues(values);

    if (values.empty())
    {
        std::cout << "Максимум не определён: список пуст.\n";
        return;
    }

    const auto maximum = std::max_element(values.begin(), values.end(),
                                         [](double left, double right)
                                         {
                                             return std::abs(left) < std::abs(right);
                                         });
    std::cout << "Наибольший по модулю элемент: " << *maximum
              << ", модуль: " << std::abs(*maximum) << '\n';
}

void RunMapTask(const std::filesystem::path& path)
{
    std::cout << "\nАссоциативный массив map<string, double>\n";
    const auto glasses = ReadGlasses(path);
    std::cout << "Название — показатель преломления\n";

    for (const auto& [name, refractiveIndex] : glasses)
    {
        std::cout << name << " — " << refractiveIndex << '\n';
    }

    const auto extremes = std::minmax_element(glasses.begin(), glasses.end(),
                                            [](const auto& left, const auto& right)
                                            {
                                                return left.second < right.second;
                                            });

    std::cout << "Минимальный показатель преломления: " << extremes.first->second << '\n';

    for (const auto& [name, refractiveIndex] : glasses)
    {
        if (refractiveIndex == extremes.first->second)
        {
            std::cout << "Стекло: " << name << '\n';
        }
    }

    std::cout << "Максимальный показатель преломления: " << extremes.second->second << '\n';

    for (const auto& [name, refractiveIndex] : glasses)
    {
        if (refractiveIndex == extremes.second->second)
        {
            std::cout << "Стекло: " << name << '\n';
        }
    }
}

void RunDequeAndSetTask(std::mt19937& generator)
{
    std::cout << "\nDeque int и set int\n";
    std::deque<int> values(15);

    for (auto& value : values)
    {
        value = RandomValue(generator);
    }

    std::cout << "Содержимое deque: ";
    PrintValues(values);
    const std::set<int> uniqueValues(values.begin(), values.end());
    std::cout << "Содержимое set: ";
    PrintValues(uniqueValues);
}
