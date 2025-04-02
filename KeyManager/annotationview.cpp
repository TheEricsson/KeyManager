#include "annotationview.h"
#include <QTextEdit>
#include <QLayout>
#include "dataobjecthandover.h"
#include "datainterface.h"
#include "viewdataannotation.h"

AnnotationView::AnnotationView (QWidget *parent)
    : WinSubmenu {parent}
{
    mDataAnnotation = 0;

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

void AnnotationView::showEvent(QShowEvent *)
{
    reset ();
}

void AnnotationView::reset()
{
    if (mDataAnnotation)
    {
        delete mDataAnnotation;
        mDataAnnotation = new ViewDataAnnotation ();
        dataInterface()->setData(mDataAnnotation);
        mTextEditor->setText("");
    }
}

void AnnotationView::onTextChanged ()
{
    if (!mDataAnnotation)
    {
        mDataAnnotation = new ViewDataAnnotation ();
        dataInterface()->setData(mDataAnnotation);
    }
    mDataAnnotation->setAnnotation(mTextEditor->toPlainText());
}

AnnotationView::~AnnotationView()
{
    // if (mDataAnnotation)
    // {
    //     delete mDataAnnotation;
    //     mDataAnnotation = 0;
    // }
}
