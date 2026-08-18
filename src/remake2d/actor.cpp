#include <remake2d/actor.hpp>
#include <remake2d/vector.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/math.hpp>

namespace rmk {

void ActorBase::addChild(ActorBase& child) noexcept {
    if (child.m_parent) child.m_parent->removeChild(child);
    child.m_parent = this->tracker();
    m_children.push_back(child.tracker());
}

void ActorBase::removeChild(ActorBase& child) noexcept {
    auto it = std::find(m_children.begin(), m_children.end(), child.tracker());
    if (it == m_children.end()) return;

	child.m_parent = nil;
	m_children.erase(it);
}

void ActorBase::active(bool stat) noexcept {
    m_active = stat;
}

bool ActorBase::active(void) const noexcept {
    return m_active;
}

Tracker<ActorBase>& ActorBase::parent(void) noexcept {
    return m_parent;
}

const Tracker<ActorBase>& ActorBase::parent(void) const noexcept {
    return m_parent;
}

std::vector<Tracker<ActorBase>>& ActorBase::children(void) noexcept {
    return m_children;
}

const std::vector<Tracker<ActorBase>>& ActorBase::children(void) const noexcept {
    return m_children;
}

void ActorBase::_updates(void) {
    for (auto* child : m_children) {
        child->update();
        child->_updates();
    }
}

ActorBase::~ActorBase(void) {
    for (auto& child : m_children) child->m_parent = nil;
}

} // namespace rmk