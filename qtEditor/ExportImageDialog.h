#ifndef EXPORTIMAGEDIALOG_H
#define EXPORTIMAGEDIALOG_H

#include <string>
#include <vector>
#include <QDialog>
#include <QWidget>
#include "ui_imagefiledialog.h"

class ExportImageDialog : public QDialog
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

    ExportImageDialog(QWidget* parent = NULL);
    virtual ~ExportImageDialog();

    void populate(const FileExportItemVector& fileExportItemList);

    std::string getSelectedFormatName() const;
    bool getFlipX() const;
    bool getFlipY() const;
    std::string getFilePath() const;

private slots:
    void selectedFormatChanged(int index);
    void selectFileButtonClicked();

private:
    void connectActions();

    FileExportItemVector mFileExportItemVector;
    Ui::ImageFileDialog* mImageFileDialogUI;
};

#endif // EXPORTIMAGEDIALOG_H
