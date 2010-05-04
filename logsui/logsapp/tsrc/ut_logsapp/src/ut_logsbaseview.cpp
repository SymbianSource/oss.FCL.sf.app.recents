/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

//USER
#include "logsbaseview.h"
#include "ut_logsbaseview.h"
#include "logscomponentrepository.h"
#include "qthighway_stub_helper.h"
#include "logscall.h"
#include "logsmessage.h"
#include "logscontact.h"
#include "hbstubs_helper.h"
#include "logsrecentcallsview.h"

//SYSTEM
#include <QtTest/QtTest>
#include <hbmainwindow.h>
#include <hbaction.h>
#include <hbinstance.h>
#include <hbmenu.h>
#include <dialpad.h>
#include <hblineedit.h>
#include <hblistviewitem.h>
#include <hblistview.h>
#include <QStringListModel>

void UT_LogsBaseView::initTestCase()
{
    mMainWindow = new HbMainWindow();
    mViewManager = new LogsViewManagerStub(*mMainWindow);
}

void UT_LogsBaseView::cleanupTestCase()
{
    delete mMainWindow;
    delete mViewManager;
}

void UT_LogsBaseView::init()
{
    mRepository = new LogsComponentRepository( *mViewManager );
    mBaseView = new LogsBaseView( LogsRecentViewId, *mRepository, *mViewManager );
}

void UT_LogsBaseView::cleanup()
{
    delete mBaseView;
    mBaseView = 0;
    delete mRepository;
    mRepository = 0;
}

void UT_LogsBaseView::testConstructor()
{
    QVERIFY( mBaseView );
    QVERIFY( mBaseView->mSoftKeyBackAction );
    QVERIFY( !mBaseView->mShowFilterMenu );
    QVERIFY( !mBaseView->mInitialized );
    QVERIFY( mBaseView->mActionMap.isEmpty() );
    QVERIFY( mBaseView->viewId() == LogsRecentViewId );
    QVERIFY( mBaseView->mDialpad );
    QVERIFY( !mBaseView->mCall );
    QVERIFY( !mBaseView->mMessage );
    QVERIFY( !mBaseView->mContact );
    QVERIFY( !mBaseView->mCallTypeMapper );
}

void UT_LogsBaseView::testActivated()
{
    mBaseView->activated(false, QVariant());
    QVERIFY( !mBaseView->mDialpad->isVisible() );
    QVERIFY( mBaseView->mInitialized );
    QVERIFY( !mBaseView->mShowFilterMenu );
    QVERIFY( mBaseView->mActionMap.count() == 4 );

    mBaseView->activated(true, QVariant());
    QVERIFY( mBaseView->mDialpad->isOpen() );
}

void UT_LogsBaseView::testDeactivated()
{
    mBaseView->deactivated(); // NOP
}

void UT_LogsBaseView::testResetView()
{
    // Opened dialpad is closed and text in it is cleared
    mBaseView->mDialpad->mIsOpen = true;
    mBaseView->mDialpad->mLineEdit->setText("testing");
    mBaseView->resetView();
    QVERIFY( !mBaseView->mDialpad->mIsOpen );
    QVERIFY( mBaseView->mDialpad->mLineEdit->text() == "" );
    
    // Text is cleared even if dialpad would be closed
    mBaseView->mDialpad->mLineEdit->setText("testingagain");
    mBaseView->resetView();
    QVERIFY( !mBaseView->mDialpad->mIsOpen );
    QVERIFY( mBaseView->mDialpad->mLineEdit->text() == "" );
}

void UT_LogsBaseView::testShowFilterMenu()
{
    HbStubHelper::reset();
    //no context menu
    QVERIFY( !mBaseView->mShowFilterMenu );
    mBaseView->showFilterMenu();
    QVERIFY( !HbStubHelper::menuShown() );
    
    //contect menu exists
    mBaseView->mShowFilterMenu = new HbMenu();
    QVERIFY( mBaseView->mShowFilterMenu );
    mBaseView->showFilterMenu();
    QVERIFY( HbStubHelper::menuShown() );
    delete mBaseView->mShowFilterMenu;
    mBaseView->mShowFilterMenu = 0;
}

void  UT_LogsBaseView::testOpenDialpad()
{
    QVERIFY( !mBaseView->mDialpad->isOpen() );
    mBaseView->openDialpad();
    QVERIFY( mBaseView->mDialpad->isOpen() );    
}

void  UT_LogsBaseView::testSetDialpadPostion()
{
    QPointF pos = mBaseView->mDialpad->pos();
    HbMainWindow& window = mBaseView->mViewManager.mainWindow();
    
    window.setOrientation( Qt::Horizontal );
    mBaseView->setDialpadPosition();
    QVERIFY( pos != mBaseView->mDialpad->pos() );
    pos = mBaseView->mDialpad->pos();
    
    window.setOrientation( Qt::Vertical );
    mBaseView->setDialpadPosition();
    QVERIFY( pos != mBaseView->mDialpad->pos() );
}

void  UT_LogsBaseView::testDialpadClosed()
{
    mBaseView->mDialpad->editor().setText( QString("hello") );
    mBaseView->dialpadClosed();
    QVERIFY( mBaseView->mDialpad->editor().text().isEmpty() );
}


void  UT_LogsBaseView::testDialpadEditorTextChanged()
{   
    //text editor is one character long
    mBaseView->mDialpad->editor().setText( QString("h") );
    mBaseView->dialpadEditorTextChanged();
    QVERIFY( mViewManager->mViewId == LogsMatchesViewId );
    
    mViewManager->reset();
    mBaseView->mDialpad->mIsCallButtonEnabled = true;
    mBaseView->mDialpad->editor().setText( QString("") );
    mBaseView->dialpadEditorTextChanged();
    QVERIFY( mViewManager->mViewId == LogsUnknownViewId );
    QVERIFY( !mBaseView->mDialpad->mIsCallButtonEnabled );
}

void  UT_LogsBaseView::testChangeFilter()
{
    QVERIFY( mViewManager->mViewId == LogsUnknownViewId );
    HbAction*  action = new HbAction();
    action->setObjectName(logsShowFilterMissedMenuActionId);
    mBaseView->changeFilter(action);
    QVERIFY( mViewManager->mViewId == LogsRecentViewId );
}

void UT_LogsBaseView::testModel()
{
    QVERIFY( !mBaseView->model() );
}

void UT_LogsBaseView::testOpenContactsApp()
{
    QtHighwayStubHelper::reset();
    mBaseView->openContactsApp();
    QVERIFY( QtHighwayStubHelper::service() == "com.nokia.services.phonebookappservices.Launch" );
    QVERIFY( QtHighwayStubHelper::message() == "launch()" );
}


void UT_LogsBaseView::testInitiateCallback()
{
    LogsCall::resetTestData();
    //call can't be created
    QVERIFY( !mBaseView->mCall );
    mBaseView->initiateCallback(QModelIndex());
    QVERIFY( LogsCall::lastCalledFuntion().isEmpty() );
    
    //call can be created should be tested in derived class    
}

void UT_LogsBaseView::testInitiateCall()
{
    LogsCall::resetTestData();
    //no call
    QVERIFY( !mBaseView->mCall );
    mBaseView->initiateCall(LogsCall::TypeLogsVoiceCall);
    QVERIFY( LogsCall::lastCalledFuntion().isEmpty() );
    
    //call exists
    mBaseView->mCall = new LogsCall();
    mBaseView->initiateCall(LogsCall::TypeLogsVideoCall);
    QVERIFY( LogsCall::lastCalledFuntion() == QString("call") );
    QVERIFY( mBaseView->mCall->mTestLastCallType == LogsCall::TypeLogsVideoCall );
}

void UT_LogsBaseView::testShowListItemMenu()
{
    HbStubHelper::reset();

    mBaseView->mDialpad->openDialpad();
    //menu doen't have any actions
    HbListViewItem* item = new HbListViewItem();
    mBaseView->showListItemMenu( item, QPointF() );
    QVERIFY( HbStubHelper::widgetActionsCount() == 0 );
    QVERIFY( !HbStubHelper::menuShown() );
    //check that dialpad has closed
    QVERIFY( !mBaseView->mDialpad->isVisible() );
    
    //menu has actions and can be executed should be tested in derived class
 }

void UT_LogsBaseView::testPopulateListItemMenu()
{
    HbStubHelper::reset();
    //no call/message/contact, menu has not actions
    HbMenu menu;
    mBaseView->populateListItemMenu(menu);
    QVERIFY( HbStubHelper::widgetActionsCount() == 0 );
    
    //call, message and contact exist, menu has actions
    mBaseView->mCall = new LogsCall();
    mBaseView->mMessage = new LogsMessage();
    mBaseView->mContact = new LogsContact();
    mBaseView->populateListItemMenu(menu);
    QVERIFY( HbStubHelper::widgetActionsCount() == 5 );
}



void UT_LogsBaseView::testCloseEmptyMenu()
{
    LogsRecentCallsView* view = mRepository->recentCallsView();
    
    //visible actions exist
    QVERIFY( view->menu()->actions().count() > 0 );
    HbStubHelper::setWidgetOpen(true);
    view->closeEmptyMenu();
    QVERIFY(HbStubHelper::isWidgetOpen());
    
    //no visible actions
    foreach (QAction* action, view->menu()->actions()) {
        action->setVisible(false);
    }
    view->closeEmptyMenu();
    QVERIFY(!HbStubHelper::isWidgetOpen());
    
    //no actions
    HbStubHelper::setWidgetOpen(true);
    view->menu()->actions().clear();
    view->closeEmptyMenu();
    QVERIFY(!HbStubHelper::isWidgetOpen());
}

void UT_LogsBaseView::testSaveContact()
{
    //no contact, no actions
    mBaseView->saveContact();
    
    //contact exists, popup would be shown
    mBaseView->mContact = new LogsContact();
    mBaseView->saveContact();    
}

void UT_LogsBaseView::testUpdateCall()
{
    mBaseView->mCall = new LogsCall();
    mBaseView->updateCall( QModelIndex() );
    QVERIFY( !mBaseView->mCall );
}

void UT_LogsBaseView::testUpdateMessage()
{    
    mBaseView->mMessage = new LogsMessage();
    mBaseView->updateMessage( QModelIndex() );
    QVERIFY( !mBaseView->mMessage );
}

void UT_LogsBaseView::testUpdateContact()
{
    mBaseView->mContact = new LogsContact();
    mBaseView->updateContact( QModelIndex() );
    QVERIFY( !mBaseView->mContact );
}

void UT_LogsBaseView::testActivateEmptyListIndicator()
{
    QStringListModel model;
    
    // Label not in repository
    QVERIFY( !mBaseView->mEmptyListLabel );
    mBaseView->activateEmptyListIndicator(&model);
    QVERIFY( !mBaseView->mEmptyListLabel );
    
    // Empty model, label is set visible
    mRepository->recentCallsView();
    QVERIFY( !mBaseView->mEmptyListLabel );
    mBaseView->activateEmptyListIndicator(&model);
    QVERIFY( mBaseView->mEmptyListLabel );
    
    // Label exists already, situation does not change
    mBaseView->activateEmptyListIndicator(&model);
    QVERIFY( mBaseView->mEmptyListLabel );
    
    // Null list does nothing
    mBaseView->activateEmptyListIndicator(0);
    QVERIFY( mBaseView->mEmptyListLabel );
}

void UT_LogsBaseView::testDeactivateEmptyListIndicator()
{
    QVERIFY( !mBaseView->mEmptyListLabel );
    mBaseView->deactivateEmptyListIndicator(0);
    HbLabel label;
    mBaseView->mEmptyListLabel = &label;
    mBaseView->deactivateEmptyListIndicator(0);
    QVERIFY( mBaseView->mEmptyListLabel ); // Label not removed at deactivation
}

void UT_LogsBaseView::testUpdateEmptyListLabelVisibility()
{
     // Base view does not have model, nothing happens
    mBaseView->updateEmptyListLabelVisibility();
}

void UT_LogsBaseView::testUpdateListLayoutName()
{
    mBaseView->mViewManager.mainWindow().setOrientation( Qt::Vertical );
    HbListView list;

    //default layout
    list.setLayoutName("dummy");
    mBaseView->updateListLayoutName(list);
    QVERIFY( list.layoutName() == logsListDefaultLayout );
    
    //same layout, not updated
    mBaseView->updateListLayoutName(list);
    QVERIFY( list.layoutName() == logsListDefaultLayout );
    
    //landscape without dialpad
    mBaseView->mViewManager.mainWindow().setOrientation( Qt::Horizontal );
    mBaseView->updateListLayoutName(list);
    QVERIFY( list.layoutName() == logsListLandscapeLayout );
    
    //landscape with dialpad
    mBaseView->mDialpad->openDialpad();
    mBaseView->updateListLayoutName(list);
    QVERIFY( list.layoutName() == logsListLandscapeDialpadLayout );
    
    //landscape with dialpad, ignoreDialpad is true
    mBaseView->updateListLayoutName(list, true);
    QVERIFY( list.layoutName() == logsListDefaultLayout );

    //landscape without dialpad, ignoreDialpad is true
    mBaseView->mDialpad->closeDialpad();
    mBaseView->updateListLayoutName(list, true);
    QVERIFY( list.layoutName() == logsListLandscapeLayout );
}

void UT_LogsBaseView::testUpdateListSize()
{
    mBaseView->mLayoutSectionName = "dummy";
    
    //default section is loaded
    mBaseView->mViewManager.mainWindow().setOrientation( Qt::Vertical );
    mBaseView->mDialpad->closeDialpad();
    mBaseView->updateListSize();
    QVERIFY( mBaseView->mLayoutSectionName == logsViewDefaultSection );
    
    //same section again, not loaded
    mBaseView->updateListSize();
    QVERIFY( mBaseView->mLayoutSectionName == logsViewDefaultSection );

    //portrait with dialpad
    mBaseView->mDialpad->openDialpad();
    mBaseView->updateListSize();
    QVERIFY( mBaseView->mLayoutSectionName == logsViewPortraitDialpadSection );
    
    //landscape with dialpad
    mBaseView->mViewManager.mainWindow().setOrientation( Qt::Horizontal );
    mBaseView->updateListSize();
    QVERIFY( mBaseView->mLayoutSectionName == logsViewLandscapeDialpadSection );
}

void UT_LogsBaseView::testHandleOrientationChanged()
{
    //dialpad position recalculated
    QPointF pos;
    mBaseView->mDialpad->setPos(pos);    
    mBaseView->mViewManager.mainWindow().setOrientation( Qt::Horizontal );
    mBaseView->handleOrientationChanged();
    QVERIFY( pos != mBaseView->mDialpad->pos() );
}

void UT_LogsBaseView::testHandleExit()
{
    mViewManager->reset();
    mBaseView->handleExit();
    QVERIFY( mViewManager->mExitCalled );
}

void UT_LogsBaseView::testIsExitAllowed()
{
    QVERIFY( mBaseView->isExitAllowed() );
}
