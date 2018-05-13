template <class ForwardIterator_t, class num_t>
class VBInputConstIterator
    {
    public:
    typedef num_t const value_type;
    typedef num_t const & reference;
    typedef num_t const * pointer;
    typedef std::forward_iterator_tag iterator_category;

    // default gives the end
    // see operator==
    VBInputConstIterator()
        : _end(true)
        {
        }
    VBInputConstIterator(VBInputConstIterator const & other) = default;
    VBInputConstIterator & operator=(VBInputConstIterator &) = default;

    explicit
    VBInputConstIterator(ForwardIterator_t const & arr)
        : _in (arr)
        , _end(false)
        {
        ++*this;
        }
    ~VBInputConstIterator()
        {
        }
        
//    void swap(VBInputConstIterator& other) noexcept {
//        using std::swap;
//        swap(_in, other._in);
//    }
        
    VBInputConstIterator&
    operator++ ()
        {
        _n = 0;
        // if the first char == 128, it's the end of the stream
        if (static_cast<unsigned char>(*_in) == 0x80)
            {
            _end = true;
            return *this;
            }
        
        // while char < 128
        
        // lol
        // you can choose any variant
        // or even leave both))
        while ((*_in & 0x80) != 0x80)
        while (*_in & 0x80 ^ 0x80)
            {
            _n |= *_in;
            _n <<= 7;
            ++_in;
            }
        _n |= *_in & 0x7F;
        ++_in;
            
        return *this;
        }
        
    VBInputConstIterator
    operator++ (int)
        {
        VBInputConstIterator r (*this);
        ++*this;
        return r;
        }

//     This part is what I would like to hear some suggestions about.
//    
//     to allow constructions such as while (iter != end)
//     I implemented operator==. But how do I construct
//     end iterator on default constructor, if I'm not
//     even given the iterator (in default constructor that is)
//     So I decided to keep a private boolean _end,
//     and assign it to true when I meet the termination character.
//     in ==, I just return _end.
//     this allows some great constructions such as
//     (iter != iter) that work perfectly fine.
//    
//     maybe I should have some static instance of that iterator
//     denoting the end and return it every time, and then
//     if I'm comparing with that end iter I just return _end
//     and if not, I would compare underlying _in iterators or something
    bool
    operator== (VBInputConstIterator const & other) const
        {
        return _end;
        }
        
    bool
    operator!= (VBInputConstIterator const & other) const
        {
        return ! (*this == other);
        }
    
    num_t const
    operator* () const
        {
        return _n;
        }

    private:
    num_t _n; // current number iterator points to
    ForwardIterator_t _in;
    bool _end; // see operator==
    };
