#include "annotationview.h"
#include <QTextEdit>
#include <QLayout>
#include "dataobjecthandover.h"
#include "viewdata.h"
#include "viewdatahandover.h"

AnnotationView::AnnotationView (QWidget *parent)
    : WinSubmenu {parent}
{
    setHeader("Zuätzliche Notizen");
    mTextEditor = new QTextEdit (this);
    mTextEditor->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    layout()->addWidget(mTextEditor);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);

    connect (mTextEditor, SIGNAL (textChanged()), this, SLOT(onTextChanged()));
}

void AnnotationView::onTextChanged ()
{
    ViewDataHandover *dataHandover = new ViewDataHandover ();
    dataHandover->setAnnotation(mTextEditor->toPlainText());
    getViewData()->setData (dataHandover);
}
