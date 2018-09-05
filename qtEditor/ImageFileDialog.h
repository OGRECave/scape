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
    struct FormatItem
    {
        std::string name;
        std::string label;
        std::string description;
        std::string formatName;
        std::string formatExtensions;
    };

    typedef std::vector<FormatItem> FormatItemVector;

    ImageFileDialog(QWidget* parent = NULL);
    virtual ~ImageFileDialog();

    void populate(const FormatItemVector& formatItemVector);

    std::string getSelectedFormatName() const;
    bool getFlipX() const;
    bool getFlipY() const;
    std::string getFilePath() const;
    int getWidth() const;
    int getHeight() const;
    int getEndiannessIndex() const;
    int getBppIndex() const;

protected:
    FormatItemVector mFormatItemVector;
    Ui::ImageFileDialog* mImageFileDialogUI;

protected slots:
    virtual void selectedFormatChanged(int index);
    virtual void selectFileButtonClicked();
    virtual void fileLineEditTextChanged(const QString & text);

private:
    void connectActions();
};

#endif // IMAGEFILEDIALOG_H
