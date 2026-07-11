#ifndef REMAKE2D_SCENE_
#define REMAKE2D_SCENE_

#include <remake2d/actor.hpp>
#include <remake2d/window.hpp>
#include <remake2d/numeric.hpp>

#include <map>
#include <span>
#include <vector>
#include <string>
#include <functional>

namespace rmk {

class Scene {
    
public:
    using Frame = std::function<void(void)>;

private:
    std::multimap<i16, ActorBase*>  m_actors_map;
    std::multimap<i16, Frame>       m_layers_map;
    std::vector<ActorBase*>         m_actors_cache;
    std::vector<Frame>              m_layers_cache;
    Frame                           m_main;
    bool                            m_cache_dirty{true};
    bool                            m_enabled{true};

public:
    Scene(void)						= default;
    Scene(Scene&&)					= default;
    Scene(const Scene&)				= delete;
    Scene& operator=(Scene&&)		= default;
    Scene& operator=(const Scene&)	= delete;

public:
    void update(void);
    void remove(ActorBase&);
    void execute(const Frame&);
    void add(ActorBase&, i16 = 0);
    void add(const Frame&, i16 = 0);

public:
    void setLayerActive(i16, bool);
    void enable(void) 		   noexcept;
    void disable(void) 		   noexcept;
    bool isEnabled(void) const noexcept;
    void setActorActive(ActorBase&, bool);

private:
    void _rebuildCache(void);
};


class Act {
private:
    std::map<std::string, Scene*>               m_scenes;
    std::map<std::string, std::vector<std::string>> m_links;
    std::vector<std::string>                    m_focused_tags;
    std::vector<Scene*>                         m_focused_cache;
    bool                                        m_focus_dirty{true};

public:
    Act(void)					= default;
    Act(Act&&)					= default;
    Act(const Act&)				= delete;
    Act& operator=(Act&&)		= default;
    Act& operator=(const Act&)	= delete;

public:
    void focus(std::string_view);
    void add(std::string_view, Scene&);
    void link(std::string_view, std::span<std::string_view>);

public:
    void update(void)				const;
    void updates(void)				const;
    void update(std::string_view)	const;

public:
    Scene* scene(std::string_view) const;

private:
    void _rebuildFocusCache(void);
    std::vector<std::string> _resolveTag(std::string_view) const;
};

} // namespace rmk

#endif