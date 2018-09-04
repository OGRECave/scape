#include "ImageFileDialog.h"

#include <QFileDialog>

#include <OgreString.h>
#include <OgreStringConverter.h>

ImageFileDialog::ImageFileDialog(QWidget* parent) : QDialog(parent)
{
    mImageFileDialogUI = new Ui::ImageFileDialog();
    mImageFileDialogUI->setupUi(this);

    connectActions();
}

ImageFileDialog::~ImageFileDialog() { delete mImageFileDialogUI; }

void ImageFileDialog::populate(const ImageFileDialog::FileExportItemVector& fileExportItemVector)
{
    mImageFileDialogUI->formatComboBox->clear();

    mFileExportItemVector = fileExportItemVector;

    for (FileExportItemVector::const_iterator it = mFileExportItemVector.begin();
         it != mFileExportItemVector.end(); it++)
    {
        mImageFileDialogUI->formatComboBox->addItem(QString(it->label.c_str()));
    }
}

std::string ImageFileDialog::getSelectedFormatName() const
{
    return mFileExportItemVector[mImageFileDialogUI->formatComboBox->currentIndex()].name;
}

bool ImageFileDialog::getFlipX() const { return mImageFileDialogUI->flipxCheckBox->isChecked(); }

bool ImageFileDialog::getFlipY() const { return mImageFileDialogUI->flipyCheckBox->isChecked(); }

std::string ImageFileDialog::getFilePath() const
{
    return mImageFileDialogUI->fileLineEdit->text().toStdString();
}

int ImageFileDialog::getWidth() const
{
    if (mImageFileDialogUI->widthLineEdit->isEnabled())
    {
        const Ogre::String widthText =
            Ogre::String(mImageFileDialogUI->widthLineEdit->text().toStdString());
        return Ogre::StringConverter::parseInt(widthText, -1);
    }
    return -1;
}

int ImageFileDialog::getHeight() const
{
    if (mImageFileDialogUI->heightLineEdit->isEnabled())
    {
        const Ogre::String heightText =
            Ogre::String(mImageFileDialogUI->heightLineEdit->text().toStdString());
        return Ogre::StringConverter::parseInt(heightText, -1);
    }
    return -1;
}

int ImageFileDialog::getEndiannessIndex() const
{
    if (mImageFileDialogUI->endiannessComboBox->isEnabled())
    {
        return mImageFileDialogUI->endiannessComboBox->currentIndex();
    }
    return -1;
}

int ImageFileDialog::getBppIndex() const
{
    if (mImageFileDialogUI->bppComboBox->isEnabled())
    {
        return mImageFileDialogUI->bppComboBox->currentIndex();
    }
    return -1;
}

void ImageFileDialog::selectedFormatChanged(int index)
{
    mImageFileDialogUI->formatDescriptionTextEdit->setPlainText(
        QString(mFileExportItemVector[index].description.c_str()));
}

void ImageFileDialog::selectFileButtonClicked()
{
    std::string formatName =
        mFileExportItemVector[mImageFileDialogUI->formatComboBox->currentIndex()].formatName;
    std::string formatExtensions =
        mFileExportItemVector[mImageFileDialogUI->formatComboBox->currentIndex()].formatExtensions;

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

void ImageFileDialog::connectActions()
{
    connect(mImageFileDialogUI->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(mImageFileDialogUI->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(mImageFileDialogUI->formatComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(selectedFormatChanged(int)));
    connect(mImageFileDialogUI->openFilePushButton, SIGNAL(clicked()), this,
            SLOT(selectFileButtonClicked()));
}
