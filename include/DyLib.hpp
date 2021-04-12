#pragma once

#include "Exception.hpp"
#include <string>
#include <dlfcn.h>

template<typename T>
class DyLib
{
private:
    void *m_handle{nullptr};
    T *m_lib{nullptr};
public:

/** Creates a dynamic library object
 */
    DyLib() noexcept = default;
/**
 *  Creates a dynamic library instance
 *
 *  @param template type of the entity returned by entry_point function inside library
 *  @param path path to the dynamic library to load (.so, .dll, .dylib)
 */
    DyLib(const std::string &path)
    {
        this->loadLib(path);
    }
    ~DyLib()
    {
        this->releaseLib();
    }

    DyLib(const DyLib&) = delete;
    DyLib(DyLib &&) = delete;
    DyLib& operator=(const DyLib&) = delete;
    DyLib& operator=(DyLib &&) = delete;

/**
 *  Load a dynamic library into the object. 
 *  If a dynamic library was already opened, it will be unload and replaced
 *
 *  @param template type of the entity returned by entry_point function inside library
 *  @param path path to the dynamic library to load (.so, .dll, .dylib)
 */
    void loadLib(const std::string &path)
    {
        this->releaseLib();
        m_handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
        if (!m_handle)
            throw Arcade::DyLibException(dlerror());
        void *sym = dlsym(m_handle, "entry_point");
        if (!sym)
            throw Arcade::DyLibException(dlerror());
        m_lib = reinterpret_cast<T *(*)()>(sym)();
    }
/** Close the dynamic library currently loaded in the object. 
 *  This function will be automatically called by the class destructor
 */
    void releaseLib() noexcept
    {
        delete m_lib;
        m_lib = nullptr;
        if (m_handle)
            dlclose(m_handle);
        m_handle = nullptr;
    }
    T *get() {return m_lib;}
};