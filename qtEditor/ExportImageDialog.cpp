#include "ExportImageDialog.h"

#include <QFileDialog>

#include <OgreString.h>

ExportImageDialog::ExportImageDialog(QWidget* parent) : QDialog(parent)
{
    mImageFileDialogUI = new Ui::ImageFileDialog();
    mImageFileDialogUI->setupUi(this);

    connectActions();
}

ExportImageDialog::~ExportImageDialog() { delete mImageFileDialogUI; }

void ExportImageDialog::populate(const ExportImageDialog::FileExportItemVector& fileExportItemVector)
{
    mImageFileDialogUI->formatComboBox->clear();

    mFileExportItemVector = fileExportItemVector;

    for (FileExportItemVector::const_iterator it = mFileExportItemVector.begin();
         it != mFileExportItemVector.end(); it++)
    {
        mImageFileDialogUI->formatComboBox->addItem(QString(it->label.c_str()));
    }
}

std::string ExportImageDialog::getSelectedFormatName() const
{
    return mFileExportItemVector[mImageFileDialogUI->formatComboBox->currentIndex()].name;
}

bool ExportImageDialog::getFlipX() const { return mImageFileDialogUI->flipxCheckBox->isChecked(); }

bool ExportImageDialog::getFlipY() const { return mImageFileDialogUI->flipyCheckBox->isChecked(); }

std::string ExportImageDialog::getFilePath() const
{
    return mImageFileDialogUI->fileLineEdit->text().toStdString();
}

void ExportImageDialog::selectedFormatChanged(int index)
{
    mImageFileDialogUI->formatDescriptionTextEdit->setPlainText(
        QString(mFileExportItemVector[index].description.c_str()));
}

void ExportImageDialog::selectFileButtonClicked()
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

void ExportImageDialog::connectActions()
{
    connect(mImageFileDialogUI->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(mImageFileDialogUI->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(mImageFileDialogUI->formatComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(selectedFormatChanged(int)));
    connect(mImageFileDialogUI->openFilePushButton, SIGNAL(clicked()), this,
            SLOT(selectFileButtonClicked()));
}
