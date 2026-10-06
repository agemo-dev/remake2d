#ifndef REMAKE2D_IVECTOR_
#define REMAKE2D_IVECTOR_

#include <remake2d/numeric.hpp>

#include <vector>
#include <initializer_list>

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
        T* m_ptr{nullptr};

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

    public:
        iterator(void) = default;
        explicit iterator(pointer);

        friend iterator operator+(difference_type n, const iterator& it) noexcept { return it + n; }

    public:
        reference operator*(void)             const;
        pointer   operator->(void)            const;
        reference operator[](difference_type) const;

        iterator& operator++(void)  noexcept;
        iterator  operator++(int)   noexcept;
        iterator& operator--(void)  noexcept;
        iterator  operator--(int)   noexcept;

        iterator& operator+=(difference_type) noexcept;
        iterator& operator-=(difference_type) noexcept;

        iterator  operator+(difference_type) const noexcept;
        iterator  operator-(difference_type) const noexcept;
        difference_type operator-(const iterator&) const noexcept;

        auto operator<=>(const iterator&) const noexcept = default;
    };

    class const_iterator {

    private:
        const_pointer m_ptr{nullptr};

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const T*;
        using reference         = const T&;

    public:
        const_iterator(void) = default;
        explicit const_iterator(const_pointer);

        friend const_iterator operator+(difference_type n, const const_iterator& it) noexcept { return it + n; }

    public:
        reference operator*(void)             const;
        pointer   operator->(void)            const;
        reference operator[](difference_type) const;

        const_iterator& operator++(void)  noexcept;
        const_iterator  operator++(int)   noexcept;
        const_iterator& operator--(void)  noexcept;
        const_iterator  operator--(int)   noexcept;

        const_iterator& operator+=(difference_type) noexcept;
        const_iterator& operator-=(difference_type) noexcept;

        const_iterator  operator+(difference_type) const noexcept;
        const_iterator  operator-(difference_type) const noexcept;
        difference_type operator-(const const_iterator&) const noexcept;

        auto operator<=>(const const_iterator&) const noexcept = default;
    };

public:
    IVector(IVector&&);
    IVector(const IVector&);
    IVector(void)                       = default;
    IVector& operator=(IVector&&)       = default;
    IVector& operator=(const IVector&)  = default;

public:
    IVector(std::vector<T>&&);
    IVector(const std::vector<T>&);
    IVector(std::initializer_list<T>);

public:
    IVector& operator=(std::initializer_list<T>);
    IVector& operator=(const std::vector<T>&);
    IVector& operator=(std::vector<T>&&);

public:
    template <typename InputIt> void assign(InputIt first, InputIt last);
    void reserve(size_type)        noexcept;
    bool push_back(const T&)       noexcept;
    bool push_back(T&&)            noexcept;

public:
    void pop(void)                 noexcept;
    void clear(void)               noexcept;
    bool push(const T&)            noexcept;
    bool pushAndSort(const T&)     noexcept;
    iterator erase(iterator)       noexcept;

public:
    pointer data(void)             noexcept;
    const_pointer data(void) const noexcept;

public:
    reference       operator[](size_type);
    const_reference operator[](size_type) const;

public:
    bool      empty(void)    const noexcept;
    size_type size(void)     const noexcept;
    size_type capacity(void) const noexcept;

public:
    iterator begin(void) noexcept;
    iterator end(void)   noexcept;
    const_iterator begin(void) const noexcept;
    const_iterator end(void)   const noexcept;

public:
    ~IVector(void);
};

} // namespace rmk

#include <remake2d/template/private/ivector.tpp>
#endif