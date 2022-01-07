//
// Created by eh on 1/4/22.
//
#include <string>
#include <iostream>
#include "tools.hpp"
#include "gameloop.hpp"

auto run() -> void
{
    std::cout << "Test from run()" << std::endl;
}

int main (int argc, char *argv[])
{
    run();
    DofM::Tools bungo;
    DofM::GameLoop loop;
    loop.Run();
}
/*
   int a;
    char ch[80];

    // Enter input from user
    // - 4 for example
    cin >> a;

    // Get input from user -
    // "GeeksforGeeks" for example
    cin.getline(ch,80);

    // Prints 4
    cout << a << endl;

    // Printing string : This does
    // not print string
    cout << ch << endl;

 * */