#ifndef __SET_HPP__
#define __SET_HPP__

#include "rbt.hpp"

#include <exception>

namespace sg
{
    template <typename Tvalue>
    class set
    {
    public:
        set();
        ~set();

        class iterator
        {
        public:
            iterator() = delete;
            iterator(const sg::set<Tvalue>::iterator& iter) = default;
            iterator(sg::set<Tvalue>::iterator&& iter) = default;

            const Tvalue& operator->();
            const Tvalue& operator*();
            sg::set<Tvalue>::iterator operator++();
            sg::set<Tvalue>::iterator operator++(int);
            sg::set<Tvalue>::iterator operator--();
            sg::set<Tvalue>::iterator operator--(int);
            bool operator==(sg::set<Tvalue>::iterator iter);
            bool operator!=(sg::set<Tvalue>::iterator iter);

        private:
            iterator(sg::node_t<Tvalue>* node, sg::rbt_t<Tvalue>* tree);
            sg::node_t<Tvalue>* __node = nullptr;
            sg::rbt_t<Tvalue>* __tree = nullptr;
            friend class sg::set<Tvalue>;
        };

        sg::set<Tvalue>::iterator search(const Tvalue& value);
        sg::set<Tvalue>::iterator insert(const Tvalue& value);
        sg::set<Tvalue>::iterator begin();
        sg::set<Tvalue>::iterator end();

    private:
        sg::rbt_t<Tvalue>* __tree;
    };

} // namespace sg


template <typename Tvalue>
inline
sg::set<Tvalue>::set()
{
    __tree = new sg::rbt_t<Tvalue>;
}

template <typename Tvalue>
inline
sg::set<Tvalue>::~set()
{
    delete __tree;
}

template <typename Tvalue>
inline const Tvalue&
sg::set<Tvalue>::iterator::operator->()
{
    if(__node == nullptr)
        throw std::runtime_error{"sg::set::iterator out of range"};
    return __node->value();
}

template <typename Tvalue>
inline const Tvalue&
sg::set<Tvalue>::iterator::operator*()
{
    if(__node == nullptr)
        throw std::runtime_error{"sg::set::iterator out of range"};
    return __node->value();
}

template <typename Tvalue>
inline typename sg::set<Tvalue>::iterator
sg::set<Tvalue>::iterator::operator++() // Prefix
{
    if(__node == nullptr)
        throw std::runtime_error{"sg::set::iterator out of range"};
    __node = __tree->successor(__node);
    return *this;
}

template <typename Tvalue>
inline typename sg::set<Tvalue>::iterator
sg::set<Tvalue>::iterator::operator++(int) // Postfix
{
    if(__node == nullptr)
        throw std::runtime_error{"sg::set::iterator out of range"};
    typename sg::set<Tvalue>::iterator old = *this;
    __node = __tree->successor(__node);
    return old;
}

template <typename Tvalue>
inline typename sg::set<Tvalue>::iterator
sg::set<Tvalue>::iterator::operator--() // Prefix
{
    // Decrementing end iterator should give the maximal element
    if(__node == nullptr)
        __node = __tree->maximal();
    else
        __node = __tree->predecessor(__node);
    return *this;
}

template <typename Tvalue>
inline typename sg::set<Tvalue>::iterator
sg::set<Tvalue>::iterator::operator--(int) // Postfix
{
    typename sg::set<Tvalue>::iterator old = this;
    // Decrementing end iterator should give the maximal element
    if(__node == nullptr)
        __node = __tree->maximal();
    else
        __node = __tree->predecessor(__node);
    return old;
}

template <typename Tvalue>
inline bool
sg::set<Tvalue>::iterator::operator==(sg::set<Tvalue>::iterator iter)
{
    return __node == iter.__node;
}

template <typename Tvalue>
inline bool
sg::set<Tvalue>::iterator::operator!=(sg::set<Tvalue>::iterator iter)
{
    return __node != iter.__node;
}

template <typename Tvalue>
inline
sg::set<Tvalue>::iterator::iterator(sg::node_t<Tvalue>* node, sg::rbt_t<Tvalue>* tree) :
    __node{node},
    __tree{tree}
{
}

template <typename Tvalue>
inline typename sg::set<Tvalue>::iterator
sg::set<Tvalue>::search(const Tvalue& value)
{
    sg::node_t<Tvalue>* node = __tree->search(value);
    return sg::set<Tvalue>::iterator{node, __tree};
}

template <typename Tvalue>
inline typename sg::set<Tvalue>::iterator
sg::set<Tvalue>::insert(const Tvalue& value)
{
    sg::node_t<Tvalue>* node = __tree->insert(value);
    return sg::set<Tvalue>::iterator{node, __tree};
}

template <typename Tvalue>
inline typename sg::set<Tvalue>::iterator
sg::set<Tvalue>::begin()
{
    sg::node_t<Tvalue>* node = __tree->minimal();
    return sg::set<Tvalue>::iterator{node, __tree};
}

template <typename Tvalue>
inline typename sg::set<Tvalue>::iterator
sg::set<Tvalue>::end()
{
    return sg::set<Tvalue>::iterator{nullptr, __tree};
}


#endif // __SET_HPP__