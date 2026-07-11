#include <remake2d/config/config.hpp>
#include <remake2d/loop.hpp>
#include <remake2d/texture.hpp>
#include <remake2d/window.hpp>
#include <remake2d/time.hpp>
#include <remake2d/random.hpp>
#include <remake2d/physic.hpp>
#include <remake2d/script.hpp>
#include <remake2d/data.hpp>
#include <remake2d/system.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaSingleton(sol::table& rmk) noexcept {

    script._registerEngineType<DeltaTime>("DeltaTime", [](SolState::Type& ut) {
        ut["tick"]   = &DeltaTime::tick;
        ut["FPS"]    = &DeltaTime::FPS;
        ut["maxFPS"] = sol::overload(
            [](DeltaTime& self) { return self.maxFPS(); },
            [](DeltaTime& self, fmax v) { self.maxFPS(v); }
        );
    });
    rmk["delta"] = &delta;

    script._registerEngineType<PhysicManager>("PhysicManager", [](SolState::Type& ut) {
        ut["world"] = sol::overload(
            [](PhysicManager& self) { return self.world(); },
            [](PhysicManager& self, const Area& a) { self.world(a); }
        );
        ut["gravitationalConstant"] = sol::overload(
            [](PhysicManager& self) { return self.gravitationalConstant(); },
            [](PhysicManager& self, f32 g) { self.gravitationalConstant(g); }
        );
        ut["pixelsPerMeter"] = sol::overload(
            [](PhysicManager& self) { return self.pixelsPerMeter(); },
            [](PhysicManager& self, f32 p) { self.pixelsPerMeter(p); }
        );
        ut["bodies"]   = &PhysicManager::bodies;
        ut["dynamics"] = &PhysicManager::dynamics;
        ut["statics"]  = &PhysicManager::statics;
    });
    rmk["physics"] = &physics;

    script._registerEngineType<Random>("Random", [](SolState::Type& ut) {
        ut["seed"] = sol::overload(
            [](Random& self) { return self.seed(); },
            [](Random& self, u32 s) { self.seed(s); }
        );
        ut["rollSeed"] = &Random::rollSeed;
        ut["chance"]   = &Random::chance;
        ut["dice"]     = &Random::dice;
        ut["rand"] = sol::overload(
            [](Random& self, f32 min, f32 max) { return self.rand(min, max); },
            [](Random& self, i32 min, i32 max) { return self.rand(min, max); },
            [](Random& self, const Vec2d& min, const Vec2d& max) { return self.rand(min, max); },
            [](Random& self, const Dim2d& min, const Dim2d& max) { return self.rand(min, max); },
            [](Random& self, const Color& a, const Color& b) { return self.rand(a, b); }
        );
        ut["mixColor"] = &Random::mixColor;
        ut["choice"]   = [](Random& self, sol::variadic_args va) -> sol::object {
                    std::vector<sol::object> vec;
                    for (auto v : va) vec.push_back(v.get<sol::object>());
                    return random.choice<sol::object>(vec);
                };
    });
    rmk["random"] = &random;

    script._registerEngineType<SaveManager>("SaveManager", [](SolState::Type& ut) {
        ut["root"] = sol::overload(
            [](SaveManager& self) { return self.root(); },
            [](SaveManager& self, std::string_view p) { self.root(p); }
        );
    });
    rmk["data"] = &data;

    script._registerEngineType<SolState>("SolState", [](SolState::Type& ut) {
        ut["loadedTypes"]      = &SolState::loadedTypes;
    });
    rmk["script"] = &script;

    script._registerEngineType<MainRenderLoop>("MainRenderLoop", [](SolState::Type& ut) {
        ut["update"]    = &MainRenderLoop::update;
        ut["isRunning"] = &MainRenderLoop::isRunning;
        ut["execute"] = sol::overload(
            [](MainRenderLoop& self, Window& win, sol::function body) { loop.execute(win, [body]() { body(); }); },
            [](MainRenderLoop& self, sol::function condition, sol::function body) { loop.execute([condition]() -> bool { return condition(); }, [body]() { body(); }); }
        );
    });
    rmk["loop"] = &loop;

    script._registerEngineType<FontManager>("FontManager", [](SolState::Type& ut) {
        ut["load"] = &FontManager::load;
    });
    rmk["font"] = &font;

    script._registerEngineType<XWindow>("XWindow");
    rmk["xwindow"] = &xwindow;

    script._registerEngineType<System::Info>("System_Info", [](SolState::Type& ut) {
        ut["platform"]     = &System::Info::platform;
        ut["cpuCount"]     = &System::Info::cpuCount;
        ut["ramMB"]        = &System::Info::ramMB;
        ut["displayCount"] = &System::Info::displayCount;
        ut["channelCount"] = &System::Info::channelCount;
        ut["screenSize"]   = &System::Info::screenSize;
        ut["currentDay"]   = &System::Info::currentDay;
        ut["currentTime"]  = &System::Info::currentTime;
    });

    script._registerEngineType<System::Toggle>("System_Toggle", [](SolState::Type& ut) {
        ut["vsync"]                   = &System::Toggle::vsync;
        ut["relativeMouseMode"]       = &System::Toggle::relativeMouseMode;
        ut["gameController"]          = &System::Toggle::gameController;
        ut["blockOnPause"]            = &System::Toggle::blockOnPause;
        ut["hideHomeIndicator"]       = &System::Toggle::hideHomeIndicator;
        ut["materialAcceleration"]    = &System::Toggle::materialAcceleration;
        ut["accelerometerAsJoystick"] = &System::Toggle::accelerometerAsJoystick;
    });

    script._registerEngineType<System::Setup>("System_Setup", [](SolState::Type& ut) {
        ut["backend"]           = &System::Setup::backend;
        ut["scalingMode"]       = &System::Setup::scalingMode;
        ut["allocateChannels"]  = &System::Setup::allocateChannels;
        ut["audioMode"]         = &System::Setup::audioMode;
        ut["audioCategory"]     = &System::Setup::audioCategory;
        ut["mouseRelativeMode"] = &System::Setup::mouseRelativeMode;
        ut["mobileOrientation"] = [](rmk::System::Setup& self, sol::variadic_args va) {
            std::vector<std::string_view> views;
            for (auto v : va) views.push_back(v.get<std::string_view>());
            self.mobileOrientation(views);
        };
    });

    script._registerEngineType<System>("System", [&](SolState::Type& ut) {
        ut["init"]    = &System::init;
        ut["isInit"]  = &System::isInit;
        ut["info"]    = &rmk::system.info;
        ut["toggle"]  = &rmk::system.toggle;
        ut["setup"]   = &rmk::system.setup;
    });
    rmk["system"] = &rmk::system;
}

} // namespace solstat
} // namespace config
} // namespace rmk