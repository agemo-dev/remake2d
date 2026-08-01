# Scene & Act

Moving from a menu to gameplay, then to a pause screen, without everything tangling into one giant update function calls for a way to split 
a game into separate states. That's what `Scene` and `Act` are for.

---

## Overview

`Scene` and `Act` are contained in the header **"remake2d/scene.hpp"**. A `Scene` holds a collection of actors and free functions, each 
assigned to a layer that determines their update order. An `Act` manages several named scenes, letting you group them and switch between them by tag.

---

## Scene

### Methods

```cpp
void update(void);                        // run one frame
void execute(const Frame&);               // set main update function
void add(ActorBase&, i16 layer = 0);      // add actor to a layer
void add(const Frame&, i16 layer = 0);    // add function to a layer
void remove(ActorBase&);                  // remove actor from scene
void enable(void) noexcept;               // enable scene updates
void disable(void) noexcept;              // disable scene updates
bool isEnabled(void) const noexcept;      // check if scene is enabled
void setLayerActive(i16 layer, bool);     // enable/disable all actors on a layer
void setActorActive(ActorBase&, bool);    // enable/disable a specific actor
```

`Frame` is a simple alias for a free function:

```cpp
using Frame = std::function<void(void)>;
```

### Usage

#### Adding actors and functions

Every actor or function added to a `Scene` gets an optional layer, which determines the order it's updated in relative to others:

```cpp
rmk::Scene scene;
Player     player;

scene.add(player, rmk::layer::world(0));
scene.add([&]() { std::cout << "frame tick\n"; }, rmk::layer::ui(0));
```

!!! info
    Valid layer values range from `rmk::layer::min` to `rmk::layer::max` (see [Utility](../tools/utility.md)).

#### Main update function

Some logic needs to run before everything else, regardless of layer; `execute` sets that main function, called once per update before any 
layered actor or function:

```cpp
scene.execute([&]() {
    handleGlobalInput();
});
```

#### Running the scene

```cpp
// In render loop
scene.update();
```

Update, in order, calls the main function, then every layered free function and every active actor, in ascending layer order.

#### Enabling/disabling

Pausing a whole menu, or hiding the HUD temporarily, doesn't require removing anything from the scene:

```cpp
scene.disable();
if (scene.isEnabled()) scene.update(); // skipped
```

Individual layers or actors can also be toggled independently:

```cpp
scene.setLayerActive(rmk::layer::ui(0), false);
scene.setActorActive(player, false);
```

---

## Act

Where a `Scene` handles one single state, `Act` handles the transition between several: it decides which scene, or scenes, are currently on screen.

### Methods

```cpp
void add(std::string_view tag, Scene&);                                // register a scene
void link(std::string_view tag, std::span<std::string_view> scenes);   // group scenes under a tag
void focus(std::string_view tag);                                      // activate a scene or link
void update(void) const;                                                // update focused scene(s)
void update(std::string_view tag) const;                                // update a specific scene or link
void updates(void) const;                                                // update all scenes
Scene* getScene(std::string_view tag) const;                            // get scene by tag
```

### Usage

#### Registering scenes

Each scene is registered under a tag, later used to retrieve it, link it to others, or bring it into focus:

```cpp
rmk::Scene menuScene;
rmk::Scene gameScene;
rmk::Act   act;

act.add("menu", menuScene);
act.add("game", gameScene);
```

#### Linking scenes

A HUD shown throughout gameplay isn't necessarily the same scene as gameplay itself; `link` groups them under a single tag so they update together:

```cpp
rmk::Scene hudScene;
act.add("hud", hudScene);

std::string_view group[] = { "game", "hud" };
act.link("gameplay", group);
```

#### Focusing and updating

Focus selects which scene, or link, gets updated by the no-argument update call, avoiding the need to repeat it every frame:

```cpp
act.focus("gameplay");

// In render loop
act.update(); // updates "game" and "hud"
```

A specific tag can also be updated directly, bypassing the current focus:

```cpp
act.update("menu");
```

Some scenes, like background music or a notification system, need to keep running regardless of what's on screen; `updates` handles that:

```cpp
act.updates();
```

#### Retrieving a scene

```cpp
rmk::Scene* menu = act.getScene("menu");
if (menu) menu->disable();
```

---

[:octicons-arrow-left-24: Previous chapter](random.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](../data/data.md){ .md-button .md-button--primary }
