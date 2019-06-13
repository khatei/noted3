// dear imgui, v1.71 WIP
// (demo code)

// Message to the person tempted to delete this file when integrating Dear ImGui into their code base:
// Do NOT remove this file from your project! Think again! It is the most useful reference code that you and other coders
// will want to refer to and call. Have the ImGui::ShowDemoWindow() function wired in an always-available debug menu of
// your game/app! Removing this file from your project is hindering access to documentation for everyone in your team,
// likely leading you to poorer usage of the library.
// Everything in this file will be stripped out by the linker if you don't call ImGui::ShowDemoWindow().
// If you want to link core Dear ImGui in your shipped builds but want an easy guarantee that the demo will not be linked,
// you can setup your imconfig.h with #define IMGUI_DISABLE_DEMO_WINDOWS and those functions will be empty.
// In other situation, whenever you have Dear ImGui available you probably want this to be available for reference.
// Thank you,
// -Your beloved friend, imgui_demo.cpp (that you won't delete)

// Message to beginner C/C++ programmers about the meaning of the 'static' keyword:
// In this demo code, we frequently we use 'static' variables inside functions. A static variable persist across calls, so it is
// essentially like a global variable but declared inside the scope of the function. We do this as a way to gather code and data
// in the same place, to make the demo source code faster to read, faster to write, and smaller in size.
// It also happens to be a convenient way of storing simple UI related information as long as your function doesn't need to be reentrant
// or used in threads. This might be a pattern you will want to use in your code, but most of the real data you would be editing is
// likely going to be stored outside your functions.

/*

Index of this file:

// [SECTION] Forward Declarations, Helpers
// [SECTION] Demo Window / ShowDemoWindow()
// [SECTION] About Window / ShowAboutWindow()
// [SECTION] Style Editor / ShowStyleEditor()
// [SECTION] Example App: Main Menu Bar / ShowExampleAppMainMenuBar()
// [SECTION] Example App: Debug Console / ShowExampleAppConsole()
// [SECTION] Example App: Debug Log / ShowExampleAppLog()
// [SECTION] Example App: Simple Layout / ShowExampleAppLayout()
// [SECTION] Example App: Property Editor / ShowExampleAppPropertyEditor()
// [SECTION] Example App: Long Text / ShowExampleAppLongText()
// [SECTION] Example App: Auto Resize / ShowExampleAppAutoResize()
// [SECTION] Example App: Constrained Resize / ShowExampleAppConstrainedResize()
// [SECTION] Example App: Simple Overlay / ShowExampleAppSimpleOverlay()
// [SECTION] Example App: Manipulating Window Titles / ShowExampleAppWindowTitles()
// [SECTION] Example App: Custom Rendering using ImDrawList API / ShowExampleAppCustomRendering()
// [SECTION] Example App: Documents Handling / ShowExampleAppDocuments()

*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#include <vector>
#include <iterator>
#include <ctype.h>          // toupper
#include "ctime"
#include <limits.h>         // INT_MIN, INT_MAX
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#include <string.h>
#include <iostream>
#include<iostream>
#include <fstream>
#include <sstream>  // for string streams 
#include <string>  // for string
#include<cstring>
#include<map>
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>         // intptr_t
#else
#include <stdint.h>         // intptr_t
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#endif
#ifdef __clang__
#pragma clang diagnostic ignored "-Wold-style-cast"             // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored "-Wdeprecated-declarations"    // warning : 'xx' is deprecated: The POSIX name for this item.. // for strdup used in demo code (so user can copy & paste the code)
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"   // warning : cast to 'void *' from smaller integer type 'int'
#pragma clang diagnostic ignored "-Wformat-security"            // warning : warning: format string is not a string literal
#pragma clang diagnostic ignored "-Wexit-time-destructors"      // warning : declaration requires an exit-time destructor       // exit-time destruction order is undefined. if MemFree() leads to users code that has been disabled before exit it might cause problems. ImGui coding style welcomes static/globals.
#pragma clang diagnostic ignored "-Wunused-macros"              // warning : warning: macro is not used                         // we define snprintf/vsnprintf on Windows so they are available, but not always used.
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"  // warning : zero as null pointer constant                  // some standard header variations use #define NULL 0
#endif
#if __has_warning("-Wdouble-promotion")
#pragma clang diagnostic ignored "-Wdouble-promotion"           // warning: implicit conversion from 'float' to 'double' when passing argument to function  // using printf() is a misery with this as C++ va_arg ellipsis changes float to double.
#endif
#if __has_warning("-Wreserved-id-macro")
#pragma clang diagnostic ignored "-Wreserved-id-macro"          // warning : macro name is a reserved identifier                //
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"          // warning: cast to pointer from integer of different size
#pragma GCC diagnostic ignored "-Wformat-security"              // warning : format string is not a string literal (potentially insecure)
#pragma GCC diagnostic ignored "-Wdouble-promotion"             // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"                   // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#if (__GNUC__ >= 6)
#pragma GCC diagnostic ignored "-Wmisleading-indentation"       // warning: this 'if' clause does not guard this statement      // GCC 6.0+ only. See #883 on GitHub.
#endif
#endif

// Play it nice with Windows users. Notepad in 2017 still doesn't display text data with Unix-style \n.
#ifdef _WIN32
#define IM_NEWLINE  "\r\n"
#define snprintf    _snprintf
#define vsnprintf   _vsnprintf
#else
#define IM_NEWLINE  "\n"
#endif

#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))

//-----------------------------------------------------------------------------
// [SECTION] Forward Declarations, Helpers
//-----------------------------------------------------------------------------

#if !defined(IMGUI_DISABLE_OBSOLETE_FUNCTIONS) && defined(IMGUI_DISABLE_TEST_WINDOWS) && !defined(IMGUI_DISABLE_DEMO_WINDOWS)   // Obsolete name since 1.53, TEST->DEMO
#define IMGUI_DISABLE_DEMO_WINDOWS
#endif

#if !defined(IMGUI_DISABLE_DEMO_WINDOWS)

//static void ShowExampleAppConsole(const char* name, bool* p_open);
static bool show_app_console = true;
//static void values(int i);

// call this function when an item in selectable is selected
//void values(int i)
//void values(int i);
std::string item_name;
//int vals[] = {0,0,0};
static std::map<std::string,int> vals = { {"note1", 0} , {"note2", 0} , {"note3", 0} };
//char buf[32];
static int count = 0;

void takeinputtodisk(char* buf)
{
    //std::cout << buf;
    std::ofstream fileout;
    fileout.open("latestone.txt");
    fileout << buf;
}

void takeinputtodisk2(char* buf)
{
    //std::cout << buf;
    std::ofstream fileout;
    fileout.open("latesttwo.txt");
    fileout << buf;
}

void takeinputtodisk3(char* buf)
{
    //std::cout << buf;
    std::ofstream fileout;
    fileout.open("latestthree.txt");
    fileout << buf;
}

void takeinputtodisk4(char* buf)
{
    //std::cout << buf;
    std::ofstream fileout;
    fileout.open("latestfour.txt");
    fileout << buf;
}

void takeinputtodisk5(char* buf)
{
    //std::cout << buf;
    std::ofstream fileout;
    fileout.open("latestfive.txt");
    fileout << buf;
}

void ImGui::ShowDemoWindow(bool* p_open)
{
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!"); // Exceptionally add an extra assert here for people confused with initial dear imgui setup

   
    static bool show_new_window = true;
    //static int count = 1;
    //static const char* c = "note 1";
    //static std::vector<std::string> item_list = { "note##1","note##2","note##3" };
    static std::vector<std::string> item_list = { "note1","note2","note3" };
    static int add = item_list.size();

    ImGuiWindowFlags window_flags = 0;
    

    // We specify a default position/size in case there's no data in the .ini file. Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);


    // Main body of the Demo window starts here.
    
    //bool show_demo_window = true;
    //char word_note[9]= "note##";
    char word_note[9] = "note";
    static char temp[56] = " ";
    
  
    //ImGui::Begin("List", p_open, window_flags);
    if (!ImGui::Begin("list ", p_open, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }
    
  
    if (ImGui::Button("New")) {
        add += 1;
        std::ostringstream str1;
        str1 << add;
        std::string note_number = str1.str();
        strcpy(temp, const_cast<char *>(note_number.c_str()));
        strcat(word_note,temp);
        item_list.push_back(word_note);
        //std::string s = word_note;
        //vals.insert({ word_note,0 });
        vals.insert({ word_note,add });
        // uncomment to see how val is populated
       // for (auto itr = vals.begin(); itr != vals.end(); ++itr) {
            //std::cout << itr->first << '\t' << itr->second << std::endl;

        //}
        
    }

    

// OLD CODE HERE - IN WORKING CONDITION
    
    if (ImGui::ListBoxHeader("List new"))
    {
        for (std::string item : item_list)
        {
            
            if (ImGui::Selectable(item.c_str(), false))
            {
               //int k = 0;
                //for (int i = 0; item.c_str()[i] != '\0'; i++)
                //{
                  //  if (isdigit(item.c_str()[i]))
                    //    k += 1;
                //}
                //std::cout << k;
                //if(k==1){

                if (strcmp(item.c_str(), "note1") == 0)
                    count = 1;
                if (strcmp(item.c_str(), "note2") == 0)
                    count = 2;
                if (strcmp(item.c_str(), "note3") == 0)
                    count = 3;
                if (strcmp(item.c_str(), "note4") == 0)
                    count = 4;

                //std::cout << item.c_str();
                //std::cout << count;
                vals[item.c_str()] = count;
                //for (auto itr = vals.begin(); itr != vals.end(); ++itr) {
                  //  std::cout << itr->first << '\t' << itr->second << std::endl;

                //}
            }
            
        } 
        ImGui::ListBoxFooter();
    }
    for (auto const& x : vals)
    {
        if (x.second == 1)
        {
            //std::cout << x.first.c_str()<<std::endl;
            //std::cout << x.second;
            static char buf[32] = " ";

            if (!ImGui::Begin("note 1",&show_new_window))
            {
                ImGui::End();
                return;
            }

            ImGui::InputTextMultiline(x.first.c_str(), buf, IM_ARRAYSIZE(buf), ImVec2(-2.0f, ImGui::GetTextLineHeight() * 4));

            static char* output = buf;
            if (ImGui::Button("Save")) {
                takeinputtodisk(output);
            }

            ImGui::End();
        }

       if (x.second == 2)
        {
           //std::cout << x.first.c_str()<<std::endl;
           //std::cout << x.second;
            static char buf2[31]="";
            char* output = buf2;
            if (!ImGui::Begin("note 2",&show_new_window))
            {
                ImGui::End();
                return;
            }

            ImGui::InputTextMultiline(x.first.c_str(), buf2, IM_ARRAYSIZE(buf2), ImVec2(-2.0f, ImGui::GetTextLineHeight() * 2));
            if (ImGui::Button("Save")) {
                takeinputtodisk2(output);
            }
            
            ImGui::End();
        }

        if (x.second == 3)
        {
            //std::cout << x.first.c_str()<<std::endl;
            //std::cout << x.second;
            static char buf3[32]="";
            char* output = buf3;
            if (!ImGui::Begin("note 3", &show_new_window))
            {
                ImGui::End();
                return;
            }
            ImGui::InputTextMultiline(x.first.c_str(), buf3, IM_ARRAYSIZE(buf3), ImVec2(-2.0f, ImGui::GetTextLineHeight() * 2));
            if (ImGui::Button("Save")) {
                takeinputtodisk3(output);
            }
            ImGui::End();
        }

        if (x.second == 4)
        {
            //std::cout << x.first.c_str()<<std::endl;
            //std::cout << x.second;
            static char buf4[32] = "";
            char* output = buf4;
            if (!ImGui::Begin("note 4", &show_new_window))
            {
                ImGui::End();
                return;
            }
            ImGui::InputTextMultiline(x.first.c_str(), buf4, IM_ARRAYSIZE(buf4), ImVec2(-2.0f, ImGui::GetTextLineHeight() * 2));
            if (ImGui::Button("Save")) {
                takeinputtodisk4(output);
            }
            ImGui::End();
        }
    
    }
    
   ImGui::End();
}


// NEW CODE TO BE TESTED HERE
    /*
if (ImGui::ListBoxHeader("List new"))
{
    //for (std::string item : item_list)
    {

        if (ImGui::Selectable("note##1", false))
        {
            //count += 1;
            vals["note##1"] = 1;
        }


        if (ImGui::Selectable("note##2", false))
        {

            vals["note##2"] = 2;
        }

        if (ImGui::Selectable("note##3", false))
        {
            vals["note##3"] = 3;
        }

        if (ImGui::Selectable("note##4", false))
        {

            vals["note##4"] = 4;
        }

        if (ImGui::Selectable("note##5", false))
        {

            vals["note##5"] = 5;
        }
    }

    ImGui::ListBoxFooter();
}
for (auto const& x : vals)
{
    if (x.second == 1)
    {
        static char buf[32] = " ";

        if (!ImGui::Begin(" "))
        {
            ImGui::End();
            return;
        }

        ImGui::InputTextMultiline(x.first.c_str(), buf, IM_ARRAYSIZE(buf), ImVec2(-2.0f, ImGui::GetTextLineHeight() * 4));

        char* output = buf;
        if (ImGui::Button("Save")) {
            takeinputtodisk(output);
        }

        ImGui::End();
    }

    //while (1)
    {
        if (x.second == 2)
        {
            static char buf1[31] = "";
            char* output = buf1;
            if (!ImGui::Begin(" "))
            {
                ImGui::End();
                return;
            }

            ImGui::InputText(x.first.c_str(), buf1, IM_ARRAYSIZE(buf1));
            if (ImGui::Button("Save")) {
                takeinputtodisk2(output);
            }

            ImGui::End();
        }

        if (x.second == 3)
        {
            static char buf3[32] = "";
            char* output = buf3;
            if (!ImGui::Begin(" "))
            {
                ImGui::End();
                return;
            }
            ImGui::InputTextMultiline(x.first.c_str(), buf3, IM_ARRAYSIZE(buf3), ImVec2(-2.0f, ImGui::GetTextLineHeight() * 2));
            if (ImGui::Button("Save")) {
                takeinputtodisk3(output);
            }
            ImGui::End();
        }

        if (x.second == 4)
        {
            static char buf4[32] = " ";

            if (!ImGui::Begin(" "))
            {
                ImGui::End();
                return;
            }

            ImGui::InputTextMultiline(x.first.c_str(), buf4, IM_ARRAYSIZE(buf4), ImVec2(-2.0f, ImGui::GetTextLineHeight() * 4));

            char* output = buf4;
            if (ImGui::Button("Save")) {
                takeinputtodisk4(output);
            }

            ImGui::End();
        }

        if (x.second == 5)
        {
            static char buf5[32] = " ";

            if (!ImGui::Begin(" "))
            {
                ImGui::End();
                return;
            }

            ImGui::InputTextMultiline(x.first.c_str(), buf5, IM_ARRAYSIZE(buf5), ImVec2(-2.0f, ImGui::GetTextLineHeight() * 4));

            char* output = buf5;
            if (ImGui::Button("Save")) {
                takeinputtodisk5(output);
            }

            ImGui::End();
        }
    }

}

ImGui::End();
}
*/
#endif
