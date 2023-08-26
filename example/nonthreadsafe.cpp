/*
 * SPDX-FileCopyrightText: Copyright 2023 Denis Glazkov <glazzk.off@mail.ru>
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <singleton>

struct NonThreadSafe : singleton::nonthreadsafe<NonThreadSafe>
{
    void process()
    {
        std::cout << "NonThreadSafe::process()" << std::endl;
    }
};

int main()
{
    NonThreadSafe::instance()->process();
    return 0;
}
