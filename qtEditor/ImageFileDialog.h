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

    virtual std::string getSelectedFormatName() const;
    virtual bool getFlipX() const;
    virtual bool getFlipY() const;
    virtual std::string getFilePath() const;
    virtual int getWidth() const;
    virtual int getHeight() const;
    virtual int getEndiannessIndex() const;
    virtual int getBppIndex() const;

protected:
    virtual void populate(const FormatItemVector& formatItemVector);

    virtual int getFormatIndexFromExtension(const std::string& fileName);

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
