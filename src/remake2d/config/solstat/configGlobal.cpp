#include <remake2d/config/config.hpp>
#include <remake2d/texture.hpp>
#include <remake2d/window.hpp>
#include <remake2d/event.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaGlobal(sol::table& rmk) noexcept {

    rmk["window"] = rmk.create();
    rmk["window"]["pos"] = rmk.create();
    rmk["window"]["pos"]["centered"]  = window::pos::centered;
    rmk["window"]["pos"]["undefined"] = window::pos::undefined;
    rmk["window"]["size"] = rmk.create();
    rmk["window"]["size"]["svga"]   = window::size::svga;
    rmk["window"]["size"]["vga"]    = window::size::vga;
    rmk["window"]["size"]["hd"]     = window::size::hd;
    rmk["window"]["size"]["fullhd"] = window::size::fullhd;
    rmk["window"]["size"]["qhd"]    = window::size::qhd;
    rmk["window"]["size"]["uhd"]    = window::size::uhd;

    rmk["channel"] = rmk.create();
    rmk["channel"]["min"] = channel::min;
    rmk["channel"]["mid"] = channel::mid;
    rmk["channel"]["max"] = channel::max;

    rmk["fps"] = rmk.create();
    rmk["fps"]["min"] = fps::min;
    rmk["fps"]["max"] = fps::max;

    rmk["edge"] = rmk.create();
    rmk["edge"]["level"]   = EdgeMode::level;
    rmk["edge"]["rising"]  = EdgeMode::rising;
    rmk["edge"]["falling"] = EdgeMode::falling;
    rmk["edge"]["both"]    = EdgeMode::both;

    rmk["anchor"] = rmk.create();
    rmk["anchor"]["x"] = rmk.create();
    rmk["anchor"]["x"]["left"]   = anchor::x::left;
    rmk["anchor"]["x"]["center"] = anchor::x::center;
    rmk["anchor"]["x"]["right"]  = anchor::x::right;
    rmk["anchor"]["y"] = rmk.create();
    rmk["anchor"]["y"]["top"]    = anchor::y::top;
    rmk["anchor"]["y"]["middle"] = anchor::y::middle;
    rmk["anchor"]["y"]["bottom"] = anchor::y::bottom;

    rmk["layer"] = rmk.create();
    rmk["layer"]["ground"] = &layer::ground;
    rmk["layer"]["world"]  = &layer::world;
    rmk["layer"]["sky"]    = &layer::sky;
    rmk["layer"]["ui"]     = &layer::ui;
    rmk["layer"]["log"]    = &layer::log;
    rmk["layer"]["min"]    = layer::min;
    rmk["layer"]["max"]    = layer::max;

    rmk["color"] = rmk.create();
    rmk["color"]["red"]     = color::red;
    rmk["color"]["green"]   = color::green;
    rmk["color"]["blue"]    = color::blue;
    rmk["color"]["white"]   = color::white;
    rmk["color"]["black"]   = color::black;
    rmk["color"]["yellow"]  = color::yellow;
    rmk["color"]["cyan"]    = color::cyan;
    rmk["color"]["magenta"] = color::fuchsia;
    rmk["color"]["gray"]    = color::gray;
    rmk["color"]["orange"]  = color::orange;
    rmk["color"]["purple"]  = color::purple;
    rmk["color"]["pink"]    = color::pink;
    rmk["color"]["lime"]    = color::lime;
    rmk["color"]["teal"]    = color::teal;
    rmk["color"]["indigo"]  = color::indigo;
    rmk["color"]["amber"]   = color::amber;
    rmk["color"]["emerald"] = color::emerald;
    rmk["color"]["aqua"]    = color::aqua;
    rmk["color"]["gold"]    = color::gold;
    rmk["color"]["silver"]  = color::silver;
    rmk["color"]["maroon"]  = color::maroon;
    rmk["color"]["skyblue"] = color::skyblue;
    rmk["color"]["darkblue"]   = color::darkblue;
    rmk["color"]["darkgreen"]  = color::darkgreen;
    rmk["color"]["raywhite"]   = color::raywhite;
    
    rmk["orientation"] = rmk.create();
    rmk["orientation"]["normal"]    = orientation::normal;
    rmk["orientation"]["reverse"]   = orientation::reverse;
    rmk["orientation"]["right"]     = orientation::right;
    rmk["orientation"]["left"]      = orientation::left;
    rmk["orientation"]["portrait"]  = orientation::portrait;
    rmk["orientation"]["landscape"] = orientation::landscape;
    rmk["orientation"]["free"]      = orientation::any;
    
    rmk["scaling"] = rmk.create();
    rmk["scaling"]["nearest"]     = scaling::nearest;
    rmk["scaling"]["linear"]      = scaling::linear;
    rmk["scaling"]["anisotropic"] = scaling::anisotropic;
    
    rmk["backend"] = rmk.create();
    rmk["backend"]["opengl"]   = backend::opengl;
    rmk["backend"]["opengles"] = backend::opengles;
    rmk["backend"]["direct3d"] = backend::direct3d;
    rmk["backend"]["vulkan"]   = backend::vulkan;
    rmk["backend"]["metal"]    = backend::metal;
    rmk["backend"]["software"] = backend::software;
    
    rmk["audiomode"] = rmk.create();
    rmk["audiomode"]["fast"]   = audiomode::fast;
    rmk["audiomode"]["medium"] = audiomode::medium;
    rmk["audiomode"]["best"]   = audiomode::best;
    
    rmk["audiocategory"] = rmk.create();
    rmk["audiocategory"]["media"] = audiocategory::media;
    rmk["audiocategory"]["voice"] = audiocategory::voice;
    rmk["audiocategory"]["alarm"] = audiocategory::alarm;
    
    rmk["relmode"] = rmk.create();
    rmk["relmode"]["raw"]  = relmode::raw;
    rmk["relmode"]["warp"] = relmode::warp;
    
    sol::table win = rmk["window"];
	win["blendmode"] = win.create();
    win["blendmode"]["none"]     = window::blendmode::none;
    win["blendmode"]["normal"]   = window::blendmode::normal;
    win["blendmode"]["advanced"] = window::blendmode::add;
}

} // namespace solstat
} // namespace config
} // namespace rmk