#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include <Common/Widgets/OpenGL/Screen.hpp>
#include <Common/Widgets/OpenGL/Obj8Scene.hpp>

#include "InsertObjectConfirmationDialog.hpp"

InsertObjectConfirmationDialog::InsertObjectConfirmationDialog(
  Obj8::FilePointer file,
  QWidget *parent,
  Qt::WindowFlags flags
)
: QDialog(parent, flags)
{
  OpenGL::Screen *preview(new OpenGL::Screen(OpenGL::ScenePointer(new OpenGL::Obj8Scene)));
  qSharedPointerDynamicCast<OpenGL::Obj8Scene>(preview->scene())->insertModel(file);
  
  QPushButton *accept(new QPushButton("accept"));
  QPushButton *reject(new QPushButton("reject"));
  
  connect(
    accept, &QAbstractButton::released,
    this, &QDialog::accept
  );
  connect(
    reject, &QAbstractButton::released,
    this, &QDialog::reject
  );
  
  QBoxLayout *layout(new QVBoxLayout());
  QBoxLayout *buttons(new QHBoxLayout());
  
  layout->addWidget(preview);
  
  buttons->addWidget(accept);
  buttons->addWidget(reject);
  layout->addLayout(buttons);
  
  setLayout(layout);
}

InsertObjectConfirmationDialog::~InsertObjectConfirmationDialog()
{
  qDebug("Passing ~InsertObjectConfirmationDialog()");
}
