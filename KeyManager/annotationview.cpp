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

    setHeader("Zusätzliche Notizen");
    mTextEditor = new QTextEdit (this);
    mTextEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *centralLayout = new QVBoxLayout();
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
}

void AnnotationView::onTextChanged ()
{
    //force update (android bug)
    mTextEditor->update();
    update();

    dataInterface()->setAnnotationText(mTextEditor->toPlainText());
}

AnnotationView::~AnnotationView()
{
    // if (mDataAnnotation)
    // {
    //     delete mDataAnnotation;
    //     mDataAnnotation = 0;
    // }
}
