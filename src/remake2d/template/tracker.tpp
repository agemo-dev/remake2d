#ifndef REMAKE2D_TRACKER_TPP_
#define REMAKE2D_TRACKER_TPP_

namespace rmk {

template<typename T>
std::shared_ptr<Slot<T>> Slot<T>::make(void) noexcept {
    return std::make_shared<Slot<T>>();
}

template<typename T>
bool Slot<T>::operator==(const Slot<T>& other) const noexcept {
    return this->ptr == other.ptr;
}

template<typename T>
bool Slot<T>::operator!=(const Slot<T>& other) const noexcept {
    return this->ptr != other.ptr;
}

template<typename T>
UnsafeTracker<T>::UnsafeTracker(const Balise<T>& b) : m_tracked(b) {}

template<typename T>
T& UnsafeTracker<T>::operator*(void) {
    if (T* p = locate<T>()) return *p;
    rmk_dynamicAssert(rmk::TrackerError, error::tracker::null_ptr_deref);
    return *locate<T>();
}

template<typename T>
const T& UnsafeTracker<T>::operator*(void) const {
    if (const T* p = locate<T>()) return *p;
    rmk_dynamicAssert(rmk::TrackerError, error::tracker::null_ptr_deref);
    return *locate<T>();
}

template<typename T>
T* UnsafeTracker<T>::operator->(void) {
    return locate<T>();
}

template<typename T>
const T* UnsafeTracker<T>::operator->(void) const {
    return locate<T>();
}

template<typename T>
UnsafeTracker<T>::operator bool() const {
    return locate() != nullptr;
}

template<typename T>
bool UnsafeTracker<T>::operator==(const UnsafeTracker<T>& other) const noexcept {
    auto ptr1 = this->m_tracked.lock();
    auto ptr2 = other.m_tracked.lock();
    return ptr1 && ptr2 ? (*ptr1) == (*ptr2) : ptr1 == ptr2;
}

template<typename T>
bool UnsafeTracker<T>::operator!=(const UnsafeTracker<T>& other) const noexcept {
    auto ptr1 = this->m_tracked.lock();
    auto ptr2 = other.m_tracked.lock();
    return ptr1 && ptr2 ? ptr1->ptr != ptr2->ptr : ptr1 != ptr2;
}

template<typename T>
template<typename U> requires IsRelatedTo<U, T>
U* UnsafeTracker<T>::locate(void) noexcept {
    auto tmp = m_tracked.lock();
    if (!tmp || !tmp->ptr) return nullptr;
    T* p = const_cast<T*>(tmp->ptr);
    if constexpr (std::same_as<U, T>)           return p;
    if constexpr (std::same_as<U, void>)        return static_cast<void*>(p);
    else if constexpr (std::derived_from<T, U>) return static_cast<U*>(p);
    else                                        return dynamic_cast<U*>(p);
}

template<typename T>
template<typename U> requires IsRelatedTo<U, T>
const U* UnsafeTracker<T>::locate(void) const noexcept {
    auto tmp = m_tracked.lock();
    if (!tmp || !tmp->ptr) return nullptr;
    const T* p = tmp->ptr;
    if constexpr (std::same_as<U, T>)           return p;
    if constexpr (std::same_as<U, void>)        return static_cast<const void*>(p);
    else if constexpr (std::derived_from<T, U>) return static_cast<const U*>(p);
    else                                        return dynamic_cast<const U*>(p);
}

template<typename Derived>
Trackable<Derived>::Trackable(Trackable&& o) : m_slot(std::move(o.m_slot)) {}

template<typename Derived>
Trackable<Derived>& Trackable<Derived>::operator=(Trackable&& o) {
    if (this == &o) return *this;
    if (m_slot) m_slot->ptr = nullptr;
    m_slot = std::move(o.m_slot);
    return *this;
}

template<typename Derived>
void Trackable<Derived>::relocate(void) noexcept {
    if (m_slot) m_slot->ptr = static_cast<const Derived*>(this);
}

template<typename Derived>
UnsafeTracker<Derived> Trackable<Derived>::tracker(void) noexcept {
    if (!m_slot) m_slot = Slot<Derived>::make();
    m_slot->ptr = static_cast<const Derived*>(this);
    return UnsafeTracker<Derived>(m_slot);
}

template<typename Derived>
UnsafeTracker<Derived> Trackable<Derived>::tracker(void) const noexcept {
    if (!m_slot) m_slot = Slot<Derived>::make();
    m_slot->ptr = static_cast<const Derived*>(this);
    return UnsafeTracker<Derived>(m_slot);
}

template<typename Derived>
Trackable<Derived>::~Trackable(void) {
    if (m_slot) m_slot->ptr = nullptr;
}

} // namespace rmk
#endif