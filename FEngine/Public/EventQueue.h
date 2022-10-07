#pragma once

#include"Reference.h"
#include<map>
#include<Windows.h>
#include"Event.h"
class KeyboardMap : public FReference
{
public:
    KeyboardMap()
    {
        _keycodeMap =
        {
            //{0x0, KEY_Undefined},

            //{VK_SPACE, KEY_Space},

            {'0', KEY_0},
            {'1', KEY_1},
            {'2', KEY_2},
            {'3', KEY_3},
            {'4', KEY_4},
            {'5', KEY_5},
            {'6', KEY_6},
            {'7', KEY_7},
            {'8', KEY_8},
            {'9', KEY_9},

            {'a', KEY_a},
            {'b', KEY_b},
            {'c', KEY_c},
            {'d', KEY_d},
            {'e', KEY_e},
            {'f', KEY_f},
            {'g', KEY_g},
            {'h', KEY_h},
            {'i', KEY_i},
            {'j', KEY_j},
            {'k', KEY_k},
            {'l', KEY_l},
            {'m', KEY_m},
            {'n', KEY_n},
            {'o', KEY_o},
            {'p', KEY_p},
            {'q', KEY_q},
            {'r', KEY_r},
            {'s', KEY_s},
            {'t', KEY_t},
            {'u', KEY_u},
            {'z', KEY_v},
            {'w', KEY_w},
            {'x', KEY_x},
            {'y', KEY_y},
            {'z', KEY_z},

            {'A', KEY_A},
            {'B', KEY_B},
            {'C', KEY_C},
            {'D', KEY_D},
            {'E', KEY_E},
            {'F', KEY_F},
            {'G', KEY_G},
            {'H', KEY_H},
            {'I', KEY_I},
            {'J', KEY_J},
            {'K', KEY_K},
            {'L', KEY_L},
            {'M', KEY_M},
            {'N', KEY_N},
            {'O', KEY_O},
            {'P', KEY_P},
            {'Q', KEY_Q},
            {'R', KEY_R},
            {'S', KEY_S},
            {'T', KEY_T},
            {'U', KEY_U},
            {'V', KEY_V},
            {'W', KEY_W},
            {'X', KEY_X},
            {'Y', KEY_Y},
            {'Z', KEY_Z},

            /* Cursor control & motion */

            {VK_HOME, KEY_Home},
            {VK_LEFT, KEY_Left},   /* Move left, left arrow */
            {VK_UP, KEY_Up},       /* Move up, up arrow */
            {VK_RIGHT, KEY_Right}, /* Move right, right arrow */
            {VK_DOWN, KEY_Down},   /* Move down, down arrow */
            {VK_PRIOR, KEY_Prior}, /* Prior, previous */
            //{ VK_, KEY_Page_Up = 0xFF55,
            {VK_NEXT, KEY_Next}, /* Next */
            //KEY_Page_Down = 0xFF56,
            {VK_END, KEY_End}, /* EOL */
            //{ KEY_Begin = 0xFF58, /* BOL */

           
            //{VK_OEM_COMMA, KEY_Comma},
            //{VK_OEM_MINUS, KEY_Minus},
            //{VK_OEM_PERIOD, KEY_Period},
            //{VK_OEM_2, KEY_Slash},
            //{':', KEY_Colon},
            //{VK_OEM_1, KEY_Semicolon},
            //{'<', KEY_Less},
            //{VK_OEM_PLUS, KEY_Equals}, // + isn't an unmodded key, why does windows map is as a virtual??
            //{'>', KEY_Greater},
            //{'?', KEY_Question},
            //{'@', KEY_At},
            //{VK_OEM_4, KEY_Leftbracket},
            //{VK_OEM_5, KEY_Backslash},
            //{VK_OEM_6, KEY_Rightbracket},
            //{'|', KEY_Caret},
            //{'_', KEY_Underscore},
            //{0xc0, KEY_Backquote},

            //{VK_BACK, KEY_BackSpace}, /* back space, back char */
            //{VK_TAB, KEY_Tab},
            ////    KEY_Linefeed = 0xFF0A, /* Linefeed, LF */
            //{VK_CLEAR, KEY_Clear},
            //{VK_RETURN, KEY_Return}, /* Return, enter */
            //{VK_PAUSE, KEY_Pause},   /* Pause, hold */
            //{VK_SCROLL, KEY_Scroll_Lock},
            ////    KEY_Sys_Req = 0xFF15,
            //{VK_ESCAPE, KEY_Escape},
            //{VK_DELETE, KEY_Delete}, /* Delete, rubout */

            ///* Misc Functions */

            //{VK_SELECT, KEY_Select}, /* Select, mark */
            //{VK_PRINT, KEY_Print},
            //{VK_EXECUTE, KEY_Execute}, /* Execute, run, do */
            //{VK_INSERT, KEY_Insert},   /* Insert, insert here */
            ////{ KEY_Undo = 0xFF65,    /* Undo, oops */
            ////KEY_Redo = 0xFF66,    /* redo, again */
            //{VK_APPS, KEY_Menu}, /* On Windows, this is VK_APPS, the context-menu key */
            //// KEY_Find = 0xFF68,    /* Find, search */
            //{VK_CANCEL, KEY_Cancel}, /* Cancel, stop, abort, exit */
            //{VK_HELP, KEY_Help},     /* Help */
            ////{ KEY_Break = 0xFF6B,
            ////KEY_Mode_switch = 0xFF7E,   /* Character set switch */
            ////KEY_Script_switch = 0xFF7E, /* Alias for mode_switch */
            //{VK_NUMLOCK, KEY_Num_Lock},

            /* Keypad Functions, keypad numbers cleverly chosen to map to ascii */

            //KEY_KP_Space = 0xFF80, /* space */
            //KEY_KP_Tab = 0xFF89,
            //KEY_KP_Enter = 0xFF8D, /* enter */
            //KEY_KP_F1 = 0xFF91,    /* PF1, KP_A, ... */
            //KEY_KP_F2 = 0xFF92,
            //KEY_KP_F3 = 0xFF93,
            //KEY_KP_F4 = 0xFF94,
            //KEY_KP_Home = 0xFF95,
            //KEY_KP_Left = 0xFF96,
            //KEY_KP_Up = 0xFF97,
            //KEY_KP_Right = 0xFF98,
            //KEY_KP_Down = 0xFF99,
            //KEY_KP_Prior = 0xFF9A,
            //KEY_KP_Page_Up = 0xFF9A,
            //KEY_KP_Next = 0xFF9B,
            //KEY_KP_Page_Down = 0xFF9B,
            //KEY_KP_End = 0xFF9C,
            //KEY_KP_Begin = 0xFF9D,
            //KEY_KP_Insert = 0xFF9E,
            //KEY_KP_Delete = 0xFF9F,
            //KEY_KP_Equal = 0xFFBD, /* equals */
            //KEY_KP_Multiply = 0xFFAA,
            //KEY_KP_Add = 0xFFAB,
            //KEY_KP_Separator = 0xFFAC, /* separator, often comma */
            //KEY_KP_Subtract = 0xFFAD,
            //KEY_KP_Decimal = 0xFFAE,
            //KEY_KP_Divide = 0xFFAF,

      /*      {VK_NUMPAD0, KEY_KP_0},
            {VK_NUMPAD1, KEY_KP_1},
            {VK_NUMPAD2, KEY_KP_2},
            {VK_NUMPAD3, KEY_KP_3},
            {VK_NUMPAD4, KEY_KP_4},
            {VK_NUMPAD5, KEY_KP_5},
            {VK_NUMPAD6, KEY_KP_6},
            {VK_NUMPAD7, KEY_KP_7},
            {VK_NUMPAD8, KEY_KP_8},
            {VK_NUMPAD9, KEY_KP_9},*/

            /*
        * Auxiliary Functions; note the duplicate definitions for left and right
        * function keys;  Sun keyboards and a few other manufactures have such
        * function key groups on the left and/or right sides of the keyboard.
        * We've not found a keyboard with more than 35 function keys total.
        */

        /*    {VK_F1, KEY_F1},
            {VK_F2, KEY_F2},
            {VK_F3, KEY_F3},
            {VK_F4, KEY_F4},
            {VK_F5, KEY_F5},
            {VK_F6, KEY_F6},
            {VK_F7, KEY_F7},
            {VK_F8, KEY_F8},
            {VK_F9, KEY_F9},
            {VK_F10, KEY_F10},
            {VK_F11, KEY_F11},
            {VK_F12, KEY_F12},
            {VK_F13, KEY_F13},
            {VK_F14, KEY_F14},
            {VK_F15, KEY_F15},
            {VK_F16, KEY_F16},
            {VK_F17, KEY_F17},
            {VK_F18, KEY_F18},
            {VK_F19, KEY_F19},
            {VK_F20, KEY_F20},
            {VK_F21, KEY_F21},
            {VK_F22, KEY_F22},
            {VK_F23, KEY_F23},
            {VK_F24, KEY_F24},*/

            //KEY_F25 = 0xFFD6,
            //KEY_F26 = 0xFFD7,
            //KEY_F27 = 0xFFD8,
            //KEY_F28 = 0xFFD9,
            //KEY_F29 = 0xFFDA,
            //KEY_F30 = 0xFFDB,
            //KEY_F31 = 0xFFDC,
            //KEY_F32 = 0xFFDD,
            //KEY_F33 = 0xFFDE,
            //KEY_F34 = 0xFFDF,
            //KEY_F35 = 0xFFE0,

            /* Modifiers */

            //{VK_LSHIFT, KEY_Shift_L},     /* Left shift */
            //{VK_RSHIFT, KEY_Shift_R},     /* Right shift */
            //{VK_LCONTROL, KEY_Control_L}, /* Left control */
            //{VK_RCONTROL, KEY_Control_R}, /* Right control */
            //{VK_CAPITAL, KEY_Caps_Lock},  /* Caps lock */
            ////KEY_Shift_Lock = 0xFFE6, /* Shift lock */

            ////KEY_Meta_L = 0xFFE7,  /* Left meta */
            ////KEY_Meta_R = 0xFFE8,  /* Right meta */
            //{VK_LMENU, KEY_Alt_L},  /* Left alt */
            //{VK_RMENU, KEY_Alt_R},  /* Right alt */
            //{VK_LWIN, KEY_Super_L}, /* Left super */
            //{VK_RWIN, KEY_Super_R}  /* Right super */
            //KEY_Hyper_L = 0xFFED, /* Left hyper */
            //KEY_Hyper_R = 0xFFEE  /* Right hyper */
        };
    }


    using VirtualKeyToKeySymbolMap = std::map<uint16_t, KeySymbol>;

    bool getKeySymbol(WPARAM wParam, LPARAM lParam, KeySymbol& keySymbol, KeySymbol& modifiedKeySymbol, KeyModifier& keyModifier)
    {
        uint16_t modifierMask = 0;

        //bool rightSide = (lParam & 0x01000000) != 0;
        uint32_t virtualKey = ::MapVirtualKeyEx((lParam >> 16) & 0xff, 3, ::GetKeyboardLayout(0));
        auto itr = _keycodeMap.find(virtualKey);
        if (itr == _keycodeMap.end()) return false;

        modifiedKeySymbol = itr->second;

        BYTE keyState[256];
        if (virtualKey == 0 || !::GetKeyboardState(keyState))
        {
            return false;
        }

        switch (virtualKey)
        {
        case VK_LSHIFT:
        case VK_RSHIFT:
            modifierMask |= KeyModifier::MODKEY_Shift;
            break;

        case VK_LCONTROL:
        case VK_RCONTROL:
            modifierMask |= KeyModifier::MODKEY_Control;
            break;

        case VK_LMENU:
        case VK_RMENU:
            modifierMask |= KeyModifier::MODKEY_Alt;
            break;

        default:
            virtualKey = static_cast<int>(wParam);
            break;
        }

        //if (keyState[VK_CAPITAL] & 0x01) modifierMask |= vsg::KeyModifier::MODKEY_CapsLock;
        //if (keyState[VK_NUMLOCK] & 0x01) modifierMask |= vsg::KeyModifier::MODKEY_NumLock;

        //keyModifier = static_cast<vsg::KeyModifier>(modifierMask);

        // our actual keystroke is what we get after the ::ToAscii call
        char asciiKey[2];
        int32_t numChars = ::ToAscii(static_cast<UINT>(wParam), (lParam >> 16) & 0xff, keyState, reinterpret_cast<WORD*>(asciiKey), 0);
        if (numChars > 0) keySymbol = static_cast<KeySymbol>(asciiKey[0]);

        return true;
    }

protected:
    VirtualKeyToKeySymbolMap _keycodeMap;
};



class BufferedEventQueue
{
public:
    typedef Ptr<Event> EventPtr;
    typedef std::list<EventPtr> BufferdEvents;
    BufferedEventQueue()
    {
        _keyBoardMap = new KeyboardMap();
    }

   // bool PushEvent(EventPtr evt) { bufferdEvents.push_back(evt); lastEvt = evt; }
    EventPtr TakeoutEvent() { EventPtr evt = bufferdEvents.front(); bufferdEvents.pop_front(); return evt; }
    inline bool Empty() { return bufferdEvents.empty(); }

    void GetKeySymbol(WPARAM wParam, LPARAM lParam, KeySymbol& keySymbol, KeySymbol& modifiedKeySymbol, KeyModifier& keyModifier){
        _keyBoardMap->getKeySymbol(wParam, lParam, keySymbol, modifiedKeySymbol, keyModifier);
    }
    void TransformMouseXY(int& x, int& y)
    {
        const int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
        const int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
       /* std::cout << "orginx" << x << " " << y << std::endl;
        x = static_cast<int>((float(x) / 65535.0f) * width);
        y = static_cast<int>((float(y) / 65535.0f) * height);*/
        
        int tempX = x - relativeX;
        int tempY = y - relativeY;
        relativeX = x;
        relativeY = y;
        x = tempX; y = tempY;
    }

    bool MouseMotion(int32_t mx, int32_t my, float delta)
    {
        //std::cout << "mx" << mx << " " << my << std::endl;
        Ptr<Event> evt = new MouseMoveEvent(mx, my, delta);
        bufferdEvents.emplace_back(evt);
        return true;
    }
    bool MouseDrag(int32_t mx, int32_t my, float delta, int btn)
    {
       
        Ptr<Event> evt = new MouseDragEvent(mx, my, delta,btn);
        bufferdEvents.emplace_back(evt);
        return true;
    }
    bool MouseScroll(ScrollingMotion motion, float delta)
    {
        Ptr<Event> evt = new MouseScrollEvent(motion, delta);
        bufferdEvents.emplace_back(evt);
        return true;
    }

    bool KeyUp(WPARAM wParam, LPARAM lParam, KeySymbol& keySymbol, KeySymbol& modifiedKeySymbol, KeyModifier& keyModifier)
    {
        if (_keyBoardMap->getKeySymbol(wParam, lParam, keySymbol, modifiedKeySymbol, keyModifier))
        {
            std::cout << keySymbol << std::endl;
            Ptr<Event> evt = new KeyPressEvent(modifiedKeySymbol, keyModifier);
            int32_t repeatCount = (lParam & 0xffff);
            bufferdEvents.emplace_back(evt);
            return true;
        }
        return false;
    }
    inline void SetDragBtn(int btn) { draggedBtn = btn; }
    inline int GetDragBtn() { return draggedBtn; }
    inline bool isDragged() { return draggedBtn != -1; }
    inline void CancelDrag() { draggedBtn = -1; }
    
private:
   
    int32_t relativeX;
    int32_t relativeY;
    int32_t draggedBtn{ -1 };
    Ptr<KeyboardMap> _keyBoardMap;
    BufferdEvents bufferdEvents;
};