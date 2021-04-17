#pragma once

#include <cstring>
#include <iostream>
#include <limits>
#include <memory>
#include <new>

namespace smtp {

// This allocator zeroes out the memory for the object that is allocated once
// the object goes out of scope. This allocator is used to construct objects
// such as vectors or strings that may contain passwords.
template <class T> 
class SecureAllocator 
{
    public:
        using value_type = T;
        using pointer = T *;
        using const_pointer = const T *;

        using void_pointer = void *;
        using const_void_pointer = const void *;

        using size_type = size_t;
        using difference_type = std::ptrdiff_t;

        template <class U> 
        struct rebind { using other = SecureAllocator<U>; };

        SecureAllocator() = default;
        ~SecureAllocator() = default;

        template <class U> SecureAllocator(const SecureAllocator<U> &other) {}

        pointer allocate(size_type num_objects) 
        {
            if (num_objects > (max_size() / sizeof(T)))
                throw std::bad_alloc();
            return static_cast<pointer>(new value_type[num_objects]);
        }

        pointer allocate(size_type num_objects, const_void_pointer hint) 
        {
            return allocate(num_objects);
        }

        void deallocate(pointer p, size_type num_objects) 
        {
            std::memset(p, 0x0, num_objects);
            delete[] p;
        }

        size_type max_size() const { return std::numeric_limits<size_type>::max(); }
};

using secure_string =
    std::basic_string<char, std::char_traits<char>, SecureAllocator<char>>;

} // namespace smtp