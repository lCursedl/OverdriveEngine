#include "ovBaseApp.h"
#include "ovBaseRenderer.h"
#include "ovBaseInputManager.h"
#include <Windows.h>

namespace ovEngineSDK {
  
  LRESULT CALLBACK WndProc(HWND hWnd, uint32 message, WPARAM wParam, LPARAM lParam);

  int32
  BaseApp::run() {
    initSystems();
    createWindow();
    g_graphicsAPI().init(m_windowHandle);    
    onCreate();
    BaseRenderer::instance().init();
    BaseInputManager::instance().init(m_windowHandle);

    MSG msg = {};

    while (WM_QUIT != msg.message) {
      m_deltaTime = m_appClock.getElapsedTime().asSeconds();
      if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      else {
        update();
        render();
      }      
    }      
    onClear();
    destroySystems();
    return 0;
  }

  void
  BaseApp::onCreate() {
  }

  void
  BaseApp::onUpdate(float) {
  }

  void
  BaseApp::onRender() {
  }

  void BaseApp::onClear() {
  }
  
  void BaseApp::update() {
    onUpdate(m_deltaTime);
    BaseInputManager::instance().update();
  }

  void BaseApp::render() {
    onRender();
    BaseRenderer::instance().render();
    g_graphicsAPI().swapBuffer();
  }

  void BaseApp::initSystems() {
    if (m_graphicPlugin.loadPlugin("ovDXGraphics_d.dll")) {
      auto createGraphicsAPI = reinterpret_cast<funCreateGraphicsAPI>(
                               m_graphicPlugin.getProcedureByName("createGraphicsAPI"));
      GraphicsAPI::startUp();
      GraphicsAPI* graphicAPI = createGraphicsAPI();
      g_graphicsAPI().setObject(graphicAPI);
      
    }
    SceneGraph::startUp();
    if (m_rendererPlugin.loadPlugin("ovRenderer_d.dll")) {
      auto createRenderer = reinterpret_cast<funcCreateBaseRenderer>(
                            m_rendererPlugin.getProcedureByName("createRenderer"));
      BaseRenderer::startUp();
      BaseRenderer* renderBase = createRenderer();
      g_baseRenderer().setObject(renderBase);
    }
    if (m_inputPlugin.loadPlugin("ovInputManagerOIS_d.dll")) {
      auto createInputManager = reinterpret_cast<funcCreateBaseInputManager>(
                                m_inputPlugin.getProcedureByName("createInputManager"));
      BaseInputManager::startUp();
      BaseInputManager* inputBase = createInputManager();
      g_baseInput().setObject(inputBase);
    }
  }

  void
  BaseApp::destroySystems() {
    g_graphicsAPI().shutdown();
    g_graphicsAPI().shutDown();
    SceneGraph::shutDown();
    BaseRenderer::shutDown();
    BaseInputManager::shutDown();
  }

  void BaseApp::createWindow() {
    //Register window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(0);
    wc.lpszClassName = "overdrive";

    RegisterClass(&wc);

    //Create window
    HWND hwnd = CreateWindowEx(
      0,
      "overdrive",
      "Overdrive Engine",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      800,
      600,
      nullptr,
      nullptr,
      wc.hInstance,
      nullptr);

    ShowWindow(hwnd, 10);
    m_windowHandle = hwnd;
  }

  LRESULT CALLBACK WndProc(HWND hWnd, uint32 message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message) {
    case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      EndPaint(hWnd, &ps);
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
      break;
    }
    return 0;
  }
}