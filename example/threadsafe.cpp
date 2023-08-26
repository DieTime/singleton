/*
 * SPDX-FileCopyrightText: Copyright 2023 Denis Glazkov <glazzk.off@mail.ru>
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <singleton>

struct ThreadSafe : singleton::threadsafe<ThreadSafe>
{
    void process()
    {
        std::cout << "ThreadSafe::process()" << std::endl;
    }
};

int main()
{
    ThreadSafe::instance()->process();
    return 0;
}
