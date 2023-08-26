# SPDX-FileCopyrightText: Copyright 2023 Denis Glazkov <glazzk.off@mail.ru>
# SPDX-License-Identifier: MIT

singleton = declare_dependency(include_directories: ['.'])

pkgconfig = import('pkgconfig')
pkgconfig.generate(name: meson.project_name(),
                   description: 'Library for creating easily mockable C++ singleton classes',
                   version: meson.project_version(),
                   subdirs: meson.project_name())

install_headers('singleton.h', 'singleton', subdir: meson.project_name())
