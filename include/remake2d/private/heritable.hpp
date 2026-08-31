#ifndef REMAKE2D_HERITABLE_
#define REMAKE2D_HERITABLE_

#define rmk_heritableBaseClass(CLASS) public: \
    CLASS(void)                     = default; \
    CLASS(CLASS&&)                  = default; \
    CLASS(const CLASS&)             = default; \
    CLASS& operator=(CLASS&&)       = default; \
    CLASS& operator=(const CLASS&)  = default; \
public: \
    virtual ~CLASS(void) = default;

#endif