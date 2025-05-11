#ifndef ANNOTATIONVIEW_H
#define ANNOTATIONVIEW_H

#include <QWidget>
#include "winsubmenu.h"

class ViewDataAnnotation;
class QTextEdit;

class AnnotationView : public WinSubmenu
{
    Q_OBJECT
public:
    explicit AnnotationView(QWidget *parent = nullptr);
    ~AnnotationView();

private slots:
    void onTextChanged ();
private:
    virtual void showEvent(QShowEvent *);
    void reset ();
    QTextEdit *mTextEditor;
    ViewDataAnnotation *mDataAnnotation;
};

#endif // ANNOTATIONVIEW_H
