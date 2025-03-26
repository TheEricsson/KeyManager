#ifndef ANNOTATIONVIEW_H
#define ANNOTATIONVIEW_H

#include <QWidget>
#include "winsubmenu.h"

class QTextEdit;

class AnnotationView : public WinSubmenu
{
    Q_OBJECT
public:
    AnnotationView(QWidget *parent = nullptr);

private slots:
    void onTextChanged ();
private:
    QTextEdit *mTextEditor;
};

#endif // ANNOTATIONVIEW_H
