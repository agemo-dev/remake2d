# Interface headers

Les multiple en-tetes peuvent devenir lourde a inclurz separement, c'est pourquoi
il est important de pouvoir s'on abstenir .

---

## Overviews

Pour eviter les inclusiants à repetition, **RE:MAKE 2D** offre les **interface headers**
pour pouvoir inclure une categorie d'en-tetes en une fois .
Ces categories ettant les suivantes :

- everything
- assets
- bases
- cores
- entities
- graphics
- maps
- times
- tools
- types

---

## Usage

Pour inclure l'une de ces categories, il suffie de faire : `#include <remake2d/all/<NomDeLaCategorie>.hpp>`
exemple:

```cpp
#include <remake2d/all/everything.hpp>
```

Ainsi, toutes les en-têtes contenue par celui- ci seront incluses avec .

---

## contenue

### Everything

l'interface `everything` inclura tous simplement toutes les en-têtes publique du moteur .

```cpp
#include <remake2d/numeric.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/croutine.hpp>
#include <remake2d/tracker.hpp>
#include <remake2d/math.hpp>
#include <remake2d/lock.hpp>
#include <remake2d/error.hpp>
#include <remake2d/color.hpp>
#include <remake2d/vector.hpp>
#include <remake2d/shape.hpp>
#include <remake2d/signal.hpp>
#include <remake2d/time.hpp>
#include <remake2d/clock.hpp>
#include <remake2d/system.hpp>
#include <remake2d/event.hpp>
#include <remake2d/sound.hpp>
#include <remake2d/texture.hpp>
#include <remake2d/actor.hpp>
#include <remake2d/scene.hpp>
#include <remake2d/window.hpp>
#include <remake2d/camera.hpp>
#include <remake2d/physic.hpp>
#include <remake2d/tilemap.hpp>
#include <remake2d/parallax.hpp>
#include <remake2d/tilegrid.hpp>
#include <remake2d/data.hpp>
#include <remake2d/stream.hpp>
#include <remake2d/random.hpp>
#include <remake2d/loop.hpp>

#ifdef REMAKE2D_ADDON_SCRIPT
#include <remake2d/script.hpp>
#endif
```

Noté qu'il inclue egalement les en-tetes des add-ons si sont lié au projet ;) .

### Assets

l'interface `assets` inclura tout module chargé de gérer des fichiers externes :

```cpp
#include <remake2d/data.hpp>
#include <remake2d/sound.hpp>
#include <remake2d/texture.hpp>

#ifdef REMAKE2D_ADDON_SCRIPT
#include <remake2d/script.hpp>
#endif
```

### Bases

l'interface `bases` inclura toutes les en-têtes supposé etre inclus par n'importe quel
projet utilisant ce moteur ; nous pouvons voir cela comme un kit de base à chaque fichiers
contenant la fonction `main` :

```cpp
#include <remake2d/loop.hpp>
#include <remake2d/scene.hpp>
#include <remake2d/event.hpp>
#include <remake2d/shape.hpp>
#include <remake2d/window.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/all/types.hpp>
```

### Cores

l'interface `core` permet d'inclure tous les modules principalement utilisé en masse ou indispensable
au fonctionnenment interne du moteur ; c'est en quelque sorte le coeur du moteur :

```cpp
#include <remake2d/math.hpp>
#include <remake2d/lock.hpp>
#include <remake2d/error.hpp>
#include <remake2d/system.hpp>
#include <remake2d/signal.hpp>
#include <remake2d/croutine.hpp>
```

### Entities

l'interface `entities` inclus tous les modules chargés de la gestion d'entité:

```cpp
#include <remake2d/actor.hpp>
#include <remake2d/shape.hpp>
#include <remake2d/physic.hpp>
```