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
}
