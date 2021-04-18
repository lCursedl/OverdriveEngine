#pragma once
#include <ovPrerequisitesCore.h>

namespace ovEngineSDK {
  class Plugin
  {
   public:
    Plugin() = default;
    ~Plugin() = default;

    bool
    loadPlugin(const String& dllName);

    void*
    getProcedureByName(const String& name);

    void
    destroy();

   public:
    void* m_instance = nullptr;
  };

  inline bool
  Plugin::loadPlugin(const String& dllName) {
    m_instance = LoadLibraryExA(dllName.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
    
    if (!m_instance) {
      DWORD err = GetLastError();
      destroy();
      return false;
    }
  }

  inline void*
  Plugin::getProcedureByName(const String& name) {
    auto function = GetProcAddress(reinterpret_cast<HINSTANCE>(m_instance),
                                   name.c_str());
    if (!function) {
      std::cout << "Failed loading function \"" << name << "\"\n";
    }
    return function;
  }

  inline void
  Plugin::destroy() {
    FreeLibrary(reinterpret_cast<HINSTANCE>(m_instance));
  }
}