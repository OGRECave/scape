/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "FileImportOptionsDialog.h"
#include "PCH/stdafx.h"

#define OPTION_SEPARATOR _T(';')
#define PROPERTY_FLIPX _T("FLIPX")
#define PROPERTY_FLIPY _T("FLIPY")
#define PROPERTY_WIDTH _T("WIDTH")
#define PROPERTY_HEIGHT _T("HEIGHT")
#define PROPERTY_BPP _T("BPP")
#define PROPERTY_BIGENDIAN _T("BIGENDIAN")

using namespace ScapeEditor;

// Required for WX
IMPLEMENT_CLASS(FileImportOptionsDialog, wxDialog)

// Required for WX
BEGIN_EVENT_TABLE(FileImportOptionsDialog, wxDialog)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
FileImportOptionsDialog::FileImportOptionsDialog(wxWindow* parentWindow, wxWindowID windowId,
                                                 const wxString& caption, const wxString& fileName,
                                                 size_t fileSize,
                                                 const FileImportOptionsDialogItemMap& itemMap,
                                                 const StringStringMap& valueMap)
    : wxDialog(parentWindow, windowId, caption, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE),
      mItemMap(itemMap), mValueMap(valueMap), mFlipXCheckBox(NULL), mFlipYCheckBox(NULL),
      mWidthTextCtrl(NULL), mHeightTextCtrl(NULL), mBigEndianButton(NULL), mDirtyValueMap(false)
{
    StringStringMap::iterator valueMapIt;

    wxArrayString integerValidatorIncludes;
    integerValidatorIncludes.push_back(_T("0"));
    integerValidatorIncludes.push_back(_T("1"));
    integerValidatorIncludes.push_back(_T("2"));
    integerValidatorIncludes.push_back(_T("3"));
    integerValidatorIncludes.push_back(_T("4"));
    integerValidatorIncludes.push_back(_T("5"));
    integerValidatorIncludes.push_back(_T("6"));
    integerValidatorIncludes.push_back(_T("7"));
    integerValidatorIncludes.push_back(_T("8"));
    integerValidatorIncludes.push_back(_T("9"));

    wxTextValidator integerValidator = wxTextValidator(wxFILTER_INCLUDE_CHAR_LIST);
    integerValidator.SetIncludes(integerValidatorIncludes);

    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* contentSizer = new wxBoxSizer(wxVERTICAL);

    // filename and size description
    string label = wxString::Format(_T("File: \"%s\""), fileName);
    if (fileSize > 0)
    {
        label = label + wxString::Format(_T("(%.2f MB / %d bytes)"), 0.0000001f * fileSize, fileSize);
    }
    contentSizer->Add(CreateTextSizer(label), wxSizerFlags().Expand().Border());

    // orientation: flip X & Y
    valueMapIt = mValueMap.find(PROPERTY_FLIPX);
    bool showFlipX = (valueMapIt != mValueMap.end());
    bool valueFlipX = showFlipX && Utils::isBoolTrue(valueMapIt->second);

    valueMapIt = mValueMap.find(PROPERTY_FLIPY);
    bool showFlipY = (valueMapIt != mValueMap.end());
    bool valueFlipY = showFlipY && Utils::isBoolTrue(valueMapIt->second);

    if (showFlipX || showFlipY)
    {
        wxStaticBoxSizer* categorySizer = new wxStaticBoxSizer(wxVERTICAL, this, _T("Orientation"));
        wxGridSizer* propertySizer = new wxGridSizer(2, 2, 10);

        if (showFlipX)
        {
            mFlipXCheckBox = new wxCheckBox(this, wxID_ANY, _T("Flip Horizontal"));
            mFlipXCheckBox->SetValue(valueFlipX);
            propertySizer->Add(mFlipXCheckBox, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
        }

        if (showFlipY)
        {
            mFlipYCheckBox = new wxCheckBox(this, wxID_ANY, _T("Flip Vertical"));
            mFlipYCheckBox->SetValue(valueFlipY);
            propertySizer->Add(mFlipYCheckBox, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
        }

        categorySizer->Add(propertySizer, wxSizerFlags().Expand().Proportion(0).Border());
        contentSizer->Add(categorySizer, wxSizerFlags().Expand().Proportion(0).Border());
    }

    // dimensions
    valueMapIt = mValueMap.find(PROPERTY_WIDTH);
    bool showWidth = (valueMapIt != mValueMap.end());
    string valueWidth = (showWidth ? valueMapIt->second : Utils::emptyString);

    valueMapIt = mValueMap.find(PROPERTY_HEIGHT);
    bool showHeight = (valueMapIt != mValueMap.end());
    string valueHeight = (showHeight ? valueMapIt->second : Utils::emptyString);

    if (showWidth || showHeight)
    {
        wxStaticBoxSizer* categorySizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _T("Dimensions"));

        if (showWidth)
        {
            categorySizer->Add(new wxStaticText(this, wxID_ANY, mItemMap[PROPERTY_WIDTH].label), 0,
                               wxALIGN_LEFT | wxRIGHT | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);

            mWidthTextCtrl = new wxTextCtrl(this, wxID_ANY, valueWidth, wxDefaultPosition, wxDefaultSize, 0,
                                            integerValidator);
            mWidthTextCtrl->SetMaxLength(5);
            categorySizer->Add(mWidthTextCtrl, 0,
                               wxALIGN_LEFT | wxRIGHT | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
        }

        if (showWidth && showHeight)
        {
            categorySizer->AddSpacer(10);
        }

        if (showHeight)
        {
            categorySizer->Add(new wxStaticText(this, wxID_ANY, mItemMap[PROPERTY_HEIGHT].label), 0,
                               wxALIGN_LEFT | wxRIGHT | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);

            mHeightTextCtrl = new wxTextCtrl(this, wxID_ANY, valueHeight, wxDefaultPosition, wxDefaultSize,
                                             0, integerValidator);
            mHeightTextCtrl->SetMaxLength(5);
            categorySizer->Add(mHeightTextCtrl, 0,
                               wxALIGN_LEFT | wxRIGHT | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
        }

        contentSizer->Add(categorySizer, wxSizerFlags().Expand().Proportion(0).Border());
    }

    // bpp
    valueMapIt = mValueMap.find(PROPERTY_BPP);
    bool showBpp = (valueMapIt != mValueMap.end());
    string valueBpp = (showBpp ? valueMapIt->second : Utils::emptyString);

    if (showBpp)
    {
        wxStaticBoxSizer* categorySizer =
            new wxStaticBoxSizer(wxHORIZONTAL, this, _T("Bits per pixel (bpp)"));

        long currentValueBpp;
        wxString(valueBpp).ToLong(&currentValueBpp);

        // const string* decoderDesc = ScapeEngine::findUIElementDesc(descs, PROPERTY_BPP);
        // assert(decoderDesc);
        wxString labelSplit = wxString(mItemMap[PROPERTY_BPP].options);
        // labelSplit = labelSplit.BeforeFirst(OPTION_SEPARATOR);
        bool firstInGroup = true;
        int bppIndex = 0;
        while (labelSplit.length())
        {
            wxString option = labelSplit.BeforeFirst(OPTION_SEPARATOR);
            labelSplit = labelSplit.AfterFirst(OPTION_SEPARATOR);

            wxRadioButton* button = new wxRadioButton(this, wxID_ANY, option, wxDefaultPosition,
                                                      wxDefaultSize, (firstInGroup ? wxRB_GROUP : 0));

            if (bppIndex == currentValueBpp)
            {
                button->SetValue(true);
            }

            mBppButtons.push_back(button);

            categorySizer->Add(button, wxSizerFlags().Expand().Center().Proportion(1).Border());
            firstInGroup = false;
            ++bppIndex;
        }

        contentSizer->Add(categorySizer, wxSizerFlags().Expand().Proportion(0).Border());
    }

    // endianness
    valueMapIt = mValueMap.find(PROPERTY_BIGENDIAN);
    bool showEndian = (valueMapIt != mValueMap.end());
    bool valueEndian = showEndian && Utils::isBoolTrue(valueMapIt->second);

    if (showEndian)
    {
        wxStaticBoxSizer* categorySizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _T("Endianness"));

        wxRadioButton* button = new wxRadioButton(this, wxID_ANY, _T("Little Endian (Intel)"),
                                                  wxDefaultPosition, wxDefaultSize, wxRB_GROUP);

        categorySizer->Add(button, wxSizerFlags().Expand().Center().Proportion(1).Border());

        mBigEndianButton =
            new wxRadioButton(this, wxID_ANY, _T("Big Endian (MAC)"), wxDefaultPosition, wxDefaultSize, 0);

        if (valueEndian)
            mBigEndianButton->SetValue(true);
        else
            button->SetValue(true);

        categorySizer->Add(mBigEndianButton, wxSizerFlags().Expand().Center().Proportion(1).Border());

        contentSizer->Add(categorySizer, wxSizerFlags().Expand().Proportion(0).Border());
    }

    topSizer->Add(contentSizer, wxSizerFlags().Expand().Proportion(0).TripleBorder(wxLEFT | wxRIGHT));

    // OK/Cancel buttons
    wxSizer* buttonSizer = CreateSeparatedButtonSizer(wxOK | wxCANCEL);
    if (buttonSizer)
    {
        topSizer->Add(buttonSizer, wxSizerFlags().Expand().DoubleBorder());
    }

    SetSizer(topSizer);

    topSizer->SetSizeHints(this);
    topSizer->Fit(this);

    Centre(wxBOTH);

    mDirtyValueMap = true;
}

// ----------------------------------------------------------------------------
const StringStringMap& FileImportOptionsDialog::getValueMap()
{
    if (mDirtyValueMap)
    {
        if (mFlipXCheckBox)
            mValueMap[PROPERTY_FLIPX] = wxString::Format("%d", mFlipXCheckBox->GetValue());
        if (mFlipYCheckBox)
            mValueMap[PROPERTY_FLIPY] = wxString::Format("%d", mFlipYCheckBox->GetValue());
        if (mWidthTextCtrl)
            mValueMap[PROPERTY_WIDTH] = mWidthTextCtrl->GetValue();
        if (mHeightTextCtrl)
            mValueMap[PROPERTY_HEIGHT] = mHeightTextCtrl->GetValue();
        if (mBigEndianButton)
            mValueMap[PROPERTY_BIGENDIAN] = wxString::Format("%d", mBigEndianButton->GetValue());

        size_t bppIndex = 0;
        std::list<wxRadioButton *>::iterator bppIt, bppItEnd = mBppButtons.end();
        for (bppIt = mBppButtons.begin(); bppIt != bppItEnd; ++bppIt, ++bppIndex)
        {
            if ((*bppIt)->GetValue())
                break;
        }
        if (bppIndex < mBppButtons.size())
            mValueMap[PROPERTY_BPP] = wxString::Format(_T("%d"), bppIndex);

        mDirtyValueMap = false;
    }
    return mValueMap;
}
