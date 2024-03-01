#pragma once

#include <cstddef>

/**
 * @brief A smart pointer that allows multiple instances to share ownership of an object.
 * @tparam T The type of the object being managed.
 */

template<typename T>
class SharedPtr
{
private:
    T *_ptr;                    // The managed pointer to the object.
    std::size_t *_ref_count;    // Reference count for shared ownership.

public:
    /**
     * @brief Default constructor for SharedPtr. Initializes the pointer and reference count to nullptr.
     */
    [[nodiscard]] constexpr SharedPtr() noexcept: _ptr(nullptr), _ref_count(nullptr)
    {}

    /**
     * @brief Constructor for SharedPtr that takes an initial raw pointer.
     * @param ptr The raw pointer to manage.
     */
    [[nodiscard]] constexpr explicit SharedPtr(T *ptr) noexcept: _ptr(ptr)
    {
        _ref_count = new std::size_t(1);
    }

    /**
     * @brief Copy constructor for SharedPtr.
     * @param other The SharedPtr to copy.
     */
    [[nodiscard]] SharedPtr(const SharedPtr &other) noexcept: _ptr(other._ptr), _ref_count(other._ref_count)
    {
        if (_ref_count)
        {
            (*_ref_count)++;
        }
    }

    /**
     * @brief Assignment operator for SharedPtr.
     * @param other The SharedPtr to assign from.
     * @return A reference to the assigned SharedPtr.
     */
    SharedPtr &operator=(const SharedPtr &other) noexcept
    {
        if (this != &other)
        {
            if (_ref_count && --(*_ref_count) == 0)
            {
                delete _ptr;
                delete _ref_count;
            }

            _ptr = other._ptr;
            _ref_count = other._ref_count;

            if (_ref_count)
            {
                (*_ref_count)++;
            }
        }

        return *this;
    }

    /**
     * @brief Destructor for SharedPtr.
     * It decreases the reference count and deletes the object if there are no more references.
     */
    ~SharedPtr() noexcept
    {
        if (_ref_count && --(*_ref_count) == 0)
        {
            delete _ptr;
            delete _ref_count;
        }
    }

    /**
     * @brief Get the raw pointer to the managed object.
     * @return A raw pointer to the managed object.
     */
    [[nodiscard]] constexpr T *Get() const noexcept
    {
        return _ptr;
    }

    /**
     * @brief Get the current reference count, indicating the number of SharedPtr instances sharing ownership.
     * @return The current reference count.
     */
    [[nodiscard]] constexpr std::size_t UseCount() const noexcept
    {
        if (_ref_count)
        {
            return *_ref_count;
        }
        return 0;
    }
};