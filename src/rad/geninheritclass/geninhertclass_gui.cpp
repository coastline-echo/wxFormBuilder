///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Dec  4 2019)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "geninhertclass_gui.h"

///////////////////////////////////////////////////////////////////////////

GenInheritedClassDlgBase::GenInheritedClassDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* instructionsSbSizer;
	instructionsSbSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Instructions") ), wxVERTICAL );

	m_instructionsStaticText = new wxStaticText( instructionsSbSizer->GetStaticBox(), wxID_ANY, wxT("1. 检查要为其创建继承类的窗体。\n2. 可以通过单击列表中的名称来编辑单个类详细信息\n然后:\n\t2a. 根据需要编辑 '类名称:' 。\n\t2b. 根据需要编辑 '文件名: (.h/.cpp)' 。\n3. 点击 'OK'."), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE );
	m_instructionsStaticText->Wrap( -1 );
	instructionsSbSizer->Add( m_instructionsStaticText, 0, wxALL|wxEXPAND, 5 );


	mainSizer->Add( instructionsSbSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );

	wxArrayString m_formsCheckListChoices;
	m_formsCheckList = new wxCheckListBox( this, ID_FORMS_CHECK_LIST, wxDefaultPosition, wxDefaultSize, m_formsCheckListChoices, 0 );
	m_formsCheckList->SetMinSize( wxSize( 350,150 ) );

	mainSizer->Add( m_formsCheckList, 0, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* classDescriptionSbSizer;
	classDescriptionSbSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("类详情") ), wxVERTICAL );

	m_classNameStaticText = new wxStaticText( classDescriptionSbSizer->GetStaticBox(), wxID_ANY, wxT("类名："), wxDefaultPosition, wxDefaultSize, 0 );
	m_classNameStaticText->Wrap( -1 );
	classDescriptionSbSizer->Add( m_classNameStaticText, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	m_classNameTextCtrl = new wxTextCtrl( classDescriptionSbSizer->GetStaticBox(), ID_CLASS_NAME_TEXT_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	classDescriptionSbSizer->Add( m_classNameTextCtrl, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );

	m_fileNameStaticText = new wxStaticText( classDescriptionSbSizer->GetStaticBox(), wxID_ANY, wxT("文件名: (.cpp/.h)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fileNameStaticText->Wrap( -1 );
	classDescriptionSbSizer->Add( m_fileNameStaticText, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

	m_fileNameTextCtrl = new wxTextCtrl( classDescriptionSbSizer->GetStaticBox(), ID_FILE_NAME_TEXT_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	classDescriptionSbSizer->Add( m_fileNameTextCtrl, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );


	mainSizer->Add( classDescriptionSbSizer, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	m_sdbSizer = new wxStdDialogButtonSizer();
	m_sdbSizerOK = new wxButton( this, wxID_OK );
	m_sdbSizer->AddButton( m_sdbSizerOK );
	m_sdbSizerCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer->AddButton( m_sdbSizerCancel );
	m_sdbSizer->Realize();

	mainSizer->Add( m_sdbSizer, 0, wxALL|wxALIGN_RIGHT, 5 );


	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_formsCheckList->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GenInheritedClassDlgBase::OnFormsSelected ), NULL, this );
	m_formsCheckList->Connect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( GenInheritedClassDlgBase::OnFormsToggle ), NULL, this );
	m_classNameTextCtrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GenInheritedClassDlgBase::OnClassNameChange ), NULL, this );
	m_fileNameTextCtrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GenInheritedClassDlgBase::OnFileNameChange ), NULL, this );
}

GenInheritedClassDlgBase::~GenInheritedClassDlgBase()
{
	// Disconnect Events
	m_formsCheckList->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GenInheritedClassDlgBase::OnFormsSelected ), NULL, this );
	m_formsCheckList->Disconnect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( GenInheritedClassDlgBase::OnFormsToggle ), NULL, this );
	m_classNameTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GenInheritedClassDlgBase::OnClassNameChange ), NULL, this );
	m_fileNameTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GenInheritedClassDlgBase::OnFileNameChange ), NULL, this );

}
