/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  HbMenu2 implementation.
*
*/


#include <hbmenu.h>
#include <hbmainwindow.h>
#include <hbinstance.h>
#include "hbstubs_helper.h"
#include <hbmessagebox.h>
#include <hbapplication.h>
#include <hbcolorscheme.h>
#include <QCoreApplication>
#include <QTimer>
#include <QGesture>

int actionCount = 0;
Qt::Orientation windowOrientation = Qt::Vertical;
bool testMenuShown = false;
bool testDialogShown = false;
HbMainWindow* testWindow = 0;
HbView* testView = 0;
int testViewCount = 0;
HbAction* testSoftkeyAction = 0;
QString selectedActionString = "none";
bool testSingleShotTimer = false;
bool testQuitCalled = false;
Qt::GestureState testState = Qt::NoGesture;
bool testIsWidgetOpen = false;
bool testIsWidgetRaised = false;
QColor testColor = Qt::white;

void HbStubHelper::reset()
{
    actionCount = 0;
    testMenuShown = false;
    testSingleShotTimer = false;
    testQuitCalled = false;
    testIsWidgetRaised = false;
    testDialogShown = false;
    testColor = Qt::white;
}



int HbStubHelper::widgetActionsCount()
{
    return actionCount;
}

bool HbStubHelper::menuShown()
{
    return testMenuShown;
}

bool HbStubHelper::dialogShown()
{
    return testDialogShown;
}


bool HbStubHelper::singleShotTimerActive()
{
    return testSingleShotTimer;
}

bool HbStubHelper::quitCalled()
{
    return testQuitCalled;
}

void HbStubHelper::setGestureState(int state)
{
    testState = static_cast<Qt::GestureState> (state);
}


Qt::GestureState QGesture::state() const
{
    return testState;
}


bool HbStubHelper::isWidgetOpen()
{
    return testIsWidgetOpen;
}

bool HbStubHelper::isWidgetRaised()
{
    return testIsWidgetRaised;
}

void HbStubHelper::setWidgetOpen(bool isOpen)
{
    testIsWidgetOpen = isOpen;
}

void HbStubHelper::setColorScheme(QColor col)
{
    testColor = col;
}


bool QGraphicsWidget::close()
{
    testIsWidgetOpen = false;
}

void QWidget::setVisible(bool visible)
{
    Q_UNUSED(visible);
}

void QWidget::raise()
{
    testIsWidgetRaised = true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
HbApplication::HbApplication(int &/*argc*/, char */*argv*/[]) 
{
}

HbApplication::~HbApplication()
{
}
    
void HbApplication::quit()
{ 
    testQuitCalled = true; 
}

void QCoreApplication::quit()
{
    testQuitCalled = true; 
}
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void HbMenu::open(QObject *receiver, const char *member)
{
   Q_UNUSED(receiver)
   Q_UNUSED(member)   
   testMenuShown = true;
}

void HbDialog::open(QObject *receiver, const char *member)
{
   Q_UNUSED(receiver)
   Q_UNUSED(member)
   testDialogShown = true;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::addAction(QAction *action)
{
    Q_UNUSED(action)
    actionCount++;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
HbMainWindow::HbMainWindow(QWidget *parent, Hb::WindowFlags windowFlags) : d_ptr(0)
{
    Q_UNUSED(parent)
    Q_UNUSED(windowFlags)
    testViewCount = 0;
    testWindow = this;
}

HbMainWindow::~HbMainWindow()
{
    testWindow = 0;
}
    
void HbMainWindow::setOrientation(Qt::Orientation orientation, bool animate)
{
    Q_UNUSED(animate)
    windowOrientation = orientation; 
}

Qt::Orientation HbMainWindow::orientation() const
{
    return windowOrientation;
}

QRectF HbMainWindow::layoutRect() const
{
    return QRectF(0, 0, 100,100);
}

HbView *HbMainWindow::addView(QGraphicsWidget *widget)
{
    Q_UNUSED(widget)
    testViewCount++;
}

void HbMainWindow::setCurrentView(HbView *view, bool animate, Hb::ViewSwitchFlags flags)
{
    Q_UNUSED(animate)
    Q_UNUSED(flags)
    testView = view;
}

int HbMainWindow::viewCount() const
{
    return testViewCount;
}
HbView *HbMainWindow::currentView() const
{
    return testView;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
HbInstance::HbInstance() : d(0)
{

}

HbInstance::~HbInstance()
{

}

HbInstance* HbInstance::instance()
{
    static HbInstance hbTestInstance;
    return &hbTestInstance;
}
    
QList<HbMainWindow *> HbInstance::allMainWindows() const
{
    QList<HbMainWindow *> list;
    list.append(testWindow);
    return list;
}

void HbMessageBox::setText(const QString &string)
{
    if (string == "Ok") {
    	selectedActionString = "primary";
    } else if (string == "Cancel") {
    	selectedActionString = "secondary";
    }
}


HbAction *HbDialog::exec()
{
    if (selectedActionString == "primary")	{
        return primaryAction();
    } else {
        return 0;
    }
}

void QTimer::singleShot(int msec, QObject *receiver, const char *member)
{
    testSingleShotTimer = true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
bool HbStyle::parameter(const QString &parameter, qreal &value, const HbDeviceProfile &profile) const
{
    Q_UNUSED( profile );
    value = parameter.length();
    return true;
}


QColor HbColorScheme::color( const QString &colorRole )
{
    Q_UNUSED(colorRole);
    return testColor;
}
