#include "StdAfx.h"
#include "Gui.h"
#include "DllBytes.h"

#include "Resources/Fonts/IconsFontAwesome6.h"
#include "Resources/Fonts/IconsFontAwesome6Brands.h"
#include "Resources/Fonts/MuseoSansCyrl500.h"


#include "auth/auth.hpp"
#include "ThemidaSDK/Include/C/ThemidaSDK.h"
//#include "utils/skCrypt.h"
#include <pasted&Detected.h>

using namespace KeyAuth;
std::string name = skCrypt("TEst!2314").decrypt();
std::string ownerid = skCrypt("XWD4lpNnUF").decrypt();
std::string secret = skCrypt("bed232ee901c175e10282455d0b4f946aee004e1520abd52d3dbeb086dfc23eb").decrypt();
std::string version = skCrypt("1.0").decrypt();
std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt(); // change if you're self-hosting

api KeyAuthApp(name, ownerid, secret, version, url);

std::string tm_to_readable_time(tm ctx) {
    char buffer[80];

    strftime(buffer, sizeof(buffer), "%m/%d/%y %H:%M:%S", &ctx);

    return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
    auto cv = strtol(timestamp.c_str(), NULL, 10); // long

    return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
    std::tm context;

    localtime_s(&context, &timestamp);

    return context;
}

__forceinline void setup_auth()
{
    

   // name.clear(); ownerid.clear(); secret.clear(); version.clear(); url.clear();

    KeyAuthApp.init();
    if (!KeyAuthApp.data.success)
    {
        MessageBox(0, KeyAuthApp.data.message.c_str(), "", MB_ICONASTERISK);
       // std::cout << KeyAuthApp.data.message << std::endl;
        LI_FN(Sleep)(1500);
      //  LI_FN(exit)(1);
    }
    
}

void Gui::Init( HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* device_context )
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL; // Remove the imgui.ini
    GImGui->NavDisableHighlight = true;

    ImGui::GetDefaultFont();
    Customize();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(device, device_context);
}

void Gui::Customize( void )
{
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    style.WindowRounding    = 6;
    style.ChildRounding     = 3;
    style.FrameRounding     = 3;
    style.PopupRounding     = 3;
    style.GrabRounding      = 3;
    style.TabRounding       = 3;
    style.ScrollbarRounding = 3;
    style.ButtonTextAlign   = { 0.5f, 0.5f };
    style.WindowTitleAlign  = { 0.5f, 0.5f };
    style.ItemSpacing       = { 8.0f, 8.0f };
    style.WindowPadding     = { 16.0f, 16.0f };
    style.FramePadding      = { 8.0f, 8.0f };
    style.WindowBorderSize  = 0;
    style.FrameBorderSize   = 0;
    style.ScrollbarSize     = 12.f;
    style.GrabMinSize       = 8.f;
    style.WindowShadowSize  = 0;

    // Font Touch
    style.Alpha             = 1.0f;
    style.DisabledAlpha     = 1.0f;
    
    style.Colors[ImGuiCol_WindowShadow]         = ImAdd::Hex2RGBA(0x0E0E0E, 0.0f);

    style.Colors[ImGuiCol_WindowBg]             = ImAdd::Hex2RGBA(0x0E0E0E, 1.0f);
    style.Colors[ImGuiCol_ChildBg]              = ImAdd::Hex2RGBA(0x181818, 1.0f);
    style.Colors[ImGuiCol_PopupBg]              = ImAdd::Hex2RGBA(0x121212, 1.0f);

    style.Colors[ImGuiCol_TitleBg]              = ImAdd::Hex2RGBA(0x121212, 0.7f);
    style.Colors[ImGuiCol_TitleBgActive]        = ImAdd::Hex2RGBA(0x121212, 0.3f);
    style.Colors[ImGuiCol_TitleBgCollapsed]     = ImAdd::Hex2RGBA(0x121212, 0.5f);

    style.Colors[ImGuiCol_PlotHistogram]        = ImAdd::Hex2RGBA(0xD6E2E0, 1.0f);
    style.Colors[ImGuiCol_SliderGrab]           = ImAdd::Hex2RGBA(0x977732, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive]     = ImAdd::Hex2RGBA(0x212121, 1.0f);
    
    style.Colors[ImGuiCol_CheckMark]            = ImAdd::Hex2RGBA(0xfafafa, 1.0f);
    style.Colors[ImGuiCol_Text]                 = ImAdd::Hex2RGBA(0xfafafa, 1.0f);
    style.Colors[ImGuiCol_TextDisabled]         = ImAdd::Hex2RGBA(0x808080, 1.0f);

    style.Colors[ImGuiCol_Border]               = ImAdd::Hex2RGBA(0xD6E2E0, 1.0f);
    style.Colors[ImGuiCol_Separator]            = ImAdd::Hex2RGBA(0x2b2b2b, 1.0f);

    style.Colors[ImGuiCol_Header]               = ImAdd::Hex2RGBA(0x212121, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered]        = ImAdd::Hex2RGBA(0x444444, 1.0f);
    style.Colors[ImGuiCol_HeaderActive]         = ImAdd::Hex2RGBA(0x363636, 1.0f);
    
    style.Colors[ImGuiCol_Button]               = ImAdd::Hex2RGBA(0xD6E2E0, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered]        = ImAdd::Hex2RGBA(0xD6E2E0, 0.7f);
    style.Colors[ImGuiCol_ButtonActive]         = ImAdd::Hex2RGBA(0xD6E2E0, 0.5f);

    style.Colors[ImGuiCol_FrameBg]              = ImAdd::Hex2RGBA(0x131313, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered]       = ImAdd::Hex2RGBA(0x444444, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive]        = ImAdd::Hex2RGBA(0x363636, 1.0f);


    pFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(MuseoSansCyrl500_compressed_data, MuseoSansCyrl500_compressed_size, 14, NULL, ImGui::GetIO().Fonts->GetGlyphRangesDefault());
    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    static const ImWchar icons_ranges_brands[] = { ICON_MIN_FAB, ICON_MAX_16_FAB, 0 };
    ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
    FontAwesome = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14.f, &icons_config, icons_ranges);
    FontAwesomeBrands = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(fa_brands_400_compressed_data, fa_brands_400_compressed_size, 14.f, &icons_config, icons_ranges_brands);

    LogoFont = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 18.f, nullptr, icons_ranges);
    BiggerFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(MuseoSansCyrl500_compressed_data, MuseoSansCyrl500_compressed_size, 18, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesDefault());
}

void Gui::DrawLogo(ImVec2 size)
{
    auto pos = ImGui::GetCursorPos();
    auto drawlist = ImGui::GetWindowDrawList();
    drawlist->AddShadowRect(pos, pos + size, ImGui::GetColorU32(ImGui::GetColorU32(ImGuiCol_Border)), 40, ImVec2(0, 0));

    ImGui::PushFont(LogoFont);
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_ChildBg));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_Border));
        {
            ImGui::BeginChild(skCrypt("Logo").decrypt(), size, false);
            {
                static const char* icon = ICON_FA_SKULL;
                ImGui::SetCursorPos(size / 2 - ImGui::CalcTextSize(icon) / 2);
                ImGui::Text(icon);
            }
            ImGui::EndChild();
        }
        ImGui::PopStyleColor(2);
    }
    ImGui::PopFont();
}

void Gui::DrawCloseButton()
{
    if (ImAdd::CloseButton(ImGui::GetCurrentWindow()->GetID(skCrypt("#CLOSE").decrypt()), ImGui::GetWindowPos() + ImVec2(ImGui::GetWindowSize().x - ImGui::GetFrameHeight(), 0)))
    {
        Window::bIsRunning = false;
    }
}
std::string expiry;
std::string sub_type;


void CheatInjection()
{
    
    if (LI_FN(strcmp)(sub_type.c_str(), skCrypt("Rust").decrypt()) == 0) { // legend was here 

        //std::vector<std::uint8_t> RustBytes = KeyAuthApp.download("Rust_DLL");
        if (RustBytes.empty())
        {
            LI_FN(exit)(9);
        }
        else
        {
            LI_FN(Sleep)(2500);
            loadDll(RustBytes, (L"RustClient.exe")); // Do not spoof return will cause bricked pc and other shit ect
        }
    }
    else if (LI_FN(strcmp)(sub_type.c_str(), skCrypt("Valorant").decrypt()) == 1) {
        // val injection
    }
    else if (LI_FN(strcmp)(sub_type.c_str(), skCrypt("Apex").decrypt()) == 0) {
        std::vector<std::uint8_t> ApexBytes = KeyAuthApp.download("Apex_DLL");
        if (ApexBytes.empty())
        {
            LI_FN(exit)(9);
        }
        else
        {
            LI_FN(Sleep)(2500);
            loadDll(ApexBytes, (L"r5apex.exe")); // Do not spoof return will cause bricked pc and other shit ect  
        }
    }
    else if (LI_FN(strcmp)(sub_type.c_str(), skCrypt("Tarkov").decrypt()) == 2) {
        std::vector<std::uint8_t> TarkovBytes = KeyAuthApp.download(skCrypt("291444").decrypt());
        if (!KeyAuthApp.data.success)
        {
        //    LI_FN(MessageBox)(0, skCrypt("Error file #191").decrypt(), skCrypt("Contact support").decrypt(), MB_ICONASTERISK);
            LI_FN(SleepEx)(3000, false);
            LI_FN(exit)(0);
        }
        map_file("Tarkov", TarkovBytes);
        LI_FN(Sleep)(2500);
        LI_FN(exit)(0);
    }
    else if (LI_FN(strcmp)(sub_type.c_str(), skCrypt("Fortnite").decrypt()) == 3) {
        // fortnite injection
    }
    else if (LI_FN(strcmp)(sub_type.c_str(), skCrypt("Spoofer").decrypt()) == 4) {

        std::vector<std::uint8_t> SpooferBytes = KeyAuthApp.download(skCrypt("881430").decrypt());
        if (!KeyAuthApp.data.success)
        {
         //   LI_FN(MessageBox)(0, skCrypt("Error file #191").decrypt(), skCrypt("Contact support").decrypt(), MB_ICONASTERISK);
            LI_FN(SleepEx)(3000, false);
            LI_FN(exit)(0);
        }

        map_file("Spoofer", SpooferBytes);
        LI_FN(Sleep)(2500);
        LI_FN(exit)(0);
    }
    
    //std::vector<std::uint8_t> RustBytes = KeyAuthApp.download("Rust_DLL");
}

void Gui::DrawInjectButton()
{
    
    if (g_Status.m_bIsInjected)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_Border));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
        ImGui::Button(ICON_FA_CHECK "##Launched", ImVec2(-0.1, 30));
        ImGui::PopStyleColor(3);
    }
    else
    {
        if (!g_Status.m_bIsInjecting)
        {
            if (sub_type.empty())
            {
                LI_FN(Sleep)(5000);
                LI_FN(exit)(3);
            }

            if (expiry.empty())
            {
                LI_FN(Sleep)(5000);
                LI_FN(exit)(3);
            }

            if (ImAdd::Button(skCrypt("Launch").decrypt(), ImVec2(-0.1, 30)))
            {
                g_Status.m_bIsInjecting = true;
            }
        }
        else
        {
           
            
            CheatInjection(); // would be nice to include a bool in CheatInjection after it injects set it to true to get the loading times synced up
                              // and put it here if (InjectorTimer == InjectorWaitTimer)


            static float InjectorTimer;
            static float InjectorWaitTimer = 2;
            InjectorTimer = min(InjectorTimer + ImGui::GetIO().DeltaTime * 1.0f, InjectorWaitTimer);

            ImGui::ProgressBar(InjectorTimer / InjectorWaitTimer, ImVec2(-0.1, 30), "");

            if (InjectorTimer == InjectorWaitTimer)
            {
                g_Status.m_bIsInjected = true;
                g_Status.m_bIsInjecting = false;
            }
        }
    }
    
}

void Gui::DrawSignInButton()
{

}
static bool has_init = false;
static bool login_once = false;
static bool hasUserdata = false;


void Gui::Render( void )
{
    
    if (!has_init)
    {
        setup_auth();
        has_init = true;
    }

    if (has_init)
    {
        // Tools
        ImGuiStyle& style = ImGui::GetStyle();

        // Window Infos
        float TitleBarHeight = 90;

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            ImGuiWindowFlags main_window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(Window::WindowSize.x, Window::WindowSize.y));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("GUI TITLE", &Window::bIsRunning, main_window_flags);
            ImGui::PopStyleVar(2);
            {
                // ImGui Settings
                GImGui->NavDisableHighlight = true;

                // Base Colors
                ImColor shadow_col = ImGui::GetColorU32(ImGuiCol_WindowBg);
                ImColor shadow_col_null = shadow_col;
                shadow_col_null.Value.w = 0;

                // Content
                if (g_Status.m_bIsLogged == true)
                {
                    if (!hasUserdata)
                    {
                        for (int i = 0; i < KeyAuthApp.data.subscriptions.size(); i++) {
                            auto sub = KeyAuthApp.data.subscriptions.at(i);

                            sub_type = "Rust";
                            expiry = tm_to_readable_time(timet_to_tm(string_to_timet(sub.expiry)));
                        }

                        if (sub_type.empty())
                        {
                            LI_FN(Sleep)(5000);
                            LI_FN(exit)(3);
                        }

                        if (expiry.empty())
                        {
                            LI_FN(Sleep)(5000);
                            LI_FN(exit)(3);
                        }

                        if (LI_FN(strcmp)(sub_type.c_str(), skCrypt("Rust").decrypt()) == 0 ) {
                            SelectedGame = 2;
                        }
                        else if (LI_FN(strcmp)(sub_type.c_str(), skCrypt("Valorant").decrypt()) == 0) {
                            SelectedGame = 1;
                        }
                        else if (LI_FN(strcmp)(sub_type.c_str(), skCrypt("Apex").decrypt()) == 0) {
                            SelectedGame = 3;
                        }
                        else if (LI_FN(strcmp)(sub_type.c_str(), skCrypt("Tarkov").decrypt()) == 0) {
                            SelectedGame = 4;
                        }
                        else if (LI_FN(strcmp)(sub_type.c_str(), skCrypt("Fortnite").decrypt()) == 0) {
                            SelectedGame = 5;
                        }
                        else if (LI_FN(strcmp)(sub_type.c_str(), skCrypt("Spoofer").decrypt()) == 0) {
                            SelectedGame = 6;
                        }
                        else if (LI_FN(strcmp)(sub_type.c_str(), skCrypt("DayZ").decrypt()) == 0) {
                            SelectedGame = 7;
                        }
                        else if (LI_FN(strcmp)(sub_type.c_str(), skCrypt("Default").decrypt()) == 0) {
                            SelectedGame = 0;
                            hasUserdata = false;
                        }

                        hasUserdata = true;
                    }

                    if (KeyAuthApp.data.success && hasUserdata)
                    {
                        ImGui::BeginChild(skCrypt("Banner").decrypt(), ImVec2(0, TitleBarHeight), false, ImGuiWindowFlags_NoBackground);
                        {
                            auto pos = ImGui::GetWindowPos();
                            auto size = ImGui::GetWindowSize();
                            auto drawlist = ImGui::GetWindowDrawList();

                            // Renders
                            drawlist->AddImageRounded(Textures::tBanner, pos, pos + ImVec2(size.x, TitleBarHeight), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255), style.WindowRounding, ImDrawFlags_RoundCornersTop);
                            drawlist->AddRectFilledMultiColor(pos + ImVec2(0, TitleBarHeight - fShadowSize), pos + ImVec2(size.x, TitleBarHeight), shadow_col_null, shadow_col_null, shadow_col, shadow_col);

                            // Welcome Message
                            ImGui::PushFont(BiggerFont);
                            {
                                ImGui::SetCursorPos(ImVec2(size.y / 2 - ImGui::GetFontSize() / 2, size.y / 2 - ImGui::GetFontSize() / 2));
                                ImGui::Text(skCrypt("Welcome, nice to see you.").decrypt());
                            }
                            ImGui::PopFont();

                            DrawCloseButton();
                        }
                        ImGui::EndChild();
                        ImGui::SetCursorPosY(TitleBarHeight);
                        ImGui::BeginGroup();
                        {
                            float GroupWidth = (ImGui::GetWindowWidth() - style.WindowPadding.x) / 2 + style.WindowPadding.x;
                            ImGui::BeginChild(skCrypt("LeftSide##GameSelection").decrypt(), ImVec2(GroupWidth, 0), true, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
                            {
                                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.WindowRounding);
                                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
                                {
                                    ImGui::BeginDisabled(g_Status.m_bIsInjecting);
                                    {

                                        switch (SelectedGame)
                                        {
                                            case 1:
                                                ImAdd::RadioButtonImage(Textures::tValorant, skCrypt("Valorant").decrypt(), 1, &SelectedGame, ImVec2(-0.1, 103));
                                            break;
                                            case 3:
                                                ImAdd::RadioButtonImage(Textures::tCS2, skCrypt("Apex").decrypt(), 3, &SelectedGame, ImVec2(-0.1, 103));
                                            break;
                                            case 2:
                                                ImAdd::RadioButtonImage(Textures::tCSGO, skCrypt("Rust").decrypt(), 2, &SelectedGame, ImVec2(-0.1, 103));
                                            break;
                                            case 4:
                                                ImAdd::RadioButtonImage(Textures::tRust, skCrypt("Tarkov").decrypt(), 4, &SelectedGame, ImVec2(-0.1, 103));
                                            break;
                                            case 5:
                                                ImAdd::RadioButtonImage(Textures::tMinecraft, skCrypt("Fortnite").decrypt(), 5, &SelectedGame, ImVec2(-0.1, 103));
                                                break;
                                            case 6:
                                                ImAdd::RadioButtonImage(Textures::tSpoofer, skCrypt("Spoofer").decrypt(), 6, &SelectedGame, ImVec2(-0.1, 103));
                                                break;
                                            case 7:
                                                ImAdd::RadioButtonImage(Textures::tDayz, skCrypt("DayZ").decrypt(), 6, &SelectedGame, ImVec2(-0.1, 103));
                                                break;
                                           // break;
                                        }

                                    }
                                    ImGui::EndDisabled();

                                    auto pos = ImGui::GetWindowPos();
                                    auto size = ImGui::GetWindowSize();
                                    auto drawlist = ImGui::GetWindowDrawList();

                                    drawlist->AddRectFilledMultiColor(pos, pos + ImVec2(size.x, style.WindowPadding.y), shadow_col, shadow_col, shadow_col_null, shadow_col_null);
                                    drawlist->AddRectFilledMultiColor(pos + ImVec2(style.WindowPadding.x, size.y - style.WindowPadding.y), pos + size, shadow_col_null, shadow_col_null, shadow_col, shadow_col);
                                }
                                ImGui::PopStyleVar(2);
                            }
                            ImGui::EndChild();
                            ImGui::SameLine(GroupWidth - style.WindowPadding.x);
                            ImGui::BeginChild(skCrypt("RightSide##LaunchingOptions").decrypt(), ImVec2(GroupWidth, 0), true, ImGuiWindowFlags_NoBackground);
                            {
                                ImAdd::TextSide(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled), ImVec4(0, 1, 0, 1), skCrypt("Status").decrypt(), skCrypt("Undetected").decrypt());
                                ImAdd::TextSide(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled), ImGui::GetStyleColorVec4(ImGuiCol_Text), skCrypt("Game").decrypt(),
                                    SelectedGame == 1 ? skCrypt("Valorant").decrypt() :
                                    SelectedGame == 3 ? skCrypt("Apex").decrypt() :
                                    SelectedGame == 2 ? skCrypt("Rust").decrypt() :
                                    SelectedGame == 4 ? skCrypt("Tarkov").decrypt() :
                                    SelectedGame == 5 ? skCrypt("Fortnite").decrypt() :
                                    SelectedGame == 6 ? skCrypt("Spoofer").decrypt() :
                                    SelectedGame == 7 ? skCrypt("DayZ").decrypt() :
                                    skCrypt("If you see this contact us to fix!").decrypt()
                                );
                                ImAdd::TextSide(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled), ImGui::GetStyleColorVec4(ImGuiCol_Text), skCrypt("Expires").decrypt(), expiry.c_str());
                                ImAdd::TextSide(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled), ImGui::GetStyleColorVec4(ImGuiCol_Text), skCrypt("Updated").decrypt(), skCrypt("Positive").decrypt());

                                ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30 - style.WindowPadding.y);
                                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));
                                {
                                    // WARNING: Do not use this injection animation as you cannot predict the injection time
                                    DrawInjectButton();
                                }
                                ImGui::PopStyleColor();
                            }
                            ImGui::EndChild();
                        }
                        ImGui::EndGroup();
                    }
                }
                else
                {
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, style.WindowPadding * 2);
                    {
                        ImGui::BeginChild(skCrypt("LoginPanel").decrypt(), ImVec2(ImGui::GetWindowWidth() - fCharacterWidth, 0), true, ImGuiWindowFlags_NoBackground);
                        {
                            // Logo Part
                            //ImGui::SameLine(ImGui::GetWindowWidth() / 2 - LogoSize.x / 2);
                            //DrawLogo(LogoSize);
                            //ImGui::Spacing();
                            //ImGui::Spacing();
                            //ImGui::Spacing();

                            // Inputs
                            ImGui::BeginDisabled(g_Status.m_bIsLogging);
                            {
                                static char UserName[64]; 
                                ImAdd::InputText(skCrypt("Enter Key:"), ICON_FA_KEY, UserName, IM_ARRAYSIZE(UserName));
                                ImGui::Spacing();
                                //static char PassWord[64]; ImAdd::InputText("Password", ICON_FA_KEY, PassWord, IM_ARRAYSIZE(PassWord), ImGuiInputTextFlags_Password);
                                //ImGui::Spacing();
                                //ImAdd::ButtonText("I'm having trouble logging in", ImVec2(-0.1, 0));

                                // Button
                                ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30 - style.WindowPadding.y);
                                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));
                                {

                                    static const char* signinlabel = ("Sign In"); // for some reason encrypting this makes it show ?
                                    auto LoginBtnPos = ImGui::GetCursorPos();
                                    if (ImAdd::Button(signinlabel, ImVec2(-0.1, 30)))
                                    {
                                        g_Status.m_bIsLogging = true;
                                    }
                                    if (g_Status.m_bIsLogging)
                                    {
                                        signinlabel = ("Sign In"); // for some reason encrypting this makes it show ?
                                        if (!login_once)
                                        {
                                            KeyAuthApp.license(UserName);
                                            login_once = true;
                                        }
                                        signinlabel = KeyAuthApp.data.message.c_str();

                                        if (KeyAuthApp.data.success)
                                        {
                                            signinlabel = skCrypt("Success"); // for some reason encrypting this makes it show ?

                                            ImGui::SameLine((ImGui::GetWindowSize().x / 2) - 11);
                                            ImGui::SetCursorPosY(LoginBtnPos.y + 2 + (30 - 11 * 2) / 2);
                                            ImAdd::Spinner(11 - 2, 4, 6, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));
                                            static float LoggingTimer;
                                            LoggingTimer = min(LoggingTimer + ImGui::GetIO().DeltaTime * 1.0f, 1.f);
                                            if (LoggingTimer == 1.f)
                                            {
                                                g_Status.m_bIsLogged = true;
                                                g_Status.m_bIsLogging = false;
                                            }
                                        }
                                        else
                                        {
                                            std::memset(UserName, '\0', sizeof(UserName));          
                                            g_Status.m_bIsLogged = false;
                                            login_once = false;
                                            g_Status.m_bIsLogging = false;
                                        }
                                    }
                                }
                                ImGui::PopStyleColor();
                            }
                            ImGui::EndDisabled();
                        }
                        ImGui::EndChild();
                        ImGui::SameLine(ImGui::GetWindowWidth() - fCharacterWidth);
                        ImGui::BeginChild(skCrypt("CharacterSide"), ImVec2(0, 0), false, ImGuiWindowFlags_NoBackground);
                        {
                            auto pos = ImGui::GetWindowPos();
                            auto size = ImGui::GetWindowSize();
                            auto drawlist = ImGui::GetWindowDrawList();

                            drawlist->AddImageRounded(Textures::tCharacter, pos + ImVec2(size.x - fCharacterWidth, 0), pos + size, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255), style.WindowRounding, ImDrawFlags_RoundCornersRight);
                            drawlist->AddRectFilledMultiColor(pos + ImVec2(size.x - fCharacterWidth, 0), pos + ImVec2(size.x - fCharacterWidth + fShadowSize, size.y), shadow_col, shadow_col_null, shadow_col_null, shadow_col);

                            DrawCloseButton();
                        }
                        ImGui::EndChild();
                    }
                    ImGui::PopStyleVar();
                }
            }
            ImGui::End();
        }
        // Rendering
        ImGui::Render();
    }

    
}

void Gui::Unload( void )
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
