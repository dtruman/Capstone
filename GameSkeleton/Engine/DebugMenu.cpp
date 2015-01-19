#include "DebugMenu.h"
#include <sstream>

DebugMenu DebugMenu::myMenu;

#if DEBUG_MENU_ON
void DebugMenu::initialize(QHBoxLayout* mainLayout)
{
	main=mainLayout;
	numLayout=0;
}

void DebugMenu::addLayout(std::string layoutName)
{
	bool useThis=false;
	for(int i=0; i<numLayout && !useThis; i++)
	{
		if(widges[i].notUsed)
		{
			useThis=true;
			lastUsed=i;
		}
	}

	if(useThis)
	{
		widges[lastUsed].widgetName=layoutName;
		widges[lastUsed].widget=new QWidget();
		widges[lastUsed].widget->setMaximumWidth(300);
		main->addWidget(widges[lastUsed].widget);
		widges[lastUsed].qtw=new QTabWidget();
		widges[lastUsed].qtw->setMovable(true);
		widges[lastUsed].qtw->setMaximumWidth(300);
		main->addWidget(widges[lastUsed].qtw);
		widges[lastUsed].notUsed=false;
		widges[lastUsed].numTabs=0;
	}
	else
	{
		widges[numLayout].widgetName=layoutName;
		widges[numLayout].widget=new QWidget();
		widges[numLayout].widget->setMaximumWidth(300);
		main->addWidget(widges[numLayout].widget);
		widges[numLayout].qtw=new QTabWidget();
		widges[numLayout].qtw->setMovable(true);
		widges[numLayout].qtw->setMaximumWidth(300);
		main->addWidget(widges[numLayout].qtw);
		widges[numLayout].notUsed=false;
		lastUsed=numLayout;
		widges[numLayout++].numTabs=0;
	}
}

int DebugMenu::lastLayout()
{
	return lastUsed;
}

void DebugMenu::removeLayout(int widgNum)
{
	widges[widgNum].widgetName="";
	widges[widgNum].notUsed=true;

	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		widges[widgNum].tabInfos[i].numBTextBoxes=0;
		widges[widgNum].tabInfos[i].numCheckBoxes=0;
		widges[widgNum].tabInfos[i].numComboBoxes=0;
		widges[widgNum].tabInfos[i].numListViews=0;
		widges[widgNum].tabInfos[i].numPushButtons=0;
		widges[widgNum].tabInfos[i].numSlideFloats=0;
		widges[widgNum].tabInfos[i].numSpinBoxes=0;
		widges[widgNum].tabInfos[i].numTextFields=0;
		widges[widgNum].tabInfos[i].numWatchFloats=0;
	}
	widges[widgNum].numTabs=0;

	delete widges[widgNum].qtw;
	delete widges[widgNum].widget;
}

void DebugMenu::newRow(const char* tabName, int widgNum)
{
	int which=-1;

	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		if(widges[widgNum].tabInfos[i].tabName==tabName)
		{
			which=i;
		}
	}

	if(which>=0)
	{
		QHBoxLayout* tempLayout;
		widges[widgNum].tabInfos[which].layoutUsed=tempLayout=new QHBoxLayout;
		widges[widgNum].tabInfos[which].gridLayout->addLayout(tempLayout, widges[widgNum].tabInfos[which].gridLayout->rowCount()+1, 0, widges[widgNum].tabInfos[which].gridLayout->rowCount()+1, 0);
	}
}

void DebugMenu::slideFloat(float* value, const char* description, const char* tabName, int widgNum, float max, float min)
{
	bool alreadyUsed=false;
	int which=-1;
	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		if(widges[widgNum].tabInfos[i].tabName==tabName)
		{
			alreadyUsed=true;
			which=i;
		}
	}

	if(!alreadyUsed)
	{
		QGridLayout* tempGrid;
		QHBoxLayout* tempLayout;

		widges[widgNum].tabInfos[widges[widgNum].numTabs].tabName=tabName;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].layoutUsed=tempLayout=new QHBoxLayout;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].gridLayout=tempGrid=new QGridLayout;

		tempGrid->addLayout(tempLayout, 1, 0, 1, 0);

		QWidget* tempWidget;
		widges[widgNum].qtw->addTab(tempWidget=new QWidget, tabName);
		tempWidget->setLayout(tempGrid);

		widges[widgNum].tabInfos[widges[widgNum].numTabs].layoutUsed->addWidget(new QLabel(description));
		widgLayout::tabInfo::slideFloatInfo& info=widges[widgNum].tabInfos[widges[widgNum].numTabs].sliderFloats[widges[widgNum].tabInfos[widges[widgNum].numTabs].numSlideFloats++];
		widges[widgNum].tabInfos[widges[widgNum].numTabs++].layoutUsed->addWidget(info.sliderData=new DebugSlider(min,max,true,40.0f));
		info.sliderData->setValue(*value);
		info.data=value;
	}
	else
	{
		widges[widgNum].tabInfos[which].layoutUsed->addWidget(new QLabel(description));
		widgLayout::tabInfo::slideFloatInfo& info=widges[widgNum].tabInfos[which].sliderFloats[widges[widgNum].tabInfos[which].numSlideFloats++];
		widges[widgNum].tabInfos[which].layoutUsed->addWidget(info.sliderData=new DebugSlider(min,max,true,40.0f));
		info.sliderData->setValue(*value);
		info.data=value;
	}
}

void DebugMenu::toggleBool(const char* description, const char* tabName, bool* value, int widgNum)
{
	bool alreadyUsed=false;
	int which=-1;
	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		if(widges[widgNum].tabInfos[i].tabName==tabName)
		{
			alreadyUsed=true;
			which=i;
		}
	}

	if(!alreadyUsed)
	{
		QGridLayout* tempGrid;
		QHBoxLayout* tempLayout;
		
		widges[widgNum].tabInfos[widges[widgNum].numTabs].tabName=tabName;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].layoutUsed=tempLayout=new QHBoxLayout;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].gridLayout=tempGrid=new QGridLayout;
		
		tempGrid->addLayout(tempLayout, 1, 0, 1, 0);
		
		QWidget* tempWidget;
		widges[widgNum].qtw->addTab(tempWidget=new QWidget, tabName);
		tempWidget->setLayout(tempGrid);
		
		widgLayout::tabInfo::checkBoxInfo& info=widges[widgNum].tabInfos[widges[widgNum].numTabs].checkBoxes[widges[widgNum].tabInfos[widges[widgNum].numTabs].numCheckBoxes++];
		widges[widgNum].tabInfos[widges[widgNum].numTabs++].layoutUsed->addWidget(info.checkBoxData=new QCheckBox(description));
		info.checkBoxData->setChecked(*value);
		info.data=value;
	}
	else
	{
		widgLayout::tabInfo::checkBoxInfo& info=widges[widgNum].tabInfos[which].checkBoxes[widges[widgNum].tabInfos[which].numCheckBoxes++];
		widges[widgNum].tabInfos[which].layoutUsed->addWidget(info.checkBoxData=new QCheckBox(description));
		info.checkBoxData->setChecked(*value);
		info.data=value;
	}
}

void DebugMenu::basicTextBox(std::string* value, const char* description, const char* tabName, int widgNum)
{
	bool alreadyUsed=false;
	int which=-1;
	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		if(widges[widgNum].tabInfos[i].tabName==tabName)
		{
			alreadyUsed=true;
			which=i;
		}
	}

	if(!alreadyUsed)
	{
		QGridLayout* tempGrid;
		QHBoxLayout* tempLayout;

		widges[widgNum].tabInfos[widges[widgNum].numTabs].tabName=tabName;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].layoutUsed=tempLayout=new QHBoxLayout;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].gridLayout=tempGrid=new QGridLayout;

		tempGrid->addLayout(tempLayout, 1, 0, 1, 0);

		QWidget* tempWidget;
		widges[widgNum].qtw->addTab(tempWidget=new QWidget, tabName);
		tempWidget->setLayout(tempGrid);

		widges[widgNum].tabInfos[widges[widgNum].numTabs].layoutUsed->addWidget(new QLabel(description));
		widgLayout::tabInfo::basicTextInfo& info=widges[widgNum].tabInfos[widges[widgNum].numTabs].textBoxes[widges[widgNum].tabInfos[widges[widgNum].numTabs].numBTextBoxes++];
		widges[widgNum].tabInfos[widges[widgNum].numTabs++].layoutUsed->addWidget(info.textEdit=new QLineEdit(description));
		info.data=value;
		info.textEdit->setText(QString::fromStdString(*value));
	}
	else
	{
		widges[widgNum].tabInfos[widges[widgNum].numTabs].layoutUsed->addWidget(new QLabel(description));
		widgLayout::tabInfo::basicTextInfo& info=widges[widgNum].tabInfos[which].textBoxes[widges[widgNum].tabInfos[which].numBTextBoxes++];
		widges[widgNum].tabInfos[which].layoutUsed->addWidget(info.textEdit=new QLineEdit(description));
		info.data=value;
		info.textEdit->setText(QString::fromStdString(*value));
	}
}

void DebugMenu::watchFloat(float* value, const char* description, const char* tabName, int widgNum)
{
	bool alreadyUsed=false;
	int which=-1;
	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		if(widges[widgNum].tabInfos[i].tabName==tabName)
		{
			alreadyUsed=true;
			which=i;
		}
	}

	if(!alreadyUsed)
	{
		QGridLayout* tempGrid;
		QHBoxLayout* tempLayout;

		widges[widgNum].tabInfos[widges[widgNum].numTabs].tabName=tabName;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].layoutUsed=tempLayout=new QHBoxLayout;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].gridLayout=tempGrid=new QGridLayout;

		tempGrid->addLayout(tempLayout, 1, 0, 1, 0);

		QWidget* tempWidget;
		widges[widgNum].qtw->addTab(tempWidget=new QWidget, tabName);
		tempWidget->setLayout(tempGrid);

		widges[widgNum].tabInfos[widges[widgNum].numTabs].layoutUsed->addWidget(new QLabel(description));
		widgLayout::tabInfo::watchFloatInfo& info=widges[widgNum].tabInfos[widges[widgNum].numTabs].watchFloat[widges[widgNum].tabInfos[widges[widgNum].numTabs].numWatchFloats++];
		widges[widgNum].tabInfos[widges[widgNum].numTabs++].layoutUsed->addWidget(info.labelData=new QLabel);
		QString temp;
		temp.setNum(*(value));
		info.labelData->setText(temp);
		info.data=value;
	}
	else
	{
		widges[widgNum].tabInfos[which].layoutUsed->addWidget(new QLabel(description));
		widgLayout::tabInfo::watchFloatInfo& info=widges[widgNum].tabInfos[which].watchFloat[widges[widgNum].tabInfos[which].numWatchFloats++];
		widges[widgNum].tabInfos[which].layoutUsed->addWidget(info.labelData=new QLabel);
		QString temp;
		temp.setNum(*(value));
		info.labelData->setText(temp);
		info.data=value;
	}
}

void DebugMenu::numberSpinBox(float* value, const char* description, const char* tabName, int widgNum)
{
	bool alreadyUsed=false;
	int which=-1;
	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		if(widges[widgNum].tabInfos[i].tabName==tabName)
		{
			alreadyUsed=true;
			which=i;
		}
	}

	if(!alreadyUsed)
	{
		QGridLayout* tempGrid;
		QHBoxLayout* tempLayout;

		widges[widgNum].tabInfos[widges[widgNum].numTabs].tabName=tabName;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].layoutUsed=tempLayout=new QHBoxLayout;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].gridLayout=tempGrid=new QGridLayout;

		tempGrid->addLayout(tempLayout, 1, 0, 1, 0);

		QWidget* tempWidget;
		widges[widgNum].qtw->addTab(tempWidget=new QWidget, tabName);
		tempWidget->setLayout(tempGrid);

		widges[widgNum].tabInfos[widges[widgNum].numTabs].layoutUsed->addWidget(new QLabel(description));
		widgLayout::tabInfo::spinBoxInfo& info=widges[widgNum].tabInfos[widges[widgNum].numTabs].spinBoxes[widges[widgNum].tabInfos[widges[widgNum].numTabs].numSpinBoxes++];
		widges[widgNum].tabInfos[widges[widgNum].numTabs++].layoutUsed->addWidget(info.spinBox=new QDoubleSpinBox());
		info.spinBox->setMinimum(-2000);
		info.spinBox->setMaximum(2000);
		info.spinBox->setSingleStep(0.1);
		info.spinBox->setValue((double)*value);
		info.data=value;
	}
	else
	{
		widges[widgNum].tabInfos[which].layoutUsed->addWidget(new QLabel(description));
		widgLayout::tabInfo::spinBoxInfo& info=widges[widgNum].tabInfos[which].spinBoxes[widges[widgNum].tabInfos[which].numSpinBoxes++];
		widges[widgNum].tabInfos[which].layoutUsed->addWidget(info.spinBox=new QDoubleSpinBox());
		info.spinBox->setMinimum(-2000);
		info.spinBox->setMaximum(2000);
		info.spinBox->setSingleStep(0.1);
		info.spinBox->setValue((double)*value);
		info.data=value;
	}
}

void DebugMenu::basicTextField(std::string* value, const char* description, const char* tabName, int widgNum)
{
	bool alreadyUsed=false;
	int which=-1;
	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		if(widges[widgNum].tabInfos[i].tabName==tabName)
		{
			alreadyUsed=true;
			which=i;
		}
	}

	if(!alreadyUsed)
	{
		QGridLayout* tempGrid;
		QHBoxLayout* tempLayout;

		widges[widgNum].tabInfos[widges[widgNum].numTabs].tabName=tabName;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].layoutUsed=tempLayout=new QHBoxLayout;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].gridLayout=tempGrid=new QGridLayout;

		tempGrid->addLayout(tempLayout, 1, 0, 1, 0);

		QWidget* tempWidget;
		widges[widgNum].qtw->addTab(tempWidget=new QWidget, tabName);
		tempWidget->setLayout(tempGrid);

		widges[widgNum].tabInfos[widges[widgNum].numTabs].layoutUsed->addWidget(new QLabel(description));
		widgLayout::tabInfo::textFieldInfo& info=widges[widgNum].tabInfos[widges[widgNum].numTabs].textField[widges[widgNum].tabInfos[widges[widgNum].numTabs].numTextFields++];
		widges[widgNum].tabInfos[widges[widgNum].numTabs++].layoutUsed->addWidget(info.textBox=new QTextEdit(QString::fromStdString(*value)));
		info.data=value;
	}
	else
	{
		widges[widgNum].tabInfos[which].layoutUsed->addWidget(new QLabel(description));
		widgLayout::tabInfo::textFieldInfo& info=widges[widgNum].tabInfos[which].textField[widges[widgNum].tabInfos[which].numTextFields++];
		widges[widgNum].tabInfos[which].layoutUsed->addWidget(info.textBox=new QTextEdit(QString::fromStdString(*value)));
		info.data=value;
	}
}

void DebugMenu::pushButton(QPushButton* value, const char* description, const char* tabName, int widgNum)
{
	bool alreadyUsed=false;
	int which=-1;
	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		if(widges[widgNum].tabInfos[i].tabName==tabName)
		{
			alreadyUsed=true;
			which=i;
		}
	}

	if(!alreadyUsed)
	{
		QGridLayout* tempGrid;
		QHBoxLayout* tempLayout;

		widges[widgNum].tabInfos[widges[widgNum].numTabs].tabName=tabName;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].layoutUsed=tempLayout=new QHBoxLayout;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].gridLayout=tempGrid=new QGridLayout;

		tempGrid->addLayout(tempLayout, 1, 0, 1, 0);

		QWidget* tempWidget;
		widges[widgNum].qtw->addTab(tempWidget=new QWidget, tabName);
		tempWidget->setLayout(tempGrid);

		widgLayout::tabInfo::pushButtonInfo& info=widges[widgNum].tabInfos[widges[widgNum].numTabs].pushButton[widges[widgNum].tabInfos[widges[widgNum].numTabs].numPushButtons++];
		widges[widgNum].tabInfos[widges[widgNum].numTabs++].layoutUsed->addWidget(value);
	}
	else
	{
		widgLayout::tabInfo::pushButtonInfo& info=widges[widgNum].tabInfos[which].pushButton[widges[widgNum].tabInfos[which].numPushButtons++];
		widges[widgNum].tabInfos[which].layoutUsed->addWidget(value);
	}
}

void DebugMenu::update()
{
	for(int j=0; j<numLayout; j++)
	{
		for(int n=0; n<widges[j].numTabs; n++)
		{
			for(uint i=0; i<widges[j].tabInfos[n].numSlideFloats; i++)
			{
				const widgLayout::tabInfo::slideFloatInfo& f=widges[j].tabInfos[n].sliderFloats[i];
				*f.data=f.sliderData->value();
			}

			for(int i=0; i<widges[j].tabInfos[n].numWatchFloats; i++)
			{
				const widgLayout::tabInfo::watchFloatInfo& f=widges[j].tabInfos[n].watchFloat[i];
				QString temp;
				temp.setNum(*(f.data));
	
				f.labelData->setText(temp);	
			}
	
			for(int i=0; i<widges[j].tabInfos[n].numCheckBoxes; i++)
			{
				const widgLayout::tabInfo::checkBoxInfo& f=widges[j].tabInfos[n].checkBoxes[i];
				*f.data=f.checkBoxData->isChecked();
			}
	
			for(int i=0; i<widges[j].tabInfos[n].numBTextBoxes; i++)
			{
				const widgLayout::tabInfo::basicTextInfo& f=widges[j].tabInfos[n].textBoxes[i];

				if(f.textEdit->isModified() && f.textEdit->text().size()>0 && !alreadyInWorld(f.textEdit->text().toUtf8().constData()))
				{
					QString temp=f.textEdit->text();
					
					std::string tempString(temp.toUtf8());
										
					*f.data=tempString;
				}
				else
				{
					f.textEdit->setText(QString::fromStdString(*f.data));
				}
			}

			for(int i=0; i<widges[j].tabInfos[n].numSpinBoxes; i++)
			{
				const widgLayout::tabInfo::spinBoxInfo& f=widges[j].tabInfos[n].spinBoxes[i];
				*f.data=(float)f.spinBox->value();
			}

			for(int i=0; i<widges[j].tabInfos[n].numTextFields; i++)
			{
				const widgLayout::tabInfo::textFieldInfo& f=widges[j].tabInfos[n].textField[i];
				
				if(f.textBox->isActiveWindow())
				{
					QString temp=f.textBox->toPlainText();
					
					std::string tempString(temp.toUtf8());
										
					*f.data=tempString;
				}
			}

			for(int i=0; i<widges[j].tabInfos[n].numListViews; i++)
			{
				for(int x=0; x<widges[j].tabInfos[n].listViews[i].numListItems; x++)
				{
					const widgLayout::tabInfo::listViewInfo::listItemInfo& f=widges[j].tabInfos[n].listViews[i].listItems[x];

					if(f.data->size()>0 && !alreadyInWorld(*f.data))
					{
						f.item->setText(QString::fromStdString(*f.data));
					}
					else
					{
						std::string newString=f.item->text().toUtf8().constData();
						*f.data=newString;
					}
				}
			}
		}
	}
	changeObjectSelected();
}

bool DebugMenu::alreadyInWorld(std::string name)
{
	for(int i=0; i<widges[0].tabInfos[0].listViews[0].numListItems; i++)
	{
		if(*widges[0].tabInfos[0].listViews[0].listItems[i].data==name)
		{
			return true;
		}
	}

	return false;
}

void DebugMenu::changeObjectSelected()
{
	int which=0;
	for(int i=0; i<widges[0].tabInfos[0].numListViews && which==0; i++)
	{
		std::string item=widges[0].tabInfos[0].listViews[i].view->selectionModel()->currentIndex().data().toString().toUtf8();
	
		for(int x=0; x<numLayout && which==0; x++)
		{
			if(!widges[x].notUsed)
			{
				if(item.size()>0 && widges[x].tabInfos[0].textBoxes[0].data && *widges[x].tabInfos[0].textBoxes[0].data==item)
				{
					which=x;
				}
			}
		}
	}

	for(int i=1; i<numLayout && which!=0; i++)
	{
		if(i!=which && !widges[i].notUsed)
		{
			if(!widges[i].widget->isHidden())
			{

				toggleVisibility(i);
			}
		}
	}

	if(widges[which].widget->isHidden())
	{
		toggleVisibility(which);
	}
}

void DebugMenu::manualChangeObjectSelected(int widgNum)
{
	for(int i=1; i<numLayout && widgNum!=0 && !widges[i].notUsed; i++)
	{
		if(i!=widgNum)
		{
			if(!widges[i].widget->isHidden())
			{
				toggleVisibility(i);
			}
		}
	}

	if(widges[widgNum].widget->isHidden())
	{
		toggleVisibility(widgNum);
	}
}

void DebugMenu::listView(const char* tabName, int widgNum)
{
	bool alreadyUsed=false;
	int which=-1;
	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		if(widges[widgNum].tabInfos[i].tabName==tabName)
		{
			alreadyUsed=true;
			which=i;
		}
	}

	if(!alreadyUsed)
	{
		QGridLayout* tempGrid;
		QHBoxLayout* tempLayout;

		widges[widgNum].tabInfos[widges[widgNum].numTabs].tabName=tabName;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].layoutUsed=tempLayout=new QHBoxLayout;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].gridLayout=tempGrid=new QGridLayout;

		tempGrid->addLayout(tempLayout, 1, 0, 1, 0);

		QWidget* tempWidget;
		widges[widgNum].qtw->addTab(tempWidget=new QWidget, tabName);
		tempWidget->setLayout(tempGrid);

		widgLayout::tabInfo::listViewInfo& info=widges[widgNum].tabInfos[widges[widgNum].numTabs].listViews[widges[widgNum].tabInfos[widges[widgNum].numTabs].numListViews++];
		widges[widgNum].tabInfos[widges[widgNum].numTabs++].layoutUsed->addWidget(info.view=new QListView());
		info.view->alternatingRowColors();
		info.model=new QStandardItemModel();
		info.view->setModel(info.model);
	}
	else
	{
		widgLayout::tabInfo::listViewInfo& info=widges[widgNum].tabInfos[which].listViews[widges[widgNum].tabInfos[which].numListViews++];
		widges[widgNum].tabInfos[which].layoutUsed->addWidget(info.view=new QListView());
		info.view->alternatingRowColors();
		info.model=new QStandardItemModel();
		info.view->setModel(info.model);
	}
}

void DebugMenu::listItem(std::string* value, const char* tabName, int viewNum, int widgNum)
{
	bool alreadyUsed=false;
	int which=-1;
	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		if(widges[widgNum].tabInfos[i].tabName==tabName)
		{
			alreadyUsed=true;
			which=i;
		}
	}

	if(!alreadyUsed)
		std::cout << "Can't add an item to a list that doesn't exist" << std::endl;

		widgLayout::tabInfo::listViewInfo& info=widges[widgNum].tabInfos[which].listViews[viewNum];
		widgLayout::tabInfo::listViewInfo::listItemInfo& f=widges[widgNum].tabInfos[which].listViews[viewNum].listItems[widges[widgNum].tabInfos[which].listViews[viewNum].numListItems++];
		info.model->appendRow(f.item=new QStandardItem(QString::fromStdString(*value)));
		f.data=value;
		info.view->setModel(info.model);
}

void DebugMenu::removeListItem(std::string value, const char* tabName, int viewNum, int widgNum)
{
	bool alreadyUsed=false;
	int which=-1;
	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		if(widges[widgNum].tabInfos[i].tabName==tabName)
		{
			alreadyUsed=true;
			which=i;
		}
	}

	int whichListItems=-1;
	for(int i=0; i<widges[widgNum].tabInfos[which].listViews[viewNum].numListItems; i++)
	{
		if(widges[widgNum].tabInfos[which].listViews[viewNum].model->item(i)->text()==QString::fromStdString(value))
			whichListItems=i;
	}

	int hideWidg=-1;
	for(int i=1; i<numLayout; i++)
	{
		if(!widges[i].notUsed && widges[i].tabInfos[0].textBoxes[0].data->size()>0 && *widges[i].tabInfos[0].textBoxes[0].data==value)
		{
			widges[i].notUsed=true;
			hideWidg=i;
		}
	}

	widgLayout::tabInfo::listViewInfo& info=widges[widgNum].tabInfos[which].listViews[viewNum];
	widgLayout::tabInfo::listViewInfo::listItemInfo& f=widges[widgNum].tabInfos[which].listViews[viewNum].listItems[whichListItems];
	info.model->removeRow(whichListItems);

	for(int i=whichListItems; i<widges[widgNum].tabInfos[which].listViews[viewNum].numListItems-1; i++)
	{
		widgLayout::tabInfo::listViewInfo::listItemInfo temp=widges[widgNum].tabInfos[which].listViews[viewNum].listItems[i+1];
		widges[widgNum].tabInfos[which].listViews[viewNum].listItems[i+1]=widges[widgNum].tabInfos[which].listViews[viewNum].listItems[i];
		widges[widgNum].tabInfos[which].listViews[viewNum].listItems[i]=temp;
	}

	widges[widgNum].tabInfos[which].listViews[viewNum].numListItems--;

	if(!widges[hideWidg].widget->isHidden())
		toggleVisibility(hideWidg);
}

int DebugMenu::comboBox(const char* description, const char* tabName, int widgNum)
{
	bool alreadyUsed=false;
	int which=-1;
	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		if(widges[widgNum].tabInfos[i].tabName==tabName)
		{
			alreadyUsed=true;
			which=i;
		}
	}

	if(!alreadyUsed)
	{
		QGridLayout* tempGrid;
		QHBoxLayout* tempLayout;

		widges[widgNum].tabInfos[widges[widgNum].numTabs].tabName=tabName;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].layoutUsed=tempLayout=new QHBoxLayout;
		widges[widgNum].tabInfos[widges[widgNum].numTabs].gridLayout=tempGrid=new QGridLayout;

		tempGrid->addLayout(tempLayout, 1, 0, 1, 0);

		QWidget* tempWidget;
		widges[widgNum].qtw->addTab(tempWidget=new QWidget, tabName);
		tempWidget->setLayout(tempGrid);

		widgLayout::tabInfo::comboBoxInfo& info=widges[widgNum].tabInfos[widges[widgNum].numTabs].comboBoxes[widges[widgNum].tabInfos[widges[widgNum].numTabs].numComboBoxes];
		which=widges[widgNum].numTabs;
		widges[widgNum].tabInfos[widges[widgNum].numTabs++].layoutUsed->addWidget(info.comboBox=new QComboBox());
	}
	else
	{
		widgLayout::tabInfo::comboBoxInfo& info=widges[widgNum].tabInfos[which].comboBoxes[widges[widgNum].tabInfos[which].numComboBoxes];
		widges[widgNum].tabInfos[which].layoutUsed->addWidget(info.comboBox=new QComboBox());
	}

	return widges[widgNum].tabInfos[which].numComboBoxes++;
}

void DebugMenu::comboBoxItem(const char* name, const char* tabName, int comboNum, int widgNum)
{
	bool alreadyUsed=false;
	int which=-1;
	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		if(widges[widgNum].tabInfos[i].tabName==tabName)
		{
			alreadyUsed=true;
			which=i;
		}
	}

	widgLayout::tabInfo::comboBoxInfo& info=widges[widgNum].tabInfos[which].comboBoxes[comboNum];
	info.comboBox->addItem(name);
}

void DebugMenu::changeComboBox(int index, const char* tabName, int comboNum, int widgNum)
{
	bool alreadyUsed=false;
	int which=-1;
	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		if(widges[widgNum].tabInfos[i].tabName==tabName)
		{
			alreadyUsed=true;
			which=i;
		}
	}
	widgLayout::tabInfo::comboBoxInfo& info=widges[widgNum].tabInfos[which].comboBoxes[comboNum];

	info.comboBox->setCurrentIndex(index);
}

int DebugMenu::checkBoxItem(const char* tabName, int comboNum, int widgNum)
{
	bool alreadyUsed=false;
	int which=-1;
	for(int i=0; i<widges[widgNum].numTabs; i++)
	{
		if(widges[widgNum].tabInfos[i].tabName==tabName)
		{
			alreadyUsed=true;
			which=i;
		}
	}
	widgLayout::tabInfo::comboBoxInfo& info=widges[widgNum].tabInfos[which].comboBoxes[comboNum];

	return info.comboBox->currentIndex();
}

void DebugMenu::toggleVisibility(int widgNum)
{
	if(widges[widgNum].widget->isHidden())
	{
		widges[widgNum].widget->show();
		widges[widgNum].qtw->show();
	}
	else
	{
		widges[widgNum].widget->hide();
		widges[widgNum].qtw->hide();
	}

	std::cout << "I am being called: " << std::endl;
}

bool DebugMenu::checkVisibility(int widgNum)
{
	return widges[widgNum].widget->isHidden();
}
#endif