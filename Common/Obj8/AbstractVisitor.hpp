#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "Obj8.hpp"

namespace Obj8
{
  struct AbstractVisitor
  {
    AbstractVisitor (QString =QString());
    virtual ~AbstractVisitor ();
    
    QStringList usableTexturePaths (StringRef) const;
    String      textureFilenamePrefix (StringRef) const;
    
    StringRef filename () const;
    void      setFilename (StringRef);
    
    virtual void visit (Command::Action::SmokeBlack *);
    virtual void visit (Command::Action::SmokeWhite *);
    virtual void visit (Command::Geometry::LightCustom *);
    virtual void visit (Command::Geometry::LightNamed *);
    virtual void visit (Command::Geometry::LightParam *);
    virtual void visit (Command::Geometry::LightSpillCustom *);
    virtual void visit (Command::Geometry::Lines *);
    virtual void visit (Command::Geometry::Lights *);
    virtual void visit (Command::Geometry::Triangles *);
    virtual void visit (Command::State::Draped *);
    virtual void visit (Command::State::Lod *);
    virtual void visit (Command::State::NoDraped *);
    
    virtual void visit (Data::Index *);
    virtual void visit (Data::Index10 *);
    virtual void visit (Data::LightVertex *);
    virtual void visit (Data::LineVertex *);
    virtual void visit (Data::Vertex *);
    
    virtual void visit (Global::PointsCounts *);
    virtual void visit (Global::Texture *);
    virtual void visit (Global::TextureDraped *);
    virtual void visit (Global::TextureLit *);
    virtual void visit (Global::TextureNormal *);
    
    virtual void reset () = 0;
    virtual bool satisfied (StringRef) = 0;
    
  protected:
    
    QStringList m_texture_file_extensions;
    String      m_filename;
  };
}

