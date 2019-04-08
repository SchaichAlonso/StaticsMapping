#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include <Common/Widgets/OpenGL/Screen.hpp>

#include "InsertObjectConfirmationDialog.hpp"

InsertObjectConfirmationDialog::InsertObjectConfirmationDialog(
  Widgets::VisualObjectPointer vop,
  QWidget *parent,
  Qt::WindowFlags flags
)
: QDialog(parent, flags)
, m_vop(vop)
{
  OpenGL::Screen *preview(new OpenGL::Screen());
#warning "!!!!"
  //preview->setModel(vop->data, vop->model);
  
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
