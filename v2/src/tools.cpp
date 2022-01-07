//
// Created by eh on 1/4/22.
//
#include "tools.hpp"
#include "tools.hpp"
#include <locale>
#include <vector>
#include <random>
namespace DofM
{
    Tools::Tools()
    {
        this->rng.seed(std::random_device()());
    }
}