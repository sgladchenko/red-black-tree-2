#ifndef __RBT_HPP__
#define __RBT_HPP__

namespace sg
{
    template <typename Tvalue> class node_t;
    template <typename Tvalue> class rbt_t;

    enum class color_t
    {
        red,
        black
    };

    template <typename Tvalue>
    class node_t
    {
    public:
        node_t(const Tvalue& val);
        ~node_t();

        const Tvalue& value();
        sg::node_t<Tvalue>* parent();
        sg::node_t<Tvalue>* left();
        sg::node_t<Tvalue>* right();
        sg::color_t color();

    private:
        Tvalue __value;
        sg::node_t<Tvalue>* __parent = nullptr;
        sg::node_t<Tvalue>* __left = nullptr;
        sg::node_t<Tvalue>* __right = nullptr;
        sg::color_t __color = sg::color_t::red; // Nodes in a RB-tree when added are first colored red.

        friend class sg::rbt_t<Tvalue>;
    };

    template <typename Tvalue>
    class rbt_t
    {
    public:
        rbt_t() = default;
        rbt_t(const sg::rbt_t<Tvalue>& obj);
        rbt_t(sg::rbt_t<Tvalue>&& obj);
        virtual ~rbt_t();

        sg::node_t<Tvalue>* search(const Tvalue& value);
        sg::node_t<Tvalue>* predecessor(sg::node_t<Tvalue>* node);
        sg::node_t<Tvalue>* successor(sg::node_t<Tvalue>* node);
        sg::node_t<Tvalue>* minimal();
        sg::node_t<Tvalue>* maximal();
        sg::node_t<Tvalue>* insert(const Tvalue& value);
#ifdef SG_TODO
        void remove(sg::node_t<Tvalue>* node);
#endif

        unsigned int size();

    protected:
        sg::color_t color(sg::node_t<Tvalue>* node);

        void left_rotate(sg::node_t<Tvalue>* upper);
        void right_rotate(sg::node_t<Tvalue>* upper);

        void insert_rebalance(sg::node_t<Tvalue>* inserted);
#ifdef SG_TODO
        void remove_rebalance(sg::node_t<Tvalue>* node);
#endif

        sg::node_t<Tvalue>* __root = nullptr;
        unsigned int __size = 0;
    };

} // namespace sg


template <typename Tvalue>
inline
sg::node_t<Tvalue>::~node_t()
{
    if(__left)
        delete __left;
    if(__right)
        delete __right;
}

template <typename Tvalue>
inline
sg::node_t<Tvalue>::node_t(const Tvalue& val) :
    __value{val}
{
}

template <typename Tvalue>
inline const Tvalue&
sg::node_t<Tvalue>::value()
{
    return __value;
}

template <typename Tvalue>
inline sg::node_t<Tvalue>*
sg::node_t<Tvalue>::parent()
{
    return __parent;
}

template <typename Tvalue>
inline sg::node_t<Tvalue>*
sg::node_t<Tvalue>::left()
{
    return __left;
}

template <typename Tvalue>
inline sg::node_t<Tvalue>*
sg::node_t<Tvalue>::right()
{
    return __right;
}

template <typename Tvalue>
inline sg::color_t
sg::node_t<Tvalue>::color()
{
    return __color;
}

template <typename Tvalue>
inline
sg::rbt_t<Tvalue>::rbt_t(const sg::rbt_t<Tvalue>& obj)
{
    // TODO: It has to copy every single item in the tree
    // TODO: one by one, re-creating the data structure.
    __size = obj.__size;
}

template <typename Tvalue>
inline
sg::rbt_t<Tvalue>::rbt_t(sg::rbt_t<Tvalue>&& obj)
{
    __root = obj.__root;
    __size = obj.__size;

    obj.__root = nullptr;
    obj.__size = 0;
}

template <typename Tvalue>
inline
sg::rbt_t<Tvalue>::~rbt_t()
{
    if(__root)
        delete __root;
}

template <typename Tvalue>
inline sg::node_t<Tvalue>*
sg::rbt_t<Tvalue>::search(const Tvalue& value)
{
    sg::node_t<Tvalue>* node = __root;
    while(node != nullptr)
    {
        if(node->value() == value)
            break;
        node = value < node->value() ? node->left() : node->right();
    }
    return node;
}

template <typename Tvalue>
inline sg::node_t<Tvalue>*
sg::rbt_t<Tvalue>::predecessor(sg::node_t<Tvalue>* node)
{
    if(node->left() != nullptr)
    {
        // If the node has a left subtree, then its predecessor is the maximal
        // element of its left subtree
        sg::node_t<Tvalue>* result = node->left();
        while(result->right() != nullptr)
        {
            result = result->right();
        }
        return result;
    }
    else
    {
        // Otherwise, the predecessor could be somewhere up in the tree at the
        // first time the tree branches to the right on the way to the given node.
        // If there's no such place, then the given node is the leftmost tree
        // element, i.e. the element with the minimal value.
        sg::node_t<Tvalue>* parent = node->parent();
        sg::node_t<Tvalue>* current = node;
        while(parent != nullptr)
        {
            if(parent->right() == current)
                return parent;
            current = parent;
            parent = current->parent();
        }
        return nullptr;
    }
}

template <typename Tvalue>
inline sg::node_t<Tvalue>*
sg::rbt_t<Tvalue>::successor(sg::node_t<Tvalue>* node)
{
    // Same as the predecessor function, but with left and right connections
    // between the tree elements swapped.
    if(node->right() != nullptr)
    {
        sg::node_t<Tvalue>* result = node->right();
        while(result->left() != nullptr)
        {
            result = result->left();
        }
        return result;
    }
    else
    {
        sg::node_t<Tvalue>* current = node;
        sg::node_t<Tvalue>* parent = node->parent();
        while(parent != nullptr)
        {
            if(parent->left() == current)
                return parent;
            current = parent;
            parent = current->parent();
        }
    }
}

template <typename Tvalue>
inline sg::color_t
sg::rbt_t<Tvalue>::color(sg::node_t<Tvalue>* node)
{
    // In this red-black tree implementation NIL nodes are depicted by nullptrs.
    if(node == nullptr)
        return sg::color_t::black;
    return node->color();
}

template <typename Tvalue>
inline void
sg::rbt_t<Tvalue>::left_rotate(sg::node_t<Tvalue>* upper)
{
    // Only works when upper has a valid (non-NIL) right child (lower).
    sg::node_t<Tvalue>* lower = upper->right();
    sg::node_t<Tvalue>* parent = upper->parent();
    sg::node_t<Tvalue>* middle = lower->left();

    if(parent != nullptr)
    {
        // If not a NIL, but a valid node, then specify its new child
        // (either left or right depending on how upper was connected to it).
        if(parent->left() == upper)
            parent->__left = lower;
        else
            parent->__right = lower;
    }
    else
    {
        // If parent of the upper node was nullptr, then upper was the root
        // of the tree; since after rotation lower is to replace
        // upper, we have to also update the variable that stores
        // the current root of the tree.
        __root = lower;
    }

    lower->__parent = parent;
    lower->__left = upper;

    upper->__parent = lower;
    upper->__right = middle;

    // If not a NIL, but a valid node, then also specify its new parent.
    if(middle != nullptr)
        middle->__parent = upper;
}

template <typename Tvalue>
inline void
sg::rbt_t<Tvalue>::right_rotate(sg::node_t<Tvalue>* upper)
{
    // Only works when upper has a valid (non-NIL) left child (lower).
    sg::node_t<Tvalue>* lower = upper->left();
    sg::node_t<Tvalue>* parent = upper->parent();
    sg::node_t<Tvalue>* middle = lower->right();

    if(parent != nullptr)
    {
        // If not a NIL, but a valid node, then specify its new child
        // (either left or right depending on how upper was connected to it).
        if(parent->left() == upper)
            parent->__left = lower;
        else
            parent->__right = lower;
    }
    else
    {
        // If parent of the upper node was nullptr, then upper was the root
        // of the tree; since after rotation lower is to replace
        // upper, we have to also update the variable that stores
        // the current root of the tree.
        __root = lower;
    }

    lower->__parent = parent;
    lower->__right = upper;

    upper->__parent = lower;
    upper->__left = middle;

    // If not a NIL, but a valid node, then also specify its new parent.
    if(middle != nullptr)
        middle->__parent = upper;
}

template <typename Tvalue>
inline sg::node_t<Tvalue>*
sg::rbt_t<Tvalue>::minimal()
{
    if(__root != nullptr)
    {
        sg::node_t<Tvalue>* result = __root;
        while(result->left() != nullptr)
        {
            result = result->left();
        }
        return result;
    }
    return nullptr;
}

template <typename Tvalue>
inline sg::node_t<Tvalue>*
sg::rbt_t<Tvalue>::maximal()
{
    if(__root != nullptr)
    {
        sg::node_t<Tvalue>* result = __root;
        while(result->right() != nullptr)
        {
            result = result->right();
        }
        return result;
    }
    return nullptr;
}

template <typename Tvalue>
inline sg::node_t<Tvalue>*
sg::rbt_t<Tvalue>::insert(const Tvalue& value)
{
    sg::node_t<Tvalue>* inserted = nullptr;

    // First, perform a basic binary-search tree insertion.
    if(__root != nullptr)
    {
        sg::node_t<Tvalue>* parent = nullptr;
        sg::node_t<Tvalue>* current = __root;
        while(current != nullptr)
        {
            if(current->value() == value)
            {
                parent = nullptr;
                break;
            }
            parent = current;
            current = value < parent->value() ? parent->left() : parent->right();
        }
        if(parent != nullptr)
        {
            inserted = new sg::node_t<Tvalue>{value};
            inserted->__parent = parent;
            if(value < parent->value())
                parent->__left = inserted;
            else
                parent->__right = inserted;
            __size++;
        }
    }
    else
    {
        // If the tree was empty
        inserted = new sg::node_t<Tvalue>{value};
        __root = inserted;
        __size++;
    }

    // If a new tree element was inserted, check if the red-black properties
    // have been violated after the insertion and rebalance the tree if they have.
    if(inserted != nullptr)
        insert_rebalance(inserted);

    return inserted;
}

template <typename Tvalue>
inline unsigned int
sg::rbt_t<Tvalue>::size()
{
    return __size;
}

template <typename Tvalue>
inline void
sg::rbt_t<Tvalue>::insert_rebalance(sg::node_t<Tvalue>* inserted)
{
    sg::node_t<Tvalue>* node = inserted;
    while(color(node->parent()) == sg::color_t::red)
    {
        sg::node_t<Tvalue>* parent = node->parent();
        sg::node_t<Tvalue>* grand = parent->parent();
        if(parent == grand->left())
        {
            sg::node_t<Tvalue>* uncle = grand->right();
            if(color(uncle) == sg::color_t::red)
            {
                parent->__color = sg::color_t::black;
                uncle->__color = sg::color_t::black;
                grand->__color = sg::color_t::red;
                node = grand;
            }
            else
            {
                if(node == parent->right())
                {
                    left_rotate(parent);
                    // Rename the nodes, so that the lower one is named 'node'
                    // and its parent is 'parent'.
                    sg::node_t<Tvalue>* temp = node;
                    node = parent;
                    parent = temp;
                }
                parent->__color = sg::color_t::black;
                grand->__color = sg::color_t::red;
                right_rotate(grand);
            }
        }
        else // parent == grand->right()
        {
            sg::node_t<Tvalue>* uncle = grand->left();
            if(color(uncle) == sg::color_t::red)
            {
                parent->__color = sg::color_t::black;
                uncle->__color = sg::color_t::black;
                grand->__color = sg::color_t::red;
                node = grand;
            }
            else
            {
                if(node == parent->left())
                {
                    right_rotate(parent);
                    // Rename the nodes, so that the lower one is named 'node'
                    // and its parent is 'parent'.
                    sg::node_t<Tvalue>* temp = node;
                    node = parent;
                    parent = temp;
                }
                parent->__color = sg::color_t::black;
                grand->__color = sg::color_t::red;
                left_rotate(grand);
            }
        }
    }
    __root->__color = sg::color_t::black;
}


#endif // __RBT_HPP__
