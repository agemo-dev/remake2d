#ifndef REMAKE2D_IVECTOR_TPP_
#define REMAKE2D_IVECTOR_TPP_

namespace rmk {

template <typename T, usize C>
IVector<T, C>::~IVector(void) {
    clear();
}

template <typename T, usize C>
bool IVector<T, C>::push(const T& value) noexcept {
    if (m_size >= C) return fail;
    ::new (static_cast<void*>(data() + m_size)) T(value);
    m_size++;
    return success;
}

template <typename T, usize Capacity>
bool IVector<T, Capacity>::pushAndSort(const T& value) {
    if (m_size >= Capacity) return fail;
    
    pointer ptr = data();
    usize low = 0;
    usize high = m_size;

    while (low < high) {
        usize mid = low + (high - low) / 2;
        if (ptr[mid] < value) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    usize insert_pos = low;

    if (m_size > insert_pos) {
        ::new (static_cast<void*>(ptr + m_size)) T(std::move(ptr[m_size - 1]));
        for (usize i = m_size - 1; i > insert_pos; --i) {
            ptr[i] = std::move(ptr[i - 1]);
        }
        ptr[insert_pos] = value;
    } else {
        ::new (static_cast<void*>(ptr + insert_pos)) T(value);
    }

    m_size++;
    return success;
}

template <typename T, usize C>
void IVector<T, C>::pop(void) noexcept {
    if (! (m_size > 0)) return;
    m_size--;
    data()[m_size].~T();
}

template <typename T, usize C>
typename IVector<T, C>::iterator IVector<T, C>::erase(typename IVector<T, C>::iterator it) noexcept {
    if (it == end() || m_size == 0) return end();

    for (auto next = it + 1; next != end(); ++it, ++next) *it = std::move(*next);

    m_size--;
    data()[m_size].~T();

    return it;
}


template <typename T, usize C>
void IVector<T, C>::clear(void) noexcept {
    while (m_size > 0) {
        popBack();
    }
}

template <typename T, usize C>
typename IVector<T, C>::pointer IVector<T, C>::data(void) noexcept {
    return reinterpret_cast<pointer>(m_data);
}

template <typename T, usize C>
typename IVector<T, C>::const_pointer IVector<T, C>::data(void) const noexcept { 
    return reinterpret_cast<const_pointer>(m_data);
}

template <typename T, usize C>
typename IVector<T, C>::reference IVector<T, C>::operator[](size_type index) {
    return data()[index];
}

template <typename T, usize C>
typename IVector<T, C>::const_reference IVector<T, C>::operator[](size_type index) const {
    return data()[index];
}

template <typename T, usize C>
typename IVector<T, C>::size_type IVector<T, C>::size(void) const noexcept {
    return m_size;
}

template <typename T, usize C>
typename IVector<T, C>::size_type IVector<T, C>::capacity(void) const noexcept {
    return C;
}

template <typename T, usize C>
typename IVector<T, C>::iterator IVector<T, C>::begin(void) noexcept {
    return iterator(data());
}

template <typename T, usize C>
typename IVector<T, C>::iterator IVector<T, C>::end(void) noexcept {
    return iterator(data() + m_size);
}

template <typename T, usize C>
IVector<T, C>::iterator::iterator(pointer p) : m_ptr(p) {}

template <typename T, usize C>
typename IVector<T, C>::iterator::reference
IVector<T, C>::iterator::operator*(void) const {
    return *m_ptr;
}

template <typename T, usize C>
typename IVector<T, C>::iterator::pointer
IVector<T, C>::iterator::operator->(void) const {
    return m_ptr;
}

template <typename T, usize C>
typename IVector<T, C>::iterator::reference
IVector<T, C>::iterator::operator[](difference_type n) const {
    return m_ptr[n];
}

template <typename T, usize C>
typename IVector<T, C>::iterator&
IVector<T, C>::iterator::operator++(void) noexcept {
    m_ptr++;
    return *this;
}

template <typename T, usize C>
typename IVector<T, C>::iterator
IVector<T, C>::iterator::operator++(int) noexcept {
    iterator tmp = *this;
    m_ptr++;
    return tmp;
}

template <typename T, usize C>
typename IVector<T, C>::iterator&
IVector<T, C>::iterator::operator--(void) noexcept {
    m_ptr--;
    return *this;
}

template <typename T, usize C>
typename IVector<T, C>::iterator
IVector<T, C>::iterator::operator--(int) noexcept {
    iterator tmp = *this;
    m_ptr--;
    return tmp;
}

template <typename T, usize C>
typename IVector<T, C>::iterator&
IVector<T, C>::iterator::operator+=(difference_type n) noexcept {
    m_ptr += n;
    return *this;
}

template <typename T, usize C>
typename IVector<T, C>::iterator&
IVector<T, C>::iterator::operator-=(difference_type n) noexcept {
    m_ptr -= n;
    return *this;
}

template <typename T, usize C>
typename IVector<T, C>::iterator
operator+(typename IVector<T, C>::iterator it, typename IVector<T, C>::iterator::difference_type n) noexcept {
    it += n;
    return it;
}

template <typename T, usize C>
typename IVector<T, C>::iterator
operator+(typename IVector<T, C>::iterator::difference_type n, typename IVector<T, C>::iterator it) noexcept {
    it += n;
    return it;
}

template <typename T, usize C>
typename IVector<T, C>::iterator
operator-(typename IVector<T, C>::iterator it, typename IVector<T, C>::iterator::difference_type n) noexcept {
    it -= n;
    return it;
}

template <typename T, usize C>
typename IVector<T, C>::iterator::difference_type
operator-(const typename IVector<T, C>::iterator& a, const typename IVector<T, C>::iterator& b) noexcept {
    return a.m_ptr - b.m_ptr;
}

} // namespace rmk

#endif