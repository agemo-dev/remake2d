#ifndef REMAKE2D_TRACKER_TPP_
#define REMAKE2D_TRACKER_TPP_

namespace rmk {

template<typename T>
std::shared_ptr<Slot<T>> Slot<T>::make(void) noexcept {
	return std::make_shared<Slot<T>>();
}

template<typename T>
Tracker<T>::Tracker(const Balise& src) : m_tracked(src) {}

template<typename T>
void Tracker<T>::track(const Balise& b) noexcept {
	m_tracked = b;
}

template<typename T>
T* Tracker<T>::operator->(void) {
	if (auto tmp = m_tracked.lock()) return tmp->ptr;
	return nullptr;
}

template<typename T>
T* Tracker<T>::operator->(void) const {
	if (auto tmp = m_tracked.lock()) return tmp->ptr;
	return nullptr;
}

template<typename T>
T* Tracker<T>::locate(void) const noexcept {
	if (auto tmp = m_tracked.lock()) return tmp->ptr;
	return nullptr;
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
	if (m_slot) m_slot->ptr = static_cast<Derived*>(this);
}

template<typename Derived>
Tracker<Derived> Trackable<Derived>::tracker(void) noexcept {
	if (!m_slot) m_slot = Slot<Derived>::make();
	m_slot->ptr = static_cast<Derived*>(this);
	return Tracker<Derived>(m_slot);
}

template<typename Derived>
Trackable<Derived>::~Trackable(void) {
	if (m_slot) m_slot->ptr = nullptr;
}

} // namespace rmk
#endif