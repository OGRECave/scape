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
    mImageFileDialogUI->flipxCheckBox->setEnabled(false);
    mImageFileDialogUI->flipyCheckBox->setEnabled(false);
    mImageFileDialogUI->widthLabel->setEnabled(false);
    mImageFileDialogUI->widthLineEdit->setEnabled(false);
    mImageFileDialogUI->heightLabel->setEnabled(false);
    mImageFileDialogUI->heightLineEdit->setEnabled(false);
    mImageFileDialogUI->bppLabel->setEnabled(false);
    mImageFileDialogUI->bppComboBox->setEnabled(false);
    mImageFileDialogUI->bppComboBox->clear();
    mImageFileDialogUI->endiannessLabel->setEnabled(false);
    mImageFileDialogUI->endiannessComboBox->setEnabled(false);
    mImageFileDialogUI->endiannessComboBox->clear();

    if (index == 0)
    {
        mImageFileDialogUI->formatDescriptionTextEdit->setPlainText(QString("Autodetect by extension"));
        mImageFileDialogUI->flipxCheckBox->setEnabled(true);
        mImageFileDialogUI->flipyCheckBox->setEnabled(true);
    }
    else
    {
        ImageFileDialog::selectedFormatChanged(index - 1);

        const std::vector<FormatOptionItem>& options =
            mFormatOptionItemMap[mFormatItemVector[index - 1].name];
        for (std::vector<FormatOptionItem>::const_iterator it = options.begin(); it != options.end(); it++)
        {
            std::vector<std::string> splitOptions = Ogre::StringUtil::split(it->options, ";");
            if (it->name == "FLIPX")
            {
                mImageFileDialogUI->flipxCheckBox->setEnabled(true);
            }
            else if (it->name == "FLIPY")
            {
                mImageFileDialogUI->flipyCheckBox->setEnabled(true);
            }
            else if (it->name == "WIDTH")
            {
                mImageFileDialogUI->widthLabel->setEnabled(true);
                mImageFileDialogUI->widthLineEdit->setEnabled(true);
            }
            else if (it->name == "HEIGHT")
            {
                mImageFileDialogUI->heightLabel->setEnabled(true);
                mImageFileDialogUI->heightLineEdit->setEnabled(true);
            }
            else if (it->name == "BPP")
            {
                mImageFileDialogUI->bppLabel->setEnabled(true);
                mImageFileDialogUI->bppComboBox->setEnabled(true);
                for (std::vector<std::string>::const_iterator opIt = splitOptions.begin();
                     opIt != splitOptions.end(); opIt++)
                {
                    mImageFileDialogUI->bppComboBox->addItem(QString(opIt->c_str()));
                }
            }
            else if (it->name == "BIGENDIAN")
            {
                mImageFileDialogUI->endiannessLabel->setEnabled(true);
                mImageFileDialogUI->endiannessComboBox->setEnabled(true);
                for (std::vector<std::string>::const_iterator opIt = splitOptions.begin();
                     opIt != splitOptions.end(); opIt++)
                {
                    mImageFileDialogUI->endiannessComboBox->addItem(QString("Little Endian (Intel)"));
                    mImageFileDialogUI->endiannessComboBox->addItem(QString("Big Endian (MAC)"));
                }
            }
        }
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
