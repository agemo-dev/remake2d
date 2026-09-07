#ifndef REMAKE2D_STRUCT_
#define REMAKE2D_STRUCT_

#define rmk_getID(CLASS) _ ## CLASS ## ID_

#define rmk_defineID(CLASS) using _ ## CLASS ## ID_ = void;

#define rmk_baseClass(CLASS) public: \
    CLASS(void)                     = default; \
    CLASS(CLASS&&)                  = default; \
    CLASS(const CLASS&)             = default; \
    CLASS& operator=(CLASS&&)       = default; \
    CLASS& operator=(const CLASS&)  = default;

#define rmk_heritableBaseClass(CLASS) public: \
    CLASS(void)                     = default; \
    CLASS(CLASS&&)                  = default; \
    CLASS(const CLASS&)             = default; \
    CLASS& operator=(CLASS&&)       = default; \
    CLASS& operator=(const CLASS&)  = default; \
public: \
    virtual ~CLASS(void) = default; \
    using _ ## CLASS ## ID_ = void;

#endif