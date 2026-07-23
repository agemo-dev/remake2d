#ifndef REMAKE2D_RESSOURCE_TPP_
#define REMAKE2D_RESSOURCE_TPP_

namespace rmk {

template<typename T, void(*F)(T*)>
Resource<T, F>::Resource(void) {
    rmk::system.init();
    m_refcount = new u32(1);
}

template<typename T, void(*F)(T*)>
Resource<T, F>::Resource(Resource&& o) noexcept
    : data(o.data), m_refcount(o.m_refcount) {
    rmk::system.init();
    o.data = nullptr;
    o.m_refcount = nullptr;
}

template<typename T, void(*F)(T*)>
Resource<T, F>::Resource(const Resource& o)
    : data(o.data), m_refcount(o.m_refcount) {
    rmk::system.init();
    if (m_refcount)
        ++(*m_refcount);
}

template<typename T, void(*F)(T*)>
Resource<T, F>& Resource<T, F>::operator=(Resource&& o) noexcept {
    if (this != &o) {
        release();
        data = o.data;
        m_refcount = o.m_refcount;
        o.data = nullptr;
        o.m_refcount = nullptr;
    }
    return *this;
}

template<typename T, void(*F)(T*)>
Resource<T, F>& Resource<T, F>::operator=(const Resource& o) {
    if (this != &o) {
        if (o.m_refcount)
            ++(*o.m_refcount);

        release();

        data = o.data;
        m_refcount = o.m_refcount;
    }
    return *this;
}

template<typename T, void(*F)(T*)>
void Resource<T, F>::release(void) {
    if (m_refcount && --(*m_refcount) == 0) {
        if (data)
            F(data);

        delete m_refcount;
    }

    data = nullptr;
    m_refcount = nullptr;
}

template<typename T, void(*F)(T*)>
Resource<T, F>::~Resource(void) {
    release();
    rmk::system.quit();
}

} // namespace rmk
#endif