#include "ImageFileDialog.h"

class ExportImageDialog : public ImageFileDialog
{
    Q_OBJECT

public:
    ExportImageDialog(QWidget* parent = NULL);
    virtual ~ExportImageDialog();
};
