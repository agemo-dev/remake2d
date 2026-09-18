#include <remake2d/actor.hpp>
#include <remake2d/vector.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/math.hpp>

namespace rmk {

void Actor::addChild(Actor& child) noexcept {
    if (child.m_parent) child.m_parent->removeChild(child);
    child.m_parent = this->tracker();
    m_children.push_back(child.tracker());
}

void Actor::removeChild(Actor& child) noexcept {
    auto it = std::find(m_children.begin(), m_children.end(), child.tracker());
    if (it == m_children.end()) return;

	child.m_parent = nil;
	m_children.erase(it);
}

void Actor::active(bool stat) noexcept {
    m_active = stat;
}

bool Actor::active(void) const noexcept {
    return m_active;
}

UnsafeTracker<Actor>& Actor::parent(void) noexcept {
    return m_parent;
}

const UnsafeTracker<Actor>& Actor::parent(void) const noexcept {
    return m_parent;
}

std::vector<UnsafeTracker<Actor>>& Actor::children(void) noexcept {
    return m_children;
}

const std::vector<UnsafeTracker<Actor>>& Actor::children(void) const noexcept {
    return m_children;
}

void Actor::_updates(void) {
    for (auto& child : m_children) {
        if (child) child->update();
        if (child) child->_updates();
    }
}

Actor::~Actor(void) {
    for (auto& child : m_children) child->m_parent = nil;
}

} // namespace rmk