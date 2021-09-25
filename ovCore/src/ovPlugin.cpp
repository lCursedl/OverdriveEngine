#include "ovPlugin.h"
#if OV_PLATFORM == OV_PLATFORM_WIN32
#include <Windows.h>
#endif // OV_PLATFORM == OV_PLATFORM_WIN32

namespace ovEngineSDK {
  bool
    Plugin::loadPlugin(const String& dllName) {
    m_instance = LoadLibraryExA(dllName.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

    if (!m_instance) {
      DWORD err = 0;
      err = GetLastError();
      wprintf(L"Couldn't load dll. Error #%d.\n", err);
      destroy();
      return false;
    }
    return true;
  }

  void*
    Plugin::getProcedureByName(const String& name) {
    auto function = GetProcAddress(reinterpret_cast<HINSTANCE>(m_instance),
      name.c_str());
    if (!function) {
      std::cout << "Failed loading function \"" << name << "\"\n";
    }
    return function;
  }

  void
    Plugin::destroy() {
    FreeLibrary(reinterpret_cast<HINSTANCE>(m_instance));
  }
}
