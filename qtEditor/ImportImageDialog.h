#ifndef IMPORTIMAGEDIALOG_H
#define IMPORTIMAGEDIALOG_H

#include <map>
#include "ImageFileDialog.h"

class ImportImageDialog : public ImageFileDialog
{
    Q_OBJECT

public:
    struct FormatOptionItem
    {
        std::string name;
        std::string type;
        std::string label;
        std::string description;
        std::string options;
    };

    typedef std::map<std::string, std::vector<FormatOptionItem>> FormatOptionItemMap;

    ImportImageDialog(QWidget* parent = NULL);
    virtual ~ImportImageDialog();

    virtual std::string getSelectedFormatName() const;

    virtual void populate(const FormatItemVector& formatItemVector,
                          const FormatOptionItemMap& formatOptionItemMap);

protected:
    virtual void selectedFormatChanged(int index);
    virtual void selectFileButtonClicked();
    virtual void fileLineEditTextChanged(const QString& text);
    virtual void accept();

private:
    FormatOptionItemMap mFormatOptionItemMap;

    void setupDialog();
};

#endif // IMPORTIMAGEDIALOG_H
