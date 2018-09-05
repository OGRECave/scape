#include "ExportImageDialog.h"

#include <QFileDialog>

#include <OgreString.h>

ExportImageDialog::ExportImageDialog(QWidget* parent) : ImageFileDialog(parent) { setupDialog(); }

ExportImageDialog::~ExportImageDialog() {}

void ExportImageDialog::populate(const FormatItemVector& formatItemVector)
{
    ImageFileDialog::populate(formatItemVector);
}

void ExportImageDialog::selectFileButtonClicked()
{
    std::string formatName =
        mFormatItemVector[mImageFileDialogUI->formatComboBox->currentIndex()].formatName;
    std::string formatExtensions =
        mFormatItemVector[mImageFileDialogUI->formatComboBox->currentIndex()].formatExtensions;

    Ogre::String formatExtensionsEdit = Ogre::String(formatExtensions);
    formatExtensionsEdit = Ogre::StringUtil::replaceAll(formatExtensionsEdit, ";", " ");
    Ogre::StringUtil::toLowerCase(formatExtensionsEdit);

    Ogre::String format =
        Ogre::String(formatName) + " (" + formatExtensionsEdit + ")" + ";;All files (*.*)";

    QString fileName =
        QFileDialog::getSaveFileName(this, QString(), QDir::currentPath(), QString(format.c_str()),
                                     Q_NULLPTR, QFileDialog::DontUseNativeDialog);
    if (!fileName.isEmpty())
    {
        mImageFileDialogUI->fileLineEdit->setText(fileName);
    }
}

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
