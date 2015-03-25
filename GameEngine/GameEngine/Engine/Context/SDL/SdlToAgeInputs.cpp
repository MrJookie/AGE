#include <context/SDL/SdlContext.hh>
#include <SDL/SDL.h>

namespace AGE
{
	std::map<int, AgeKeys> IRenderContext::_toAgeMappedKey =
	{
		{ SDLK_UNKNOWN, AGE_KEY_UNKNOWN },
		{ SDLK_RETURN, AGE_RETURN },
		{ SDLK_ESCAPE, AGE_ESCAPE },
		{ SDLK_BACKSPACE, AGE_BACKSPACE },
		{ SDLK_TAB, AGE_TAB },
		{ SDLK_SPACE, AGE_SPACE },
		{ SDLK_EXCLAIM, AGE_EXCLAIM },
		{ SDLK_QUOTEDBL, AGE_QUOTEDBL },
		{ SDLK_HASH, AGE_HASH },
		{ SDLK_PERCENT, AGE_PERCENT },
		{ SDLK_DOLLAR, AGE_DOLLAR },
		{ SDLK_AMPERSAND, AGE_AMPERSAND },
		{ SDLK_QUOTE, AGE_QUOTE },
		{ SDLK_LEFTPAREN, AGE_LEFTPAREN },
		{ SDLK_RIGHTPAREN, AGE_RIGHTPAREN },
		{ SDLK_ASTERISK, AGE_ASTERISK },
		{ SDLK_PLUS, AGE_PLUS },
		{ SDLK_COMMA, AGE_COMMA },
		{ SDLK_MINUS, AGE_MINUS },
		{ SDLK_PERIOD, AGE_PERIOD },
		{ SDLK_SLASH, AGE_SLASH },
		{ SDLK_0, AGE_0 },
		{ SDLK_1, AGE_1 },
		{ SDLK_2, AGE_2 },
		{ SDLK_3, AGE_3 },
		{ SDLK_4, AGE_4 },
		{ SDLK_5, AGE_5 },
		{ SDLK_6, AGE_6 },
		{ SDLK_7, AGE_7 },
		{ SDLK_8, AGE_8 },
		{ SDLK_9, AGE_9 },
		{ SDLK_COLON, AGE_COLON },
		{ SDLK_SEMICOLON, AGE_SEMICOLON },
		{ SDLK_LESS, AGE_LESS },
		{ SDLK_EQUALS, AGE_EQUALS },
		{ SDLK_GREATER, AGE_GREATER },
		{ SDLK_QUESTION, AGE_QUESTION },
		{ SDLK_AT, AGE_AT },
		{ SDLK_LEFTBRACKET, AGE_LEFTBRACKET },
		{ SDLK_BACKSLASH, AGE_BACKSLASH },
		{ SDLK_RIGHTBRACKET, AGE_RIGHTBRACKET },
		{ SDLK_CARET, AGE_CARET },
		{ SDLK_UNDERSCORE, AGE_UNDERSCORE },
		{ SDLK_BACKQUOTE, AGE_BACKQUOTE },
		{ SDLK_a, AGE_a },
		{ SDLK_b, AGE_b },
		{ SDLK_c, AGE_c },
		{ SDLK_d, AGE_d },
		{ SDLK_e, AGE_e },
		{ SDLK_f, AGE_f },
		{ SDLK_g, AGE_g },
		{ SDLK_h, AGE_h },
		{ SDLK_i, AGE_i },
		{ SDLK_j, AGE_j },
		{ SDLK_k, AGE_k },
		{ SDLK_l, AGE_l },
		{ SDLK_m, AGE_m },
		{ SDLK_n, AGE_n },
		{ SDLK_o, AGE_o },
		{ SDLK_p, AGE_p },
		{ SDLK_q, AGE_q },
		{ SDLK_r, AGE_r },
		{ SDLK_s, AGE_s },
		{ SDLK_t, AGE_t },
		{ SDLK_u, AGE_u },
		{ SDLK_v, AGE_v },
		{ SDLK_w, AGE_w },
		{ SDLK_x, AGE_x },
		{ SDLK_y, AGE_y },
		{ SDLK_z, AGE_z },
		{ SDLK_KP_DIVIDE, AGE_KEYPAD_DIVIDE },
		{ SDLK_KP_MULTIPLY, AGE_KEYPAD_MULTIPLY },
		{ SDLK_KP_MINUS, AGE_KEYPAD_MINUS },
		{ SDLK_KP_PLUS, AGE_KEYPAD_PLUS },
		{ SDLK_KP_ENTER, AGE_KEYPAD_ENTER },
		{ SDLK_KP_1, AGE_KEYPAD_1 },
		{ SDLK_KP_2, AGE_KEYPAD_2 },
		{ SDLK_KP_3, AGE_KEYPAD_3 },
		{ SDLK_KP_4, AGE_KEYPAD_4 },
		{ SDLK_KP_5, AGE_KEYPAD_5 },
		{ SDLK_KP_6, AGE_KEYPAD_6 },
		{ SDLK_KP_7, AGE_KEYPAD_7 },
		{ SDLK_KP_8, AGE_KEYPAD_8 },
		{ SDLK_KP_9, AGE_KEYPAD_9 },
		{ SDLK_KP_0, AGE_KEYPAD_0 },
		{ SDLK_KP_PERIOD, AGE_KEYPAD_PERIOD },
		{ SDLK_KP_EQUALS, AGE_KEYPAD_EQUALS },
		{ SDLK_CAPSLOCK, AGE_CAPSLOCK },
		{ SDLK_F1, AGE_F1 },
		{ SDLK_F2, AGE_F2 },
		{ SDLK_F3, AGE_F3 },
		{ SDLK_F4, AGE_F4 },
		{ SDLK_F5, AGE_F5 },
		{ SDLK_F6, AGE_F6 },
		{ SDLK_F7, AGE_F7 },
		{ SDLK_F8, AGE_F8 },
		{ SDLK_F9, AGE_F9 },
		{ SDLK_F10, AGE_F10 },
		{ SDLK_F11, AGE_F11 },
		{ SDLK_F12, AGE_F12 },
		{ SDLK_PRINTSCREEN, AGE_PRINTSCREEN },
		{ SDLK_SCROLLLOCK, AGE_SCROLLLOCK },
		{ SDLK_PAUSE, AGE_PAUSE },
		{ SDLK_INSERT, AGE_INSERT },
		{ SDLK_HOME, AGE_HOME },
		{ SDLK_PAGEUP, AGE_PAGEUP },
		{ SDLK_DELETE, AGE_DELETE },
		{ SDLK_END, AGE_END },
		{ SDLK_PAGEDOWN, AGE_PAGEDOWN },
		{ SDLK_RIGHT, AGE_RIGHT },
		{ SDLK_LEFT, AGE_LEFT },
		{ SDLK_DOWN, AGE_DOWN },
		{ SDLK_UP, AGE_UP },
		{ SDLK_NUMLOCKCLEAR, AGE_NUMLOCKCLEAR },
		{ SDLK_APPLICATION, AGE_APPLICATION },
		{ SDLK_POWER, AGE_POWER },
		{ SDLK_F13, AGE_F13 },
		{ SDLK_F14, AGE_F14 },
		{ SDLK_F15, AGE_F15 },
		{ SDLK_F16, AGE_F16 },
		{ SDLK_F17, AGE_F17 },
		{ SDLK_F18, AGE_F18 },
		{ SDLK_F19, AGE_F19 },
		{ SDLK_F20, AGE_F20 },
		{ SDLK_F21, AGE_F21 },
		{ SDLK_F22, AGE_F22 },
		{ SDLK_F23, AGE_F23 },
		{ SDLK_F24, AGE_F24 },
		{ SDLK_EXECUTE, AGE_EXECUTE },
		{ SDLK_HELP, AGE_HELP },
		{ SDLK_MENU, AGE_MENU },
		{ SDLK_SELECT, AGE_SELECT },
		{ SDLK_STOP, AGE_STOP },
		{ SDLK_AGAIN, AGE_AGAIN },
		{ SDLK_UNDO, AGE_UNDO },
		{ SDLK_CUT, AGE_CUT },
		{ SDLK_COPY, AGE_COPY },
		{ SDLK_PASTE, AGE_PASTE },
		{ SDLK_FIND, AGE_FIND },
		{ SDLK_MUTE, AGE_MUTE },
		{ SDLK_VOLUMEUP, AGE_VOLUMEUP },
		{ SDLK_VOLUMEDOWN, AGE_VOLUMEDOWN },
		{ SDLK_ALTERASE, AGE_ALTERASE },
		{ SDLK_SYSREQ, AGE_SYSREQ },
		{ SDLK_CANCEL, AGE_CANCEL },
		{ SDLK_CLEAR, AGE_CLEAR },
		{ SDLK_PRIOR, AGE_PRIOR },
		{ SDLK_RETURN2, AGE_RETURN2 },
		{ SDLK_SEPARATOR, AGE_SEPARATOR },
		{ SDLK_OUT, AGE_OUT },
		{ SDLK_OPER, AGE_OPER },
		{ SDLK_CLEARAGAIN, AGE_CLEARAGAIN },
		{ SDLK_CRSEL, AGE_CRSEL },
		{ SDLK_EXSEL, AGE_EXSEL },
		{ SDLK_LCTRL, AGE_LCTRL },
		{ SDLK_LSHIFT, AGE_LSHIFT },
		{ SDLK_LALT, AGE_LALT },
		{ SDLK_LGUI, AGE_LGUI },
		{ SDLK_RCTRL, AGE_RCTRL },
		{ SDLK_RSHIFT, AGE_RSHIFT },
		{ SDLK_RALT, AGE_RALT },
		{ SDLK_RGUI, AGE_RGUI },
		{ SDLK_MODE, AGE_MODE },
		{ SDLK_AUDIONEXT, AGE_AUDIONEXT },
		{ SDLK_AUDIOPREV, AGE_AUDIOPREV },
		{ SDLK_AUDIOSTOP, AGE_AUDIOSTOP },
		{ SDLK_AUDIOPLAY, AGE_AUDIOPLAY },
		{ SDLK_AUDIOMUTE, AGE_AUDIOMUTE },
		{ SDLK_MEDIASELECT, AGE_MEDIASELECT },
		{ SDLK_WWW, AGE_WWW },
		{ SDLK_MAIL, AGE_MAIL },
		{ SDLK_CALCULATOR, AGE_CALCULATOR },
		{ SDLK_COMPUTER, AGE_COMPUTER },
		{ SDLK_AC_SEARCH, AGE_AC_SEARCH },
		{ SDLK_AC_HOME, AGE_AC_HOME },
		{ SDLK_AC_BACK, AGE_AC_BACK },
		{ SDLK_AC_FORWARD, AGE_AC_FORWARD },
		{ SDLK_AC_STOP, AGE_AC_STOP },
		{ SDLK_AC_REFRESH, AGE_AC_REFRESH },
		{ SDLK_AC_BOOKMARKS, AGE_AC_BOOKMARKS },
		{ SDLK_BRIGHTNESSDOWN, AGE_BRIGHTNESSDOWN },
		{ SDLK_BRIGHTNESSUP, AGE_BRIGHTNESSUP },
		{ SDLK_DISPLAYSWITCH, AGE_DISPLAYSWITCH },
		{ SDLK_KBDILLUMTOGGLE, AGE_KBDILLUMTOGGLE },
		{ SDLK_KBDILLUMDOWN, AGE_KBDILLUMDOWN },
		{ SDLK_KBDILLUMUP, AGE_KBDILLUMUP },
		{ SDLK_EJECT, AGE_EJECT },
		{ SDLK_SLEEP, AGE_SLEEP }
	};

	std::map<int, AgeKeys> IRenderContext::_toAgePhysicalKey =
	{
		{ SDL_SCANCODE_UNKNOWN, AGE_KEY_UNKNOWN },
		{ SDL_SCANCODE_RETURN, AGE_RETURN },
		{ SDL_SCANCODE_ESCAPE, AGE_ESCAPE },
		{ SDL_SCANCODE_BACKSPACE, AGE_BACKSPACE },
		{ SDL_SCANCODE_TAB, AGE_TAB },
		{ SDL_SCANCODE_SPACE, AGE_SPACE },
		{ SDL_SCANCODE_APOSTROPHE, AGE_QUOTE },
		{ SDL_SCANCODE_COMMA, AGE_COMMA },
		{ SDL_SCANCODE_MINUS, AGE_MINUS },
		{ SDL_SCANCODE_PERIOD, AGE_PERIOD },
		{ SDL_SCANCODE_SLASH, AGE_SLASH },
		{ SDL_SCANCODE_0, AGE_0 },
		{ SDL_SCANCODE_1, AGE_1 },
		{ SDL_SCANCODE_2, AGE_2 },
		{ SDL_SCANCODE_3, AGE_3 },
		{ SDL_SCANCODE_4, AGE_4 },
		{ SDL_SCANCODE_5, AGE_5 },
		{ SDL_SCANCODE_6, AGE_6 },
		{ SDL_SCANCODE_7, AGE_7 },
		{ SDL_SCANCODE_8, AGE_8 },
		{ SDL_SCANCODE_9, AGE_9 },
		{ SDL_SCANCODE_SEMICOLON, AGE_SEMICOLON },
		{ SDL_SCANCODE_EQUALS, AGE_EQUALS },
		{ SDL_SCANCODE_LEFTBRACKET, AGE_LEFTBRACKET },
		{ SDL_SCANCODE_BACKSLASH, AGE_BACKSLASH },
		{ SDL_SCANCODE_RIGHTBRACKET, AGE_RIGHTBRACKET },
		{ SDL_SCANCODE_GRAVE, AGE_BACKQUOTE },
		{ SDL_SCANCODE_A, AGE_a },
		{ SDL_SCANCODE_B, AGE_b },
		{ SDL_SCANCODE_C, AGE_c },
		{ SDL_SCANCODE_D, AGE_d },
		{ SDL_SCANCODE_E, AGE_e },
		{ SDL_SCANCODE_F, AGE_f },
		{ SDL_SCANCODE_G, AGE_g },
		{ SDL_SCANCODE_H, AGE_h },
		{ SDL_SCANCODE_I, AGE_i },
		{ SDL_SCANCODE_J, AGE_j },
		{ SDL_SCANCODE_K, AGE_k },
		{ SDL_SCANCODE_L, AGE_l },
		{ SDL_SCANCODE_M, AGE_m },
		{ SDL_SCANCODE_N, AGE_n },
		{ SDL_SCANCODE_O, AGE_o },
		{ SDL_SCANCODE_P, AGE_p },
		{ SDL_SCANCODE_Q, AGE_q },
		{ SDL_SCANCODE_R, AGE_r },
		{ SDL_SCANCODE_S, AGE_s },
		{ SDL_SCANCODE_T, AGE_t },
		{ SDL_SCANCODE_U, AGE_u },
		{ SDL_SCANCODE_V, AGE_v },
		{ SDL_SCANCODE_W, AGE_w },
		{ SDL_SCANCODE_X, AGE_x },
		{ SDL_SCANCODE_Y, AGE_y },
		{ SDL_SCANCODE_Z, AGE_z },
		{ SDL_SCANCODE_KP_DIVIDE, AGE_KEYPAD_DIVIDE },
		{ SDL_SCANCODE_KP_MULTIPLY, AGE_KEYPAD_MULTIPLY },
		{ SDL_SCANCODE_KP_MINUS, AGE_KEYPAD_MINUS },
		{ SDL_SCANCODE_KP_PLUS, AGE_KEYPAD_PLUS },
		{ SDL_SCANCODE_KP_ENTER, AGE_KEYPAD_ENTER },
		{ SDL_SCANCODE_KP_1, AGE_KEYPAD_1 },
		{ SDL_SCANCODE_KP_2, AGE_KEYPAD_2 },
		{ SDL_SCANCODE_KP_3, AGE_KEYPAD_3 },
		{ SDL_SCANCODE_KP_4, AGE_KEYPAD_4 },
		{ SDL_SCANCODE_KP_5, AGE_KEYPAD_5 },
		{ SDL_SCANCODE_KP_6, AGE_KEYPAD_6 },
		{ SDL_SCANCODE_KP_7, AGE_KEYPAD_7 },
		{ SDL_SCANCODE_KP_8, AGE_KEYPAD_8 },
		{ SDL_SCANCODE_KP_9, AGE_KEYPAD_9 },
		{ SDL_SCANCODE_KP_0, AGE_KEYPAD_0 },
		{ SDL_SCANCODE_KP_PERIOD, AGE_KEYPAD_PERIOD },
		{ SDL_SCANCODE_KP_EQUALS, AGE_KEYPAD_EQUALS },
		{ SDL_SCANCODE_CAPSLOCK, AGE_CAPSLOCK },
		{ SDL_SCANCODE_F1, AGE_F1 },
		{ SDL_SCANCODE_F2, AGE_F2 },
		{ SDL_SCANCODE_F3, AGE_F3 },
		{ SDL_SCANCODE_F4, AGE_F4 },
		{ SDL_SCANCODE_F5, AGE_F5 },
		{ SDL_SCANCODE_F6, AGE_F6 },
		{ SDL_SCANCODE_F7, AGE_F7 },
		{ SDL_SCANCODE_F8, AGE_F8 },
		{ SDL_SCANCODE_F9, AGE_F9 },
		{ SDL_SCANCODE_F10, AGE_F10 },
		{ SDL_SCANCODE_F11, AGE_F11 },
		{ SDL_SCANCODE_F12, AGE_F12 },
		{ SDL_SCANCODE_PRINTSCREEN, AGE_PRINTSCREEN },
		{ SDL_SCANCODE_SCROLLLOCK, AGE_SCROLLLOCK },
		{ SDL_SCANCODE_PAUSE, AGE_PAUSE },
		{ SDL_SCANCODE_INSERT, AGE_INSERT },
		{ SDL_SCANCODE_HOME, AGE_HOME },
		{ SDL_SCANCODE_PAGEUP, AGE_PAGEUP },
		{ SDL_SCANCODE_DELETE, AGE_DELETE },
		{ SDL_SCANCODE_END, AGE_END },
		{ SDL_SCANCODE_PAGEDOWN, AGE_PAGEDOWN },
		{ SDL_SCANCODE_RIGHT, AGE_RIGHT },
		{ SDL_SCANCODE_LEFT, AGE_LEFT },
		{ SDL_SCANCODE_DOWN, AGE_DOWN },
		{ SDL_SCANCODE_UP, AGE_UP },
		{ SDL_SCANCODE_NUMLOCKCLEAR, AGE_NUMLOCKCLEAR },
		{ SDL_SCANCODE_APPLICATION, AGE_APPLICATION },
		{ SDL_SCANCODE_POWER, AGE_POWER },
		{ SDL_SCANCODE_F13, AGE_F13 },
		{ SDL_SCANCODE_F14, AGE_F14 },
		{ SDL_SCANCODE_F15, AGE_F15 },
		{ SDL_SCANCODE_F16, AGE_F16 },
		{ SDL_SCANCODE_F17, AGE_F17 },
		{ SDL_SCANCODE_F18, AGE_F18 },
		{ SDL_SCANCODE_F19, AGE_F19 },
		{ SDL_SCANCODE_F20, AGE_F20 },
		{ SDL_SCANCODE_F21, AGE_F21 },
		{ SDL_SCANCODE_F22, AGE_F22 },
		{ SDL_SCANCODE_F23, AGE_F23 },
		{ SDL_SCANCODE_F24, AGE_F24 },
		{ SDL_SCANCODE_EXECUTE, AGE_EXECUTE },
		{ SDL_SCANCODE_HELP, AGE_HELP },
		{ SDL_SCANCODE_MENU, AGE_MENU },
		{ SDL_SCANCODE_SELECT, AGE_SELECT },
		{ SDL_SCANCODE_STOP, AGE_STOP },
		{ SDL_SCANCODE_AGAIN, AGE_AGAIN },
		{ SDL_SCANCODE_UNDO, AGE_UNDO },
		{ SDL_SCANCODE_CUT, AGE_CUT },
		{ SDL_SCANCODE_COPY, AGE_COPY },
		{ SDL_SCANCODE_PASTE, AGE_PASTE },
		{ SDL_SCANCODE_FIND, AGE_FIND },
		{ SDL_SCANCODE_MUTE, AGE_MUTE },
		{ SDL_SCANCODE_VOLUMEUP, AGE_VOLUMEUP },
		{ SDL_SCANCODE_VOLUMEDOWN, AGE_VOLUMEDOWN },
		{ SDL_SCANCODE_ALTERASE, AGE_ALTERASE },
		{ SDL_SCANCODE_SYSREQ, AGE_SYSREQ },
		{ SDL_SCANCODE_CANCEL, AGE_CANCEL },
		{ SDL_SCANCODE_CLEAR, AGE_CLEAR },
		{ SDL_SCANCODE_PRIOR, AGE_PRIOR },
		{ SDL_SCANCODE_RETURN2, AGE_RETURN2 },
		{ SDL_SCANCODE_SEPARATOR, AGE_SEPARATOR },
		{ SDL_SCANCODE_OUT, AGE_OUT },
		{ SDL_SCANCODE_OPER, AGE_OPER },
		{ SDL_SCANCODE_CLEARAGAIN, AGE_CLEARAGAIN },
		{ SDL_SCANCODE_CRSEL, AGE_CRSEL },
		{ SDL_SCANCODE_EXSEL, AGE_EXSEL },
		{ SDL_SCANCODE_LCTRL, AGE_LCTRL },
		{ SDL_SCANCODE_LSHIFT, AGE_LSHIFT },
		{ SDL_SCANCODE_LALT, AGE_LALT },
		{ SDL_SCANCODE_LGUI, AGE_LGUI },
		{ SDL_SCANCODE_RCTRL, AGE_RCTRL },
		{ SDL_SCANCODE_RSHIFT, AGE_RSHIFT },
		{ SDL_SCANCODE_RALT, AGE_RALT },
		{ SDL_SCANCODE_RGUI, AGE_RGUI },
		{ SDL_SCANCODE_MODE, AGE_MODE },
		{ SDL_SCANCODE_AUDIONEXT, AGE_AUDIONEXT },
		{ SDL_SCANCODE_AUDIOPREV, AGE_AUDIOPREV },
		{ SDL_SCANCODE_AUDIOSTOP, AGE_AUDIOSTOP },
		{ SDL_SCANCODE_AUDIOPLAY, AGE_AUDIOPLAY },
		{ SDL_SCANCODE_AUDIOMUTE, AGE_AUDIOMUTE },
		{ SDL_SCANCODE_MEDIASELECT, AGE_MEDIASELECT },
		{ SDL_SCANCODE_WWW, AGE_WWW },
		{ SDL_SCANCODE_MAIL, AGE_MAIL },
		{ SDL_SCANCODE_CALCULATOR, AGE_CALCULATOR },
		{ SDL_SCANCODE_COMPUTER, AGE_COMPUTER },
		{ SDL_SCANCODE_AC_SEARCH, AGE_AC_SEARCH },
		{ SDL_SCANCODE_AC_HOME, AGE_AC_HOME },
		{ SDL_SCANCODE_AC_BACK, AGE_AC_BACK },
		{ SDL_SCANCODE_AC_FORWARD, AGE_AC_FORWARD },
		{ SDL_SCANCODE_AC_STOP, AGE_AC_STOP },
		{ SDL_SCANCODE_AC_REFRESH, AGE_AC_REFRESH },
		{ SDL_SCANCODE_AC_BOOKMARKS, AGE_AC_BOOKMARKS },
		{ SDL_SCANCODE_BRIGHTNESSDOWN, AGE_BRIGHTNESSDOWN },
		{ SDL_SCANCODE_BRIGHTNESSUP, AGE_BRIGHTNESSUP },
		{ SDL_SCANCODE_DISPLAYSWITCH, AGE_DISPLAYSWITCH },
		{ SDL_SCANCODE_KBDILLUMTOGGLE, AGE_KBDILLUMTOGGLE },
		{ SDL_SCANCODE_KBDILLUMDOWN, AGE_KBDILLUMDOWN },
		{ SDL_SCANCODE_KBDILLUMUP, AGE_KBDILLUMUP },
		{ SDL_SCANCODE_EJECT, AGE_EJECT },
		{ SDL_SCANCODE_SLEEP, AGE_SLEEP }
	};

	std::map<int, AgeWindowInputs> IRenderContext::_toAgeWindowInput =
	{
		{ SDL_WINDOWEVENT_SHOWN, AGE_WINDOW_SHOWN },
		{ SDL_WINDOWEVENT_HIDDEN, AGE_WINDOW_HIDDEN },
		{ SDL_WINDOWEVENT_EXPOSED, AGE_WINDOW_EXPOSED },
		{ SDL_WINDOWEVENT_MOVED, AGE_WINDOW_MOVED },
		{ SDL_WINDOWEVENT_RESIZED, AGE_WINDOW_RESIZED },
		{ SDL_WINDOWEVENT_MINIMIZED, AGE_WINDOW_MINIMIZED },
		{ SDL_WINDOWEVENT_MAXIMIZED, AGE_WINDOW_MAXIMIZED },
		{ SDL_WINDOWEVENT_RESTORED, AGE_WINDOW_RESTORED },
		{ SDL_WINDOWEVENT_ENTER, AGE_WINDOW_ENTER },
		{ SDL_WINDOWEVENT_LEAVE, AGE_WINDOW_LEAVE },
		{ SDL_WINDOWEVENT_FOCUS_GAINED, AGE_WINDOW_FOCUS_GAINED },
		{ SDL_WINDOWEVENT_FOCUS_LOST, AGE_WINDOW_FOCUS_LOST },
		{ SDL_WINDOWEVENT_CLOSE, AGE_WINDOW_CLOSE },
		{ SDL_QUIT, AGE_WINDOW_CLOSE }
	};

	std::map<int, AgeMouseButtons> IRenderContext::_toAgeMouseButton =
	{
		{ SDL_BUTTON_LEFT, AGE_MOUSE_LEFT },
		{ SDL_BUTTON_MIDDLE, AGE_MOUSE_MIDDLE },
		{ SDL_BUTTON_RIGHT, AGE_MOUSE_RIGHT },
	};

	std::map<int, AgeJoystickAxis> IRenderContext::_toAgeJoystickAxis =
	{
		{ SDL_CONTROLLER_AXIS_INVALID, AGE_JOYSTICK_AXIS_UNKNOWN },
		{ SDL_CONTROLLER_AXIS_LEFTX, AGE_JOYSTICK_AXIS_LEFTX },
		{ SDL_CONTROLLER_AXIS_LEFTY, AGE_JOYSTICK_AXIS_LEFTY },
		{ SDL_CONTROLLER_AXIS_RIGHTX, AGE_JOYSTICK_AXIS_RIGHTX },
		{ SDL_CONTROLLER_AXIS_RIGHTY, AGE_JOYSTICK_AXIS_RIGHTY },
		{ SDL_CONTROLLER_AXIS_TRIGGERLEFT, AGE_JOYSTICK_AXIS_TRIGGERLEFT },
		{ SDL_CONTROLLER_AXIS_TRIGGERRIGHT, AGE_JOYSTICK_AXIS_TRIGGERRIGHT },
	};

	std::map<int, AgeJoystickButtons> IRenderContext::_toAgeJoystickButtons =
	{
		{ SDL_CONTROLLER_BUTTON_INVALID, AGE_JOYSTICK_BUTTON_UNKNOWN },
		{ SDL_CONTROLLER_BUTTON_A, AGE_JOYSTICK_BUTTON_A },
		{ SDL_CONTROLLER_BUTTON_B, AGE_JOYSTICK_BUTTON_B },
		{ SDL_CONTROLLER_BUTTON_X, AGE_JOYSTICK_BUTTON_X },
		{ SDL_CONTROLLER_BUTTON_Y, AGE_JOYSTICK_BUTTON_Y },
		{ SDL_CONTROLLER_BUTTON_BACK, AGE_JOYSTICK_BUTTON_BACK },
		{ SDL_CONTROLLER_BUTTON_GUIDE, AGE_JOYSTICK_BUTTON_GUIDE },
		{ SDL_CONTROLLER_BUTTON_START, AGE_JOYSTICK_BUTTON_START },
		{ SDL_CONTROLLER_BUTTON_LEFTSTICK, AGE_JOYSTICK_BUTTON_LEFTSTICK },
		{ SDL_CONTROLLER_BUTTON_RIGHTSTICK, AGE_JOYSTICK_BUTTON_RIGHTSTICK },
		{ SDL_CONTROLLER_BUTTON_LEFTSHOULDER, AGE_JOYSTICK_BUTTON_LEFTSHOULDER },
		{ SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, AGE_JOYSTICK_BUTTON_RIGHTSHOULDER },
		{ SDL_CONTROLLER_BUTTON_DPAD_UP, AGE_JOYSTICK_BUTTON_DPAD_UP },
		{ SDL_CONTROLLER_BUTTON_DPAD_DOWN, AGE_JOYSTICK_BUTTON_DPAD_DOWN },
		{ SDL_CONTROLLER_BUTTON_DPAD_LEFT, AGE_JOYSTICK_BUTTON_DPAD_LEFT },
		{ SDL_CONTROLLER_BUTTON_DPAD_RIGHT, AGE_JOYSTICK_BUTTON_DPAD_RIGHT },
	};

	std::map<int, AgeJoystickHatDirections> IRenderContext::_toAgeJoystickHatDirections =
	{
		{ SDL_HAT_CENTERED, AGE_JOYSTICK_HAT_CENTERED },
		{ SDL_HAT_UP, AGE_JOYSTICK_HAT_UP },
		{ SDL_HAT_RIGHT, AGE_JOYSTICK_HAT_RIGHT },
		{ SDL_HAT_DOWN, AGE_JOYSTICK_HAT_DOWN },
		{ SDL_HAT_LEFT, AGE_JOYSTICK_HAT_LEFT },
		{ SDL_HAT_LEFTUP, AGE_JOYSTICK_HAT_LEFT_UP },
		{ SDL_HAT_LEFTDOWN, AGE_JOYSTICK_HAT_LEFT_DOWN },
		{ SDL_HAT_RIGHTUP, AGE_JOYSTICK_HAT_RIGHT_UP },
		{ SDL_HAT_RIGHTDOWN, AGE_JOYSTICK_HAT_RIGHT_DOWN },
	};
}