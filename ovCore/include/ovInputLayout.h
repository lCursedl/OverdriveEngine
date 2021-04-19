#pragma once
#include <ovPrerequisitesCore.h>
#include <ovFormats.h>

namespace ovEngineSDK {
  /**
  * @enum        SEMANTIC
  *	@brief       Enum which contains definitions for different shader semantics.
  */
  namespace SEMANTIC {
    enum E {
      POSITION = 0,
      TEXCOORD,
      NORMAL,
      BINORMAL,
      TANGENT
    };
  }
  /** 
  * @struct      LAYOUT
  *	@brief       Structure which contains a single layout element.
  */
  struct LAYOUT {
    /**
    * @fn LAYOUT(SEMANTIC semantic, FORMATS formats, int offset, int elements)
    *	@brief Struct constructor
    *	@param[in] ssemantic Input SEMANTIC
    *	@param[in] formats Input FORMAT
    *	@param[in] offset int with value for the layout offset
    *	@param[in] elements int with value for the amount of elements in layout
    */
    LAYOUT(SEMANTIC::E semantic,
           FORMATS::E formats,
           int32 offset,
           int32 elements) : m_semantic(semantic),
                             m_format(formats),
                             m_offset(offset),
                             m_numElements(elements) {}
    SEMANTIC::E m_semantic; /**< SEMANTIC member to store semantic type*/
    FORMATS::E m_format;    /**< FORMATS member to store format type*/
    int32 m_offset;         /**< int member to store layout offset*/
    int32 m_numElements;    /**< int member to store number of elements in layout*/
  };

  /**
  * @struct      LAYOUT_DESC
  *	\brief       Structure which contains a collection of layouts.
  */
  struct LAYOUT_DESC
  {
    /**
    * @fn void addToDesc(SEMANTIC semantic, FORMATS format, int offset, int elements)
    *	@brief Create & add a layout element to the collection.
    *	@param[in] semantic Input SEMANTIC.
    *	@param[in] format	Input FORMAT
    *	@param[in] offset	int with value for the layout offset
    *	@param[in] elements	int with value for the amount of elements in layout
    */
    void addToDesc(SEMANTIC::E semantic, FORMATS::E format, int32 offset, int32 elements)
    {
      LAYOUT L(semantic, format, offset, elements);
      v_Layout.push_back(L);
    }

    Vector<LAYOUT>v_Layout;	/**< vector of LAYOUT's */
  };

  /**
  * @class       InputLayout
  *	\brief       Class which serves as an interface for both CDXInputLayout & COGLInputLayout
  *
  *	             Contains only a constructor and virtual destructor
  */
  class InputLayout
  {
   public:
    InputLayout() = default;
    virtual ~InputLayout() = default;
  };
}