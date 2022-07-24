#pragma once
#include"Reference.h"

enum class KeySymbol : uint16_t
{
    KEY_Undefined = 0x0,

    KEY_Space = 0x20,

    KEY_0 = '0',
    KEY_1 = '1',
    KEY_2 = '2',
    KEY_3 = '3',
    KEY_4 = '4',
    KEY_5 = '5',
    KEY_6 = '6',
    KEY_7 = '7',
    KEY_8 = '8',
    KEY_9 = '9',

    KEY_a = 'a',
    KEY_b = 'b',
    KEY_c = 'c',
    KEY_d = 'd',
    KEY_e = 'e',
    KEY_f = 'f',
    KEY_g = 'g',
    KEY_h = 'h',
    KEY_i = 'i',
    KEY_j = 'j',
    KEY_k = 'k',
    KEY_l = 'l',
    KEY_m = 'm',
    KEY_n = 'n',
    KEY_o = 'o',
    KEY_p = 'p',
    KEY_q = 'q',
    KEY_r = 'r',
    KEY_s = 's',
    KEY_t = 't',
    KEY_u = 'u',
    KEY_v = 'v',
    KEY_w = 'w',
    KEY_x = 'x',
    KEY_y = 'y',
    KEY_z = 'z',

    KEY_A = 'A',
    KEY_B = 'B',
    KEY_C = 'C',
    KEY_D = 'D',
    KEY_E = 'E',
    KEY_F = 'F',
    KEY_G = 'G',
    KEY_H = 'H',
    KEY_I = 'I',
    KEY_J = 'J',
    KEY_K = 'K',
    KEY_L = 'L',
    KEY_M = 'M',
    KEY_N = 'N',
    KEY_O = 'O',
    KEY_P = 'P',
    KEY_Q = 'Q',
    KEY_R = 'R',
    KEY_S = 'S',
    KEY_T = 'T',
    KEY_U = 'U',
    KEY_V = 'V',
    KEY_W = 'W',
    KEY_X = 'X',
    KEY_Y = 'Y',
    KEY_Z = 'Z',



    KEY_Home = 0xFF50,
    KEY_Left = 0xFF51,  /* Move left, left arrow */
    KEY_Up = 0xFF52,    /* Move up, up arrow */
    KEY_Right = 0xFF53, /* Move right, right arrow */
    KEY_Down = 0xFF54,  /* Move down, down arrow */
    KEY_Prior = 0xFF55, /* Prior, previous */
    KEY_Page_Up = 0xFF55,
    KEY_Next = 0xFF56, /* Next */
    KEY_Page_Down = 0xFF56,
    KEY_End = 0xFF57,   /* EOL */
    KEY_Begin = 0xFF58, /* BOL */
};

enum KeyModifier : uint16_t
{
    MODKEY_Shift = 1,
    MODKEY_CapsLock = 2,
    MODKEY_Control = 4,
    MODKEY_Alt = 8,
    MODKEY_NumLock = 16,
    MODKEY_Meta = 128
};

enum class ScrollingMotion : uint16_t
{
    SCROLL_NONE = 1,
    SCROLL_LEFT = 2,
    SCROLL_RIGHT = 4,
    SCROLL_UP = 8,
    SCROLL_DOWN = 16,
    SCROLL_2D = 128
};
enum class MouseYOrientation :uint16_t
{
    Y_INCREASING_UPWARDS = 1 << 0,
    Y_INCREASING_DOWNWARDS = 1 << 1
};
enum class ButtonMask :uint16_t {
    LEFT_MOUSE_BUTTON = 1 << 0,
    MIDDLE_MOUSE_BUTTON = 1 << 1,
    RIGHT_MOUSE_BUTTON = 1 << 2
};

class Event;
class MouseScrollEvent;
class KeyPressEvent;
class EventHandler : public FReference
{
public:
    EventHandler() = default;
    virtual bool Handle(const Event& event) { return false; }
    virtual bool Handle(const MouseScrollEvent& event) { return false; }
    virtual bool Handle(const KeyPressEvent& event) { return false; }
   
};
class Event: public FReference
{
public:
    virtual void Handled(EventHandler& visitor) = 0;
};
class KeyEvent : public Event
{
public:
    KeyEvent() = default;
    KeyEvent(KeySymbol ks, KeyModifier km) :
        keySymbol(ks), modifier(km) {}
    KeySymbol keySymbol = {};
    KeyModifier modifier = {};
    virtual void Handled(EventHandler& visitor) { visitor.Handle(*this); }
};
class KeyPressEvent : public KeyEvent
{
public:
    virtual void Handled(EventHandler& visitor) { visitor.Handle(*this); }
    KeyPressEvent(KeySymbol ks, KeyModifier km):KeyEvent(ks,km){}
};
class KeyReleaseEvent : public KeyEvent
{
public:
    virtual void Handled(EventHandler& visitor) { visitor.Handle(*this); }
    KeyReleaseEvent(KeySymbol ks, KeyModifier km):KeyEvent(ks, km) {}
 
};

class MouseScrollEvent : public KeyEvent
{
public:
    virtual void Handled(EventHandler& visitor) { visitor.Handle(*this); }
};

class ButtonEvent : public KeyEvent
{
public:
    
    ButtonEvent(int32_t in_x, int32_t in_y, ButtonMask in_buttonMask) 
        :_in_x(in_x), _in_y(in_y), _in_buttonMask(in_buttonMask){}
    int32_t _in_x; 
    int32_t _in_y; 
    ButtonMask _in_buttonMask;
    virtual void Handled(EventHandler& visitor) { visitor.Handle(*this); }
};