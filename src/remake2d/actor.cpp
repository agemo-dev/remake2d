#include <remake2d/actor.hpp>

namespace rmk {

void ActorBase::addChild(ActorBase* child) noexcept {
    if (child->m_parent) {
        child->m_parent->removeChild(child);
    }
	
    child->m_parent = this;
    m_children.push_back(child);
}

void ActorBase::removeChild(ActorBase* child) noexcept {
    auto it = std::find(m_children.begin(), m_children.end(), child);
	
    if (it != m_children.end()) {
        child->m_parent = nullptr;
        m_children.erase(it);
    }
}

void ActorBase::active(bool stat) noexcept {
    m_active = stat;
}

bool ActorBase::active(void) const noexcept {
    return m_active;
}

ActorBase* ActorBase::parent(void) const noexcept {
    return m_parent;
}

const std::vector<ActorBase*>& ActorBase::children(void) const noexcept {
    return m_children;
}

void ActorBase::_updates(void) {
    for (auto* child : m_children) {
        child->update();
        child->_updates();
    }
}

ActorBase::~ActorBase(void) {
    for (auto* child : m_children) {
        child->m_parent = nullptr;
    }
}

} // namespace rmk