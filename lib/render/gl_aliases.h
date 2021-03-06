// *************************************************************
// File:    gl_aliases.h
// Descr:   most used aliases in render lib
// Author:  Novoselov Anton @ 2017
// *************************************************************

#ifndef GC_DRAW_ALIASES_H
#define GC_DRAW_ALIASES_H

#include <vector>
#include <functional>
#include <memory>

// Names convention:
//  - prefix V_Element - std::vector<Element>
//  - prefix P_Element - std::unique_ptr<Element> (sometimes shared_ptr)
//  - prefix cElement  - const Element
//  - prefix A3_Element - std::array<Element, 3>

namespace anshub {

  // Forward declarations

  class Bitmap;
  struct Vector;
  template<class T> struct Color;
  struct GlObject;
  struct Triangle;
  struct Vertex;
  struct Face;
  struct TrigTable;
  
  // Simple aliases

  using uint  = unsigned int;
  using uchar = unsigned char;
  using byte  = unsigned char;
  using FColor = Color<float>;
  using cFColor = const Color<float>;
  using cChar = const char;
  using cTrigTable = const TrigTable;

  // Pointer aliases

  using P_Bitmap = std::shared_ptr<Bitmap>;

  // Containers aliases

  using V_Uint = std::vector<uint>;
  using V_Float = std::vector<float>;
  using V_Uchar = std::vector<uchar>;
  using V_Triangle = std::vector<Triangle>;
  using V_TrianglePtr = std::vector<Triangle*>;
  using A3_Int = std::array<int,3>;
  using A3_Vertex = std::array<Vertex,3>;
  using A3_Vector = std::array<Vector,3>;
  using A3_Float = std::array<float,3>;
  using A3_FColor = std::array<FColor,3>;
  using V_Face = std::vector<Face>;
  using VV_Face = std::vector<V_Face>;
  using V_Vertex = std::vector<Vertex>;
  using V_Vector = std::vector<Vector>;
  using V_GlObject = std::vector<GlObject>;
  using V_GlObjectP = std::vector<GlObject*>;
  using V_FColor = std::vector<Color<float>>;
  using V_Color = std::vector<Color<uchar>>;
  using V_Bitmap = std::vector<P_Bitmap>;
  using Matrix2d  = std::vector<std::vector<double>>;
  using Vector2d  = std::vector<std::vector<double>>;
  
  // Const containers aliases

  using cVertex = const Vertex;
  using cV_Vertex = const V_Vertex;
  using cV_Face = const V_Face;  
  using cV_Triangle = const V_Triangle;
  using cV_Vector = const V_Vector;
  using cV_FColor = const V_FColor;
  using cMatrix2d = const Matrix2d;
  using cVector2d = const Vector2d;

} // namespace anshub

#endif  // GC_DRAW_ALIASES_H