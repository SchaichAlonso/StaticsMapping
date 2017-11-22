#pragma once

#include <Obj8/Obj8.hpp>
#include <Obj8/AbstractVertex.hpp>
#include <Obj8/AbstractVisitor.hpp>
#include <Obj8/Parser/Number.hpp>
#include <Classification/Object.hpp>

struct ObjFileInstaller : Obj8::AbstractVisitor
{
  ObjFileInstaller ();
  virtual ~ObjFileInstaller ();
  
  void            setMaxTextureResolution (int);
  void            setTranslation (const Obj8::Parser::Number &, const Obj8::Parser::Number &, const Obj8::Parser::Number &);
  void            setRotation (Classification::Object::Rotate);
  void            setTargetFilename (Obj8::StringRef);
  Obj8::StringRef targetFilename () const;
  
  virtual void visit (Obj8::Command::Action::SmokeBlack *) Q_DECL_OVERRIDE;
  virtual void visit (Obj8::Command::Action::SmokeWhite *) Q_DECL_OVERRIDE;
  
  virtual void visit (Obj8::Command::Geometry::LightCustom *) Q_DECL_OVERRIDE;
  virtual void visit (Obj8::Command::Geometry::LightNamed *) Q_DECL_OVERRIDE;
  virtual void visit (Obj8::Command::Geometry::LightParam *) Q_DECL_OVERRIDE;
  virtual void visit (Obj8::Command::Geometry::LightSpillCustom *) Q_DECL_OVERRIDE;
  
  virtual void visit (Obj8::Data::LightVertex *) Q_DECL_OVERRIDE;
  virtual void visit (Obj8::Data::LineVertex *) Q_DECL_OVERRIDE;
  virtual void visit (Obj8::Data::Vertex *) Q_DECL_OVERRIDE;
  
  virtual void visit (Obj8::Global::Texture *) Q_DECL_OVERRIDE;
  virtual void visit (Obj8::Global::TextureDraped *) Q_DECL_OVERRIDE;
  virtual void visit (Obj8::Global::TextureLit *) Q_DECL_OVERRIDE;
  virtual void visit (Obj8::Global::TextureNormal *) Q_DECL_OVERRIDE;
  
  virtual void reset () Q_DECL_OVERRIDE;
  virtual bool satisfied (Obj8::StringRef) Q_DECL_OVERRIDE;
  
protected:
  void applyTransformation (Obj8::Parser::Number *, Obj8::Parser::Number *, Obj8::Parser::Number *) const;
  void installTexture (Obj8::String, Obj8::AbstractTexture *);
  bool installTextureImpl (Obj8::AbstractTexture *, Obj8::String, Obj8::String);
  
  Classification::Object::Rotate m_rotate;
  Obj8::Parser::Number m_translation[3];
  Obj8::String         m_target_filename;
  int                  m_texture_limit;
};
