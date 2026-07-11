#ifndef REMAKE2D_ERROR_
#define REMAKE2D_ERROR_


#include <iostream>
#include <exception>
#include <source_location>

#define rmk_dynamicAssert(type, msg) \
    do {\
        type err(msg); \
        err.info = std::source_location::current(); \
        std::cerr   << "\033[31m[!] error generate : "\
                    << "\n\033[34m> file     : \033[0m" << err.info.file_name() \
                    << "\n\033[34m> line     : \033[0m" << err.info.line() \
                    << "\n\033[34m> function : \033[0m" << err.info.function_name() \
                    << "\n\033[34m> type     : \033[0m" << #type \
                    << "\n\033[34m> info     : \033[0m" << err.what() << "\n"; \
                std::exit(1); \
    } while(0)


namespace rmk {

class Error : public std::exception {
private:
    std::string m_msg;

public:
    mutable std::source_location info;
 
public:
    Error(std::string_view msg) : m_msg(msg) {}
    
    const char *what() const noexcept override { return m_msg.c_str(); }
};


class LoadingError : public Error {
    using Error::Error;
};

class LogicError : public Error {
    using Error::Error;
};

class RunTimeError : public Error {
    using Error::Error;
};

class OutOfRangeError : public Error {
    using Error::Error;
};


class SystemError : public Error {
    using Error::Error;
};

class TimerError : public Error {
    using Error::Error;
};

class WindowError : public LoadingError {
    using LoadingError::LoadingError;
};

class DataError : public Error {
    using Error::Error;
};

class DateError  : public Error { 
    using Error::Error;
};

class ClockError : public Error {
    using Error::Error;
};

class LayerError : public LogicError {
    using LogicError::LogicError;
};

class SceneError : public LogicError {
    using LogicError::LogicError;
};

class CroutineError : public RunTimeError {
    using RunTimeError::RunTimeError;
};

class SoundError : public LoadingError {
    using LoadingError::LoadingError;
};

class TextureError : public LoadingError {
    using LoadingError::LoadingError;
};

class ScriptError : public LogicError {
    using LogicError::LogicError;
};

class PhysicError : public RunTimeError {
    using RunTimeError::RunTimeError;
};

class TileMapError : public LogicError {
    using LogicError::LogicError;
};

class SignalError : public OutOfRangeError {
    using OutOfRangeError::OutOfRangeError;
};



namespace error {

namespace window {
inline static constexpr const char *window_no_create   = "window no create — initialization failed";
inline static constexpr const char *renderer_no_create = "renderer no create — initialization failed";
inline static constexpr const char *texture_unlinked   = "texture is not linked to this window";
} // namespace window

namespace scene {
inline static constexpr const char *any_focus               = "any focus scene or link found — please choose a focus scene or link with Act::focus(std::string_view)";
inline static constexpr const char *scene_unexist           = "scene don\'t exist";
inline static constexpr const char *multiple_scene_declared = "cannot initialize multiple scene with same name";
inline static constexpr const char *layer_is_overlimits     = "cannot place function in a invalid layer, please respect a layers range (-256, 1023) or range of layers function if used (0, 255)";
}//namespace scene

namespace croutine {
inline static constexpr const char *function_is_invalid = "function has not a compatible parameter";
inline static constexpr const char *any_function_linked = "any function is linked with croutine";
}//namespace croutine

namespace sound {
inline static constexpr const char *sound_no_load = "sound no load — initialization failed";
}//namespace sound

namespace system {
inline static constexpr const char *bad_call                = "Methods of System::Setup and System::Toggle must be called before initialisation";
inline static constexpr const char *sdl_init_fail           = "SDL initialization failed";
inline static constexpr const char *sdl_ttf_init_fail       = "SDL ttf initialization failed";
inline static constexpr const char *sdl_image_init_fail     = "SDL image initialization failed";
inline static constexpr const char *sdl_mixer_init_fail     = "SDL mixer initialization failed";
inline static constexpr const char *channel_overflow        = "you have allocate so more chanel (max == 32)";
}//namespace system

namespace texture {
inline static constexpr const char *font_no_load    = "font no load — initialization failed";
inline static constexpr const char *glyph_no_load   = "glyph no load — initialization failed";
inline static constexpr const char *texture_no_load = "texture no load — initialization failed";
inline static constexpr const char *surface_no_load = "surface no load — initialization failed";
inline static constexpr const char *font_unexist    = "font don\'t exist";
}//namespace texture

namespace script {
inline static constexpr const char *file_unexist                = "this file don't exist";
inline static constexpr const char *type_already_registered     = "type already registered";
} // namespace script

namespace physic {
inline static constexpr const char *invalid_shape      = "unsupported shape type for PhysicBody — please use a compatible shape (triangle, rectangle, circle, ellipse (h == w))";
inline static constexpr const char *body_not_found     = "this PhysicBody are not registred in PhysicManager";
inline static constexpr const char *oval_not_supported = "no uniform ellipse unsupported for a PhysicBody — please create a regular ellipse (h == w) or a circle";
inline static constexpr const char *animation_no_found = "animation no found — ";
} // namespace physic

namespace tilemap {
inline static constexpr const char *undefined_id                = "this ID don\'t exist in this tile map";
inline static constexpr const char *undefined_tag               = "this tag don\'t exist in this tile map";
inline static constexpr const char *not_built                   = "TileMap::buildMap() must be called before Window::draw()";
inline static constexpr const char *unexcepted_tile_template    = "size of tile template must be egal to matrix's area (W * H)";
} // namespace tilemap

namespace signal {
inline static constexpr const char *slot_unexist     = "Slot not found in connected slots";
inline static constexpr const char *function_unexist = "Function not found in connected funcs";
inline static constexpr const char *invalid_slot     = "Invalid slot passed to connectOnce";
} //namespace signal

namespace data {
inline static constexpr const char *file_not_found  = "DataFile not found — please call DataFile::write() before DataFile::read()";
inline static constexpr const char *invalid_field   = "field does not exist in DataFile";
inline static constexpr const char *invalid_type    = "field type mismatch in DataFile";
inline static constexpr const char *root_not_init   = "SaveManager not initialized — call data._init() first";
inline static constexpr const char *type_unsavable  = "type must implement rmk::ISavable (sdata/ldata)";
} //namespace data

namespace date {
inline static constexpr const char *invalid_format    = "invalid format string — use day/month/year keywords";
inline static constexpr const char *invalid_date      = "invalid date string — cannot parse";
inline static constexpr const char *invalid_separator = "no valid separator found — use '-', '/', '.' or ' '";
inline static constexpr const char *invalid_month     = "invalid month value — use numeric, full name or abbreviation";
} //namespace date

namespace clock {
inline static constexpr const char *invalid_format    = "invalid format — use hour/minute/second keywords";
inline static constexpr const char *invalid_time      = "invalid time string — cannot parse";
inline static constexpr const char *invalid_separator = "invalid separator — use ':', '-' or '*'";
}


} // namespace error
} // namespace rmk

#endif