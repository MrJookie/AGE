#include <context/SDL/SdlContext.hh>
#include <SDL/SDL.h>

namespace AGE
{
	std::map<int, AgeKeys> IRenderContext::_toAgeKey =
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

	std::map<int, AgeInputs> IRenderContext::_toAgeInput =
	{
		{ SDL_BUTTON_LEFT, AGE_MOUSE_LEFT },
		{ SDL_BUTTON_MIDDLE, AGE_MOUSE_MIDDLE },
		{ SDL_BUTTON_RIGHT, AGE_MOUSE_RIGHT },
		{ SDL_QUIT, AGE_EXIT }
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
