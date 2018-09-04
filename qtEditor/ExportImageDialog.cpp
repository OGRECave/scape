#include "ExportImageDialog.h"

ExportImageDialog::ExportImageDialog(QWidget* parent) : ImageFileDialog(parent) { setupDialog(); }

ExportImageDialog::~ExportImageDialog() {}

void ExportImageDialog::setupDialog()
{
    mImageFileDialogUI->widthLabel->hide();
    mImageFileDialogUI->widthLineEdit->hide();
    mImageFileDialogUI->heightLabel->hide();
    mImageFileDialogUI->heightLineEdit->hide();
    mImageFileDialogUI->bppLabel->hide();
    mImageFileDialogUI->bppComboBox->hide();
    mImageFileDialogUI->endiannessLabel->hide();
    mImageFileDialogUI->endiannessComboBox->hide();

    mImageFileDialogUI->buttonBox->addButton(QDialogButtonBox::Save);
}
