#include "ovBaseApp.h"
#include "ovBaseRenderer.h"
#include "ovBaseInputManager.h"
#include "ovBaseOmniverse.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace ovEngineSDK {
  
  LRESULT CALLBACK WndProc(HWND hWnd, uint32 message, WPARAM wParam, LPARAM lParam);

  int32
  BaseApp::run() {
    initSystems();
    createWindow();
    g_graphicsAPI().init(m_windowHandle);    
    BaseInputManager::instance().init(m_windowHandle);
    onCreate();
    BaseRenderer::instance().init();
    BaseOmniverse::instance().init();
    MSG msg = {};

    while (WM_QUIT != msg.message) {
      m_deltaTime = static_cast<float>(m_appClock.getElapsedTime().asMilliseconds());
      m_appClock.restart();
      if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
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
    g_baseInput().update();
    SceneGraph::instance().update(m_deltaTime);
    BaseOmniverse::instance().update();
    onUpdate(m_deltaTime);
    BaseRenderer::instance().update();
  }

  void BaseApp::render() {
    BaseRenderer::instance().render();
    onRender();
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
      BaseInputManager::instance().setObject(inputBase);
      //g_baseInput().setObject(inputBase);
    }
    if (m_omniPlugin.loadPlugin("ovOmniverse_d.dll")) {
      auto createOmniverse = reinterpret_cast<funcCreateBaseOmniverse>(
                             m_omniPlugin.getProcedureByName("createOmniverse"));
      BaseOmniverse::startUp();
      BaseOmniverse* omniBase = createOmniverse();
      BaseOmniverse::instance().setObject(omniBase);
    }
  }

  void
  BaseApp::destroySystems() {
    g_graphicsAPI().shutdown();
    g_graphicsAPI().shutDown();
    SceneGraph::shutDown();
    BaseRenderer::shutDown();
    BaseInputManager::shutDown();
    BaseOmniverse::shutDown();
  }

  void BaseApp::createWindow() {
    //Register window class
    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = 0;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = 0;
    wc.lpszMenuName = 0;
    wc.lpszClassName = "overdrive";
    wc.hIconSm = 0;

    if (!RegisterClassEx(&wc)) {
      return;
    }
    RECT rc = {0, 0, 800, 600};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
    //Create window
    m_windowHandle = CreateWindow(
      "overdrive",
      "Overdrive Engine",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,
      0,
      rc.right - rc.left,
      rc.bottom - rc.top,
      0,
      0,
      0,
      0);

    ShowWindow(m_windowHandle, 1);
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
    }
    return 0;
  }
}