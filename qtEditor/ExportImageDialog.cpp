#include "ExportImageDialog.h"

#include <QFileDialog>

#include <OgreString.h>

ExportImageDialog::ExportImageDialog(QWidget* parent) : QDialog(parent)
{
    mExportImageDialogUI = new Ui::ExportImageDialog();
    mExportImageDialogUI->setupUi(this);

    connectActions();
}

ExportImageDialog::~ExportImageDialog() { delete mExportImageDialogUI; }

void ExportImageDialog::populate(const ExportImageDialog::FileExportItemVector& fileExportItemVector)
{
    mExportImageDialogUI->formatComboBox->clear();

    mFileExportItemVector = fileExportItemVector;

    for (FileExportItemVector::const_iterator it = mFileExportItemVector.begin();
         it != mFileExportItemVector.end(); it++)
    {
        mExportImageDialogUI->formatComboBox->addItem(QString(it->label.c_str()));
    }
}

std::string ExportImageDialog::getSelectedFormatName() const
{
    return mFileExportItemVector[mExportImageDialogUI->formatComboBox->currentIndex()].name;
}

bool ExportImageDialog::getFlipX() const { return mExportImageDialogUI->flipxCheckBox->isChecked(); }

bool ExportImageDialog::getFlipY() const { return mExportImageDialogUI->flipyCheckBox->isChecked(); }

std::string ExportImageDialog::getFilePath() const
{
    return mExportImageDialogUI->fileLineEdit->text().toStdString();
}

void ExportImageDialog::selectedFormatChanged(int index)
{
    mExportImageDialogUI->formatDescriptionTextEdit->setPlainText(
        QString(mFileExportItemVector[index].description.c_str()));
}

void ExportImageDialog::selectFileButtonClicked()
{
    std::string formatName =
        mFileExportItemVector[mExportImageDialogUI->formatComboBox->currentIndex()].formatName;
    std::string formatExtensions =
        mFileExportItemVector[mExportImageDialogUI->formatComboBox->currentIndex()].formatExtensions;

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
        mExportImageDialogUI->fileLineEdit->setText(fileName);
    }
}

void ExportImageDialog::connectActions()
{
    connect(mExportImageDialogUI->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(mExportImageDialogUI->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(mExportImageDialogUI->formatComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(selectedFormatChanged(int)));
    connect(mExportImageDialogUI->openFilePushButton, SIGNAL(clicked()), this,
            SLOT(selectFileButtonClicked()));
}
