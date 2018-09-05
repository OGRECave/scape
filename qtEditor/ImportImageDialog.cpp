#include "ImportImageDialog.h"

ImportImageDialog::ImportImageDialog(QWidget* parent) : ImageFileDialog(parent) { setupDialog(); }

ImportImageDialog::~ImportImageDialog() {}

void ImportImageDialog::populate(const FormatItemVector& formatItemVector)
{
    ImageFileDialog::populate(formatItemVector);
}

void ImportImageDialog::setupDialog() { mImageFileDialogUI->buttonBox->addButton(QDialogButtonBox::Open); }
