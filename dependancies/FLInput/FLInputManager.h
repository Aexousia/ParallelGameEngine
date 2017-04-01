#pragma once

#include "../sdl/SDL.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include<unordered_map>
#include<vector>
#include<iostream>
#include<queue>
#include<thread>
#include<functional>
#include <assert.h>

#define MAX_CONTROLLERS 4

struct ControllerData
{
	//* Joy Stick Axis
	const int stick_Dead_Zone = 8000;
	const int controllerButtonDelay = 1;
	const int controllerTriggerDelay = 500;

	//* Left Stick Variables
	float stick_Left_X = 0;
	float stick_Left_Y = 0;
	float stick_Left_T = 0;

	//* Right Stick Variables
	float stick_Right_X = 0;
	float stick_Right_Y = 0;
	float stick_Right_T = 0;

	//* Controller Timers
	int countedButtonFrames = -controllerButtonDelay * 8;;
	int countedTriggerFrames = 0;

	int controllerId = -1;

	SDL_GameController* mGameController = nullptr;
};

//* Vector2f Class for JoyStick values
class Vector2f
{
public:
	Vector2f(float _x, float _y) : x(_x), y(_y) {}
	Vector2f() : x(0.f), y(0.f) {}

	Vector2f operator*(float m) { x *= m; y *= m; return *this; }
	Vector2f operator+(const Vector2f v) { x += v.x; y += v.y; return *this; }
	float magnitude() { return sqrt(x * x + y * y); }
	float dotProduct(const Vector2f v2) { return (x * v2.x) + (y * v2.y); }

	Vector2f normalise() {
		if (magnitude() != 0) { return Vector2f((x / magnitude()), (y / magnitude())); }
		else { return Vector2f(0, 0); }
	}

	float x;
	float y;
};

//* Abstract class for input keys, input types, mouse and controller.
class EventListener
{
public:
	//* List of possible input events
	enum KeyboardEvent {
		UNKNOWN = 0,

		RETURN = '\r',
		ESCAPE = '\033',
		BACKSPACE = '\b',
		TAB = '\t',
		SPACE = ' ',
		EXCLAIM = '!',
		QUOTEDBL = '"',
		HASH = '#',
		PERCENT = '%',
		DOLLAR = '$',
		AMPERSAND = '&',
		QUOTE = '\'',
		LEFTPAREN = '(',
		RIGHTPAREN = ')',
		ASTERISK = '*',
		PLUS = '+',
		COMMA = ',',
		MINUS = '-',
		PERIOD = '.',
		SLASH = '/',
		NUM_0 = '0',
		NUM_1 = '1',
		NUM_2 = '2',
		NUM_3 = '3',
		NUM_4 = '4',
		NUM_5 = '5',
		NUM_6 = '6',
		NUM_7 = '7',
		NUM_8 = '8',
		NUM_9 = '9',
		COLON = ':',
		SEMICOLON = ';',
		LESS = '<',
		EQUALS = '=',
		GREATER = '>',
		QUESTION = '?',
		AT = '@',
		// Skip uppercase letters
		LEFTBRACKET = '[',
		BACKSLASH = '\\',
		RIGHTBRACKET = ']',
		CARET = '^',
		UNDERSCORE = '_',
		BACKQUOTE = '`',
		a = 'a',
		b = 'b',
		c = 'c',
		d = 'd',
		e = 'e',
		f = 'f',
		g = 'g',
		h = 'h',
		i = 'i',
		j = 'j',
		k = 'k',
		l = 'l',
		m = 'm',
		n = 'n',
		o = 'o',
		p = 'p',
		q = 'q',
		r = 'r',
		s = 's',
		t = 't',
		u = 'u',
		v = 'v',
		w = 'w',
		x = 'x',
		y = 'y',
		z = 'z',
		// Arrows
		ARROW_UP = 3001,
		ARROW_LEFT,
		ARROW_DOWN,
		ARROW_RIGHT,

		// Mouse
		MOUSE_LEFT = 2001, // A Space Odyssey
		MOUSE_RIGHT,
		MOUSE_MIDDLE,
		MOUSE_WHEEL_UP,
		MOUSE_WHEEL_DOWN
	};

	enum ControllerEvent {
		// Controller
		BUTTON_INVALID = 999,
		BUTTON_A = 1000,
		BUTTON_B,
		BUTTON_X,
		BUTTON_Y,
		BUTTON_BACK,
		BUTTON_GUIDE,
		BUTTON_START,
		BUTTON_LEFTSTICK,
		BUTTON_RIGHTSTICK,
		BUTTON_LEFTSHOULDER,
		BUTTON_RIGHTSHOULDER,
		BUTTON_DPAD_UP,
		BUTTON_DPAD_DOWN,
		BUTTON_DPAD_LEFT,
		BUTTON_DPAD_RIGHT,
		TRIGGER_SOFT_LEFT,
		TRIGGER_LEFT,
		TRIGGER_SOFT_RIGHT,
		TRIGGER_RIGHT
	};

	//* List of possible event types
	enum EventType {
		Press,
		Release,
		Hold,
		None
	};

	virtual ~EventListener();
};

//* Abstract class for input commands
//* Override examples are at the bottom of FLInputManager.h
class Command
{
public:
	//* Constructor for Custom/ once off calls
	Command() {}
	//* Constructor for Keys with Event Type
	Command(EventListener::EventType type) : m_type(type) { }
	//* Default Destructor
	virtual ~Command() {}

	//* Type of Event used to differentiate commands
	EventListener::EventType m_type;

	//// Execution calls for firing custom function sent in, based on type
	virtual void execute() {};
	virtual void executePress() {};
	virtual void executeRelease() {};
	virtual void executeHold() {};
};

//* Singleton class for input management
class InputManager
{
public:
	//* Default Constructor
	InputManager();
	//* Default Destructor
	~InputManager();

	//* Used to get the Class Instance
	static InputManager* GetInstance();

	void DeleteCallbacksOwnedBy(EventListener* owner);

private:

	std::unordered_map<int, ControllerData> m_controllerVars;

	//* Dictionary holding a bool for each event
	std::unordered_map<EventListener::KeyboardEvent, bool> m_eventWasFiring; //* Bool used to detect if the desired Event was firing last frame
	std::unordered_map<EventListener::KeyboardEvent, std::vector<Command*>> m_eventCommands;
	std::unordered_map<EventListener*, std::vector<Command*>> m_eventOwnerMap;
	std::unordered_map<Uint8, EventListener::ControllerEvent> m_controllerEvents;

	std::unordered_map<EventListener::ControllerEvent, std::vector<Command*>> m_controllerCommands[MAX_CONTROLLERS];
	std::unordered_map<EventListener::ControllerEvent, bool> m_controllerEventWasFiring[MAX_CONTROLLERS];
	//* Used to return the same instance of the Class
	static InputManager* inputManagerInstance;

public:

	//* Required to update the input
	void ProcessInput();

	void RegisterEventCallback(EventListener::KeyboardEvent evt, Command* command, EventListener* owner);
	void RegisterEventCallback(EventListener::ControllerEvent evt, Command* command, EventListener* owner, int controllerId);

	//// Input Functions
private:

	//* Find a Command object in the commands dictionary, and call its execute function based on Event Type
	void Execute(EventListener::EventType, EventListener::KeyboardEvent);
	void Execute(EventListener::EventType, EventListener::ControllerEvent, int controllerId=0);

	int getJoystickIdByControllerId(int controllerId);

	//* Add Contoller if Detected
	void AddController(int id);
	//* Disconnect Controller
	void RemoveController(int id);

public:
	//* Left Stick Functions
	Vector2f GetLeftStickVector(int controllerId);
	Vector2f GetLeftStickVectorNormal(int controllerId);
	float GetLeftStickAngle(int controllerId);
	float GetLeftTrigger(int controllerId);

	//* Left Stick Functions
	Vector2f GetRightStickVector(int controllerId);
	Vector2f GetRightStickVectorNormal(int controllerId);
	float GetRightStickAngle(int controllerId);
	float GetRightTrigger(int controllerId);
};



#include "FLInputManager.inl"