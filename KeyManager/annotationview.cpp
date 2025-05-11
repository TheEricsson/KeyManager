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

    //setHeader("Zuätzliche Notizen");
    mTextEditor = new QTextEdit (this);
    mTextEditor->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

    QHBoxLayout *centralLayout = new QHBoxLayout();
    centralLayout->addWidget(mTextEditor);
    setCentralLayout(centralLayout);

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
    dataInterface()->resetAnnotationData();
    mTextEditor->setText("");
    update();
}

void AnnotationView::onTextChanged ()
{
    dataInterface()->setAnnotationText(mTextEditor->toPlainText());
    update();
}

AnnotationView::~AnnotationView()
{
    // if (mDataAnnotation)
    // {
    //     delete mDataAnnotation;
    //     mDataAnnotation = 0;
    // }
}
