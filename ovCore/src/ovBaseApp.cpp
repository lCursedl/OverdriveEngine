#include "ovBaseApp.h"

namespace ovEngineSDK {
  int32
  BaseApp::run() {
    initSystems();
    return 0;
  }

  void
  BaseApp::eventHandler(sf::Event event) {
    
  }

  void
  BaseApp::onCreate() {
  }

  void
  BaseApp::onUpdate() {
  }

  void
  BaseApp::onRender() {
  }

  void BaseApp::onClear() {
  }

  void BaseApp::createWindow() {
  }

  void BaseApp::update(float delta) {
  }

  void BaseApp::render() {
  }

  void BaseApp::initSystems() {
    if (m_directXPlugin.loadPlugin("ovDXGraphics_d.dll")) {
      auto createGraphicsAPI = reinterpret_cast<funCreateGraphicsAPI>(
                               m_directXPlugin.getProcedureByName("createGraphicsAPI"));
      GraphicsAPI::startUp();
      GraphicsAPI* graphicAPI = createGraphicsAPI();
      g_graphicsAPI().setObject(graphicAPI);
      m_graphicsAPI = &g_graphicsAPI();
      m_graphicsAPI->init(m_windowHandle);
    }
  }

  void
  BaseApp::destroySystems() {
    g_graphicsAPI().shutDown();
  }
}