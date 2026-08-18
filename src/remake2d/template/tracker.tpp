#ifndef REMAKE2D_TRACKER_TPP_
#define REMAKE2D_TRACKER_TPP_

namespace rmk {

template<IsTrackable T>
std::shared_ptr<Slot<T>> Slot<T>::make(void) noexcept {
	return std::make_shared<Slot<T>>();
}

template<IsTrackable T>
bool Slot<T>::operator==(const Slot<T>& other) const noexcept {
	return this->ptr == other.ptr;
}

template<IsTrackable T>
bool Slot<T>::operator!=(const Slot<T>& other) const noexcept {
	return this->ptr != other.ptr;
}

template<IsTrackable T>
Tracker<T>::Tracker(const Balise& src) : m_tracked(src) {}

template<IsTrackable T>
void Tracker<T>::track(const Balise& b) noexcept {
	m_tracked = b;
}

template<IsTrackable T>
T& Tracker<T>::operator*(void) {
	if (T* p = locate<T>()) return *p;
	rmk_dynamicAssert(rmk::TrackerError, error::tracker::null_ptr_deref);
}

template<IsTrackable T>
const T& Tracker<T>::operator*(void) const {
	if (const T* p = locate<T>()) return *p;
	rmk_dynamicAssert(rmk::TrackerError, error::tracker::null_ptr_deref);
}

template<IsTrackable T>
T* Tracker<T>::operator->(void) {
	return locate<T>();
}

template<IsTrackable T>
const T* Tracker<T>::operator->(void) const {
	return locate<T>();
}

template<IsTrackable T>
Tracker<T>::operator bool() const {
	return locate() != nullptr;
}
template<IsTrackable T>
bool Tracker<T>::operator==(const Tracker<T>& other) const noexcept {
	auto ptr1 = this->m_tracked.lock();
	auto ptr2 = other.m_tracked.lock();
	return ptr1 && ptr2 ? (*ptr1) == (*ptr2) : ptr1 == ptr2;
}

template<IsTrackable T>
bool Tracker<T>::operator!=(const Tracker<T>& other) const noexcept {
	auto ptr1 = this->m_tracked.lock();
	auto ptr2 = other.m_tracked.lock();
	return ptr1 && ptr2 ? ptr1->ptr != ptr2->ptr : ptr1 != ptr2;
}

template<IsTrackable T>
template<IsTrackable U> requires IsRelatedTo<U, T>
U* Tracker<T>::locate(void) noexcept {
	auto tmp = m_tracked.lock();
	if (!tmp || !tmp->ptr) return nullptr;
	T* p = const_cast<T*>(tmp->ptr);
	if constexpr (std::same_as<U, void>)        return static_cast<void*>(p);
	else if constexpr (std::derived_from<T, U>) return static_cast<U*>(p);
	else                                        return dynamic_cast<U*>(p);
}

template<IsTrackable T>
template<typename U> requires IsRelatedTo<U, T>
const U* Tracker<T>::locate(void) const noexcept {
	auto tmp = m_tracked.lock();
	if (!tmp || !tmp->ptr) return nullptr;
	const T* p = tmp->ptr;
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
Tracker<Derived> Trackable<Derived>::tracker(void) noexcept {
	if (!m_slot) m_slot = Slot<Derived>::make();
	m_slot->ptr = static_cast<const Derived*>(this);
	return Tracker<Derived>(m_slot);
}
template<typename Derived>
Tracker<Derived> Trackable<Derived>::tracker(void) const noexcept {
	if (!m_slot) m_slot = Slot<Derived>::make();
	m_slot->ptr = static_cast<const Derived*>(this);
	return Tracker<Derived>(m_slot);
}

template<typename Derived>
Trackable<Derived>::~Trackable(void) {
	if (m_slot) m_slot->ptr = nullptr;
}

} // namespace rmk
#endif