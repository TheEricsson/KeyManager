#ifndef CODEGENERATORVIEW_H
#define CODEGENERATORVIEW_H

#include "winsubmenu.h"
#include "QZXing.h"

class CodeGeneratorView : public WinSubmenu
{
    Q_OBJECT

    public:
        explicit CodeGeneratorView(QWidget *parent = nullptr);
    private slots:
        virtual void onMenuBtnClicked (Gui::MenuButton btnType);
        void onNumberOfCodesChanged(QString text);
        void onprintHeightChanged(QString value);
        void onprintWidthChanged(QString value);
        void onBorderCheckStateChanged(Qt::CheckState value);
        void onFoldableCheckStateChanged(Qt::CheckState value);
        void onOpenPdfCheckStateChanged(Qt::CheckState value);

    private:
        bool checkValues();
        void generatePDF();
        QZXing decoder;
        QString mNumberOfCodes;

        QString mPrintHeight;
        QString mPrintWidth;
        Qt::CheckState mBorderCheckState;
        Qt::CheckState mFoldableCheckState;

        bool mShowPdfFile;
};

#endif // CODEGENERATORVIEW_H
