#include "ImportImageDialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <OgreString.h>
#include <OgreStringConverter.h>

ImportImageDialog::ImportImageDialog(QWidget* parent) : ImageFileDialog(parent) { setupDialog(); }

ImportImageDialog::~ImportImageDialog() {}

void ImportImageDialog::populate(const FormatItemVector& formatItemVector,
                                 const FormatOptionItemMap& formatOptionItemMap)
{
    mFormatOptionItemMap = formatOptionItemMap;
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

void ImportImageDialog::selectFileButtonClicked()
{
    int index = mImageFileDialogUI->formatComboBox->currentIndex();

    std::string formatName;
    std::string formatExtensions;
    if (index == 0)
    {
        formatName = "Importable files";
        formatExtensions = "";
        for (FormatItemVector::const_iterator it = mFormatItemVector.begin(); it != mFormatItemVector.end();
             it++)
        {
            Ogre::String fileFilterString = it->formatExtensions;
            std::vector<Ogre::String> fileFilters = Ogre::StringUtil::split(fileFilterString, ";");
            for (std::vector<Ogre::String>::const_iterator fileFilterIt = fileFilters.begin();
                 fileFilterIt != fileFilters.end(); fileFilterIt++)
            {
                formatExtensions =
                    formatExtensions + (formatExtensions.length() == 0 ? "" : " ") + *fileFilterIt;
            }
        }
    }
    else
    {
        formatName = mFormatItemVector[index - 1].formatName;
        formatExtensions = mFormatItemVector[index - 1].formatExtensions;
    }

    Ogre::String formatExtensionsEdit = Ogre::String(formatExtensions);
    formatExtensionsEdit = Ogre::StringUtil::replaceAll(formatExtensionsEdit, ";", " ");
    Ogre::StringUtil::toLowerCase(formatExtensionsEdit);

    Ogre::String format =
        Ogre::String(formatName) + " (" + formatExtensionsEdit + ")" + ";;All files (*.*)";

    QString fileName =
        QFileDialog::getOpenFileName(this, QString(), QDir::currentPath(), QString(format.c_str()),
                                     Q_NULLPTR, QFileDialog::DontUseNativeDialog);
    if (!fileName.isEmpty())
    {
        mImageFileDialogUI->fileLineEdit->setText(fileName);
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
