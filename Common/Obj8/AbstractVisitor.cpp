#include <QtCore/QDir>

#include "AbstractVisitor.hpp"



Obj8::AbstractVisitor::AbstractVisitor (QString filename)
: m_texture_file_extensions ()
, m_filename (filename)
{
  m_texture_file_extensions.append ("png");
  m_texture_file_extensions.append ("dds");
}



Obj8::AbstractVisitor::~AbstractVisitor ()
{
}



Obj8::StringRef
Obj8::AbstractVisitor::filename () const
{
  return (m_filename);
}



void
Obj8::AbstractVisitor::setFilename (StringRef filename)
{
  m_filename = filename;
}



QStringList
Obj8::AbstractVisitor::usableTexturePaths (StringRef img_path) const
{
  QStringList retval;
  
  QDir objdir(filename());
  objdir.cdUp();
  
  String relative_img_path = textureFilenamePrefix (img_path);
  QDir imgdir(objdir.absoluteFilePath(relative_img_path));
  imgdir.cdUp();
  
  QFileInfo img (relative_img_path);
  QString img_base = img.fileName();
  
  QStringList namefilters = QDir::nameFiltersFromString (img_base + "*");
  QStringList files = imgdir.entryList (namefilters);
  
  Q_FOREACH (QString file, files) {
    Q_FOREACH (QString extension, m_texture_file_extensions) {
      if (file.toLower().endsWith(extension)) {
        retval.append (imgdir.absoluteFilePath(file));
      }
    }
  }
  
  return (retval);
}



Obj8::String
Obj8::AbstractVisitor::textureFilenamePrefix (StringRef texture_path) const
{
  QString lowered = texture_path.toLower ();
  
  Q_FOREACH (QString extension, m_texture_file_extensions) {
    if (lowered.endsWith(extension)) {
      return (texture_path.mid(0, texture_path.length() - extension.length()));
    }
  }
  
  return (texture_path);
}



void Obj8::AbstractVisitor::visit (Command::Action::SmokeBlack *)
{
}



void Obj8::AbstractVisitor::visit (Command::Action::SmokeWhite *)
{
}



void Obj8::AbstractVisitor::visit (Command::Geometry::LightCustom *)
{
}



void Obj8::AbstractVisitor::visit (Command::Geometry::LightNamed *)
{
}



void Obj8::AbstractVisitor::visit (Command::Geometry::LightParam *)
{
}



void Obj8::AbstractVisitor::visit (Command::Geometry::LightSpillCustom *)
{
}



void Obj8::AbstractVisitor::visit (Command::Geometry::Lines *)
{
}



void Obj8::AbstractVisitor::visit (Command::Geometry::Lights *)
{
}



void Obj8::AbstractVisitor::visit (Command::Geometry::Triangles *)
{
}



void Obj8::AbstractVisitor::visit (Command::State::Draped *)
{
}



void Obj8::AbstractVisitor::visit (Command::State::Lod *)
{
}



void Obj8::AbstractVisitor::visit (Command::State::NoDraped *)
{
}



void Obj8::AbstractVisitor::visit (Data::Index *)
{
}



void Obj8::AbstractVisitor::visit (Data::Index10 *)
{
}



void Obj8::AbstractVisitor::visit (Data::LightVertex *)
{
}



void Obj8::AbstractVisitor::visit (Data::LineVertex *)
{
}



void Obj8::AbstractVisitor::visit (Data::Vertex *)
{
}



void Obj8::AbstractVisitor::visit (Global::PointsCounts *)
{
}



void Obj8::AbstractVisitor::visit (Global::Texture *)
{
}



void Obj8::AbstractVisitor::visit (Global::TextureDraped *)
{
}


void Obj8::AbstractVisitor::visit (Global::TextureLit *)
{
}


void Obj8::AbstractVisitor::visit (Global::TextureNormal *)
{
}
