// Dear ImGui: standalone example application for DirectX 9
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "../ImGui/imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include <iostream>
#include "../Header/CreateAndSearchCatalog.h"
#include "../Header/User.h"
#include "../Header/issueBook.h"

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)"); // readonly text 
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
// Main code
int main(int, char**)
{
    
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX9 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        {
            ImVec2 buttonSize = ImVec2(ImGui::GetFontSize() * 20, 20);

            ImGuiIO& jf = ImGui::GetIO();
            jf.ConfigInputTextCursorBlink = true;

            static std::tuple<std::string, unsigned long long> requiredUser;
            static bool logRegOpen = true;
            static bool regOpen = false;
            static bool loginOpen = false;
            static bool displayId = false;
            static bool infoWin = true;
            static bool errorWin = false;
            static bool closeLogReg = false;
            static bool loginSuccessfull = false;
            static bool openSearchBox = false;
            static char firstName[100] = "";
            static char lastName[100] = "";
            static char userIdChar[12] = "";
            static std::string userIdString= "";
            static unsigned long long userIdInt = 0;
            static unsigned long long id = 0;
            
            

            if (logRegOpen)
            {
                ImGui::Begin("Login/Register");
                ImGui::Button("Register", buttonSize);
                if (ImGui::IsItemClicked(0))
                {
                    regOpen = !regOpen;
                }
                if (regOpen)
                {
                    ImGui::Begin("Register Portal", &regOpen);
                    ImGui::InputTextWithHint("First Name", "Enter your first name", firstName, IM_ARRAYSIZE(firstName));
                    ImGui::InputTextWithHint("Last Name", "Enter your last name", lastName, IM_ARRAYSIZE(lastName));
                    ImGui::Button("Submit", buttonSize);
                    if (ImGui::IsItemClicked(0))
                    {
                        User hello = User(false, firstName, lastName);
                        id = hello.addUserToDatabase();
                        regOpen = false;
                        displayId = true;
                        infoWin = true;
                    }

                    ImGui::End();
                }
                if (infoWin && displayId)
                {
                    ImGui::Begin("UserId");
                    ImGui::Text("This is your user id %llu", id);
                    ImGui::Button("Close", buttonSize);
                    if (ImGui::IsItemClicked(0))
                    {
                        infoWin = false;
                    }

                    ImGui::End();
                }

                ImGui::Button("Login", buttonSize);
                if (ImGui::IsItemClicked(0))
                {
                    loginOpen = !loginOpen;
                }
                if (loginOpen)
                {
                    ImGui::Begin("Login Portal");
                    ImGui::InputTextWithHint("First Name", "Enter your first name", firstName, IM_ARRAYSIZE(firstName));
                    ImGui::InputTextWithHint("Last Name", "Enter your last name", lastName, IM_ARRAYSIZE(lastName));
                    ImGui::InputTextWithHint("Library ID", "Enter your library Id", userIdChar, IM_ARRAYSIZE(userIdChar), ImGuiInputTextFlags_Password);
                    userIdString = std::string(userIdChar);
                    if (!userIdString.empty())
                    {
                        userIdInt = std::stoull(userIdString);
                    }
                    
                    ImGui::Button("Submit", buttonSize);
                    if (ImGui::IsItemClicked(0))
                    {
                        User welcome = User(true, firstName, lastName, userIdInt);
                        requiredUser = welcome.findUserInDatabase();
                        loginOpen = false;
                        loginSuccessfull = true;
                        errorWin = false;

                    }
                    ImGui::End();
                }
                if (std::get<0>(requiredUser) == std::string("false"))
                {
                    if (!errorWin)
                    {
                        loginSuccessfull = false;
                        ImGui::Begin("Error");
                        ImGui::Text("Cannot find the user, please register");
                        ImGui::Button("Close", buttonSize);
                        if (ImGui::IsItemClicked(0))
                        {
                            errorWin = true;
                        }
                        ImGui::End();
                    }
                }
                else if(std::get<0>(requiredUser) == firstName && std::get<1>(requiredUser) == userIdInt && loginSuccessfull)
                {
                    closeLogReg = true;
                }
                if (closeLogReg)
                {
                    logRegOpen = false;
                    openSearchBox = true;
                }
                
                ImGui::End();
            }

            if (openSearchBox)
            {
                static std::tuple<std::vector<std::string>, std::vector<int>> catalog{};
                static std::vector<std::string> searchResult{};
                static bool displayResult = false;
                static int partialCurrent = 0;
                static int perfectCurrent = 0;
                static bool firstRun = false;
                static bool loadData = true;
                static bool confirmationWin = false;
                static std::string requiredBook = "";
                ImGui::Begin("Library");
                static char bookName[1000] = "";
                ImGui::SameLine();
                ImGui::Button("Create Catalog", buttonSize);
                if (ImGui::IsItemClicked(0))
                {
                    catalog = CreateAndSearchCatalog::checkData(true);
                    loadData = false;
                }
                ImGui::SameLine(); HelpMarker("Click this if this is your first time running the program or catalog file doesn't exsist");
                ImGui::SameLine();
                ImGui::Button("Clear Catalog", buttonSize);
                if (ImGui::IsItemClicked(0))
                {
                    CreateAndSearchCatalog::clearCatalog();
                }
                ImGui::SameLine(); HelpMarker("This will clear everything in the catalog file. Please click create catalog after pressing this otherwise the program will not work");

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::InputTextWithHint("Book Name", "Enter the name of the book you would like to search", bookName, IM_ARRAYSIZE(bookName));
                ImGui::Button("Submit", buttonSize);
                if (ImGui::IsItemClicked(0))
                {
                    if (loadData)
                    {
                        catalog = CreateAndSearchCatalog::checkData(false);
                    }
                    CreateAndSearchCatalog user = CreateAndSearchCatalog(bookName);
                    std::cout << std::get<0>(catalog).size();
                    searchResult = user.searchBook(&catalog);
                    displayResult = true;
                    std::cout << "ok";
                }
                if (displayResult)
                {
                    static int item_current_idx = 0; // Here we store our selection data as an index.
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Spacing();
                    ImGui::Spacing();
                    if (ImGui::BeginListBox("Search Results"))
                    {
                        for (int n = 0; n < searchResult.size(); n++)
                        {
                            const bool is_selected = (item_current_idx == n);
                            if (ImGui::Selectable(searchResult.at(n).c_str(), is_selected))
                                item_current_idx = n;

                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndListBox();
                    }
                    ImGui::Button("Confirm", buttonSize);
                    if (ImGui::IsItemClicked(0))
                    {
                        requiredBook = searchResult.at(item_current_idx);
                        IssueBook final = IssueBook();
                        final.finalizeProcess(&requiredUser, requiredBook);
                        confirmationWin = true;
                    }
                    if (confirmationWin)
                    {
                        ImGui::Begin("Confirmation");
                        ImGui::Text("Book Issued Successfully");
                        ImGui::Button("Close", buttonSize);
                        if (ImGui::IsItemClicked(0))
                        {
                            confirmationWin = false;
                        }
                        ImGui::End();
                    }
                }
                //if (ImGui::Button("Search", buttonSize))
                //{
                //    CreateAndSearchCatalog user = CreateAndSearchCatalog(bookName);
                //    searchResult = user.searchBook(&catalog);
                //    perfectMatchVec = std::get<0>(searchResult);
                //    partialMatchVec = std::get<1>(searchResult);
                //    static int item_current_idx = 0; // Here we store our selection data as an index.
                //    if (ImGui::BeginListBox("listbox 1"))
                //    {
                //        for (int n = 0; n < perfectMatchVec.size(); n++)
                //        {
                //            const bool is_selected = (item_current_idx == n);
                //            if (ImGui::Selectable(perfectMatchVec.at(n).c_str(), is_selected))
                //                item_current_idx = n;
                //
                //            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                //            if (is_selected)
                //                ImGui::SetItemDefaultFocus();
                //        }
                //        ImGui::EndListBox();
                //    }
                //    
                //}



                ImGui::End();
            }
            


            //ImGui::Begin("Login/Register", &logRegOpen);
            //if (ImGui::Button("Register", buttonSize))
            //{
            //    
            //    
            //}
            //
            //else if (ImGui::Button("Login", buttonSize))
            //{
            //    ImGui::Begin("Login Portal", &loginOpen);
            //    ImGui::InputTextWithHint("First Name", "Enter your first name", firstName, IM_ARRAYSIZE(firstName));
            //    ImGui::InputTextWithHint("Last Name", "Enter your last name", lastName, IM_ARRAYSIZE(lastName));
            //    ImGui::InputTextWithHint("Library ID", "Enter your library Id", userIdChar, IM_ARRAYSIZE(userIdChar), ImGuiInputTextFlags_Password);
            //    std::string userIdString(userIdChar);
            //    if (ImGui::Button("Submit", buttonSize))
            //    {
            //        unsigned long long userId = stoull(userIdString);
            //        ImGui::End();
            //    }
            //    
            //}
            //ImGui::End();

            
            

            
            
        }

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
