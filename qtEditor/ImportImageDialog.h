#ifndef IMPORTIMAGEDIALOG_H
#define IMPORTIMAGEDIALOG_H

#include "ImageFileDialog.h"

class ImportImageDialog : public ImageFileDialog
{
    Q_OBJECT

public:
    ImportImageDialog(QWidget* parent = NULL);
    virtual ~ImportImageDialog();

    virtual void populate(const FormatItemVector& formatItemVector);

protected:
    virtual void selectedFormatChanged(int index);
    virtual void selectFileButtonClicked();
    virtual void fileLineEditTextChanged(const QString& text);
    virtual void accept();

private:
    void setupDialog();
};

#endif // IMPORTIMAGEDIALOG_H
