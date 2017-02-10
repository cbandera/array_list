#pragma once
#include <bitset>
#include <cassert>
#include <stdexcept>

namespace cdt {

/// \brief An array like container
/// - supports insertion only at the end
/// - allows deletion anywhere
/// - does not guarantee order
/// - allows random access

template <typename _Tp, size_t _N>
class FixedVector {
public:
    // types:
    typedef FixedVector<_Tp, _N> list_type;
    typedef _Tp value_type;
    typedef size_t size_type;
    typedef size_t position_type;
    typedef size_t difference_type;
    typedef _Tp* pointer;
    typedef _Tp& reference;
    typedef const reference const_reference;
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;

    enum {
        MAX_SIZE = _N /// Maximum size, defined at compile time
    };

public:
    // construct/copy/destroy:
    FixedVector() : _end_index(0){};

    size_type size() const {
        return _end_index;
    };
    size_type capacity() const {
        return MAX_SIZE;
    };
    bool empty() const {
        return _end_index == 0;
    };

    /// Iterator element access:
    iterator begin() {
        return &data[0];
    };
    iterator end() {
        return &data[_end_index];
    };

    /// Reference element access:
    reference front() {
        return *begin();
    }
    const_reference front() const {
        return *begin();
    }
    reference back() {
        return data[_end_index - 1];
    }
    const_reference back() const {
        return data[_end_index - 1];
    }

    /// Access container elements by subscript
    reference operator[](std::size_t idx) {
        assert_valid(idx);
        return data[idx];
    }
    const_reference operator[](std::size_t idx) const {
        assert_valid(idx);
        return data[idx];
    }

    /// Copy element into container
    void push_back(const value_type& x) {
        assert_capacity();
        data[_end_index++] = x;
    }

    /// Move element into container
    void push_back(value_type&& x) {
        assert_capacity();
        data[_end_index++] = std::move(x);
    }

    /// Create new object in container
    // template <typename... _Args>
    // void emplace_back(_Args&&... __args) {
    // assert_valid(_end_index);
    //     data[_end_index++] = (std::forward<_Args>(__args)...);
    // }

    /// Erase last element
    void pop_back() {
        this->erase(_end_index - 1); // end points one past the last element, so decrement it first.
    }

    /// Erase first element
    void pop_front() {
        this->erase(begin());
    }

    /// Erase arbitrary element
    void erase(position_type position) {
        this->erase(&data[position]);
    }
    /// Erase arbitrary element
    void erase(iterator position) {
        /// Assert iterator is within range
        assert_valid(position);

        /// Copy last element to this position to guarantee gapless memory usage
        *position = std::move(data[_end_index - 1]);

        /// Adjust end index
        --_end_index;
    }

    /// Erase all elements from vector.
    void clear() {
        _end_index = 0;
    };

private:
    /// Check whether iterator is within bounds
    void assert_valid(const_iterator it) {
        if (it < &data[0] || it >= &data[_end_index]) {
            throw std::out_of_range("Out of range error.");
        }
    }

    /// Check whether index is within bounds
    void assert_valid(position_type idx) {
        if (idx < 0 || idx >= _end_index) {
            throw std::out_of_range("Out of range error.");
        }
    }

    /// Check whether there is still space left
    void assert_capacity() {
        if (size() >= capacity()) {
            throw std::out_of_range("No space left in container.");
        }
    }

    value_type data[_N];
    position_type _end_index;
};
} // Namespace cdt