#include <QtWidgets/QDialog>

#include <Common/Widgets/FilenameLineEdit.hpp>

struct AptDatExtractDialog : QDialog
{
  AptDatExtractDialog (QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
  virtual ~AptDatExtractDialog ();
  
  static bool extract (QString dst, QString src);
  
protected:
  void createGui();
  
  FilenameLineEdit *m_xp_path;
};
