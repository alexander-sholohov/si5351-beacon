#pragma once

//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//


class JTEncodeBase
{
public:

    // Simple Iterator.
    class iterator
    {
    public:
        iterator() : _pos(0), _owner(0){};
        iterator(unsigned pos, JTEncodeBase* owner) : _pos(pos), _owner(owner){};

        unsigned char operator*() const { return (_owner)? _owner->get_packed_symbol( _pos ) : 0; }
        bool operator != ( const iterator &other ) const { return _pos != other._pos; }
        bool operator == ( const iterator &other ) const { return !operator!=( other ); }
        iterator& operator++()   { ++_pos; return *this; }
        

    private:
        unsigned _pos;
        JTEncodeBase* _owner;
    };

    JTEncodeBase(void);
    ~JTEncodeBase(void);


private:
    virtual unsigned char get_packed_symbol(unsigned pos) const;
};
