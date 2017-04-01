//* Default Constructor
inline InputManager::InputManager()
{
	SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

	m_controllerEvents[SDL_CONTROLLER_BUTTON_INVALID] = EventListener::ControllerEvent::BUTTON_INVALID; 
	m_controllerEvents[SDL_CONTROLLER_BUTTON_A] = EventListener::ControllerEvent::BUTTON_A; 
	m_controllerEvents[SDL_CONTROLLER_BUTTON_B] = EventListener::ControllerEvent::BUTTON_B; 
	m_controllerEvents[SDL_CONTROLLER_BUTTON_X] = EventListener::ControllerEvent::BUTTON_X; 
	m_controllerEvents[SDL_CONTROLLER_BUTTON_Y] = EventListener::ControllerEvent::BUTTON_Y; 
	m_controllerEvents[SDL_CONTROLLER_BUTTON_BACK] = EventListener::ControllerEvent::BUTTON_BACK; 
	m_controllerEvents[SDL_CONTROLLER_BUTTON_GUIDE] = EventListener::ControllerEvent::BUTTON_GUIDE; 
	m_controllerEvents[SDL_CONTROLLER_BUTTON_START] = EventListener::ControllerEvent::BUTTON_START; 
	m_controllerEvents[SDL_CONTROLLER_BUTTON_LEFTSTICK] = EventListener::ControllerEvent::BUTTON_LEFTSTICK; 
	m_controllerEvents[SDL_CONTROLLER_BUTTON_RIGHTSTICK] = EventListener::ControllerEvent::BUTTON_RIGHTSTICK; 
	m_controllerEvents[SDL_CONTROLLER_BUTTON_LEFTSHOULDER] = EventListener::ControllerEvent::BUTTON_LEFTSHOULDER; 
	m_controllerEvents[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] = EventListener::ControllerEvent::BUTTON_RIGHTSHOULDER; 
	m_controllerEvents[SDL_CONTROLLER_BUTTON_DPAD_UP] = EventListener::ControllerEvent::BUTTON_DPAD_UP; 
	m_controllerEvents[SDL_CONTROLLER_BUTTON_DPAD_DOWN] = EventListener::ControllerEvent::BUTTON_DPAD_DOWN; 
	m_controllerEvents[SDL_CONTROLLER_BUTTON_DPAD_LEFT] = EventListener::ControllerEvent::BUTTON_DPAD_LEFT; 
	m_controllerEvents[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] = EventListener::ControllerEvent::BUTTON_DPAD_RIGHT; 

}

//* Used to get the Class Instance
inline InputManager* InputManager::GetInstance()
{
	if (inputManagerInstance == nullptr)
	{
		inputManagerInstance = new InputManager();
	}
	//* Used to return the same instance of the Class
	return inputManagerInstance;
}


//* Find a Command object in the commands dictionary, and call its execute function based on Event Type
inline void InputManager::Execute(EventListener::EventType type, EventListener::KeyboardEvent evt)
{

	for (auto const &command : m_eventCommands[evt])
	{
		//// Execution calls for firing custom function sent in
		if (type == EventListener::EventType::Press) { command->executePress(); }
		else if (type == EventListener::EventType::Release) { command->executeRelease(); }
		else if (type == EventListener::EventType::Hold) { command->executeHold(); }
		else { command->execute(); }
	}

	//* Set Held to true for Event if the key was released, based on type
	if (type == EventListener::EventType::Press)
	{
		m_eventWasFiring[evt] = true;
	}
	else if (type == EventListener::EventType::Release)
	{
		m_eventWasFiring[evt] = false;
	}
}

//* Find a Command object in the commands dictionary, and call its execute function based on Event Type
inline void InputManager::Execute(EventListener::EventType type, EventListener::ControllerEvent evt, int controllerId)
{

	for (auto const &command : m_controllerCommands[controllerId][evt])
	{
		//// Execution calls for firing custom function sent in
		if (type == EventListener::EventType::Press) { command->executePress(); }
		else if (type == EventListener::EventType::Release) { command->executeRelease(); }
		else if (type == EventListener::EventType::Hold) { command->executeHold(); }
		else { command->execute(); }
	}

	//* Set Held to true for Event if the key was released, based on type
	if (type == EventListener::EventType::Press)
	{
		m_controllerEventWasFiring[controllerId][evt] = true;
	}
	else if (type == EventListener::EventType::Release)
	{
		m_controllerEventWasFiring[controllerId][evt] = false;
	}
}


//* Required to update the input
inline void InputManager::ProcessInput()
{
	//* New Input Library Event
	SDL_Event evn;

	//* New Event Listener Type
	EventListener::EventType type;

	while (SDL_PollEvent(&evn) != 0)
	{
		//* Get Event Type
		switch (evn.type)
		{
		case SDL_KEYDOWN: type = EventListener::EventType::Press;	break;
		case SDL_KEYUP: type = EventListener::EventType::Release;	break;

		default: type = EventListener::EventType::None;				break;
		}

		//* Get Key Event and call Input Manager Execute for that key
		switch (evn.key.keysym.sym)
		{
		case SDLK_UNKNOWN:		Execute(type, EventListener::KeyboardEvent::UNKNOWN);			break;
		case SDLK_RETURN:		Execute(type, EventListener::KeyboardEvent::RETURN);			break;
		case SDLK_ESCAPE:		Execute(type, EventListener::KeyboardEvent::ESCAPE);			break;
		case SDLK_BACKSPACE:	Execute(type, EventListener::KeyboardEvent::BACKSPACE);		break;
		case SDLK_TAB:			Execute(type, EventListener::KeyboardEvent::TAB);				break;
		case SDLK_SPACE:		Execute(type, EventListener::KeyboardEvent::SPACE);			break;
		case SDLK_EXCLAIM:		Execute(type, EventListener::KeyboardEvent::EXCLAIM);			break;
		case SDLK_QUOTEDBL:		Execute(type, EventListener::KeyboardEvent::QUOTEDBL);			break;
		case SDLK_HASH:			Execute(type, EventListener::KeyboardEvent::HASH);				break;
		case SDLK_PERCENT:		Execute(type, EventListener::KeyboardEvent::PERCENT);			break;
		case SDLK_DOLLAR:		Execute(type, EventListener::KeyboardEvent::DOLLAR);			break;
		case SDLK_AMPERSAND:	Execute(type, EventListener::KeyboardEvent::AMPERSAND);		break;
		case SDLK_QUOTE:		Execute(type, EventListener::KeyboardEvent::QUOTE);			break;
		case SDLK_LEFTPAREN:	Execute(type, EventListener::KeyboardEvent::LEFTPAREN);		break;
		case SDLK_RIGHTPAREN:	Execute(type, EventListener::KeyboardEvent::RIGHTPAREN);		break;
		case SDLK_ASTERISK:		Execute(type, EventListener::KeyboardEvent::ASTERISK);			break;
		case SDLK_PLUS:			Execute(type, EventListener::KeyboardEvent::PLUS);				break;
		case SDLK_COMMA:		Execute(type, EventListener::KeyboardEvent::COMMA);			break;
		case SDLK_MINUS:		Execute(type, EventListener::KeyboardEvent::MINUS);			break;
		case SDLK_PERIOD:		Execute(type, EventListener::KeyboardEvent::PERIOD);			break;
		case SDLK_SLASH:		Execute(type, EventListener::KeyboardEvent::SLASH);			break;
		case SDLK_0:			Execute(type, EventListener::KeyboardEvent::NUM_0);			break;
		case SDLK_1:			Execute(type, EventListener::KeyboardEvent::NUM_1);			break;
		case SDLK_2:			Execute(type, EventListener::KeyboardEvent::NUM_2);			break;
		case SDLK_3:			Execute(type, EventListener::KeyboardEvent::NUM_3);			break;
		case SDLK_4:			Execute(type, EventListener::KeyboardEvent::NUM_4);			break;
		case SDLK_5:			Execute(type, EventListener::KeyboardEvent::NUM_5);			break;
		case SDLK_6:			Execute(type, EventListener::KeyboardEvent::NUM_6);			break;
		case SDLK_7:			Execute(type, EventListener::KeyboardEvent::NUM_7);			break;
		case SDLK_8:			Execute(type, EventListener::KeyboardEvent::NUM_8);			break;
		case SDLK_9:			Execute(type, EventListener::KeyboardEvent::NUM_9);			break;
		case SDLK_COLON:		Execute(type, EventListener::KeyboardEvent::COLON);			break;
		case SDLK_SEMICOLON:	Execute(type, EventListener::KeyboardEvent::SEMICOLON);		break;
		case SDLK_LESS:			Execute(type, EventListener::KeyboardEvent::LESS);				break;
		case SDLK_EQUALS:		Execute(type, EventListener::KeyboardEvent::EQUALS);			break;
		case SDLK_GREATER:		Execute(type, EventListener::KeyboardEvent::GREATER);			break;
		case SDLK_QUESTION:		Execute(type, EventListener::KeyboardEvent::QUESTION);			break;
		case SDLK_AT:			Execute(type, EventListener::KeyboardEvent::AT);				break;
		case SDLK_LEFTBRACKET:	Execute(type, EventListener::KeyboardEvent::LEFTBRACKET);		break;
		case SDLK_BACKSLASH:	Execute(type, EventListener::KeyboardEvent::BACKSLASH);		break;
		case SDLK_RIGHTBRACKET: Execute(type, EventListener::KeyboardEvent::RIGHTBRACKET);		break;
		case SDLK_CARET:		Execute(type, EventListener::KeyboardEvent::CARET);			break;
		case SDLK_UNDERSCORE:	Execute(type, EventListener::KeyboardEvent::UNDERSCORE);		break;
		case SDLK_BACKQUOTE:	Execute(type, EventListener::KeyboardEvent::BACKQUOTE);		break;
		case SDLK_a:			Execute(type, EventListener::KeyboardEvent::a);				break;
		case SDLK_b:			Execute(type, EventListener::KeyboardEvent::b);				break;
		case SDLK_c:			Execute(type, EventListener::KeyboardEvent::c);				break;
		case SDLK_d:			Execute(type, EventListener::KeyboardEvent::d);				break;
		case SDLK_e:			Execute(type, EventListener::KeyboardEvent::e);				break;
		case SDLK_f:			Execute(type, EventListener::KeyboardEvent::f);				break;
		case SDLK_g:			Execute(type, EventListener::KeyboardEvent::g);				break;
		case SDLK_h:			Execute(type, EventListener::KeyboardEvent::h);				break;
		case SDLK_i:			Execute(type, EventListener::KeyboardEvent::i);				break;
		case SDLK_j:			Execute(type, EventListener::KeyboardEvent::j);				break;
		case SDLK_k:			Execute(type, EventListener::KeyboardEvent::k);				break;
		case SDLK_l:			Execute(type, EventListener::KeyboardEvent::l);				break;
		case SDLK_m:			Execute(type, EventListener::KeyboardEvent::m);				break;
		case SDLK_n:			Execute(type, EventListener::KeyboardEvent::n);				break;
		case SDLK_o:			Execute(type, EventListener::KeyboardEvent::o);				break;
		case SDLK_p:			Execute(type, EventListener::KeyboardEvent::p);				break;
		case SDLK_q:			Execute(type, EventListener::KeyboardEvent::q);				break;
		case SDLK_r:			Execute(type, EventListener::KeyboardEvent::r);				break;
		case SDLK_s:			Execute(type, EventListener::KeyboardEvent::s);				break;
		case SDLK_t:			Execute(type, EventListener::KeyboardEvent::t);				break;
		case SDLK_u:			Execute(type, EventListener::KeyboardEvent::u);				break;
		case SDLK_v:			Execute(type, EventListener::KeyboardEvent::v);				break;
		case SDLK_w:			Execute(type, EventListener::KeyboardEvent::w);				break;
		case SDLK_x:			Execute(type, EventListener::KeyboardEvent::x);				break;
		case SDLK_y:			Execute(type, EventListener::KeyboardEvent::y);				break;
		case SDLK_z:			Execute(type, EventListener::KeyboardEvent::z);				break;
		case SDLK_UP:			Execute(type, EventListener::KeyboardEvent::ARROW_UP);				break;
		case SDLK_LEFT:			Execute(type, EventListener::KeyboardEvent::ARROW_LEFT);				break;
		case SDLK_DOWN:			Execute(type, EventListener::KeyboardEvent::ARROW_DOWN);				break;
		case SDLK_RIGHT:		Execute(type, EventListener::KeyboardEvent::ARROW_RIGHT);				break;

		default: break;
		}


		//* Controller
		//* Get Event Listener Type
		switch (evn.type)
		{
			//* Controller Connect / Disconnect
		case SDL_CONTROLLERDEVICEADDED: AddController(evn.cdevice.which);	break;
		case SDL_CONTROLLERDEVICEREMOVED: RemoveController(evn.cdevice.which); break;

		default: type = EventListener::EventType::None;				break;
		}

		//* Controller is connected
		int joystickId = evn.cdevice.which;

		if (m_controllerVars.find(joystickId) != m_controllerVars.end())
		{
			int controllerId = m_controllerVars[joystickId].controllerId;

			if (m_controllerVars[joystickId].mGameController && SDL_GameControllerGetAttached(m_controllerVars[joystickId].mGameController))
			{
				//* Controller Button Pressed
				if (evn.type == SDL_CONTROLLERBUTTONDOWN)
				{
					//* Set Type
					type = EventListener::EventType::Press;

					Execute(type, m_controllerEvents[evn.cbutton.button], controllerId);

					m_controllerVars[joystickId].countedButtonFrames = -m_controllerVars[joystickId].controllerButtonDelay * 8;
				}

				else if (evn.type == SDL_CONTROLLERBUTTONUP)
				{
					//* Set Type
					type = EventListener::EventType::Release;

					//* Execute
					Execute(type, m_controllerEvents[evn.cbutton.button], controllerId);

					//* Used to Delay Hold
					m_controllerVars[joystickId].countedButtonFrames = -m_controllerVars[joystickId].controllerButtonDelay * 8;
				}

				//* Joystick
				//* Get If Axis Is Moving
				if (evn.type == SDL_JOYAXISMOTION)
				{
					//* Left Stick X Axis
					if (evn.jaxis.axis == 0) { m_controllerVars[joystickId].stick_Left_X = evn.jaxis.value; }
					//* Left Stick Y Axis	   
					if (evn.jaxis.axis == 1) { m_controllerVars[joystickId].stick_Left_Y = evn.jaxis.value; }
					//* Left Trigger Axis	   
					if (evn.jaxis.axis == 2) { m_controllerVars[joystickId].stick_Left_T = evn.jaxis.value; }
					//* Right Stick X Axis	   
					if (evn.jaxis.axis == 3) { m_controllerVars[joystickId].stick_Right_X = evn.jaxis.value; }
					//* Right Stick Y Axis	   
					if (evn.jaxis.axis == 4) { m_controllerVars[joystickId].stick_Right_Y = evn.jaxis.value; }
					//* Right Trigger Axis	   
					if (evn.jaxis.axis == 5) { m_controllerVars[joystickId].stick_Right_T = evn.jaxis.value; }

					// Left Trigger Press/ Relaase
					if (evn.jaxis.axis == 2) {
						if (GetLeftTrigger(joystickId) > 32000) { Execute(EventListener::EventType::Press, EventListener::ControllerEvent::TRIGGER_LEFT, controllerId); }
						if (GetLeftTrigger(joystickId) < -32000) { Execute(EventListener::EventType::Release, EventListener::ControllerEvent::TRIGGER_LEFT, controllerId); }
					}

					// Right Trigger Press/ Relaase
					if (evn.jaxis.axis == 5) {
						if (GetRightTrigger(joystickId) > 32000) { Execute(EventListener::EventType::Hold, EventListener::ControllerEvent::TRIGGER_RIGHT, controllerId); }
						if (GetRightTrigger(joystickId) < -32000) { Execute(EventListener::EventType::Hold, EventListener::ControllerEvent::TRIGGER_RIGHT, controllerId); }
					}
				}
			}

		}

		//* Mouse
		if (evn.type == SDL_MOUSEBUTTONDOWN)
		{
			type = EventListener::EventType::Press;
			if (evn.button.button == SDL_BUTTON_LEFT) { Execute(type, EventListener::KeyboardEvent::MOUSE_LEFT); }
			else if (evn.button.button == SDL_BUTTON_RIGHT) { Execute(type, EventListener::KeyboardEvent::MOUSE_RIGHT); }
			else if (evn.button.button == SDL_BUTTON_MIDDLE) { Execute(type, EventListener::KeyboardEvent::MOUSE_MIDDLE); }
		}
		if (evn.type == SDL_MOUSEBUTTONUP)
		{
			type = EventListener::EventType::Release;
			if (evn.button.button == SDL_BUTTON_LEFT) { Execute(type, EventListener::KeyboardEvent::MOUSE_LEFT); }
			else if (evn.button.button == SDL_BUTTON_RIGHT) { Execute(type, EventListener::KeyboardEvent::MOUSE_RIGHT); }
			else if (evn.button.button == SDL_BUTTON_MIDDLE) { Execute(type, EventListener::KeyboardEvent::MOUSE_MIDDLE); }
		}

		if (evn.type == SDL_MOUSEWHEEL)
		{
			type = EventListener::EventType::Press;
			if (evn.wheel.y > 0)
			{
				Execute(type, EventListener::KeyboardEvent::MOUSE_WHEEL_UP);
			}
			else if (evn.wheel.y < 0)
			{
				Execute(type, EventListener::KeyboardEvent::MOUSE_WHEEL_DOWN);
			}
		}

	}


	for (auto& idControllerData : m_controllerVars)
	{
		auto& controllerData = idControllerData.second;
		int id = idControllerData.first;
		if (id == -1)
		{
			continue;
		}
		//* Controller is connected
		if (controllerData.mGameController && SDL_GameControllerGetAttached(controllerData.mGameController))
		{
			//* Controller Button Hold Update
			controllerData.countedButtonFrames++;
			if (controllerData.countedButtonFrames > controllerData.controllerButtonDelay)
			{
				//* For Each Button in the Map
				for (auto const &button : m_controllerEventWasFiring[controllerData.controllerId])
				{
					if (button.second) { Execute(EventListener::EventType::Hold, button.first, controllerData.controllerId); }
					else { m_controllerEventWasFiring[controllerData.controllerId][button.first] = false; }
				}
				controllerData.countedButtonFrames = 0;
			}

			//* Controller Trigger Hold Update
			controllerData.countedTriggerFrames++;
			if (controllerData.countedTriggerFrames > controllerData.controllerButtonDelay)
			{
				if (GetLeftTrigger(controllerData.controllerId) > 0) { Execute(EventListener::EventType::Hold, EventListener::ControllerEvent::TRIGGER_LEFT, controllerData.controllerId); }
				else if (GetLeftTrigger(controllerData.controllerId) > -30000 && GetLeftTrigger(controllerData.controllerId) < 0) { Execute(EventListener::EventType::Hold, EventListener::ControllerEvent::TRIGGER_SOFT_LEFT, controllerData.controllerId); }

				if (GetRightTrigger(controllerData.controllerId) > 0) { Execute(EventListener::EventType::Hold, EventListener::ControllerEvent::TRIGGER_RIGHT, controllerData.controllerId); }
				else if (GetRightTrigger(controllerData.controllerId) > -30000 && GetRightTrigger(controllerData.controllerId) < 0) { Execute(EventListener::EventType::Hold, EventListener::ControllerEvent::TRIGGER_SOFT_RIGHT, controllerData.controllerId); }

				controllerData.countedTriggerFrames = 0;
			}
		}
	}
}

//// Controller  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//* Add Contoller if Detected
inline void InputManager::AddController(int id)
{
	//* Assign Current Controller
	if (SDL_IsGameController(id))
	{
		SDL_GameController* gameController = SDL_GameControllerOpen(id);

		if (gameController)
		{
			//* Assign Joysticks
			SDL_Joystick* joystick = SDL_GameControllerGetJoystick(gameController);
			int joystickId = SDL_JoystickInstanceID(joystick);
			m_controllerVars[joystickId].mGameController = gameController;
			m_controllerVars[joystickId].controllerId = id;
		}
	}
}

//* Disconnect Controller
inline void InputManager::RemoveController(int id)
{
	SDL_GameControllerClose(m_controllerVars[id].mGameController);
	m_controllerVars.erase(id);
}





//-------------------------------------------------------

inline int InputManager::getJoystickIdByControllerId(int controllerId)
{
	for (auto& controllerData : m_controllerVars)
	{
		if (controllerId == controllerData.second.controllerId)
		{
			return controllerData.first;
		}
	}
	return -1;
}

//* Return X, Y for Left Stick Vector
inline Vector2f InputManager::GetLeftStickVector(int controllerId)
{
	int id = getJoystickIdByControllerId(controllerId);
	if (Vector2f(m_controllerVars[id].stick_Left_X, m_controllerVars[id].stick_Left_Y).magnitude() > m_controllerVars[id].stick_Dead_Zone)
	{
		return Vector2f(m_controllerVars[id].stick_Left_X, m_controllerVars[id].stick_Left_Y);
	}
	else
	{
		return Vector2f(0, 0);
	}
}

//* Return X, Y for Left Stick Vector Normal
inline Vector2f InputManager::GetLeftStickVectorNormal(int controllerId)
{
	int id = getJoystickIdByControllerId(controllerId);
	if (Vector2f(m_controllerVars[id].stick_Left_X, m_controllerVars[id].stick_Left_Y).magnitude() > m_controllerVars[id].stick_Dead_Zone)
	{
		return Vector2f(m_controllerVars[id].stick_Left_X, m_controllerVars[id].stick_Left_Y).normalise();
	}
	else
	{
		return Vector2f(0, 0);
	}
}

inline void InputManager::DeleteCallbacksOwnedBy(EventListener* owner)
{
	for (auto& command : m_eventOwnerMap[owner])
	{
		for (auto& keyVector : m_eventCommands)
		{
			keyVector.second.erase(std::remove(keyVector.second.begin(), keyVector.second.end(), command), keyVector.second.end());
		}
		for (int i = 0; i < MAX_CONTROLLERS; i++)
		{
			for (auto& keyVector : m_controllerCommands[i])
			{
				keyVector.second.erase(std::remove(keyVector.second.begin(), keyVector.second.end(), command), keyVector.second.end());
			}
		}

		delete command;
	}
}

//* Return X, Y for Left Stick Angle
inline float InputManager::GetLeftStickAngle(int controllerId)
{
	int id = getJoystickIdByControllerId(controllerId);
	return (180 - (180 * (atan2(m_controllerVars[id].stick_Left_X, m_controllerVars[id].stick_Left_Y)) / M_PI));
}

//* Return Left Trigger Value
inline float InputManager::GetLeftTrigger(int controllerId)
{
	int id = getJoystickIdByControllerId(controllerId);
	return m_controllerVars[id].stick_Left_T;
}

inline EventListener::~EventListener()
{
	InputManager::GetInstance()->DeleteCallbacksOwnedBy(this);
}

//* Return X, Y for Right Stick Vector
inline Vector2f InputManager::GetRightStickVector(int controllerId)
{
	int id = getJoystickIdByControllerId(controllerId);
	if (Vector2f(m_controllerVars[id].stick_Right_X, m_controllerVars[id].stick_Right_Y).magnitude() > m_controllerVars[id].stick_Dead_Zone)
	{
		return Vector2f(m_controllerVars[id].stick_Right_X, m_controllerVars[id].stick_Right_Y);
	}
	else
	{
		return Vector2f(0, 0);
	}
}

//* Return X, Y for Right Stick Vector Normal
inline Vector2f InputManager::GetRightStickVectorNormal(int controllerId)
{
	int id = getJoystickIdByControllerId(controllerId);
	if (Vector2f(m_controllerVars[id].stick_Right_X, m_controllerVars[id].stick_Right_Y).magnitude() > m_controllerVars[id].stick_Dead_Zone)
	{
		return Vector2f(m_controllerVars[id].stick_Right_X, m_controllerVars[id].stick_Right_Y);
	}
	else
	{
		return Vector2f(0, 0);
	}
}

//* Return X, Y for Right Stick Angle
inline float InputManager::GetRightStickAngle(int controllerId)
{
	int id = getJoystickIdByControllerId(controllerId);
	return (180 - (180 * (atan2(m_controllerVars[id].stick_Right_X, m_controllerVars[id].stick_Right_Y)) / M_PI));
}

//* Return Right Trigger Value
inline float InputManager::GetRightTrigger(int controllerId)
{
	int id = getJoystickIdByControllerId(controllerId);
	return m_controllerVars[id].stick_Right_T;
}


inline void InputManager::RegisterEventCallback(EventListener::KeyboardEvent evt, Command* command, EventListener* owner)
{
	assert(owner != nullptr); //need to pass valid eventlistener pointer for memory management
	m_eventCommands[evt].push_back(command);
	m_eventOwnerMap[owner].push_back(command);
}

inline void InputManager::RegisterEventCallback(EventListener::ControllerEvent evt, Command* command, EventListener* owner, int controllerId)
{
	assert(owner != nullptr); //need to pass valid eventlistener pointer for memory management
	m_controllerCommands[controllerId][evt].push_back(command);
	m_eventOwnerMap[owner].push_back(command);
}