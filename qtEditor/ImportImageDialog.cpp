#include "ImportImageDialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <OgreString.h>
#include <OgreStringConverter.h>

ImportImageDialog::ImportImageDialog(QWidget* parent) : ImageFileDialog(parent) { setupDialog(); }

ImportImageDialog::~ImportImageDialog() {}

void ImportImageDialog::populate(const FormatItemVector& formatItemVector)
{
    ImageFileDialog::populate(formatItemVector);
    mImageFileDialogUI->formatComboBox->insertItem(0, QString("Autodetect"));
    mImageFileDialogUI->formatComboBox->setCurrentIndex(0);
}

void ImportImageDialog::selectedFormatChanged(int index)
{
    if (index == 0)
    {
        mImageFileDialogUI->formatDescriptionTextEdit->setPlainText(QString("Autodetect by extension"));
    }
    else
    {
        ImageFileDialog::selectedFormatChanged(index - 1);
    }
}

void ImportImageDialog::fileLineEditTextChanged(const QString& text)
{
    if (mImageFileDialogUI->formatComboBox->currentIndex() == 0)
    {
        int detIndex = getFormatIndexFromExtension(mImageFileDialogUI->fileLineEdit->text().toStdString());
        mImageFileDialogUI->formatComboBox->setCurrentIndex(detIndex + 1);
    }
}

void ImportImageDialog::accept()
{
    if (mImageFileDialogUI->formatComboBox->currentIndex() == 0)
    {
        int detIndex = getFormatIndexFromExtension(mImageFileDialogUI->fileLineEdit->text().toStdString());
        mImageFileDialogUI->formatComboBox->setCurrentIndex(detIndex + 1);
        if (detIndex == -1)
        {
            QMessageBox::warning(this, "Import failed!",
                                 QString("Failed to autodetect file type!\nPlease select manually!"));
        }
    }
    else
    {
        ImageFileDialog::accept();
    }
}

void ImportImageDialog::setupDialog() { mImageFileDialogUI->buttonBox->addButton(QDialogButtonBox::Open); }
