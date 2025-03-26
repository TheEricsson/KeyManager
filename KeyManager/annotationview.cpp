#include "annotationview.h"
#include <QTextEdit>
#include <QLayout>
#include "dataobjecthandover.h"

AnnotationView::AnnotationView (QWidget *parent)
    : WinSubmenu {parent}
{
    setHeader("Zuätzliche Notizen");
    mTextEditor = new QTextEdit (this);
    mTextEditor->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    layout()->addWidget(mTextEditor);
    setMenuButtons(UiSpecs::eMenuButton::BackButton, UiSpecs::eMenuButton::NextButton);

    connect (mTextEditor, SIGNAL (textChanged()), this, SLOT(onTextChanged()));
}

void AnnotationView::onTextChanged ()
{
    if (!mDataObject)
        return;

    DataObjectHandover *dataObj = (DataObjectHandover*)mDataObject;
    if (dataObj)
    {
        dataObj->setAnnotation(mTextEditor->toPlainText());
    }
}
