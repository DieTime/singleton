# SPDX-FileCopyrightText: Copyright 2023 Denis Glazkov <glazzk.off@mail.ru>
# SPDX-License-Identifier: MIT

deps = [ singleton ]
args = [ '-DSINGLETON_MOCK_ENABLED=1' ]

examples = [
    {
        'name': 'threadsafe-example',
        'source': 'threadsafe.cpp',
    },
    {
        'name': 'nonthreadsafe-example',
        'source': 'nonthreadsafe.cpp',
    },
    {
        'name': 'mock-example',
        'source': 'mock.cpp',
        'args': args,
    },
]

foreach example : examples
    test(example['name'],
         executable(example['name'],
                    example['source'],
                    dependencies: deps,
                    cpp_args: example.get('args', [])))
endforeach
