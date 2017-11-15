#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>

struct AptDatExtractDialog : QDialog
{
  AptDatExtractDialog (QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
  virtual ~AptDatExtractDialog ();
  
  static bool extract (QString dst, QString src);
  
protected:
  void createGui();
  
  QLineEdit *m_xp_path;
};
