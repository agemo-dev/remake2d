#include <remake2d/scene.hpp>
#include <remake2d/utility.hpp>

#include <functional>
#include <utility>
#include <algorithm>
#include <string>
#include <vector>

namespace rmk {

void Scene::_rebuildCache(void) {
    m_actors_cache.clear();
    m_layers_cache.clear();
    
    for (auto& [layer, actor] : m_actors_map) {
        m_actors_cache.push_back(actor);
    }
    
    for (auto& [layer, frame] : m_layers_map) {
        m_layers_cache.push_back(frame);
    }
    
    m_cache_dirty = false;
}

void Scene::execute(const Frame& function) {
    m_main = function;
}

void Scene::add(const Frame& function, i16 layer_value) {
    if (layer_value < (i16)layer::min || layer_value > (i16)layer::max) {
        rmk_dynamicAssert(rmk::SceneError, error::scene::layer_is_overlimits);
    }
    m_layers_map.emplace(layer_value, function);
    m_cache_dirty = true;
}

void Scene::add(ActorBase& actor, i16 layer_value) {
    if (layer_value < (i16)layer::min || layer_value > (i16)layer::max) {
        rmk_dynamicAssert(rmk::SceneError, error::scene::layer_is_overlimits);
    }
    m_actors_map.emplace(layer_value, &actor);
    m_cache_dirty = true;
}

void Scene::remove(ActorBase& actor) {
    for (auto it = m_actors_map.begin(); it != m_actors_map.end(); ++it) {
        if (it->second == &actor) {
            m_actors_map.erase(it);
            m_cache_dirty = true;
            break;
        }
    }
}

void Scene::setLayerActive(i16 layer, bool active) {
    auto range = m_actors_map.equal_range(layer);
    for (auto it = range.first; it != range.second; ++it) {
        it->second->active(active);
    }
}

void Scene::setActorActive(ActorBase& actor, bool active) {
    actor.active(active);
}

void Scene::enable(void) noexcept {
	m_enabled = true;
}

void Scene::disable(void) noexcept {
	m_enabled = false;
}

bool Scene::isEnabled(void) const noexcept {
	return m_enabled;
}

void Scene::update(void) {
    if (!m_enabled) return;
    
    if (m_main) m_main();
    
    if (m_cache_dirty) _rebuildCache();
    
    for (auto* actor : m_actors_cache) {
        if (actor->active()) {
            actor->update();
            actor->_updates();
        }
    }
    
    for (auto& frame : m_layers_cache) {
        frame();
    }
}

void Act::add(std::string_view tag, Scene& scene) {
    std::string key(tag);
    if (m_scenes.count(key)) {
        rmk_dynamicAssert(rmk::SceneError, error::scene::multiple_scene_declared);
    }
    m_scenes.emplace(key, &scene);
}
    
void Act::link(std::string_view tag, std::span<std::string_view> scenes) {
    std::string key(tag);
    std::vector<std::string> list;
    for (auto& scene_tag : scenes) {
        std::string s(scene_tag);
        if (!m_scenes.count(s)) {
            rmk_dynamicAssert(rmk::SceneError, error::scene::scene_unexist);
        }
        list.emplace_back(s);
    }
    m_links.emplace(key, std::move(list));
}

std::vector<std::string> Act::_resolveTag(std::string_view tag) const {
    std::string key(tag);
    std::vector<std::string> result;
    
    if (m_links.count(key)) {
        result = m_links.at(key);
    } else if (m_scenes.count(key)) {
        result.emplace_back(key);
    } else {
        rmk_dynamicAssert(rmk::SceneError, error::scene::scene_unexist);
    }
    
    return result;
}

void Act::_rebuildFocusCache(void) {
    m_focused_cache.clear();
    for (auto& tag : m_focused_tags) {
        auto it = m_scenes.find(tag);
        if (it != m_scenes.end()) {
            m_focused_cache.push_back(it->second);
        }
    }
    m_focus_dirty = false;
}

void Act::focus(std::string_view tag) {
    m_focused_tags = _resolveTag(tag);
    m_focus_dirty = true;
}

void Act::update(std::string_view tag) const {
    auto tags = _resolveTag(tag);
    for (const auto& scene_tag : tags) {
        auto it = m_scenes.find(scene_tag);
        if (it != m_scenes.end()) {
            it->second->update();
        }
    }
}

void Act::update(void) const {
    if (m_focused_tags.empty()) {
        rmk_dynamicAssert(rmk::SceneError, error::scene::any_focus);
    }
    
    if (m_focus_dirty) {
        const_cast<Act*>(this)->_rebuildFocusCache();
    }
    
    for (auto* scene : m_focused_cache) {
        scene->update();
    }
}

void Act::updates(void) const {
    for (auto& entry : m_scenes) {
        entry.second->update();
    }
}

Scene* Act::scene(std::string_view tag) const {
    std::string key(tag);
    auto it = m_scenes.find(key);
    if (it != m_scenes.end()) {
        return it->second;
    }
    return nullptr;
}

} // namespace rmk