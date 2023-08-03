///////////////////////////////////////////////////////////////////////////////
//
// wxFormBuilder - A Visual Dialog Editor for wxWidgets.
// Copyright (C) 2005 José Antonio Hurtado
// Copyright (C) 2005 José Antonio Hurtado
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// Written by
//   José Antonio Hurtado - joseantonio.hurtado@gmail.com
//   Juan Antonio Ortega  - jortegalalmolda@gmail.com
//
///////////////////////////////////////////////////////////////////////////////

#include "mainframe.h"

#include <wx/config.h>

#include <common/xmlutils.h>

#include "model/xrcfilter.h"
#include "rad/about.h"
#include "rad/appdata.h"
#include "rad/auitabart.h"
#include "rad/bitmaps.h"
#include "rad/cpppanel/cpppanel.h"
#include "rad/designer/visualeditor.h"
#include "rad/dialogfindcomponent.h"
#include "rad/geninheritclass/geninhertclass.h"
#include "rad/inspector/objinspect.h"
#include "rad/luapanel/luapanel.h"
#include "rad/objecttree/objecttree.h"
#include "rad/palette.h"
#include "rad/phppanel/phppanel.h"
#include "rad/pythonpanel/pythonpanel.h"
#include "rad/revision.h"
#include "rad/title.h"
#include "rad/version.h"
#include "rad/wxfbevent.h"
#include "rad/wxfbmanager.h"
#include "rad/xrcpanel/xrcpanel.h"
#include "utils/wxfbexception.h"


enum {
    ID_SAVE_PRJ = wxID_HIGHEST + 1,
    ID_OPEN_PRJ,
    ID_NEW_PRJ,
    ID_GENERATE_CODE,
    ID_IMPORT_XRC,
    ID_UNDO,
    ID_REDO,
    ID_SAVE_AS_PRJ,
    ID_CUT,
    ID_DELETE,
    ID_COPY,
    ID_PASTE,
    ID_EXPAND,
    ID_STRETCH,
    ID_MOVE_UP,
    ID_MOVE_DOWN,
    ID_RECENT_0,  // Tienen que tener ids consecutivos
    ID_RECENT_1,  // ID_RECENT_n+1 == ID_RECENT_n + 1
    ID_RECENT_2,  //
    ID_RECENT_3,  //
    ID_RECENT_SEP,

    ID_ALIGN_LEFT,
    ID_ALIGN_CENTER_H,
    ID_ALIGN_RIGHT,
    ID_ALIGN_TOP,
    ID_ALIGN_CENTER_V,
    ID_ALIGN_BOTTOM,

    ID_BORDER_LEFT,
    ID_BORDER_RIGHT,
    ID_BORDER_TOP,
    ID_BORDER_BOTTOM,
    ID_EDITOR_FNB,
    ID_MOVE_LEFT,
    ID_MOVE_RIGHT,

    ID_PREVIEW_XRC,
    ID_GEN_INHERIT_CLS,

    // The preference dialog must use wxID_PREFERENCES for wxMAC
    // ID_SETTINGS_GLOBAL, // For the future preference dialogs
    ID_SETTINGS_PROJ,  // For the future preference dialogs

    ID_FIND,

    ID_CLIPBOARD_COPY,
    ID_CLIPBOARD_PASTE,

    // added by tyysoft to define the swap button ID.
    ID_WINDOW_SWAP,

    // added by michallukowski to find component from menu.
    ID_FIND_COMPONENT,
};

#define STATUS_FIELD_OBJECT 2
#define STATUS_FIELD_PATH 1

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(ID_NEW_PRJ, MainFrame::OnNewProject)
EVT_MENU(ID_SAVE_PRJ, MainFrame::OnSaveProject)
EVT_MENU(ID_SAVE_AS_PRJ, MainFrame::OnSaveAsProject)
EVT_MENU(ID_OPEN_PRJ, MainFrame::OnOpenProject)
EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
EVT_MENU(wxID_EXIT, MainFrame::OnExit)
EVT_MENU(ID_IMPORT_XRC, MainFrame::OnImportXrc)
EVT_MENU(ID_GENERATE_CODE, MainFrame::OnGenerateCode)
EVT_MENU(ID_UNDO, MainFrame::OnUndo)
EVT_MENU(ID_REDO, MainFrame::OnRedo)
EVT_MENU(ID_DELETE, MainFrame::OnDelete)
EVT_MENU(ID_CUT, MainFrame::OnCut)
EVT_MENU(ID_COPY, MainFrame::OnCopy)
EVT_MENU(ID_PASTE, MainFrame::OnPaste)
EVT_MENU(ID_EXPAND, MainFrame::OnToggleExpand)
EVT_MENU(ID_STRETCH, MainFrame::OnToggleStretch)
EVT_MENU(ID_MOVE_UP, MainFrame::OnMoveUp)
EVT_MENU(ID_MOVE_DOWN, MainFrame::OnMoveDown)
EVT_MENU(ID_MOVE_LEFT, MainFrame::OnMoveLeft)
EVT_MENU(ID_MOVE_RIGHT, MainFrame::OnMoveRight)
EVT_MENU(ID_RECENT_0, MainFrame::OnOpenRecent)
EVT_MENU(ID_RECENT_1, MainFrame::OnOpenRecent)
EVT_MENU(ID_RECENT_2, MainFrame::OnOpenRecent)
EVT_MENU(ID_RECENT_3, MainFrame::OnOpenRecent)
EVT_MENU(ID_ALIGN_RIGHT, MainFrame::OnChangeAlignment)
EVT_MENU(ID_ALIGN_LEFT, MainFrame::OnChangeAlignment)
EVT_MENU(ID_ALIGN_CENTER_H, MainFrame::OnChangeAlignment)
EVT_MENU(ID_ALIGN_TOP, MainFrame::OnChangeAlignment)
EVT_MENU(ID_ALIGN_BOTTOM, MainFrame::OnChangeAlignment)
EVT_MENU(ID_ALIGN_CENTER_V, MainFrame::OnChangeAlignment)
EVT_MENU_RANGE(ID_BORDER_LEFT, ID_BORDER_BOTTOM, MainFrame::OnChangeBorder)
EVT_MENU(ID_PREVIEW_XRC, MainFrame::OnXrcPreview)
EVT_MENU(ID_GEN_INHERIT_CLS, MainFrame::OnGenInhertedClass)
EVT_MENU(ID_CLIPBOARD_COPY, MainFrame::OnClipboardCopy)
EVT_MENU(ID_CLIPBOARD_PASTE, MainFrame::OnClipboardPaste)
EVT_MENU(ID_WINDOW_SWAP, MainFrame::OnWindowSwap)
EVT_MENU(ID_FIND_COMPONENT, MainFrame::OnFindComponent)

EVT_UPDATE_UI(ID_CLIPBOARD_PASTE, MainFrame::OnClipboardPasteUpdateUI)
EVT_CLOSE(MainFrame::OnClose)

EVT_FB_CODE_GENERATION(MainFrame::OnCodeGeneration)
EVT_FB_OBJECT_CREATED(MainFrame::OnObjectCreated)
EVT_FB_OBJECT_REMOVED(MainFrame::OnObjectRemoved)
EVT_FB_OBJECT_EXPANDED(MainFrame::OnObjectExpanded)
EVT_FB_OBJECT_SELECTED(MainFrame::OnObjectSelected)
EVT_FB_PROJECT_LOADED(MainFrame::OnProjectLoaded)
EVT_FB_PROJECT_REFRESH(MainFrame::OnProjectRefresh)
EVT_FB_PROJECT_SAVED(MainFrame::OnProjectSaved)
EVT_FB_PROPERTY_MODIFIED(MainFrame::OnPropertyModified)
EVT_FB_EVENT_HANDLER_MODIFIED(MainFrame::OnEventHandlerModified)

EVT_MENU(ID_FIND, MainFrame::OnFindDialog)
EVT_FIND(wxID_ANY, MainFrame::OnFind)
EVT_FIND_NEXT(wxID_ANY, MainFrame::OnFind)
EVT_FIND_CLOSE(wxID_ANY, MainFrame::OnFindClose)
END_EVENT_TABLE()


// Used to kill focus from propgrid when toolbar or menu items are clicked
// This forces the propgrid to save the cell being edited
class FocusKillerEvtHandler : public wxEvtHandler
{
public:
    void OnMenuEvent(wxCommandEvent& event)
    {
        // Get window with focus
        wxWindow* windowWithFocus = wxWindow::FindFocus();

        // Only send the event if the focus is on the propgrid
        while (windowWithFocus != NULL) {
            wxPropertyGrid* propgrid = wxDynamicCast(windowWithFocus, wxPropertyGrid);
            if (propgrid != NULL) {
                wxFocusEvent focusEvent(wxEVT_KILL_FOCUS);
                propgrid->GetEventHandler()->ProcessEvent(focusEvent);
                break;
            }
            windowWithFocus = windowWithFocus->GetParent();
        }

        // Add the event to the mainframe's original handler
        // Add as pending so propgrid saves the property before the event is processed
        GetNextHandler()->AddPendingEvent(event);
    }

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(FocusKillerEvtHandler, wxEvtHandler)
EVT_MENU(wxID_ANY, FocusKillerEvtHandler::OnMenuEvent)
END_EVENT_TABLE()


MainFrame::MainFrame(wxWindow* parent, int id, int style, wxPoint pos, wxSize size) :
  wxFrame(parent, id, wxEmptyString, pos, size, wxDEFAULT_FRAME_STYLE),
  m_leftSplitterWidth(300),
  m_rightSplitterWidth(-300),
  m_style(style),
  m_page_selection(0),
  m_rightSplitter_sash_pos(300),
  m_autoSash(false),  // autosash function is temporarily disabled due to possible bug(?) in wxMSW event system
                      // (workaround is needed)
  m_findData(wxFR_DOWN),
  m_findDialog(NULL)
{

    // initialize the splitters, wxAUI doesn't use them
    m_leftSplitter = m_rightSplitter = NULL;

    /////////////////////////////////////////////////////////////////////////////
    // Setup frame icons, title bar, status bar, menubar and toolbar
    /////////////////////////////////////////////////////////////////////////////
    wxIconBundle bundle;
    wxIcon ico16;
    ico16.CopyFromBitmap(AppBitmaps::GetBitmap(wxT("app16"), AppBitmaps::Size::Icon_Medium));
    bundle.AddIcon(ico16);

    wxIcon ico32;
    ico32.CopyFromBitmap(AppBitmaps::GetBitmap(wxT("app32"), AppBitmaps::Size::Icon_Big));
    bundle.AddIcon(ico32);

    SetIcons(bundle);

    SetTitle(wxT("wxFormBuilder"));

    // SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

    SetMenuBar(CreateFBMenuBar());
    CreateStatusBar(3);
    SetStatusBarPane(0);
    int widths[3] = {-1, -1, 300};
    SetStatusWidths(sizeof(widths) / sizeof(int), widths);
    CreateFBToolBar();

    /////////////////////////////////////////////////////////////////////////////
    // Create the gui
    /////////////////////////////////////////////////////////////////////////////

    /*
    //  --- wxAUI version --
    wxWindow *objectTree      = CreateObjectTree(this);
    wxWindow *objectInspector = CreateObjectInspector(this);
    wxWindow *palette         = CreateComponentPalette(this);
    wxWindow *designer        = CreateDesignerWindow(this);


    m_mgr.SetFrame(this);
    m_mgr.AddPane(objectTree,
                 wxPaneInfo().Name(wxT("tree")).
                              Caption(wxT("Object Tree")).
                               Left().Layer(1).
                               BestSize(wxSize(300,400)).
                               CloseButton(false));
    m_mgr.AddPane(objectInspector,
                 wxPaneInfo().Name(wxT("inspector")).
                              Caption(wxT("Object Properties")).
                               Right().BestSize(wxSize(300,400)).
                               CloseButton(false));

    m_mgr.AddPane(designer,
                 wxPaneInfo().Name(wxT("editor")).
                              Caption(wxT("Editor")).
                               Center().
                               CloseButton(false));

    m_mgr.AddPane(palette,
                 wxPaneInfo().Name(wxT("palette")).
                 Caption(wxT("Component Palette")).
                 Top().
                 RightDockable(false).
                 LeftDockable(false).
                 CloseButton(false));

    m_mgr.Update();*/

    RestorePosition(wxT("mainframe"));
    Layout();

    // Init. m_cpp and m_xrc first
    m_cpp = NULL;
    m_xrc = NULL;
    m_python = NULL;
    m_lua = NULL;
    m_php = NULL;

    switch (style) {

        case wxFB_DOCKABLE_GUI:
            // TO-DO
            CreateWideGui();

            break;

        case wxFB_CLASSIC_GUI:

            /*  //  --- Classic style Gui --
                 //
                 //  +------++-----------------------+
                 //  | Obj. ||  Palette              |
                 //  | Tree ++-----------------------+
                 //  |      ||  Editor               |
                 //  |______||                       |
                 //  |------||                       |
                 //  | Obj. ||                       |
                 //  | Insp.||                       |
                 //  |      ||                       |
                 //  |      ||                       |
                 //  +------++-----------------------+ 	*/

            CreateClassicGui();

            break;

        case wxFB_DEFAULT_GUI:

        case wxFB_WIDE_GUI:

        default:

            /*  //  --- Wide style Gui --
                  //
                  //  +------++-----------------------+
                  //  | Obj. ||  Palette              |
                  //  | Tree ++-------------++--------+
                  //  |      ||  Editor     || Obj.   |
                  //  |      ||             || Insp.  |
                  //  |      ||             ||        |
                  //  |      ||             ||        |
                  //  |      ||             ||        |
                  //  |      ||             ||        |
                  //  |      ||             ||        |
                  //  +------++-------------++--------+ 	*/

            CreateWideGui();
    }

    AppData()->AddHandler(this->GetEventHandler());

    wxTheApp->SetTopWindow(this);

    m_focusKillEvtHandler = new FocusKillerEvtHandler;
    PushEventHandler(m_focusKillEvtHandler);

    // So splitter windows can be restored correctly
    Connect(wxEVT_IDLE, wxIdleEventHandler(MainFrame::OnIdle));

    m_notebook->Connect(
      wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler(MainFrame::OnAuiNotebookPageChanged), NULL,
      this);
}


MainFrame::~MainFrame()
{
    m_notebook->Disconnect(
      wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler(MainFrame::OnAuiNotebookPageChanged), NULL,
      this);

#ifdef __WXMAC__
    // work around problem on wxMac
    m_rightSplitter->GetWindow1()->GetSizer()->Detach(m_notebook);
    m_notebook->Destroy();
#endif

    /*m_mgr.UnInit();*/

    // the focus killer event handler
    RemoveEventHandler(m_focusKillEvtHandler);
    delete m_focusKillEvtHandler;

    AppData()->RemoveHandler(this->GetEventHandler());
    delete m_findDialog;
}

void MainFrame::RestorePosition(const wxString& name)
{
    m_currentDir = wxT("./projects");

    wxConfigBase* config = wxConfigBase::Get();
    config->SetPath(name);

    bool maximized;
    config->Read(wxT("IsMaximized"), &maximized, true);

    if (maximized) {
        Maximize(maximized);
    } else {
        bool iconized;
        config->Read(wxT("IsIconized"), &iconized, false);

        if (iconized) {
            Iconize(iconized);
        }
    }

    config->Read(wxT("LeftSplitterWidth"), &m_leftSplitterWidth, 300);
    config->Read(wxT("RightSplitterWidth"), &m_rightSplitterWidth, -300);
    config->Read(wxT("RightSplitterType"), &m_rightSplitterType, _("editor"));

    config->Read(wxT("AutoSash"), &m_autoSash, false);  // disabled in default due to possible bug(?) in wxMSW

    config->Read(wxT("CurrentDirectory"), &m_currentDir);

    config->Read(wxT("RecentFile0"), &m_recentProjects[0]);
    config->Read(wxT("RecentFile1"), &m_recentProjects[1]);
    config->Read(wxT("RecentFile2"), &m_recentProjects[2]);
    config->Read(wxT("RecentFile3"), &m_recentProjects[3]);

    config->SetPath(wxT(".."));
    UpdateRecentProjects();
}

void MainFrame::SavePosition(const wxString& name)
{
    m_objInsp->SavePosition();
    m_palette->SavePosition();

    wxConfigBase* config = wxConfigBase::Get();
    bool isIconized = IsIconized();
    bool isMaximized = IsMaximized();

    config->SetPath(name);

    if (!isMaximized) {
        config->Write(wxT("PosX"), isIconized ? -1 : GetPosition().x);
        config->Write(wxT("PosY"), isIconized ? -1 : GetPosition().y);
        config->Write(wxT("SizeW"), isIconized ? -1 : GetSize().GetWidth());
        config->Write(wxT("SizeH"), isIconized ? -1 : GetSize().GetHeight());
    }

    config->Write(wxT("IsMaximized"), isMaximized);

    config->Write(wxT("IsIconized"), isIconized);
    config->Write(wxT("CurrentDirectory"), m_currentDir);

    config->Write(wxT("RecentFile0"), m_recentProjects[0]);
    config->Write(wxT("RecentFile1"), m_recentProjects[1]);
    config->Write(wxT("RecentFile2"), m_recentProjects[2]);
    config->Write(wxT("RecentFile3"), m_recentProjects[3]);

    if (m_leftSplitter) {
        int leftSashWidth = m_leftSplitter->GetSashPosition();
        config->Write(wxT("LeftSplitterWidth"), leftSashWidth);
    }

    if (m_rightSplitter) {
        switch (m_style) {

            case wxFB_WIDE_GUI: {
                int rightSash = -1 * (m_rightSplitter->GetSize().GetWidth() - m_rightSplitter->GetSashPosition());
                config->Write(wxT("RightSplitterWidth"), rightSash);

                if (m_rightSplitter->GetWindow1()->GetChildren()[0]->GetChildren()[0]->GetLabel() == _("Editor")) {
                    config->Write(wxT("RightSplitterType"), _("editor"));
                } else {
                    config->Write(wxT("RightSplitterType"), _("prop"));
                }
                break;
            }

            case wxFB_CLASSIC_GUI: {
                int rightSash = -1 * (m_rightSplitter->GetSize().GetHeight() - m_rightSplitter->GetSashPosition());
                config->Write(wxT("RightSplitterWidth"), rightSash);
                break;
            }

            default:
                break;
        }
    }

    config->SetPath(wxT(".."));
}

void MainFrame::OnSaveProject(wxCommandEvent& event)

{
    wxString filename = AppData()->GetProjectFileName();

    if (filename == wxT(""))
        OnSaveAsProject(event);
    else {
        try {
            AppData()->SaveProject(filename);
            InsertRecentProject(filename);
        } catch (wxFBException& ex) {
            wxLogError(ex.what());
        }
    }
}

void MainFrame::OnSaveAsProject(wxCommandEvent&)
{
    wxFileDialog* dialog = new wxFileDialog(
      this, wxT("保存项目"), m_currentDir, wxT(""),
      wxT("wxFormBuilder 项目文件 (*.fbp)|*.fbp|All files (*.*)|*.*"), wxFD_SAVE);

    if (dialog->ShowModal() == wxID_OK) {
        m_currentDir = dialog->GetDirectory();
        wxString filename = dialog->GetPath();

        // Add the default extension if none was chosen
        wxFileName file(filename);

        if (!file.HasExt()) {
            file.SetExt(wxT("fbp"));
            filename = file.GetFullPath();
        }

        // Check the file whether exists or not
        if (file.FileExists() == true) {
            wxMessageDialog msg_box(
              this, wxT("该文件已经存在。要替换它吗？"), wxT("Overwrite the file"),
              wxYES_NO | wxICON_INFORMATION | wxNO_DEFAULT);
            if (msg_box.ShowModal() == wxID_NO) {
                dialog->Destroy();
                return;
            }
        }

        try {
            AppData()->SaveProject(filename);
            InsertRecentProject(filename);
        } catch (wxFBException& ex) {
            wxLogError(ex.what());
        }
    };

    dialog->Destroy();
}

void MainFrame::OnOpenProject(wxCommandEvent&)
{
    if (!SaveWarning())
        return;

    wxFileDialog* dialog = new wxFileDialog(
      this, wxT("打开项目"), m_currentDir, wxT(""),
      wxT("wxFormBuilder 项目文件 (*.fbp)|*.fbp|All files (*.*)|*.*"), wxFD_OPEN);

    if (dialog->ShowModal() == wxID_OK) {
        m_currentDir = dialog->GetDirectory();
        wxString filename = dialog->GetPath();

        if (AppData()->LoadProject(filename))
            InsertRecentProject(filename);
    };

    dialog->Destroy();
}

void MainFrame::OnOpenRecent(wxCommandEvent& event)
{
    if (!SaveWarning())
        return;

    int i = event.GetId() - ID_RECENT_0;

    assert(i >= 0 && i < 4);

    wxFileName filename(m_recentProjects[i]);

    if (filename.FileExists()) {
        if (AppData()->LoadProject(filename.GetFullPath())) {
            m_currentDir = filename.GetPath();
            InsertRecentProject(filename.GetFullPath());
        }
    } else {
        if (
          wxMessageBox(
            wxString::Format(
              wxT("项目文件 '%s' 不存在。是否要将其从最近文件列表中删除"),
              filename.GetName().GetData()),
            wxT("打开近期的项目"), wxICON_WARNING | wxYES_NO) == wxYES) {
            m_recentProjects[i] = wxT("");
            UpdateRecentProjects();
        }
    }
}

void MainFrame::OnImportXrc([[maybe_unused]] wxCommandEvent& event)
{
    wxFileDialog dlg(
        this,
        _("导入 XRC 文件"),
        m_currentDir,
        wxEmptyString,
        _("XRC files (*.xrc)|*.xrc|All files (*.*)|*.*"),
        wxFD_OPEN|wxFD_FILE_MUST_EXIST
    );
    if (dlg.ShowModal() != wxID_OK) {
        return;
    }

    wxFileName xrcFile(dlg.GetPath());
    m_currentDir = xrcFile.GetPath();
    XrcLoader xrcLoader(AppData()->GetObjectDatabase());
    auto doc = XMLUtils::LoadXMLFile(xrcFile.GetFullPath(), false);
    if (!doc) {
        wxLogError(_("%s: 无法打开文件"), xrcFile.GetFullPath());
        return;
    }
    if (doc->Error()) {
        wxLogError(doc->ErrorStr());
        return;
    }
    try {
        auto project = xrcLoader.GetProject(doc.get());
        if (project) {
            AppData()->MergeProject(project);
        }
    } catch (wxFBException& ex) {
        wxLogError(ex.what());
    }
}


void MainFrame::OnNewProject(wxCommandEvent&)
{
    if (!SaveWarning())
        return;

    AppData()->NewProject();
}

void MainFrame::OnGenerateCode(wxCommandEvent&)
{
    AppData()->GenerateCode(false);
}

void MainFrame::OnAbout(wxCommandEvent&)
{
    AboutDialog* dlg = new AboutDialog(this);
    dlg->ShowModal();
    dlg->Destroy();
}

void MainFrame::OnExit(wxCommandEvent&)
{
    Close();
}

void MainFrame::OnClose(wxCloseEvent& event)
{
    if (!SaveWarning())
        return;

    SavePosition(wxT("mainframe"));
    m_rightSplitter->Disconnect(
      wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, wxSplitterEventHandler(MainFrame::OnSplitterChanged));
    event.Skip();
}

void MainFrame::OnProjectLoaded(wxFBEvent&)
{
    GetStatusBar()->SetStatusText(wxT("项目已加载!"));
    PObjectBase project = AppData()->GetProjectData();

    if (project) {
        wxString objDetails = wxString::Format(
          wxT("名称: %s | 类型: %s"), project->GetPropertyAsString(wxT("name")), project->GetClassName());
        GetStatusBar()->SetStatusText(objDetails, STATUS_FIELD_OBJECT);
    }

    UpdateFrame();
}

void MainFrame::OnProjectSaved(wxFBEvent&)
{
    GetStatusBar()->SetStatusText(wxT("项目已保存!"));
    UpdateFrame();
}

void MainFrame::OnObjectExpanded(wxFBObjectEvent&)
{
    UpdateFrame();
}

void MainFrame::OnObjectSelected(wxFBObjectEvent& event)
{
    PObjectBase obj = event.GetFBObject();

    LogDebug(wxT("MainFrame::OnObjectSelected"));

    // resize sash position if necessary
    if (m_autoSash) {
        wxSize panel_size;
        int sash_pos;

        if (m_style != wxFB_CLASSIC_GUI) {
            switch (m_page_selection) {
                case 1:  // CPP panel
                    break;

                case 2:  // Python panel
                    break;

                case 3:  // PHP panel
                    break;

                case 4:  // LUA panel
                    break;

                case 5:  // XRC panel
                    break;

                default:
                    if (m_visualEdit != NULL) {

                        // If selected object is not a Frame or a Panel or a dialog, we won't
                        // adjust the sash position
                        if (
                          obj->GetObjectTypeName() == wxT("form") || obj->GetObjectTypeName() == wxT("wizard") ||
                          obj->GetObjectTypeName() == wxT("menubar_form") ||
                          obj->GetObjectTypeName() == wxT("toolbar_form")) {
                            sash_pos = m_rightSplitter->GetSashPosition();
                            panel_size = m_visualEdit->GetVirtualSize();

                            LogDebug(wxT("MainFrame::OnObjectSelected > sash pos = %d"), sash_pos);
                            LogDebug(wxT("MainFrame::OnObjectSelected > virtual width = %d"), panel_size.GetWidth());

                            if (panel_size.GetWidth() > sash_pos) {
                                // set the sash position
                                LogDebug(wxT("MainFrame::OnObjectSelected > set sash position"));
                                m_rightSplitter_sash_pos = panel_size.GetWidth();
                                m_rightSplitter->SetSashPosition(m_rightSplitter_sash_pos);
                            }
                        }
                    }
                    break;
            }
        }
    }

    wxString name;
    PProperty prop(obj->GetProperty(wxT("name")));

    if (prop)
        name = prop->GetValueAsString();
    else
        name = wxT("\"Unknown\"");

    // GetStatusBar()->SetStatusText( wxT( "Object " ) + name + wxT( " Selected!" ) );

    wxString objDetails = wxString::Format(wxT("名称: %s | 类型: %s"), name, obj->GetClassName());

    GetStatusBar()->SetStatusText(objDetails, STATUS_FIELD_OBJECT);

    UpdateFrame();
}

void MainFrame::OnObjectCreated(wxFBObjectEvent& event)
{
    wxString message;

    LogDebug(wxT("MainFrame::OnObjectCreated"));

    if (event.GetFBObject()) {
        message.Printf(
          wxT("'%s' 类的对象  '%s' 已创建。"), event.GetFBObject()->GetClassName(), 
          event.GetFBObject()->GetPropertyAsString(wxT("name")));
    } else {
        message = wxT(
          "无法创建对象。您是否忘记添加sizer/父对象或打开/关闭AUI管理?");
        wxMessageBox(message, wxT("wxFormBuilder"), wxICON_WARNING | wxOK);
    }

    GetStatusBar()->SetStatusText(message);

    UpdateFrame();
}

void MainFrame::OnObjectRemoved(wxFBObjectEvent& event)
{
    wxString message;
    message.Printf(wxT("项目 '%s' 已被删除。"), event.GetFBObject()->GetPropertyAsString(wxT("name")));
    GetStatusBar()->SetStatusText(message);
    UpdateFrame();
}

void MainFrame::OnPropertyModified(wxFBPropertyEvent& event)
{
    PProperty prop = event.GetFBProperty();

    if (prop) {
        if (prop->GetObject() == AppData()->GetSelectedObject()) {
            if (0 == prop->GetName().CmpNoCase(wxT("name"))) {
                wxString oldDetails = GetStatusBar()->GetStatusText(STATUS_FIELD_OBJECT);
                wxString newDetails;
                size_t pipeIdx = oldDetails.find(wxT("|"));

                if (pipeIdx != oldDetails.npos) {
                    newDetails.Printf(wxT("Name: %s %s"), prop->GetValueAsString(), oldDetails.substr(pipeIdx));
                    GetStatusBar()->SetStatusText(newDetails, STATUS_FIELD_OBJECT);
                }
            }

            GetStatusBar()->SetStatusText(wxT("修改属性!"));
        }

        // When you change the sizeritem properties, the object modified is not
        // the same that the selected object because is a sizeritem object.
        // It's necessary to update the frame for the toolbar buttons.
        UpdateFrame();
    }
}

void MainFrame::OnEventHandlerModified(wxFBEventHandlerEvent& event)
{
    wxString message;
    message.Printf(
      wxT("已修改对象 '%s' 的事件处理程序 '%s' 。"), event.GetFBEventHandler()->GetName(),
      event.GetFBEventHandler()->GetObject()->GetPropertyAsString(wxT("name")));

    GetStatusBar()->SetStatusText(message);
    UpdateFrame();
}

void MainFrame::OnCodeGeneration(wxFBEvent& event)
{
    // Using the previously unused Id field in the event to carry a boolean
    bool panelOnly = (event.GetId() != 0);

    if (panelOnly) {
        GetStatusBar()->SetStatusText(wxT("已生成代码!"));
    }
}

void MainFrame::OnProjectRefresh(wxFBEvent&)
{
    PObjectBase project = AppData()->GetProjectData();

    if (project) {
        wxString objDetails = wxString::Format(
          wxT("Name: %s | Class: %s"), project->GetPropertyAsString(wxT("name")), project->GetClassName());
        GetStatusBar()->SetStatusText(objDetails, STATUS_FIELD_OBJECT);
    }

    UpdateFrame();
}

void MainFrame::OnUndo(wxCommandEvent&)
{
    AppData()->Undo();
}

void MainFrame::OnRedo(wxCommandEvent&)
{
    AppData()->Redo();
}

void MainFrame::UpdateLayoutTools()
{
    int option = -1;
    int border = 0;
    int flag = 0;
    int orient = 0;

    bool gotLayoutSettings =
      AppData()->GetLayoutSettings(AppData()->GetSelectedObject(), &flag, &option, &border, &orient);
    wxToolBar* toolbar = GetToolBar();
    wxMenu* menuEdit = GetMenuBar()->GetMenu(GetMenuBar()->FindMenu(wxT("Edit")));

    // Enable the layout tools if there are layout settings, else disable the tools
    menuEdit->Enable(ID_EXPAND, gotLayoutSettings);
    toolbar->EnableTool(ID_EXPAND, gotLayoutSettings);
    menuEdit->Enable(ID_STRETCH, option >= 0);
    toolbar->EnableTool(ID_STRETCH, option >= 0);

    bool enableHorizontalTools = (orient != wxHORIZONTAL) && gotLayoutSettings;
    menuEdit->Enable(ID_ALIGN_LEFT, enableHorizontalTools);
    toolbar->EnableTool(ID_ALIGN_LEFT, enableHorizontalTools);
    menuEdit->Enable(ID_ALIGN_CENTER_H, enableHorizontalTools);
    toolbar->EnableTool(ID_ALIGN_CENTER_H, enableHorizontalTools);
    menuEdit->Enable(ID_ALIGN_RIGHT, enableHorizontalTools);
    toolbar->EnableTool(ID_ALIGN_RIGHT, enableHorizontalTools);

    bool enableVerticalTools = (orient != wxVERTICAL) && gotLayoutSettings;
    menuEdit->Enable(ID_ALIGN_TOP, enableVerticalTools);
    toolbar->EnableTool(ID_ALIGN_TOP, enableVerticalTools);
    menuEdit->Enable(ID_ALIGN_CENTER_V, enableVerticalTools);
    toolbar->EnableTool(ID_ALIGN_CENTER_V, enableVerticalTools);
    menuEdit->Enable(ID_ALIGN_BOTTOM, enableVerticalTools);
    toolbar->EnableTool(ID_ALIGN_BOTTOM, enableVerticalTools);

    toolbar->EnableTool(ID_BORDER_TOP, gotLayoutSettings);
    toolbar->EnableTool(ID_BORDER_RIGHT, gotLayoutSettings);
    toolbar->EnableTool(ID_BORDER_LEFT, gotLayoutSettings);
    toolbar->EnableTool(ID_BORDER_BOTTOM, gotLayoutSettings);

    // Toggle the toolbar buttons according to the properties, if there are layout settings
    toolbar->ToggleTool(ID_EXPAND, ((flag & wxEXPAND) != 0) && gotLayoutSettings);
    toolbar->ToggleTool(ID_STRETCH, (option > 0) && gotLayoutSettings);
    toolbar->ToggleTool(
      ID_ALIGN_LEFT, !((flag & (wxALIGN_RIGHT | wxALIGN_CENTER_HORIZONTAL)) != 0) && enableHorizontalTools);
    toolbar->ToggleTool(ID_ALIGN_CENTER_H, ((flag & wxALIGN_CENTER_HORIZONTAL) != 0) && enableHorizontalTools);
    toolbar->ToggleTool(ID_ALIGN_RIGHT, ((flag & wxALIGN_RIGHT) != 0) && enableHorizontalTools);
    toolbar->ToggleTool(
      ID_ALIGN_TOP, !((flag & (wxALIGN_BOTTOM | wxALIGN_CENTER_VERTICAL)) != 0) && enableVerticalTools);
    toolbar->ToggleTool(ID_ALIGN_CENTER_V, ((flag & wxALIGN_CENTER_VERTICAL) != 0) && enableVerticalTools);
    toolbar->ToggleTool(ID_ALIGN_BOTTOM, ((flag & wxALIGN_BOTTOM) != 0) && enableVerticalTools);

    toolbar->ToggleTool(ID_BORDER_TOP, ((flag & wxTOP) != 0) && gotLayoutSettings);
    toolbar->ToggleTool(ID_BORDER_RIGHT, ((flag & wxRIGHT) != 0) && gotLayoutSettings);
    toolbar->ToggleTool(ID_BORDER_LEFT, ((flag & wxLEFT) != 0) && gotLayoutSettings);
    toolbar->ToggleTool(ID_BORDER_BOTTOM, ((flag & wxBOTTOM) != 0) && gotLayoutSettings);
}

void MainFrame::UpdateFrame()
{
    // Build the title
    wxString filename = AppData()->GetProjectFileName();
    wxString file;

    if (filename.empty()) {
        file = wxT("untitled");
    } else {
        wxFileName fn(filename);
        file = fn.GetName();
    }

    SetTitle(wxString::Format(
      wxT("%s%s - wxFormBuilder %s%s%s"), AppData()->IsModified() ? wxT("*") : wxT(""), file, getVersion(),
      getReleaseRevision(getVersion()).c_str(), getReleaseBranch(getBranch()).c_str()));
    GetStatusBar()->SetStatusText(filename, STATUS_FIELD_PATH);

    // Enable/Disable toolbar and menu entries
    wxToolBar* toolbar = GetToolBar();

    wxMenu* menuFile = GetMenuBar()->GetMenu(GetMenuBar()->FindMenu(_("File")));
    menuFile->Enable(ID_SAVE_PRJ, AppData()->IsModified());
    toolbar->EnableTool(ID_SAVE_PRJ, AppData()->IsModified());

    wxMenu* menuEdit = GetMenuBar()->GetMenu(GetMenuBar()->FindMenu(wxT("Edit")));

    bool redo = AppData()->CanRedo();
    menuEdit->Enable(ID_REDO, redo);
    toolbar->EnableTool(ID_REDO, redo);

    bool undo = AppData()->CanUndo();
    menuEdit->Enable(ID_UNDO, undo);
    toolbar->EnableTool(ID_UNDO, undo);

    bool copy = AppData()->CanCopyObject();
    bool isEditor = (_("Designer") != m_notebook->GetPageText(m_notebook->GetSelection()));
    menuEdit->Enable(ID_FIND, isEditor);

    menuEdit->Enable(ID_CLIPBOARD_COPY, copy);

    menuEdit->Enable(ID_COPY, copy || isEditor);
    toolbar->EnableTool(ID_COPY, copy || isEditor);

    menuEdit->Enable(ID_CUT, copy);
    toolbar->EnableTool(ID_CUT, copy);

    menuEdit->Enable(ID_DELETE, copy);
    toolbar->EnableTool(ID_DELETE, copy);

    menuEdit->Enable(ID_MOVE_UP, copy);
    menuEdit->Enable(ID_MOVE_DOWN, copy);
    menuEdit->Enable(ID_MOVE_LEFT, copy);
    menuEdit->Enable(ID_MOVE_RIGHT, copy);

    bool paste = AppData()->CanPasteObject();
    menuEdit->Enable(ID_PASTE, paste);
    toolbar->EnableTool(ID_PASTE, paste);

    menuEdit->Enable(ID_CLIPBOARD_PASTE, AppData()->CanPasteObjectFromClipboard());

    UpdateLayoutTools();
}

void MainFrame::UpdateRecentProjects()
{
    wxMenu* menuFile = GetMenuBar()->GetMenu(GetMenuBar()->FindMenu(wxT("File")));

    // borramos los items del menu de los projectos recientes

    for (int i = 0; i < 4; i++) {
        if (menuFile->FindItem(ID_RECENT_0 + i))
            menuFile->Destroy(ID_RECENT_0 + i);
    }

    wxMenuItem* mruSep = menuFile->FindItemByPosition(menuFile->GetMenuItemCount() - 1);
    if (mruSep->IsSeparator()) {
        menuFile->Destroy(mruSep);
    }

    // remove empty filenames and 'compress' the rest
    int fi = 0;
    for (int i = 0; i < 4; i++) {
        if (!m_recentProjects[i].IsEmpty())
            m_recentProjects[fi++] = m_recentProjects[i];
    }
    for (int i = fi; i < 4; i++) m_recentProjects[i] = wxT("");

    if (!m_recentProjects[0].IsEmpty()) {
        menuFile->AppendSeparator();
    }

    // creamos los nuevos ficheros recientes
    for (unsigned int i = 0; i < 4 && !m_recentProjects[i].IsEmpty(); i++)
        menuFile->Append(ID_RECENT_0 + i, m_recentProjects[i], wxT(""));
}

void MainFrame::InsertRecentProject(const wxString& file)
{
    bool found = false;
    int i;

    for (i = 0; i < 4 && !found; i++) found = (file == m_recentProjects[i]);

    if (found)  // en i-1 está la posición encontrada (0 < i < 4)
    {
        // desplazamos desde 0 hasta i-1 una posición a la derecha

        for (i = i - 1; i > 0; i--) m_recentProjects[i] = m_recentProjects[i - 1];
    } else if (!found) {
        for (i = 3; i > 0; i--) m_recentProjects[i] = m_recentProjects[i - 1];
    }

    m_recentProjects[0] = file;

    UpdateRecentProjects();
}

void MainFrame::OnCopy(wxCommandEvent&)

{
    wxWindow* focusedWindow = wxWindow::FindFocus();

    if (focusedWindow != NULL && focusedWindow->IsKindOf(wxCLASSINFO(wxStyledTextCtrl))) {
        ((wxStyledTextCtrl*)focusedWindow)->Copy();
    } else {
        AppData()->CopyObject(AppData()->GetSelectedObject());
        UpdateFrame();
    }
}

void MainFrame::OnCut(wxCommandEvent&)
{
    wxWindow* focusedWindow = wxWindow::FindFocus();

    if (focusedWindow != NULL && focusedWindow->IsKindOf(wxCLASSINFO(wxStyledTextCtrl))) {
        ((wxStyledTextCtrl*)focusedWindow)->Cut();
    } else {
        AppData()->CutObject(AppData()->GetSelectedObject());
        UpdateFrame();
    }
}

void MainFrame::OnDelete(wxCommandEvent&)
{
    AppData()->RemoveObject(AppData()->GetSelectedObject());
    UpdateFrame();
}

void MainFrame::OnPaste(wxCommandEvent&)
{
    wxWindow* focusedWindow = wxWindow::FindFocus();

    if (focusedWindow != NULL && focusedWindow->IsKindOf(wxCLASSINFO(wxStyledTextCtrl))) {
        ((wxStyledTextCtrl*)focusedWindow)->Paste();
    } else {
        AppData()->PasteObject(AppData()->GetSelectedObject());
        UpdateFrame();
    }
}

void MainFrame::OnClipboardCopy(wxCommandEvent&)
{
    AppData()->CopyObjectToClipboard(AppData()->GetSelectedObject());
    UpdateFrame();
}

void MainFrame::OnClipboardPaste(wxCommandEvent&)
{
    AppData()->PasteObjectFromClipboard(AppData()->GetSelectedObject());
    UpdateFrame();
}

void MainFrame::OnClipboardPasteUpdateUI(wxUpdateUIEvent& e)
{
    e.Enable(AppData()->CanPasteObjectFromClipboard());
}

void MainFrame::OnToggleExpand(wxCommandEvent&)
{
    AppData()->ToggleExpandLayout(AppData()->GetSelectedObject());
}

void MainFrame::OnToggleStretch(wxCommandEvent&)
{
    AppData()->ToggleStretchLayout(AppData()->GetSelectedObject());
}

void MainFrame::OnMoveUp(wxCommandEvent&)
{
    AppData()->MovePosition(AppData()->GetSelectedObject(), false, 1);
}

void MainFrame::OnMoveDown(wxCommandEvent&)
{
    AppData()->MovePosition(AppData()->GetSelectedObject(), true, 1);
}

void MainFrame::OnMoveLeft(wxCommandEvent&)
{
    AppData()->MoveHierarchy(AppData()->GetSelectedObject(), true);
}

void MainFrame::OnMoveRight(wxCommandEvent&)
{
    AppData()->MoveHierarchy(AppData()->GetSelectedObject(), false);
}

void MainFrame::OnChangeAlignment(wxCommandEvent& event)
{
    int align = 0;
    bool vertical =
      (event.GetId() == ID_ALIGN_TOP || event.GetId() == ID_ALIGN_BOTTOM || event.GetId() == ID_ALIGN_CENTER_V);

    switch (event.GetId()) {

        case ID_ALIGN_RIGHT:
            align = wxALIGN_RIGHT;

            break;

        case ID_ALIGN_CENTER_H:
            align = wxALIGN_CENTER_HORIZONTAL;

            break;

        case ID_ALIGN_BOTTOM:
            align = wxALIGN_BOTTOM;

            break;

        case ID_ALIGN_CENTER_V:
            align = wxALIGN_CENTER_VERTICAL;

            break;
    }

    AppData()->ChangeAlignment(AppData()->GetSelectedObject(), align, vertical);

    UpdateLayoutTools();
}

void MainFrame::OnChangeBorder(wxCommandEvent& e)
{
    int border;

    switch (e.GetId()) {
        case ID_BORDER_LEFT:
            border = wxLEFT;
            break;

        case ID_BORDER_RIGHT:
            border = wxRIGHT;
            break;

        case ID_BORDER_TOP:
            border = wxTOP;
            break;

        case ID_BORDER_BOTTOM:
            border = wxBOTTOM;
            break;

        default:
            border = 0;
            break;
    }

    AppData()->ToggleBorderFlag(AppData()->GetSelectedObject(), border);

    UpdateLayoutTools();
}

void MainFrame::OnXrcPreview(wxCommandEvent& WXUNUSED(e))
{
    AppData()->ShowXrcPreview();

    /*wxPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    for ( int i = 0, count = all_panes.GetCount(); i < count; ++i )
    {
            wxPaneInfo info = all_panes.Item( i );
    }*/
}

void MainFrame::OnGenInhertedClass(wxCommandEvent& WXUNUSED(e))
{
    wxString filePath;
    try {
        // Get the output path
        filePath = AppData()->GetOutputPath();
    } catch (wxFBException& ex) {
        wxLogWarning(ex.what());
        return;
    }

    // Show the dialog
    PObjectBase project = AppData()->GetProjectData();
    if (project->IsPropertyNull(_("file"))) {
        wxLogWarning(_("在生成继承类之前，必须设置项目的 \"file\" 属性。"));
        return;
    }
    GenInheritedClassDlg dlg(this, project);

    if (wxID_OK != dlg.ShowModal()) {
        return;
    }

    std::vector<GenClassDetails> selectedForms;
    dlg.GetFormsSelected(&selectedForms);

    for (size_t i = 0; i < selectedForms.size(); ++i) {
        const GenClassDetails& details = selectedForms[i];

        // Create the class and files.
        AppData()->GenerateInheritedClass(details.m_form, details.m_className, filePath, details.m_fileName);
    }

    wxMessageBox(wxString::Format(wxT("Class(es) generated to \'%s\'."), filePath), wxT("wxFormBuilder"));
}

bool MainFrame::SaveWarning()
{
    int result = wxYES;

    if (AppData()->IsModified()) {
        result = ::wxMessageBox(
          wxT("当前项目文件已被修改...\n") wxT("要保存更改吗?"),
          wxT("保存项目"), wxYES | wxNO | wxCANCEL, this);

        if (result == wxYES) {
            wxCommandEvent dummy;
            OnSaveProject(dummy);
        }
    }

    return (result != wxCANCEL);
}

void MainFrame::OnAuiNotebookPageChanged(wxAuiNotebookEvent& event)
{
    UpdateFrame();

    if (m_autoSash) {
        m_page_selection = event.GetSelection();
        LogDebug(wxT("MainFrame::OnFlatNotebookPageChanged > selection = %d"), m_page_selection);

        wxSize panel_size;
        int sash_pos;

        if (m_style != wxFB_CLASSIC_GUI) {
            switch (m_page_selection) {
                case 1:  // CPP panel
                    if ((m_cpp != NULL) && (m_rightSplitter != NULL)) {
                        panel_size = m_cpp->GetClientSize();
                        sash_pos = m_rightSplitter->GetSashPosition();

                        LogDebug(
                          wxT("MainFrame::OnFlatNotebookPageChanged > CPP panel: width = %d sash pos = %d"),
                          panel_size.GetWidth(), sash_pos);

                        if (panel_size.GetWidth() > sash_pos) {
                            // set the sash position
                            LogDebug(wxT("MainFrame::OnFlatNotebookPageChanged > reset sash position"));
                            m_rightSplitter->SetSashPosition(panel_size.GetWidth());
                        }
                    }
                    break;

                case 2:  // Python panel
                    if ((m_python != NULL) && (m_rightSplitter != NULL)) {
                        panel_size = m_python->GetClientSize();
                        sash_pos = m_rightSplitter->GetSashPosition();

                        LogDebug(
                          wxT("MainFrame::OnFlatNotebookPageChanged > Python panel: width = %d sash pos = %d"),
                          panel_size.GetWidth(), sash_pos);

                        if (panel_size.GetWidth() > sash_pos) {
                            // set the sash position
                            LogDebug(wxT("MainFrame::OnFlatNotebookPageChanged > reset sash position"));
                            m_rightSplitter->SetSashPosition(panel_size.GetWidth());
                        }
                    }
                    break;

                case 3:  // PHP panel
                    if ((m_php != NULL) && (m_rightSplitter != NULL)) {
                        panel_size = m_xrc->GetClientSize();
                        sash_pos = m_rightSplitter->GetSashPosition();

                        LogDebug(
                          wxT("MainFrame::OnFlatNotebookPageChanged > PHP panel: width = %d sash pos = %d"),
                          panel_size.GetWidth(), sash_pos);

                        if (panel_size.GetWidth() > sash_pos) {
                            // set the sash position
                            LogDebug(wxT("MainFrame::OnFlatNotebookPageChanged > reset sash position"));
                            m_rightSplitter->SetSashPosition(panel_size.GetWidth());
                        }
                    }
                    break;

                case 4:  // Lua panel
                    if ((m_lua != NULL) && (m_rightSplitter != NULL)) {
                        panel_size = m_lua->GetClientSize();
                        sash_pos = m_rightSplitter->GetSashPosition();

                        LogDebug(
                          wxT("MainFrame::OnFlatNotebookPageChanged > Lua panel: width = %d sash pos = %d"),
                          panel_size.GetWidth(), sash_pos);

                        if (panel_size.GetWidth() > sash_pos) {
                            // set the sash position
                            LogDebug(wxT("MainFrame::OnFlatNotebookPageChanged > reset sash position"));
                            m_rightSplitter->SetSashPosition(panel_size.GetWidth());
                        }
                    }
                    break;

                case 5:  // XRC panel
                    if ((m_xrc != NULL) && (m_rightSplitter != NULL)) {
                        panel_size = m_xrc->GetClientSize();
                        sash_pos = m_rightSplitter->GetSashPosition();

                        LogDebug(
                          wxT("MainFrame::OnFlatNotebookPageChanged > XRC panel: width = %d sash pos = %d"),
                          panel_size.GetWidth(), sash_pos);

                        if (panel_size.GetWidth() > sash_pos) {
                            // set the sash position
                            LogDebug(wxT("MainFrame::OnFlatNotebookPageChanged > reset sash position"));
                            m_rightSplitter->SetSashPosition(panel_size.GetWidth());
                        }
                    }
                    break;


                default:
                    if (m_visualEdit != NULL) {
                        sash_pos = m_rightSplitter->GetSashPosition();

                        if (m_rightSplitter_sash_pos < sash_pos) {
                            // restore the sash position
                            LogDebug(
                              wxT("MainFrame::OnFlatNotebookPageChanged > restore sash position: sash pos = %d"),
                              m_rightSplitter_sash_pos);
                            m_rightSplitter->SetSashPosition(m_rightSplitter_sash_pos);
                        } else {
                            // update position
                            m_rightSplitter_sash_pos = sash_pos;
                        }
                    }
                    break;
            }
        }
    }

    AppData()->GenerateCode(true);
}

void MainFrame::OnFindDialog(wxCommandEvent&)
{
    if (NULL == m_findDialog) {
        m_findDialog = new wxFindReplaceDialog(this, &m_findData, wxT("Find"));
        m_findDialog->Centre(wxCENTRE_ON_SCREEN | wxBOTH);
    }
    m_findDialog->Show(true);
}

void MainFrame::OnFindClose(wxFindDialogEvent&)
{
    m_findDialog->Destroy();
    m_findDialog = 0;
}

void MainFrame::OnFind(wxFindDialogEvent& event)
{
    /*for ( int page = 0; page < m_notebook->GetPageCount(); ++page )
    {
            event.StopPropagation();
            event.SetClientData( m_findDialog );
            m_notebook->GetPage( page )->GetEventHandler()->ProcessEvent( event );
    }*/

    wxWindow* page = m_notebook->GetCurrentPage();
    if (page) {
        event.StopPropagation();
        event.SetClientData(m_findDialog);
        page->GetEventHandler()->ProcessEvent(event);
    }
}

/////////////////////////////////////////////////////////////////////////////

wxMenuBar* MainFrame::CreateFBMenuBar()
{
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_NEW_PRJ, wxT("&新项目\tCtrl+N"), wxT("创建一个空项目"));
    menuFile->Append(ID_OPEN_PRJ, wxT("&打开...\tCtrl+O"), wxT("打开一个项目"));

    menuFile->Append(ID_SAVE_PRJ, wxT("&保存\tCtrl+S"), wxT("保存当前项目"));
    menuFile->Append(ID_SAVE_AS_PRJ, wxT("另存为...\tCtrl+Shift+S"), wxT("另存当前项目为..."));
    menuFile->AppendSeparator();
    menuFile->Append(ID_IMPORT_XRC, wxT("&导入 XRC..."), wxT("导入 XRC 文件"));
    menuFile->AppendSeparator();
    menuFile->Append(ID_GENERATE_CODE, wxT("&生成代码\tF8"), wxT("生成代码"));
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, wxT("退出\tAlt-F4"), wxT("退出 wxFormBuilder"));

    wxMenu* menuEdit = new wxMenu;
menuEdit->Append(ID_UNDO, wxT("&撤消 \tCtrl+Z"), wxT("“撤消修改"));
    menuEdit->Append(ID_REDO, wxT("恢复 \tCtrl+Y"), wxT("恢复更改"));
    menuEdit->AppendSeparator();
    menuEdit->Append(ID_COPY, wxT("&复制 \tCtrl+C"), wxT("复制选定对象"));
    menuEdit->Append(ID_CUT, wxT("剪切 \tCtrl+X"), wxT("剪切选定对象"));
    menuEdit->Append(ID_PASTE, wxT("粘贴 \tCtrl+V"), wxT("粘贴到所选对象上"));
    menuEdit->Append(ID_DELETE, wxT("删除 \tCtrl+D"), wxT("删除选定对象"));
    menuEdit->AppendSeparator();
    menuEdit->Append(ID_CLIPBOARD_COPY, wxT("复制对象到剪贴板\tCtrl+Shift+C"), wxT("复制对象到剪贴板"));
    menuEdit->Append(
      ID_CLIPBOARD_PASTE, wxT("从剪贴板粘贴对象\tCtrl+Shift+V"), wxT("从剪贴板粘贴对象"));
    menuEdit->AppendSeparator();
    menuEdit->Append(ID_EXPAND, wxT("切换扩展\tAlt+W"), wxT("切换sizeitem属性的wxEXPAND标志"));
    menuEdit->Append(ID_STRETCH, wxT("切换Stretch\tAlt+S"), wxT("切换 sizeritem 属性的选项属性"));
    menuEdit->Append(ID_MOVE_UP, wxT("向上移动\tAlt+Up"), wxT("上移所选对象"));
    menuEdit->Append(ID_MOVE_DOWN, wxT("向下移动\tAlt+Down"), wxT("下移所选对象"));
    menuEdit->Append(ID_MOVE_LEFT, wxT("向左移动\tAlt+Left"), wxT("左移所选对象"));
    menuEdit->Append(ID_MOVE_RIGHT, wxT("向右移动\tAlt+Right"), wxT("右移所选对象"));
    menuEdit->AppendSeparator();
    menuEdit->Append(ID_FIND, wxT("&查找\tCtrl+F"), wxT("在活动代码查看器中查找文本"));
    menuEdit->AppendSeparator();
    menuEdit->Append(ID_ALIGN_LEFT, wxT("向左对齐\tAlt+Shift+Left"), wxT("将项目向左对齐"));
    menuEdit->Append(ID_ALIGN_CENTER_H, wxT("水平居中对齐\tAlt+Shift+H"), wxT("将项目水平对齐到中心"));
    menuEdit->Append(ID_ALIGN_RIGHT, wxT("向右对齐\tAlt+Shift+Right"), wxT("将项目向右对齐"));
    menuEdit->Append(ID_ALIGN_TOP, wxT("向上对齐\tAlt+Shift+Up"), wxT("将项目对齐到顶部"));
    menuEdit->Append(ID_ALIGN_CENTER_V, wxT("垂直居中对齐\tAlt+Shift+V"), wxT("将项目垂直对齐到中心"));
    menuEdit->Append(ID_ALIGN_BOTTOM, wxT("底部对齐\tAlt+Shift+Down"), wxT("将项目对齐到底部"));

    wxMenu* menuComponents = CreateMenuComponents();

    wxMenu* menuView = new wxMenu;
    menuView->Append(ID_PREVIEW_XRC, wxT("XRC 窗口\tF5"), wxT("显示XRC窗口的预览"));
    menuView->AppendSeparator();
    menuView->Append(ID_WINDOW_SWAP, wxT("切换编辑器和属性窗口\tF12"), wxT("交换编辑器和属性窗口"));

    wxMenu* menuTools = new wxMenu;
    menuTools->Append(
      ID_GEN_INHERIT_CLS, wxT("生成继承类\tF6"),
      wxT("创建所需的文件和类，以便正确继承所设计的GUI"));

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, wxT("关于\tF1"), wxT("显示关于对话框"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(menuEdit, wxT("&Edit"));
    menuBar->Append(menuComponents, wxT("&Components"));
    menuBar->Append(menuView, wxT("&View"));
    menuBar->Append(menuTools, wxT("&Tools"));
    menuBar->Append(menuHelp, wxT("&Help"));

    return menuBar;
}

wxMenu* MainFrame::CreateMenuComponents()
{
    wxMenu* menuComponents = new wxMenu;

    menuComponents->Append(ID_FIND_COMPONENT, _("&Find Component...\tCtrl+Shift+F"), _("显示组件搜索对话框"));

    // Package count
    unsigned int pkg_count = AppData()->GetPackageCount();
    // Lookup map of all packages
    std::map<wxString, PObjectPackage> packages;
    // List of pages to add to the menu in the same order like notebook
    std::vector<std::pair<wxString, PObjectPackage>> pages;
    pages.reserve(pkg_count);

    // Fill lookup map of packages
    for (unsigned int i = 0; i < pkg_count; ++i) {
        auto pkg = AppData()->GetPackage(i);
        packages.insert(std::make_pair(pkg->GetPackageName(), pkg));
    }

    // Read the page order from settings and build the list of pages from it
    auto* config = wxConfigBase::Get();
    wxStringTokenizer pageOrder(
      config->Read(
        wxT("/palette/pageOrder"), wxT("通用类,附加类,数据类,容器类,菜单类/工具栏类,布局类,表单类,功能区类")),
      wxT(","));
    while (pageOrder.HasMoreTokens()) {
        const auto packageName = pageOrder.GetNextToken();
        auto package = packages.find(packageName);
        if (packages.end() == package) {
            // Plugin missing - move on
            continue;
        }

        // Add package to pages list and remove from lookup map
        pages.push_back(std::make_pair(package->first, package->second));
        packages.erase(package);
    }

    // The remaining packages from the lookup map need to be added to the page list
    for (auto& package : packages) { pages.push_back(std::make_pair(package.first, package.second)); }
    packages.clear();

    for (size_t i = 0; i < pages.size(); ++i) {
        const auto& page = pages[i];

        auto* submenu = CreateSubmenuComponents(page.second);
        wxMenuItem* menuItem =
          new wxMenuItem(menuComponents, wxID_ANY, page.first, wxEmptyString, wxITEM_NORMAL, submenu);

        //        menuComponents->AppendSubMenu( submenu, page.first );
        menuItem->SetBitmap(page.second->GetPackageIcon());
        menuComponents->Append(menuItem);
    }

    return menuComponents;
}

wxMenu* MainFrame::CreateSubmenuComponents(PObjectPackage pkg)
{
    wxMenu* submenu = new wxMenu;
    unsigned int j = 0;

    while (j < pkg->GetObjectCount()) {
        PObjectInfo info = pkg->GetObjectInfo(j);

        if (info->IsStartOfGroup()) {
            submenu->AppendSeparator();
        }

        if (nullptr == info->GetComponent()) {
            LogDebug(
              _("Missing Component for Class \"" + info->GetClassName() + "\" of Package \"" + pkg->GetPackageName() +
                "\"."));
        } else {
            auto* item = new wxMenuItem(submenu, wxID_ANY, info->GetClassName());
            item->SetBitmap(info->GetIconFile());
            submenu->Append(item);
        }
        j++;
    }
    submenu->Bind(wxEVT_MENU, &MainFrame::OnMenuComponentsClick, this);

    return submenu;
}

wxToolBar* MainFrame::CreateFBToolBar()
{
    wxToolBar* toolbar = CreateToolBar();
    toolbar->SetToolBitmapSize(wxSize(AppBitmaps::GetPixelSize(AppBitmaps::Size::Tool), AppBitmaps::GetPixelSize(AppBitmaps::Size::Tool)));
    toolbar->AddTool(
      ID_NEW_PRJ, wxT("新项目"), AppBitmaps::GetBitmap(wxT("new"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_NORMAL,
      wxT("新项目 (Ctrl+N)"), wxT("开始一个新项目"));
    toolbar->AddTool(
      ID_OPEN_PRJ, wxT("打开项目"), AppBitmaps::GetBitmap(wxT("open"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_NORMAL,
      wxT("打开项目 (Ctrl+O)"), wxT("打开一个现有的项目"));
    toolbar->AddTool(
      ID_SAVE_PRJ, wxT("保存项目"), AppBitmaps::GetBitmap(wxT("save"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_NORMAL,
      wxT("保存项目 (Ctrl+S)"), wxT("保存当前项目"));
    toolbar->AddSeparator();
    toolbar->AddTool(
      ID_UNDO, wxT("撤消"), AppBitmaps::GetBitmap(wxT("undo"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_NORMAL,
      wxT("撤消 (Ctrl+Z)"), wxT("撤消上一个操作。"));
    toolbar->AddTool(
      ID_REDO, wxT("恢复"), AppBitmaps::GetBitmap(wxT("redo"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_NORMAL,
      wxT("恢复 (Ctrl+Y)"), wxT("恢复上次取消的操作。"));
    toolbar->AddSeparator();
    toolbar->AddTool(
      ID_CUT, wxT("剪切"), AppBitmaps::GetBitmap(wxT("cut"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_NORMAL,
      wxT("剪切 (Ctrl+X)"), wxT("删除所选对象并将其放置在剪贴板上。"));
    toolbar->AddTool(
      ID_COPY, wxT("复制"), AppBitmaps::GetBitmap(wxT("copy"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_NORMAL,
      wxT("复制 (Ctrl+C)"), wxT("将所选对象复制到剪贴板"));
    toolbar->AddTool(
      ID_PASTE, wxT("粘贴"), AppBitmaps::GetBitmap(wxT("paste"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_NORMAL,
      wxT("粘贴 (Ctrl+V)"), wxT("从剪贴板插入一个对象"));
    toolbar->AddTool(
      ID_DELETE, wxT("删除"), AppBitmaps::GetBitmap(wxT("delete"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_NORMAL,
      wxT("删除 (Ctrl+D)"), wxT("删除选定对象"));
    toolbar->AddSeparator();
    toolbar->AddTool(
      ID_GENERATE_CODE, wxT("生成代码"), AppBitmaps::GetBitmap(wxT("generate"), AppBitmaps::Size::Tool), wxNullBitmap,
      wxITEM_NORMAL, wxT("生成代码 (F8)"), wxT("“从当前项目创建代码"));
    toolbar->AddSeparator();
    toolbar->AddTool(
      ID_ALIGN_LEFT, wxT(""), AppBitmaps::GetBitmap(wxT("lalign"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_CHECK,
      wxT("向左对齐"), wxT("该项目将对齐到由Sizer分配给它的空间的左边。"));
    toolbar->AddTool(
      ID_ALIGN_CENTER_H, wxT(""), AppBitmaps::GetBitmap(wxT("chalign"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_CHECK,
      wxT("水平居中对齐"), wxT("该项目将水平居中在Sizer分配给它的空间中"));
    toolbar->AddTool(
      ID_ALIGN_RIGHT, wxT(""), AppBitmaps::GetBitmap(wxT("ralign"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_CHECK,
      wxT("向右对齐"), wxT("该项目将对齐到由Sizer分配给它的空间的右边"));
    toolbar->AddSeparator();
    toolbar->AddTool(
      ID_ALIGN_TOP, wxT(""), AppBitmaps::GetBitmap(wxT("talign"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_CHECK,
      wxT("向上对齐"), wxT("该项目将对齐到由Sizer分配给它的空间的顶部"));
    toolbar->AddTool(
      ID_ALIGN_CENTER_V, wxT(""), AppBitmaps::GetBitmap(wxT("cvalign"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_CHECK,
      wxT("垂直居中对齐"), wxT("该项目将垂直居中在Sizer分配给它的空间中"));
    toolbar->AddTool(
      ID_ALIGN_BOTTOM, wxT(""), AppBitmaps::GetBitmap(wxT("balign"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_CHECK,
      wxT("底部对齐"), wxT("该项目将对齐到由Sizer分配给它的空间的底部"));
    toolbar->AddSeparator();
    toolbar->AddTool(
      ID_EXPAND, wxT(""), AppBitmaps::GetBitmap(wxT("expand"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_CHECK,
      wxT("展开 (Alt+W)"), wxT("项目将被展开以填充分配给该项的空间。"));
    toolbar->AddTool(
      ID_STRETCH, wxT(""), AppBitmaps::GetBitmap(wxT("stretch"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_CHECK,
      wxT("拉伸 (Alt+S)"), wxT("该物品将随着Sizer变大和缩小."));
    toolbar->AddSeparator();
    toolbar->AddTool(
      ID_BORDER_LEFT, wxT(""), AppBitmaps::GetBitmap(wxT("left"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_CHECK,
      wxT("左边框"), wxT("将在项目的左侧添加边框。"));
    toolbar->AddTool(
      ID_BORDER_RIGHT, wxT(""), AppBitmaps::GetBitmap(wxT("right"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_CHECK,
      wxT("右边框"), wxT("将在项目的右侧添加边框。"));
    toolbar->AddTool(
      ID_BORDER_TOP, wxT(""), AppBitmaps::GetBitmap(wxT("top"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_CHECK,
      wxT("上边框"), wxT("将在项目的上部添加边框。"));
    toolbar->AddTool(
      ID_BORDER_BOTTOM, wxT(""), AppBitmaps::GetBitmap(wxT("bottom"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_CHECK,
      wxT("下边框"), wxT("将在项目的下部添加边框。"));
    toolbar->AddSeparator();
    toolbar->AddTool(
      ID_WINDOW_SWAP, wxT(""), AppBitmaps::GetBitmap(wxT("swap"), AppBitmaps::Size::Tool), wxNullBitmap, wxITEM_NORMAL,
      wxT("交换编辑器和属性窗口 (F12)"), wxT("交换设计窗口和属性窗口。"));
    toolbar->Realize();

    return toolbar;
}

wxWindow* MainFrame::CreateDesignerWindow(wxWindow* parent)
{
    m_notebook = new wxAuiNotebook(parent, ID_EDITOR_FNB, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TOP);
    m_notebook->SetArtProvider(new AuiTabArt());

    m_visualEdit = new VisualEditor(m_notebook);
    AppData()->GetManager()->SetVisualEditor(m_visualEdit);

    m_notebook->InsertPage(0, m_visualEdit, wxT("Designer"), false);
    m_notebook->SetPageBitmap(0, AppBitmaps::GetBitmap(wxT("designer"), AppBitmaps::Size::Icon_Medium));

    m_cpp = new CppPanel(m_notebook, wxID_ANY);
    m_notebook->InsertPage(1, m_cpp, wxT("C++"), false);
    m_notebook->SetPageBitmap(1, AppBitmaps::GetBitmap(wxT("c++"), AppBitmaps::Size::Icon_Medium));

    m_python = new PythonPanel(m_notebook, wxID_ANY);
    m_notebook->InsertPage(2, m_python, wxT("Python"), false);
    m_notebook->SetPageBitmap(2, AppBitmaps::GetBitmap(wxT("python"), AppBitmaps::Size::Icon_Medium));

    m_php = new PHPPanel(m_notebook, wxID_ANY);
    m_notebook->InsertPage(3, m_php, wxT("PHP"), false);
    m_notebook->SetPageBitmap(3, AppBitmaps::GetBitmap(wxT("php"), AppBitmaps::Size::Icon_Medium));

    m_lua = new LuaPanel(m_notebook, wxID_ANY);
    m_notebook->InsertPage(4, m_lua, wxT("Lua"), false);
    m_notebook->SetPageBitmap(4, AppBitmaps::GetBitmap(wxT("lua"), AppBitmaps::Size::Icon_Medium));

    m_xrc = new XrcPanel(m_notebook, wxID_ANY);
    m_notebook->InsertPage(5, m_xrc, wxT("XRC"), false);
    m_notebook->SetPageBitmap(5, AppBitmaps::GetBitmap(wxT("xrc"), AppBitmaps::Size::Icon_Medium));

    return m_notebook;
}

wxWindow* MainFrame::CreateComponentPalette(wxWindow* parent)
{
    // la paleta de componentes, no es un observador propiamente dicho, ya
    // que no responde ante los eventos de la aplicación
    m_palette = new wxFbPalette(parent, wxID_ANY);
    m_palette->Create();
    // m_palette->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DFACE ) );

    return m_palette;
}

wxWindow* MainFrame::CreateObjectTree(wxWindow* parent)
{
    m_objTree = new ObjectTree(parent, wxID_ANY);
    m_objTree->Create();

    return m_objTree;
}

wxWindow* MainFrame::CreateObjectInspector(wxWindow* parent)
{
    // TO-DO: make object inspector style selectable.
    int style = (m_style == wxFB_CLASSIC_GUI ? wxFB_OI_MULTIPAGE_STYLE : wxFB_OI_SINGLE_PAGE_STYLE);
    m_objInsp = new ObjectInspector(parent, wxID_ANY, style);
    return m_objInsp;
}

void MainFrame::CreateWideGui()
{
    // MainFrame only contains m_leftSplitter window
    m_leftSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE);

    wxWindow* objectTree = Title::CreateTitle(CreateObjectTree(m_leftSplitter), wxT("对象树"));

    // panel1 contains Palette and splitter2 (m_rightSplitter)
    wxPanel* panel1 = new wxPanel(m_leftSplitter, wxID_ANY);

    wxWindow* palette = Title::CreateTitle(CreateComponentPalette(panel1), wxT("组件面板"));
    m_rightSplitter = new wxSplitterWindow(panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE);

    wxBoxSizer* panel1_sizer = new wxBoxSizer(wxVERTICAL);
    panel1_sizer->Add(palette, 0, wxEXPAND);
    panel1_sizer->Add(m_rightSplitter, 1, wxEXPAND);
    panel1->SetSizer(panel1_sizer);

    // splitter2 contains the editor and the object inspector
    wxWindow* designer = Title::CreateTitle(CreateDesignerWindow(m_rightSplitter), wxT("编辑器"));
    wxWindow* objectInspector = Title::CreateTitle(CreateObjectInspector(m_rightSplitter), wxT("对象属性"));

    m_leftSplitter->SplitVertically(objectTree, panel1, m_leftSplitterWidth);

    // Need to update the left splitter so the right one is drawn correctly
    wxSizeEvent update(GetSize(), GetId());
    ProcessEvent(update);
    m_leftSplitter->UpdateSize();
    m_leftSplitter->SetMinimumPaneSize(2);

    // modified by tyysoft to restore the last layout.
    if (m_rightSplitterType == _("editor")) {
        m_rightSplitter->SplitVertically(designer, objectInspector, m_rightSplitterWidth);
    } else {
        m_rightSplitter->SplitVertically(objectInspector, designer, m_rightSplitterWidth);
    }
    m_rightSplitter->SetSashGravity(1);
    m_rightSplitter->SetMinimumPaneSize(2);

    m_style = wxFB_WIDE_GUI;

    SetMinSize(wxSize(700, 380));
}

void MainFrame::CreateClassicGui()
{
    // Give ID to left splitter
    // m_leftSplitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE );
    m_leftSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE);
    m_rightSplitter =
      new wxSplitterWindow(m_leftSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE);
    wxWindow* objectTree = Title::CreateTitle(CreateObjectTree(m_rightSplitter), wxT("对象树"));
    wxWindow* objectInspector = Title::CreateTitle(CreateObjectInspector(m_rightSplitter), wxT("对象属性"));

    // panel1 contains palette and designer
    wxPanel* panel1 = new wxPanel(m_leftSplitter, wxID_ANY);

    wxWindow* palette = Title::CreateTitle(CreateComponentPalette(panel1), wxT("组件面板"));
    wxWindow* designer = Title::CreateTitle(CreateDesignerWindow(panel1), wxT("编辑器"));

    wxBoxSizer* panel1_sizer = new wxBoxSizer(wxVERTICAL);
    panel1_sizer->Add(palette, 0, wxEXPAND);
    panel1_sizer->Add(designer, 1, wxEXPAND);
    panel1->SetSizer(panel1_sizer);

    m_leftSplitter->SplitVertically(m_rightSplitter, panel1, m_leftSplitterWidth);

    // Need to update the left splitter so the right one is drawn correctly
    wxSizeEvent update(GetSize(), GetId());
    ProcessEvent(update);
    m_leftSplitter->UpdateSize();

    m_rightSplitter->SplitHorizontally(objectTree, objectInspector, m_rightSplitterWidth);
    m_rightSplitter->SetMinimumPaneSize(2);

    SetMinSize(wxSize(700, 465));
}

void MainFrame::OnIdle(wxIdleEvent&)
{
    if (m_leftSplitter) {
        m_leftSplitter->SetSashPosition(m_leftSplitterWidth);
    }

    if (m_rightSplitter) {
        m_rightSplitter->SetSashPosition(m_rightSplitterWidth);
    }

    Disconnect(wxEVT_IDLE, wxIdleEventHandler(MainFrame::OnIdle));

    if (m_autoSash) {
        // Init. m_rightSplitter_sash_pos
        m_rightSplitter_sash_pos = m_rightSplitter->GetSashPosition();
        m_rightSplitter->Connect(
          wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, wxSplitterEventHandler(MainFrame::OnSplitterChanged));
    }
}

void MainFrame::OnSplitterChanged(wxSplitterEvent& event)
{
    LogDebug(wxT("MainFrame::OnSplitterChanged > pos = %d"), event.GetSashPosition());

    // update position
    m_rightSplitter_sash_pos = event.GetSashPosition();
}

void MainFrame::OnWindowSwap(wxCommandEvent&)
{
    wxWindow* win1 = m_rightSplitter->GetWindow1();
    wxWindow* win2 = m_rightSplitter->GetWindow2();

    int pos = m_rightSplitter->GetSashPosition();
    wxSize sz = m_rightSplitter->GetClientSize();

    m_rightSplitter->Unsplit();
    m_rightSplitter->SplitVertically(win2, win1);

    m_rightSplitter->SetSashPosition(sz.GetWidth() - pos);
}

void MainFrame::OnFindComponent(wxCommandEvent& WXUNUSED(event))
{
    DialogFindComponent dlg(this);
    if (dlg.ShowModal() == wxID_OK) {
        AppData()->CreateObject(dlg.GetSelected());
    }
}

void MainFrame::OnMenuComponentsClick(wxCommandEvent& e)
{
    wxMenu* menu = static_cast<wxMenu*>(e.GetEventObject());
    wxMenuItem* item = menu->FindChildItem(e.GetId());

    AppData()->CreateObject(item->GetItemLabelText());
}
