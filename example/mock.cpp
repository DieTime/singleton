/*
 * SPDX-FileCopyrightText: Copyright 2023 Denis Glazkov <glazzk.off@mail.ru>
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <singleton>
#include <string>

struct Interface
{
    virtual ~Interface() = default;
    virtual void process() = 0;
};

struct Singleton : singleton::threadsafe<Singleton, Interface>
{
    Singleton(std::string message)
        : message(std::move(message))
    {
    }

    void process()
    {
        std::cout << "Singleton::process(\"" << message << "\")";
    }

private:
    std::string message;
};

struct Mock : public Interface
{
    Mock(std::string message)
        : message(std::move(message))
    {
    }

    void process()
    {
        std::cout << "Mock::process(\"" << message << "\")";
    }

private:
    std::string message;
};

int main()
{
    Singleton::initialize("message");
    Singleton::instance()->process();

    Singleton::mock<Mock>("message");
    Singleton::instance()->process();

    return 0;
}
