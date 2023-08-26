/*
 * SPDX-FileCopyrightText: Copyright 2023 Denis Glazkov <glazzk.off@mail.ru>
 * SPDX-License-Identifier: MIT
 */

#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>
#include <mutex>
#include <type_traits>

#ifndef SINGLETON_MOCK_ENABLED
#define SINGLETON_MOCK_ENABLED 0
#endif

namespace singleton {

namespace details {

struct threadsafe
{
    using primitive = std::mutex;
};

struct nonthreadsafe
{
};

struct noninterface
{
};

struct mutex
{
};

struct noncopy
{
protected:
    noncopy() = default;
    ~noncopy() = default;

private:
    noncopy(const noncopy&) = delete;
    noncopy& operator=(const noncopy&) = delete;
};

struct nonmove
{
protected:
    nonmove() = default;
    ~nonmove() = default;

private:
    nonmove(nonmove&&) = delete;
    nonmove& operator=(nonmove&&) = delete;
};

template<typename T, typename Threadsafe>
struct synchronized
{
protected:
    static inline details::mutex mutex;
};

template<typename T>
struct synchronized<T, threadsafe>
{
protected:
    static inline threadsafe::primitive mutex;
};

template<typename ThreadSafe>
struct guard : public std::lock_guard<threadsafe::primitive>
{
    using std::lock_guard<threadsafe::primitive>::lock_guard;
};

template<>
struct guard<nonthreadsafe>
{
    guard(mutex&) {}
};

template<typename Type, typename Interface>
struct instance : public Interface
{
protected:
    static inline std::shared_ptr<Interface> implementation = nullptr;
};

template<typename Type>
struct instance<Type, noninterface>
{
protected:
    static inline std::shared_ptr<Type> implementation = nullptr;
};

template<typename Interface, typename U = details::noninterface>
using interface_t = std::conditional_t<SINGLETON_MOCK_ENABLED, Interface, U>;

} /* namespace details */

template<typename ThreadSafe, typename Type, typename Interface>
class base
    : public details::noncopy
    , public details::nonmove
    , public details::synchronized<Type, ThreadSafe>
    , public details::instance<Type, details::interface_t<Interface>>
{
private:
    using Instance = details::instance<Type, details::interface_t<Interface>>;
    using Syncronizer = details::synchronized<Type, ThreadSafe>;

public:
    static auto instance()
    {
        details::guard<ThreadSafe> guard(Syncronizer::mutex);

        if (!Instance::implementation) {
            if constexpr (std::is_default_constructible_v<Type>) {
                Instance::implementation = std::make_shared<Type>();
            } else {
                throw std::runtime_error(
                    std::string(__PRETTY_FUNCTION__) +
                    std::string(" attempt to use not initialized singleton class, please use") +
                    std::string(" initialize() before first use"));
            }
        }

        return Instance::implementation;
    }

    template<typename... Args>
    static void initialize(Args&&... args)
    {
        details::guard<ThreadSafe> guard(Syncronizer::mutex);
        Instance::implementation = std::make_shared<Type>(std::forward<Args>(args)...);
    }

#if SINGLETON_MOCK_ENABLED == 1
    template<typename Mock, typename... Args>
    static void mock(Args&&... args)
    {
        details::guard<ThreadSafe> guard(Syncronizer::mutex);
        Instance::implementation = std::make_shared<Mock>(std::forward<Args>(args)...);
    }
#endif
};

template<typename T, typename Interface = details::noninterface>
using threadsafe = base<details::threadsafe, T, Interface>;

template<typename T, typename Interface = details::noninterface>
using nonthreadsafe = base<details::nonthreadsafe, T, Interface>;

} /* namespace singleton */

#endif /* SINGLETON_H */
