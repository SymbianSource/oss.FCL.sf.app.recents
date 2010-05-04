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

void UT_LogsViewManager::initTestCase()
{
    //mMainWindow =  new LogsMainWindow();
}

void UT_LogsViewManager::cleanupTestCase()
{
}


void UT_LogsViewManager::init()
{
    mMainWindow =  new LogsMainWindow();
    LogsServiceHandler* service = new LogsServiceHandler(*mMainWindow);
    mLogsViewManager = new LogsViewManager(*mMainWindow, *service);
}

void UT_LogsViewManager::cleanup()
{
    delete mLogsViewManager;
    mLogsViewManager = 0;
    delete mMainWindow;
    mMainWindow = 0;
}

void UT_LogsViewManager::testConstructorDestructor()
{
    QVERIFY( mLogsViewManager );
    QVERIFY( mLogsViewManager->mComponentsRepository );
    QVERIFY( mLogsViewManager->mMainWindow.viewCount() == 3 );
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 0 );
    QVERIFY( mLogsViewManager->mViewStack.count() == 3 );
    
    delete mLogsViewManager;
    mLogsViewManager = 0;
    //TODO: removeView deprecated => this will fail
    //QVERIFY( mMainWindow->viewCount() == 0 );
}

void UT_LogsViewManager::testActivateView()
{
    // Activate already active view
    QVERIFY( mLogsViewManager->activateView(LogsRecentViewId) );
    QVERIFY( mLogsViewManager->activateView(LogsRecentViewId) );
    QVERIFY( mLogsViewManager->mMainWindow.viewCount() == 3 );
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->recentCallsView() );
    
    // Activate other view
    QVERIFY( mLogsViewManager->activateView(LogsDetailsViewId) );
    QVERIFY( mLogsViewManager->mMainWindow.viewCount() == 3 );
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->detailsView() );
    QVERIFY( mLogsViewManager->mViewStack.at(0) == 
             mLogsViewManager->mComponentsRepository->detailsView() );
    
    // Try to activate unknown view
    QVERIFY( !mLogsViewManager->activateView(LogsUnknownViewId) );
    QVERIFY( mLogsViewManager->mMainWindow.viewCount() == 3 );
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->detailsView() );
    QVERIFY( mLogsViewManager->mViewStack.at(0) == 
             mLogsViewManager->mComponentsRepository->detailsView() );
    
    // Go back to previous view
    QVERIFY( mLogsViewManager->activatePreviousView() );
    QVERIFY( mLogsViewManager->mMainWindow.viewCount() == 3 );
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->recentCallsView() );
    QVERIFY( mLogsViewManager->mViewStack.at(0) == 
             mLogsViewManager->mComponentsRepository->recentCallsView() );
}

void UT_LogsViewManager::testchangeMatchesView()
{
    //Open Matches view, dialpad visible with predefined number
    mLogsViewManager->changeMatchesView(QString("+123456"));
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->matchesView() );
}

void UT_LogsViewManager::testExitApplication()
{
    // Exit immediately possible, app sent to bg and data is compressed
    HbStubHelper::reset();
    mLogsViewManager->mComponentsRepository->model()->mCompressCalled = false;
    mLogsViewManager->mMainWindow.mForeground = true;
    mLogsViewManager->exitApplication();
    QVERIFY( mLogsViewManager->mComponentsRepository->model()->mCompressCalled );
    QVERIFY( !mLogsViewManager->mMainWindow.isForeground() );
    
    // Exit not yet possible, app sent only to bg
    mLogsViewManager->mComponentsRepository->model()->mCompressCalled = false;
    mLogsViewManager->mMainWindow.mForeground = true;
    mLogsViewManager->mComponentsRepository->recentCallsView()->mMarkingMissedAsSeen = true;
    HbStubHelper::reset();
    mLogsViewManager->exitApplication();
    QVERIFY( !mLogsViewManager->mComponentsRepository->model()->mCompressCalled );
    QVERIFY( !mLogsViewManager->mMainWindow.isForeground() );
    
    // Simulate view allowing exit after denying it first
    mLogsViewManager->mComponentsRepository->recentCallsView()->mMarkingMissedAsSeen = false;
    mLogsViewManager->proceedExit();
    QVERIFY( mLogsViewManager->mComponentsRepository->model()->mCompressCalled );
}

void UT_LogsViewManager::testAppFocusGained()
{
    mLogsViewManager->appFocusGained();
    // Nothing to verify
}

void UT_LogsViewManager::testStartingWithService()
{     
    // If app is started with highway service, view is not activated
    // before service method call comes.
    LogsMainWindow window;
    window.setCurrentView(0); // clear stub static data
    LogsServiceHandler service(*mMainWindow);
    service.mIsAppStartedUsingService = true;
    LogsViewManager vm(window, service);
    QVERIFY( vm.mComponentsRepository );
    QVERIFY( vm.mMainWindow.viewCount() == 3 );
    QVERIFY( vm.mMainWindow.currentView() == 0 );
    QVERIFY( vm.mViewStack.count() == 3 );
}

void UT_LogsViewManager::testHandleOrientationChanged()
{
    mLogsViewManager->handleOrientationChanged();
}

void UT_LogsViewManager::testCompleteViewActivation()
{
    // On first activation, wait that view has painted itself once
    // before completing activation
    QVERIFY(mLogsViewManager->mFirstActivation);
    mLogsViewManager->activateView(LogsRecentViewId, true, QVariant());
    QVERIFY(!mLogsViewManager->mFirstActivation);
    QVERIFY(mLogsViewManager->mViewActivationShowDialpad);
    QVERIFY(!mLogsViewManager->mComponentsRepository->model()->mMissedCallsCounterCleared);
    mLogsViewManager->completeViewActivation(); // Simulate paint completion (viewReady signal)
    QVERIFY(mLogsViewManager->mComponentsRepository->model()->mMissedCallsCounterCleared);
}
