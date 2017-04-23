#include <Windows.h>
#include <stdio.h>
#include <iostream>

//Maximum amount of clicks in one macro sequence.
const int MacroLength = 100;

struct Macro
{
	int SleepTime;
	int SleepTimeBeforeClick;
	int MacroIndex;
	POINT CursorPositions[MacroLength];
};



void MouseLeftClick ()
{
     INPUT Input = {0};
     Input.type = INPUT_MOUSE;
     Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
     ::SendInput(1,&Input,sizeof(INPUT));
     
     ::ZeroMemory(&Input,sizeof(INPUT));
     Input.type = INPUT_MOUSE;
     Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
     ::SendInput(1,&Input,sizeof(INPUT));
}


//Insert new position if there is space left.
void InsertMacroPos(Macro* macro)
{
	if(macro->MacroIndex + 1 < MacroLength)
	{
		macro->MacroIndex++;
		GetCursorPos(&macro->CursorPositions[macro->MacroIndex]);

		std::cout << "New Macro Position Inserted. Index: " << macro->MacroIndex << " Position: " << 
		macro->CursorPositions[macro->MacroIndex].x << ", " <<  macro->CursorPositions[macro->MacroIndex].y << std::endl;
	}
}


//Execute macro.
void ExecuteMacro(Macro* macro)
{
	for (int i = 0; i < macro->MacroIndex + 1; i++)
	{
		SetCursorPos(macro->CursorPositions[i].x, macro->CursorPositions[i].y);
		Sleep(macro->SleepTimeBeforeClick);
		MouseLeftClick();
		Sleep(macro->SleepTime);
	}
}

//Initialize new macro.
Macro CreateNewMacro(int sleepBeforeClick, int sleep)
{
	Macro macro;
	macro.SleepTime = sleep;
	macro.SleepTimeBeforeClick = sleepBeforeClick;
	macro.MacroIndex = -1;
	return macro;
}

int main()
{
	bool macroInsertDone = true;
	bool execMacroDone = true;
	Macro m = CreateNewMacro(50, 50);

	while(true)
	{
		//Get 'S' Key
		if(GetAsyncKeyState('S'))
		{
			macroInsertDone = false;
		}

		else if(!GetAsyncKeyState('S'))
		{
			if(!macroInsertDone)
			{
				InsertMacroPos(&m);
			}
			macroInsertDone = true;
		}


		//Get 'Q' Key
		if(GetAsyncKeyState('Q'))
		{
			execMacroDone = false;
		}

		else if(!GetAsyncKeyState('Q'))
		{
			if(!execMacroDone)
			{
				ExecuteMacro(&m);
			}
			execMacroDone = true;
		}

		Sleep(10);
	}
	
	return 0;
}
