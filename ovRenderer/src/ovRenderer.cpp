#include "ovRenderer.h"
#include "ovGraphicsAPI.h"
#include "ovModel.h"

namespace ovEngineSDK {
  void
  Renderer::init() {
    auto& graphicAPI =  g_graphicsAPI();
    m_gBufferRS = graphicAPI.createRasterizerState(FILL_MODE::kSOLID,
                                                   CULL_MODE::kFRONT,
                                                    true);
    m_screenQuadRS = graphicAPI.createRasterizerState(FILL_MODE::kSOLID,
                                                      CULL_MODE::kNONE,
                                                      false);

    m_gBufferDS = graphicAPI.createDepthStencilState(true, true);
    m_screenQuadDS = graphicAPI.createDepthStencilState(false, false);

    m_screenQuad = make_shared<Model>();
    m_screenQuad->load("resources/models/ScreenAlignedQuad.3ds");
  }

  void
  Renderer::render() {
    /*for (auto& model : m_models) {
      
    }*/
  }

  void
  Renderer::setModels(Vector<SPtr<Model>> models) {
    for (auto& model : models) {
      m_models.push_back(model);
    }
  }

  OV_RENDERER_EXPORT Renderer& g_renderer() {
    return Renderer::instance();
  }
}
