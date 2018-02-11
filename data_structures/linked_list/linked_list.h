//
//  linked_list.h
//  Office
//
//  Created by Oleg on 2/7/18.
//  Copyright (c) 2018 OlegSyniakevych. All rights reserved.
//

#ifndef Office_linked_list_h
#define Office_linked_list_h

template <class T> class linked_list_node;

template <class T>
class linked_list {
    public:
    explicit
    linked_list(T const & e)
        : _first(new linked_list_node<T>(e, nullptr))
        , _last(_first)
        , _size(0)
        {
        }
    
    void
    add_last(T const & e)
        {
        _last->_next = new linked_list_node<T>(e, nullptr);
        ++_size;
        }

    void
    add_first(T const & e)
        {
        _first = new linked_list_node<T> (e, _first);
        ++_size;
        }
    
    size_t
    size() 
        {
        return _size;
        }
    
    private:
    linked_list_node<T> * _first;
    linked_list_node<T> * _last;
    size_t _size;
    };

template <typename T>
class 
linked_list_node 
    {
    friend void linked_list<T>::add_last(const T &e);
    public:
    linked_list_node(T const & e, linked_list_node<T> * next)
        : _elem(e), _next(next)
        {
        }
    
    private:
    T _elem;
    linked_list_node<T> * _next;
    };

#endif
