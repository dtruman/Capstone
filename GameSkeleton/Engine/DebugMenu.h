#pragma once

#include <QtGui\qwidget.h>
#include <QtGui\QHboxLayout>
#include <QtGui\QVboxLayout>
#include <QtGui\qcheckbox.h>
#include <Qt\qtablewidget.h>
#include <Qt\qlineedit.h>
#include <Qt\qtextedit.h>
#include <Qt\qspinbox.h>
#include <QtGui\qpushbutton.h>
#include <QtGui\qlistview.h>
#include <Qt\qlabel.h>
#include <Qt\qstandarditemmodel.h>
#include <Qt\qobject.h>
#include <Qt\qcombobox.h>
#include <Qt\qdebug.h>
#include <iostream>

#include "ExportHeader.h"
#include "DebugSlider.h"

class ENGINE_SHARED DebugMenu : public QObject
{
#if DEBUG_MENU_ON
	struct widgLayout
	{
		std::string widgetName;
		QWidget* widget;
		QTabWidget* qtw;
		struct tabInfo
		{
			std::string tabName;
			QLayout* layoutUsed;
			QGridLayout* gridLayout;

			struct watchFloatInfo
			{
				const float* data;
				QLabel* labelData;
			} watchFloat[100];

			struct slideFloatInfo
			{
				float* data;
				DebugSlider* sliderData;
			}sliderFloats[100];

			struct checkBoxInfo
			{
				bool* data;
				QCheckBox* checkBoxData;
			} checkBoxes[100];

			struct basicTextInfo
			{
				std::string* data;
				QLineEdit* textEdit;
			} textBoxes[100];
			
			struct spinBoxInfo
			{
				float* data;
				QDoubleSpinBox* spinBox;
			}	spinBoxes[100];

			struct textFieldInfo
			{
				std::string* data;
				QTextEdit* textBox;
			} textField[100];

			struct pushButtonInfo
			{
				QPushButton* button;
			} pushButton[100];

			struct listViewInfo
			{
				QListView* view;
				QStandardItemModel* model;

				struct listItemInfo
				{
					QStandardItem* item;
					std::string* data;
				} listItems[100];

				int numListItems;
			} listViews[10];

			struct comboBoxInfo
			{
				QComboBox* comboBox;
			} comboBoxes[100];

			int numWatchFloats;
			int numSlideFloats;
			int numCheckBoxes;
			int numBTextBoxes;
			int numSpinBoxes;
			int numTextFields;
			int numPushButtons;
			int numListViews;
			int numComboBoxes;
		} tabInfos[10];

		bool notUsed;
		int numTabs;
	} widges[1000];

	int lastUsed;
	int numLayout;
	QHBoxLayout* main;

public:
	static DebugMenu myMenu;

	void initialize(QHBoxLayout* mainLayout);

	void addLayout(std::string layoutName);

	void newRow(const char* tabName, int widgNum=0);

	void slideFloat(float* value, const char* description, const char* tabName, int widgNum=0, float max=1, float min=0);

	void toggleBool(const char* description, const char* tabName, bool* value=false, int widgNum=0);

	void watchFloat(float* value, const char* description, const char* tabName, int widgNum=0);

	void basicTextBox(std::string* value, const char* description, const char* tabName, int widgNum=0);

	void numberSpinBox(float* value, const char* description, const char* tabName, int widgNum=0);
	
	void basicTextField(std::string* value, const char* description, const char* tabName, int widgNum=0);

	void pushButton(QPushButton* value, const char* description, const char* tabName, int widgNum=0);

	void listView(const char* tabName, int widgNum=0);

	void listItem(std::string* value, const char* tabName, int viewNum=0, int widgNum=0);

	void removeListItem(std::string value, const char* tabName, int viewNum=0, int widgNum=0);

	int comboBox(const char* description, const char* tabName, int widgNum=0);

	void changeComboBox(int index, const char* tabName, int comboNum, int widgNum=0);

	void comboBoxItem(const char* name, const char* tabName, int comboNum, int widgNum=0);

	int checkBoxItem(const char* tabName, int comboNum, int widgNum=0);

	bool alreadyInWorld(std::string name);
public:

	void update();

	void changeObjectSelected();

	void manualChangeObjectSelected(int widgNum=0);

	void toggleVisibility(int widgNum=0);

	bool checkVisibility(int widgNum=0);

	int lastLayout();

	void removeLayout(int widgNum=0);

	static DebugMenu& getTheMenu() {return myMenu;}
#else

	QWidget* myWidget;
public:
	QVBoxLayout* myLayout;

	static DebugMenu myMenu;

	void initialize(QWidget* myWidg)
	{myWidget=myWidg;
	myWidget->setLayout(myLayout=new QVBoxLayout);
	myWidget->hide();}

	void slideFloat(float* value, const char* description, float max=1, float min=0)
	{}

	void newRow(const char* tabName)
	{}

	void toggleBool(const char* description, bool* value=false)
	{}

	void watchFloat(float* value, const char* description)
	{}

	int lastLayout()
	{
		return 0;
	}

	void update()
	{}

	void toggleVisibility()
	{myWidget->hide();}

	static DebugMenu& getTheMenu() {return myMenu;}
#endif
};

#define debugMenu DebugMenu::getTheMenu()