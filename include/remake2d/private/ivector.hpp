#ifndef REMAKE2D_IVECTOR_
#define REMAKE2D_IVECTOR_

#include <remake2d/utility.hpp>
#include <remake2d/numeric.hpp>

// IVector = Inline Vector

namespace rmk {

template <typename T, usize C> class IVector {

public:
    using value_type = T;
    using size_type = usize;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

private:
    usize           m_size{0};
    alignas(T) byte m_data[C * sizeof(T)];

public:
    class iterator {
    private:
        pointer m_ptr;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        explicit iterator(pointer p);

        reference operator*(void) const;
        pointer   operator->(void) const;
        reference operator[](difference_type) const;

        iterator& operator++(void);
        iterator  operator++(int);
        iterator& operator--(void);
        iterator  operator--(int);

        iterator& operator+=(difference_type);
        iterator& operator-=(difference_type);

        friend iterator operator+(iterator, difference_type);
        friend iterator operator+(difference_type, iterator);
        friend iterator operator-(iterator, difference_type);
        friend difference_type operator-(const iterator&, const iterator&);

        auto operator<=>(const iterator&) const noexcept = default;
    };

public:
    IVector(void)                       = default;
    IVector(IVector&&)                  = default;
    IVector(const IVector&)             = default;
    IVector& operator=(IVector&&)       = default;
    IVector& operator=(const IVector&)  = default;

public:
    void pop(void)                 noexcept;
    void clear(void)               noexcept;
    bool push(const T&)            noexcept;
    bool pushAndSort(const T&)     noexcept;
    IVector::iterator erase(void)  noexcept;

public:
    pointer data(void)             noexcept;
    const_pointer data(void) const noexcept;

public:
    reference       operator[](size_type);
    const_reference operator[](size_type) const;

public:
    size_type size(void)     const noexcept;
    size_type capacity(void) const noexcept;

public:
    iterator begin(void) noexcept;
    iterator end(void)   noexcept;

public:
    ~IVector(void);
};

} // namespace rmk

#endif