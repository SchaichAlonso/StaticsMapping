#include "AbstractHard.hpp"

const MappingElement<Obj8::AbstractHard::Surface>
Obj8::AbstractHard::g_surfaces[] = {
  MAPPING_ELEMENT(Obj8::AbstractHard, Default, Obj8::String()),
  
  MAPPING_ELEMENT(Obj8::AbstractHard, Water, "water"),
  MAPPING_ELEMENT(Obj8::AbstractHard, Concrete, "concrete"),
  MAPPING_ELEMENT(Obj8::AbstractHard, Asphalt, "asphalt"),
  MAPPING_ELEMENT(Obj8::AbstractHard, Grass, "grass"),
  MAPPING_ELEMENT(Obj8::AbstractHard, Dirt, "dirt"),
  MAPPING_ELEMENT(Obj8::AbstractHard, Gravel, "gravel"),
  MAPPING_ELEMENT(Obj8::AbstractHard, Lakebed, "lakebed"),
  MAPPING_ELEMENT(Obj8::AbstractHard, Snow, "snow"),
  MAPPING_ELEMENT(Obj8::AbstractHard, Shoulder, "shoulder"),
  MAPPING_ELEMENT(Obj8::AbstractHard, Blastpad, "blastpad")
};



Obj8::AbstractHard::AbstractHard ()
  : Record ()
  , m_surface ()
{
}



Obj8::AbstractHard::AbstractHard (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_surface ()
{
  Parser::LexerContext backup (ctx);
  
  try {
    m_surface = Parameter::Word (str, ctx);
  } catch (const Parser::SyntaxError &) {
    *ctx = backup;
  }
  surface ();
}



Obj8::AbstractHard::~AbstractHard ()
{
}



void
Obj8::AbstractHard::accept (AbstractVisitor *, bool)
{
}



Obj8::String
Obj8::AbstractHard::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_surface.toString ();
  
  return (retval);
}



Obj8::AbstractHard::Surface
Obj8::AbstractHard::surface () const
{
  Surface retval;
  bool ok;
  
  ok = string_to_name (g_surfaces, &retval, m_surface.value());
  if (not ok) {
    m_surface.reject ();
  }
  
  return (retval);
}



void
Obj8::AbstractHard::setSurface (Surface surface)
{
  QString value;
  bool ok;
  
  ok = name_to_string (g_surfaces, &value, surface);
  Q_ASSERT (ok);
  
  if (m_surface.indentation().isEmpty()) {
    m_surface.setIndentation(" ");
  }
  
  m_surface.setValue (value);
}
