#include "InputComponent.h"
#include "DebugMenu.h"

InputComponent::InputComponent(std::string nameOfTab, int upNum, int actNum, std::string actionScript)
{
	tabName=nameOfTab;
	updateNum=upNum;
	actionNum=actNum;
	sc=new ScriptComponent(actionScript);
}

void InputComponent::Initialize(ObjectHandle* oh, int menuNum)
{
	parent=oh;
	sc->InitializeInput(oh);
	time=0;
#if EDITOR_ON
	keyCode=debugMenu.comboBox("KeyCode", tabName.c_str(), parent->menuNum);
	debugMenu.comboBoxItem("None", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Up Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Down Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Left Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Right Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Q Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("W Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("E Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("R Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("T Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Y Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("U Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("I Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("O Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("P Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("A Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("S Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("D Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("F Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("G Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("H Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("J Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("K Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("L Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Z Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("X Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("C Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("V Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("B Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("N Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("M Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Space Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("1 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("2 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("3 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("4 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("5 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("6 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("7 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("8 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("9 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.changeComboBox(updateNum, tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.newRow(tabName.c_str(), parent->menuNum);

	defaultCode=debugMenu.comboBox("Action", tabName.c_str(), parent->menuNum);
	debugMenu.comboBoxItem("None", tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.comboBoxItem("Force Up", tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.comboBoxItem("Force Down", tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.comboBoxItem("Force Forward", tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.comboBoxItem("Use Script", tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.changeComboBox(actionNum, tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.newRow(tabName.c_str(), parent->menuNum);

	debugMenu.basicTextField(&sc->script, "Action", tabName.c_str(), parent->menuNum);
#endif
	type="Input";
}

void InputComponent::InitializeLight(AmbientLight* lh, int menuNum)
{
	ambientParent=lh;
#if EDITOR_ON
	keyCode=debugMenu.comboBox("KeyCode", tabName.c_str(), ambientParent->menuNum);
	debugMenu.comboBoxItem("None", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Up Key", tabName.c_str(), keyCode, ambientParent->menuNum);
	debugMenu.comboBoxItem("Down Key", tabName.c_str(), keyCode, ambientParent->menuNum);
	debugMenu.comboBoxItem("Left Key", tabName.c_str(), keyCode, ambientParent->menuNum);
	debugMenu.comboBoxItem("Right Key", tabName.c_str(), keyCode, ambientParent->menuNum);
	debugMenu.comboBoxItem("Q Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("W Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("E Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("R Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("T Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Y Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("U Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("I Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("O Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("P Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("A Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("S Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("D Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("F Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("G Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("H Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("J Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("K Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("L Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Z Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("X Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("C Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("V Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("B Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("N Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("M Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Space Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("1 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("2 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("3 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("4 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("5 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("6 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("7 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("8 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("9 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.changeComboBox(updateNum, tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.newRow(tabName.c_str(), parent->menuNum);

	defaultCode=debugMenu.comboBox("Action", tabName.c_str(), parent->menuNum);
	debugMenu.comboBoxItem("None", tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.comboBoxItem("Force Up", tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.comboBoxItem("Force Down", tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.comboBoxItem("Force Forward", tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.comboBoxItem("Use Script", tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.changeComboBox(actionNum, tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.newRow(tabName.c_str(), parent->menuNum);

	debugMenu.basicTextField(&sc->script, "Action", tabName.c_str(), parent->menuNum);
#endif
	type="Input";
}

void InputComponent::InitializeDLight(DiffuseLight* lh, int menuNum)
{
	diffuseParent=lh;
#if EDITOR_ON
	keyCode=debugMenu.comboBox("KeyCode", tabName.c_str(), lh->menuNum);
	debugMenu.comboBoxItem("None", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Up Key", tabName.c_str(), keyCode, lh->menuNum);
	debugMenu.comboBoxItem("Down Key", tabName.c_str(), keyCode, lh->menuNum);
	debugMenu.comboBoxItem("Left Key", tabName.c_str(), keyCode, lh->menuNum);
	debugMenu.comboBoxItem("Right Key", tabName.c_str(), keyCode, lh->menuNum);
	debugMenu.comboBoxItem("Q Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("W Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("E Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("R Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("T Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Y Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("U Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("I Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("O Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("P Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("A Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("S Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("D Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("F Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("G Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("H Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("J Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("K Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("L Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Z Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("X Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("C Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("V Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("B Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("N Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("M Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("Space Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("0 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("1 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("2 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("3 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("4 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("5 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("6 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("7 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("8 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.comboBoxItem("9 Key", tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.changeComboBox(updateNum, tabName.c_str(), keyCode, parent->menuNum);
	debugMenu.newRow(tabName.c_str(), parent->menuNum);

	defaultCode=debugMenu.comboBox("Action", tabName.c_str(), parent->menuNum);
	debugMenu.comboBoxItem("None", tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.comboBoxItem("Force Up", tabName.c_str() , defaultCode, parent->menuNum);
	debugMenu.comboBoxItem("Force Down", tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.comboBoxItem("Force Forward", tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.comboBoxItem("Use Script", tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.changeComboBox(actionNum, tabName.c_str(), defaultCode, parent->menuNum);
	debugMenu.newRow(tabName.c_str(), parent->menuNum);

	debugMenu.basicTextField(&sc->script, "Action", tabName.c_str(), parent->menuNum);
#endif
	type="Input";
}

void InputComponent::handleAction(float dt)
{
	if(actionNum==0)
	{

	}
	else if(actionNum==1)
	{
		parent->setYForce(100*time);
	}
	else if(actionNum==2)
	{
		parent->setYForce(-100*time);
	}
	else if(actionNum==3)
	{
		parent->moveForward(parent->camDirection*(100.0f*time));
	}
	else if(actionNum==4)
	{
		sc->update(dt);
	}
}

void InputComponent::update(float dt)
{
	if(parent)
	{
#if EDITOR_ON
		updateNum=debugMenu.checkBoxItem(tabName.c_str(), keyCode, parent->menuNum);
		actionNum=debugMenu.checkBoxItem(tabName.c_str(), defaultCode, parent->menuNum);
#endif
	}
	time+=dt;
	if(updateNum==0)
	{

	}
	else if(updateNum==1)
	{
		if(GetAsyncKeyState(VK_UP))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==2)
	{
		if(GetAsyncKeyState(VK_DOWN))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==3)
	{
		if(GetAsyncKeyState(VK_LEFT))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==4)
	{
		if(GetAsyncKeyState(VK_RIGHT))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==5)
	{
		if(GetAsyncKeyState(0x51))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==6)
	{
		if(GetAsyncKeyState(0x57))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==7)
	{
		if(GetAsyncKeyState(0x45))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==8)
	{
		if(GetAsyncKeyState(0x52))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==9)
	{
		if(GetAsyncKeyState(0x54))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==10)
	{
		if(GetAsyncKeyState(0x59))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==11)
	{
		if(GetAsyncKeyState(0x55))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==12)
	{
		if(GetAsyncKeyState(0x49))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==13)
	{
		if(GetAsyncKeyState(0x4F))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==14)
	{
		if(GetAsyncKeyState(0x50))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==15)
	{
		if(GetAsyncKeyState(0x41))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==16)
	{
		if(GetAsyncKeyState(0x53))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==17)
	{
		if(GetAsyncKeyState(0x44))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==18)
	{
		if(GetAsyncKeyState(0x46))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==19)
	{
		if(GetAsyncKeyState(0x47))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==20)
	{
		if(GetAsyncKeyState(0x48))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==21)
	{
		if(GetAsyncKeyState(0x4A))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==22)
	{
		if(GetAsyncKeyState(0x4B))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==23)
	{
		if(GetAsyncKeyState(0x4C))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==24)
	{
		if(GetAsyncKeyState(0x5A))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==25)
	{
		if(GetAsyncKeyState(0x58))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==26)
	{
		if(GetAsyncKeyState(0x43))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==27)
	{
		if(GetAsyncKeyState(0x56))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==28)
	{
		if(GetAsyncKeyState(0x42))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==29)
	{
		if(GetAsyncKeyState(0x4E))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==30)
	{
		if(GetAsyncKeyState(0x4D))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==31)
	{
		if(GetAsyncKeyState(VK_SPACE))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==32)
	{
		if(GetAsyncKeyState(0x30))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==33)
	{
		if(GetAsyncKeyState(0x31))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==34)
	{
		if(GetAsyncKeyState(0x32))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==35)
	{
		if(GetAsyncKeyState(0x33))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==36)
	{
		if(GetAsyncKeyState(0x34))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==37)
	{
		if(GetAsyncKeyState(0x35))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==38)
	{
		if(GetAsyncKeyState(0x36))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==39)
	{
		if(GetAsyncKeyState(0x37))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==40)
	{
		if(GetAsyncKeyState(0x38))
		{
			handleAction(dt);
		}
	}
	else if(updateNum==41)
	{
		if(GetAsyncKeyState(0x39))
		{
			handleAction(dt);
		}
	}
	time=0;
}