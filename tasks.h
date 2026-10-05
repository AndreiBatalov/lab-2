#pragma once

#include <filesystem>
#include <random>

void RunArrayTask();
void RunVectorTask(std::mt19937& generator);
void RunListTask();
void RunMapTask(const std::filesystem::path& path);
void RunDequeAndSetTask(std::mt19937& generator);
