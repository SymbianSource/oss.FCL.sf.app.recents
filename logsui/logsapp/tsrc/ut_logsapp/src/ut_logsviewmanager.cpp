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
#include "ut_logsviewmanager.h"
#include "logsviewmanager.h"
#include "logsservicehandler.h"
#include "logsservicehandlerold.h"
#include "logsmainwindow.h"
#include "logscomponentrepository.h"
#include "logsrecentcallsview.h"
#include "logsmatchesview.h"
#include "logsdetailsview.h"
#include "hbstubs_helper.h"

//SYSTEM
#include <HbMainWindow.h>
#include <QtTest/QtTest>
#include <HbView.h>
#include <hbapplication.h>
#include <hbactivitymanager.h>
#include <dialpad.h>

void UT_LogsViewManager::initTestCase()
{
}

void UT_LogsViewManager::cleanupTestCase()
{
}


void UT_LogsViewManager::init()
{
    mMainWindow =  new LogsMainWindow();
    mService = new LogsServiceHandler(*mMainWindow);
    mServiceOld = new LogsServiceHandlerOld(*mMainWindow);
    mLogsViewManager = new LogsViewManager(*mMainWindow, *mService, *mServiceOld);
}

void UT_LogsViewManager::cleanup()
{
    delete mLogsViewManager;
    mLogsViewManager = 0;
    delete mService;
    mService = 0;
    delete mServiceOld;
    mServiceOld = 0;
    delete mMainWindow;
    mMainWindow = 0;
}

void UT_LogsViewManager::testConstructorDestructor()
{
    QVERIFY( mLogsViewManager );
    QVERIFY( mLogsViewManager->mComponentsRepository );
    QVERIFY( mLogsViewManager->mMainWindow.views().count() == 1 );
    QVERIFY( mLogsViewManager->mMainWindow.currentView() != 0 );
    QVERIFY( static_cast<LogsBaseView*>( mLogsViewManager->mMainWindow.currentView() )->viewId() == LogsRecentViewId );
    QVERIFY( mLogsViewManager->mViewStack.count() == 1 );
    
    delete mLogsViewManager;
    mLogsViewManager = 0;
    
    // Contructor when activity restoring started the app
    delete mMainWindow;
    mMainWindow = 0;
    mMainWindow = new LogsMainWindow();
    HbStubHelper::setActivityReason(Hb::ActivationReasonActivity);
    mLogsViewManager = new LogsViewManager(*mMainWindow, *mService, *mServiceOld);
    QVERIFY( mLogsViewManager->mComponentsRepository );
    QVERIFY( static_cast<LogsBaseView*>( mLogsViewManager->mMainWindow.currentView() )->viewId() == LogsRecentViewId );
    QVERIFY( mLogsViewManager->mViewStack.count() == 1 );
    HbStubHelper::reset();
}


void UT_LogsViewManager::testActivateView()
{
    // Activate already active view
    QVERIFY( mLogsViewManager->activateView(LogsRecentViewId) );
    QVERIFY( mLogsViewManager->activateView(LogsRecentViewId) );
    QCOMPARE( mLogsViewManager->mMainWindow.views().count(), 1 );
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->recentCallsView() );
    
    // Activate other view
    QVERIFY( mLogsViewManager->activateView(LogsDetailsViewId) );
    QVERIFY( mLogsViewManager->mMainWindow.views().count() == 2 );
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->detailsView() );
    QVERIFY( mLogsViewManager->mViewStack.at(0) == 
             mLogsViewManager->mComponentsRepository->detailsView() );
    
    // Try to activate unknown view
    QVERIFY( !mLogsViewManager->activateView(LogsUnknownViewId) );
    QVERIFY( mLogsViewManager->mMainWindow.views().count() == 2 );
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->detailsView() );
    QVERIFY( mLogsViewManager->mViewStack.at(0) == 
             mLogsViewManager->mComponentsRepository->detailsView() );
    
    // Go back to previous view
    QVERIFY( mLogsViewManager->activatePreviousView() );
    QVERIFY( mLogsViewManager->mMainWindow.views().count() == 2 );
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->recentCallsView() );
    QVERIFY( mLogsViewManager->mViewStack.at(0) == 
             mLogsViewManager->mComponentsRepository->recentCallsView() );
}

void UT_LogsViewManager::testchangeMatchesView()
{
    //Open Matches view, dialpad visible with predefined number
    mLogsViewManager->changeMatchesViewViaService(QString("+123456"));
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->matchesView() );
             
    // Contact search disabled, go to recent calls view instead
    mLogsViewManager->mComponentsRepository->mModel->mPredectiveSearchStatus = 0;
    QString dialString("+123456777");
    mLogsViewManager->changeMatchesViewViaService(dialString);
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->recentCallsView() );
    QVERIFY( mLogsViewManager->mComponentsRepository->mDialpad->mLineEdit->text() == dialString );
}

void UT_LogsViewManager::testchangeRecentView()
{
    QString dialString("+123456777");
    mLogsViewManager->changeRecentViewViaService(LogsServices::ViewCalled, false, dialString);
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->recentCallsView() );
    QVERIFY( mLogsViewManager->mComponentsRepository->mDialpad->mLineEdit->text() == dialString );
    
    // Empty string clears dialpad input
    mLogsViewManager->changeRecentViewViaService(LogsServices::ViewCalled, false, "");
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->recentCallsView() );
    QVERIFY( mLogsViewManager->mComponentsRepository->mDialpad->mLineEdit->text().isEmpty() );
}

void UT_LogsViewManager::testExitApplication()
{
    // Exit immediately possible, app sent to bg and data is compressed
    HbStubHelper::reset();
    mLogsViewManager->mComponentsRepository->model()->mCompressCalled = false;
    mLogsViewManager->mMainWindow.mForeground = true;
    mLogsViewManager->exitApplication();
    QVERIFY( HbStubHelper::quitCalled() );
    QVERIFY( !mLogsViewManager->mMainWindow.isForeground() );
    
    // Exit not yet possible, app sent only to bg
    mLogsViewManager->mComponentsRepository->model()->mCompressCalled = false;
    mLogsViewManager->mMainWindow.mForeground = true;
    mLogsViewManager->mComponentsRepository->recentCallsView()->mMarkingMissedAsSeen = true;
    HbStubHelper::reset();
    mLogsViewManager->exitApplication();
    QVERIFY( !HbStubHelper::quitCalled() );
    QVERIFY( !mLogsViewManager->mMainWindow.isForeground() );
    
    // Simulate view allowing exit after denying it first
    mLogsViewManager->mComponentsRepository->recentCallsView()->mMarkingMissedAsSeen = false;
    mLogsViewManager->proceedExit();
    QVERIFY( HbStubHelper::quitCalled() );
}

void UT_LogsViewManager::testStartingWithService()
{     
    // If app is started with highway service, view is not activated
    // before service method call comes.
    LogsMainWindow window;
    window.setCurrentView(0); // clear stub static data
    LogsServiceHandler service(*mMainWindow);
    LogsServiceHandlerOld serviceOld(*mMainWindow);
    service.mIsAppStartedUsingService = true;
    LogsViewManager vm(window, service, serviceOld);
    QVERIFY( vm.mComponentsRepository );
    QVERIFY( vm.mMainWindow.views().count() == 0 );
    QVERIFY( vm.mMainWindow.currentView() == 0 );
    QVERIFY( vm.mViewStack.count() == 0 );
}

void UT_LogsViewManager::testHandleOrientationChanged()
{
    mLogsViewManager->handleOrientationChanged();
}

void UT_LogsViewManager::testCompleteViewActivation()
{
    // On first activation, wait that view has painted itself once
    // before completing activation
    mLogsViewManager->mFirstActivation = true;
    mLogsViewManager->activateView(LogsRecentViewId, true, QVariant());
    QVERIFY(!mLogsViewManager->mFirstActivation);
    QVERIFY(mLogsViewManager->mViewActivationShowDialpad);
    QVERIFY(!mLogsViewManager->mComponentsRepository->model()->mMissedCallsCounterCleared);
    mLogsViewManager->completeViewActivation(); // Simulate paint completion (viewReady signal)
    QVERIFY(mLogsViewManager->mComponentsRepository->model()->mMissedCallsCounterCleared);
}

void UT_LogsViewManager::testSaveActivity()
{
    mLogsViewManager->activateView(LogsRecentViewId, false, QVariant());
    HbStubHelper::setActivityReason(Hb::ActivationReasonActivity);
    HbApplication* hbApp = static_cast<HbApplication*>(qApp);
    HbActivityManager* manager = hbApp->activityManager();
    QCOMPARE( manager->activities().count(), 0 );
    mLogsViewManager->saveActivity();
    QCOMPARE( manager->activities().count(), 1 );
    QVERIFY( !manager->activities().at(0).value(logsActivityParamShowDialpad).toBool() );
    QVERIFY( manager->activities().at(0).value(logsActivityParamDialpadText).toString().isEmpty() );
    QCOMPARE( manager->activities().at(0).value(logsActivityParamInternalViewId).toInt(), (int)LogsRecentViewId );
    
    // Make sure that only one activity is reported
    mLogsViewManager->mComponentsRepository->dialpad()->mIsOpen = true;
    mLogsViewManager->mComponentsRepository->dialpad()->mLineEdit->setText("12345");
    mLogsViewManager->saveActivity();
    QCOMPARE( manager->activities().count(), 1 );
    QVERIFY( manager->activities().at(0).value(logsActivityParamShowDialpad).toBool() );
    QCOMPARE( manager->activities().at(0).value(logsActivityParamDialpadText).toString(), QString("12345") );
    QCOMPARE( manager->activities().at(0).value(logsActivityParamInternalViewId).toInt(), (int)LogsMatchesViewId );
    
    // No views, nothing to save
    mLogsViewManager->mViewStack.clear();
    mLogsViewManager->saveActivity();
    QCOMPARE( manager->activities().count(), 1 );
}

void UT_LogsViewManager::testLoadActivity()
{
    mLogsViewManager->mComponentsRepository->mModel->mPredectiveSearchStatus = 1;
    HbStubHelper::setActivityReason(Hb::ActivationReasonActivity);
    HbApplication* hbApp = static_cast<HbApplication*>(qApp);
    HbActivityManager* manager = hbApp->activityManager();
    
    // View activity loaded, no dialpad shown, nor text in it
    HbStubHelper::setActivityId(logsActivityIdViewRecent);
    mLogsViewManager->mComponentsRepository->dialpad()->mIsOpen = false;
    mLogsViewManager->mComponentsRepository->dialpad()->mLineEdit->setText("");
    QVariantHash params;
    params.insert(logsActivityParamShowDialpad, false);
    params.insert(logsActivityParamDialpadText, QString(""));
    params.insert(logsActivityParamInternalViewId, LogsRecentViewId);
    manager->addActivity(QString(), QVariant(), params);
    QVERIFY( mLogsViewManager->loadActivity() );
    QVERIFY( static_cast<LogsBaseView*>( mLogsViewManager->mMainWindow.currentView() )->viewId() == LogsRecentViewId );
    QVERIFY( !mLogsViewManager->mComponentsRepository->dialpad()->mIsOpen );
    QCOMPARE( mLogsViewManager->mComponentsRepository->dialpad()->mLineEdit->text(), QString("") );
  
    // Other activity loaded, show dialpad with text
    HbStubHelper::setActivityId(logsActivityIdViewMatches);
    manager->removeActivity(QString());
    QVariantHash params2;
    params2.insert(logsActivityParamShowDialpad, true);
    params2.insert(logsActivityParamDialpadText, QString("33333"));
    params.insert(logsActivityParamInternalViewId, LogsMatchesViewId);
    manager->addActivity(QString(), QVariant(), params2);
    QVERIFY( mLogsViewManager->loadActivity() );
    QVERIFY( static_cast<LogsBaseView*>( mLogsViewManager->mMainWindow.currentView() )->viewId() == LogsMatchesViewId );
    QVERIFY( mLogsViewManager->mComponentsRepository->dialpad()->mIsOpen );
    QCOMPARE( mLogsViewManager->mComponentsRepository->dialpad()->mLineEdit->text(), QString("33333") );
    
    // View activity not loaded
    HbStubHelper::setActivityId("unknownActivity");
    QVERIFY( !mLogsViewManager->loadActivity() );
}
