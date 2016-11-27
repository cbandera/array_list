#pragma once
#include <bitset>
#include <cassert>
#include <stdexcept>

namespace Per {

template <typename _Tp, size_t _N>
class DLList {
public:
    // types:
    typedef DLList<_Tp, _N> list_type;
    typedef _Tp value_type;
    typedef size_t size_type;
    typedef size_t position_type;
    typedef size_t difference_type;
    typedef _Tp* pointer;
    typedef _Tp& reference;
    typedef const reference const_reference;

private:
    struct Node {
        template <typename... _Args>
        Node(_Args&&... __args)
                : data(std::forward<_Args>(__args)...), next(_N), prev(_N) // Initialize idx as invalid
                  {};
        value_type data;
        position_type next;
        position_type prev;
    };
    typedef Node node_type;

    class ListIterator {
        friend DLList;

    public:
        ListIterator(node_type* start, position_type offset) : m_start(start), m_offset(offset) {
        }

        reference operator*() const {
            return (m_start + m_offset)->data;
        }

        pointer operator->() const {
            return &(m_start + m_offset)->data;
        }

        ListIterator& operator++() {
            m_offset = get_node()->next;
            return *this;
        }

        ListIterator operator++(int) {
            ListIterator __tmp = *this;
            ++*this;
            return __tmp;
        }

        ListIterator& operator--() {
            m_offset = get_node()->prev;
            return *this;
        }

        ListIterator operator--(int) {
            ListIterator __tmp = *this;
            --*this;
            return __tmp;
        }

        bool operator==(const ListIterator& x) const {
            return (m_start == x.m_start && m_offset == x.m_offset);
        }

        bool operator!=(const ListIterator& x) const {
            return !(*this == x);
        }

    private:
        node_type* get_node() const {
            return (m_start + m_offset);
        }

        node_type* const m_start;
        difference_type m_offset;
    };

    class Allocator {
    public:
        Allocator(){};
        position_type allocate() {
            if (index.all()) {
                throw std::length_error("No space left in DLList.");
            }
            size_type i;
            for (i = 0; i < this->max_size(); i++) {
                if (!index.test(i)) {
                    index.set(i, true);
                    break;
                }
            }
            return i;
        };

        void deallocate(position_type i) {
            index.set(i, false);
        }
        size_type size() const {
            return index.count();
        }
        size_type max_size() const {
            return index.size();
        }
        bool empty() const {
            return index.none();
        }
        void clear() {
            return index.reset();
        }

    private:
        std::bitset<_N> index;
    };

public:
    typedef ListIterator iterator;
    typedef const iterator const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // construct/copy/destroy:
    DLList() : first(0), last(0){};

    // iterators:
    iterator begin() {
        return ListIterator(&data[0], first);
    };
    iterator end() {
        return ListIterator(&data[0], _N);
    };

    // const_iterator begin() const;
    // const_iterator end() const;
    // reverse_iterator rbegin();
    // const_reverse_iterator rbegin() const;
    // reverse_iterator rend();
    // const_reverse_iterator rend() const;
    // const_iterator cbegin() const;
    // const_iterator cend() const;
    // const_reverse_iterator crbegin() const;
    // const_reverse_iterator crend() const;

    // capacity:
    size_type size() const {
        return this->allocator.size();
    };
    size_type max_size() const {
        return this->allocator.max_size();
    };
    bool empty() const {
        return this->allocator.empty();
    };

    // element access:
    reference front() {
        return *begin();
    }
    const_reference front() const {
        return *begin();
    }
    reference back() {
        iterator __tmp = end();
        --__tmp;
        return *__tmp;
    }
    const_reference back() const {
        const_iterator __tmp = end();
        --__tmp;
        return *__tmp;
    }
    reference operator[](std::size_t idx) {
        if (idx > size()) {
            throw std::out_of_range("Demanded idx is out of range.");
        }
        iterator it = begin();
        for (size_t i = 0; i < idx; i++) {
            it++;
        }
        return *it;
    }
    const_reference operator[](std::size_t idx) const {
        if (idx > size()) {
            throw std::out_of_range("Demanded idx is out of range.");
        }
        iterator it = begin();
        for (size_t i = 0; i < idx; i++) {
            it++;
        }
        return *it;
    }

    // modifiers:
    void push_front(const value_type& x) {
        this->insert(begin(), x);
    }
    void push_front(value_type&& x) {
        this->insert(begin(), std::move(x));
    }
    template <typename... _Args>
    void emplace_front(_Args&&... __args) {
        this->insert(begin(), std::forward<_Args>(__args)...);
    }

    void pop_front() {
        this->erase(begin());
    }
    void push_back(const value_type& x) {
        this->insert(end(), x);
    }
    void push_back(value_type&& x) {
        this->insert(end(), std::move(x));
    }
    template <typename... _Args>
    void emplace_back(_Args&&... __args) {
        this->insert(end(), std::forward<_Args>(__args)...);
    }
    void pop_back() {
        this->erase(iterator(this->_M_impl._M_node._M_prev));
    } // TODO

    // template <class... Args>
    // iterator emplace(const_iterator position, Args&&... args);
    // iterator insert(const_iterator position, const value_type& x);
    iterator insert(const_iterator position, value_type&& x) {
        // Allocate memory
        position_type i = allocator.allocate();
        data[i].data = x;

        // Catch end() Iterator
        if(position.m_offset == _N){
            data[i].prev = last;
            if(size()>1){
                data[last].next = i;
            }
            last = i;
            return ListIterator(&data[0],i);
        }

        // Adjust neighbors
        if (is_idx_valid(position.get_node()->prev)) {
            data[position.get_node()->prev].next = i;
            data[i].prev = position.get_node()->prev;
        } else {
            first = i;
            data[i].prev = _N;
        }
        if (is_idx_valid(position.get_node()->next)) {
            data[position.get_node()->next].prev = i;
            data[i].next = position.get_node()->next;
        } else {
            last = i;
            data[i].next = _N;
        }
        return ListIterator(&data[0],i);
    };

    // iterator insert(const_iterator position, size_type n, const value_type& x);
    // template <class InputIterator>
    // iterator insert(const_iterator position, InputIterator first, InputIterator last);
    // iterator insert(const_iterator position, initializer_list<T>);

    iterator erase(const_iterator position) {
        // Get index of element
        size_t i = reinterpret_cast<position_type>(position.m_offset);

        // Adjust neighbors
        if (is_idx_valid(position.get_node()->prev)) {
            data[position.get_node()->prev].next = position.get_node()->next;
        } else {
            first = position.get_node()->next;
        }
        if (is_idx_valid(position.get_node()->next)) {
            data[position.get_node()->next].prev = position.get_node()->prev;
        } else {
            last = position.get_node()->prev;
        }

        // Free memory and reinitialize
        allocator.deallocate(i);
        data[i] = Node();
    }
    // iterator erase(const_iterator first, const_iterator last);
    // void swap(list<T, Allocator>&);
    void clear() {
        for (auto& e : data) {
            e = Node();
        }
        first = 0;
        last = 0;
        allocator.clear();
    };

    void remove(const value_type& value);
    // template <class Predicate>
    // void remove_if(Predicate pred);

    bool is_idx_valid(position_type& idx) {
        return idx >= 0 && idx < _N;
    }

private:
    node_type data[_N];
    Allocator allocator;
    position_type first;
    position_type last;
};
} // Namespace Per