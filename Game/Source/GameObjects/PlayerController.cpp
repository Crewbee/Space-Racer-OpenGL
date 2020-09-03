#include "GamePCH.h"

#include "GameObjects/PlayerController.h"

void PlayerController::OnEvent(Event* pEvent)
{
    if( pEvent->GetEventType() == EventType_Input )
    {
#if WIN32
        InputEvent* pInput = static_cast<InputEvent*>( pEvent );

        if( pInput->GetInputDeviceType() == InputDeviceType_Keyboard )
        {
            int keyID = pInput->GetID();

            if( pInput->GetInputState() == InputState_Pressed )
            {
                if( keyID == VK_UP    || keyID == 'W' ) { m_Directions |= DIR_UP; }
                if( keyID == VK_DOWN  || keyID == 'S' ) { m_Directions |= DIR_DOWN; }
                if( keyID == VK_LEFT  || keyID == 'A' ) { m_Directions |= DIR_LEFT; }
                if( keyID == VK_RIGHT || keyID == 'D' ) { m_Directions |= DIR_RIGHT; }
                if( keyID == 'Z'      || keyID == ' ' ) { m_Directions |= DIR_JUMP; }
            }

            if( pInput->GetInputState() == InputState_Released )
            {
                if( keyID == VK_UP    || keyID == 'W' ) { m_Directions &= ~DIR_UP; }
                if( keyID == VK_DOWN  || keyID == 'S' ) { m_Directions &= ~DIR_DOWN; }
                if( keyID == VK_LEFT  || keyID == 'A' ) { m_Directions &= ~DIR_LEFT; }
                if( keyID == VK_RIGHT || keyID == 'D' ) { m_Directions &= ~DIR_RIGHT; }
                if( keyID == 'Z'      || keyID == ' ' ) { m_Directions &= ~DIR_JUMP; }
            }
        }
		if (pInput->GetInputDeviceType() == InputDeviceType_Mouse)
		{
			if (pInput->GetInputState() == InputState_Pressed && pInput->GetID() == 0)
			{
				FireWeapon = 1;
			}
			else
			{
				FireWeapon = 0;
			}

		}
#endif
    }
}
