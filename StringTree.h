#ifndef STRINGTREE_H
#define STRINGTREE_H


#include <vector>
#include <iostream>

namespace details{

/**
 * @brief Element of the tree. it has a single parent and N >= 0 children.
 */
template <typename T> class TreeElement{

public:

    TreeElement(TreeElement<T> *parent, const T& value );

    const TreeElement* parent() const               { return _parent; }
    const T& value() const                          { return _value; }
    const std::vector<TreeElement>& children()const { return _children; }
    std::vector<TreeElement>& children()            { return _children; }

    TreeElement& addChild(const T& child );

private:
    TreeElement*             _parent;
    T                        _value;
    std::vector<TreeElement> _children;
};


template <typename T> class Tree
{
public:
    Tree(): _root(nullptr,"root") {}

    template<typename Vect> void append(const Vect& concatenated_values);

    template<typename Vect> const TreeElement<T>* find( const Vect& concatenated_values, bool partial_allowed = false);

    void print();

private:

    friend std::ostream& operator<<(std::ostream& os, const Tree& _this)
    {
        _this.print_impl(os, &(_this._root.children()), 0);
        return os;
    }


    void print_impl(std::ostream& os, const std::vector<TreeElement<T>> *children, int indent ) const;

    TreeElement<T> _root;
};

//-----------------------------------------


template <typename T> inline
void Tree<T>::print_impl(std::ostream &os, const std::vector<TreeElement<T>> *children, int indent) const
{
    for (const auto& child: (*children))
    {
        for (int i=0; i<indent; i++)
        {
            os << " ";
        }
        os << child.value() << std::endl;
        print_impl(os, &(child.children()), indent+3);
    }
}

template <typename T> inline
TreeElement<T>::TreeElement(TreeElement<T> *parent, const T& value):
    _parent(parent), _value(value)
{

}

template <typename T> inline
TreeElement<T>& TreeElement<T>::addChild(const T& value)
{
    _children.push_back( TreeElement<T>(this, value));
    return _children.back();
}


template <typename T> template<typename Vect> inline
void Tree<T>::append(const Vect &concatenated_values)
{
    TreeElement<T>* node = &_root;

    for (const auto& value: concatenated_values)
    {
        bool found = false;
        for (auto& child: (node->children() ) )
        {
            if( child.value() == value)
            {
                node = &(child);
                found = true;
                break;
            }
        }
        if(!found){
            node = &( node->addChild( value ) );
        }
    }
}

template <typename T> template<typename Vect> inline
const TreeElement<T> *Tree<T>::find(const Vect& concatenated_values, bool partial_allowed )
{
    TreeElement<T>* node = &_root;

    for (const auto& value: concatenated_values)
    {
        bool found = false;
        for (auto& child: (node->children() ) )
        {
            if( child.value() == value)
            {
                node = &(child);
                found = true;
                break;
            }
        }
        if( !found ) return nullptr;
    }

    if( partial_allowed || node->children().empty() )
    {
        return  node;
    }
    return nullptr;
}

}

typedef details::TreeElement<std::string> StringElement;
typedef details::Tree<std::string> StringTree;


#endif // STRINGTREE_H
