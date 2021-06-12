#include "ovBaseApp.h"

namespace ovEngineSDK {
  int32
  BaseApp::run() {
    initSystems();
    createWindow();
    m_graphicsAPI->init(m_windowHandle);
    onCreate();
    MSG msg = {};
    while (WM_QUIT != msg.message) {
      m_deltaTime = m_appClock.getElapsedTime().asSeconds();
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
  BaseApp::onUpdate(float delta) {
  }

  void
  BaseApp::onRender() {
  }

  void BaseApp::onClear() {
  }
  
  void BaseApp::update() {
    onUpdate(m_deltaTime);
  }

  void BaseApp::render() {
    onRender();
    m_graphicsAPI->swapBuffer();
  }

  void BaseApp::initSystems() {
    if (m_directXPlugin.loadPlugin("ovDXGraphics_d.dll")) {
      auto createGraphicsAPI = reinterpret_cast<funCreateGraphicsAPI>(
                               m_directXPlugin.getProcedureByName("createGraphicsAPI"));
      GraphicsAPI::startUp();
      GraphicsAPI* graphicAPI = createGraphicsAPI();
      g_graphicsAPI().setObject(graphicAPI);
      m_graphicsAPI = &g_graphicsAPI();      
    }
  }

  void
  BaseApp::destroySystems() {
    m_graphicsAPI->shutdown();
    m_graphicsAPI->shutDown();
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
  LRESULT BaseApp::WndProc(HWND hWnd, uint32 message, WPARAM wParam, LPARAM lParam) {
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