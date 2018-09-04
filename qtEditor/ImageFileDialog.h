#ifndef IMAGEFILEDIALOG_H
#define IMAGEFILEDIALOG_H

#include <string>
#include <vector>
#include <QDialog>
#include <QWidget>
#include "ui_imagefiledialog.h"

class ImageFileDialog : public QDialog
{
    Q_OBJECT

public:
    struct FileExportItem
    {
        std::string name;
        std::string label;
        std::string description;
        std::string formatName;
        std::string formatExtensions;
    };

    typedef std::vector<FileExportItem> FileExportItemVector;

    ImageFileDialog(QWidget* parent = NULL);
    virtual ~ImageFileDialog();

    void populate(const FileExportItemVector& fileExportItemList);

    std::string getSelectedFormatName() const;
    bool getFlipX() const;
    bool getFlipY() const;
    std::string getFilePath() const;

protected:
    FileExportItemVector mFileExportItemVector;
    Ui::ImageFileDialog* mImageFileDialogUI;

private slots:
    void selectedFormatChanged(int index);
    void selectFileButtonClicked();

private:
    void connectActions();
};

#endif // IMAGEFILEDIALOG_H
