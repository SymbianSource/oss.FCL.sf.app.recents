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
#include "logscontact.h"
#include "logsmessage.h"
#include "qthighway_stub_helper.h"
#include "hbapplication.h"
#include "logsappsettings.h"
#include "logsforegroundwatcher.h"
#include "af_stub_helper.h"

//SYSTEM
#include <QtTest/QtTest>
#include <HbView.h>
#include <hbapplication.h>
#include <afactivitystorage.h>
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
    mService = new LogsServiceHandler();
    int argc = 0;
    char* argv = 0;
    mSettings = new LogsAppSettings(argc, &argv);
    mLogsViewManager = new LogsViewManager(*mMainWindow, *mService, *mSettings);
}

void UT_LogsViewManager::cleanup()
{
    delete mLogsViewManager;
    mLogsViewManager = 0;
    delete mService;
    mService = 0;
    delete mMainWindow;
    mMainWindow = 0;
    delete mSettings;
    mSettings = 0;
    
}

void UT_LogsViewManager::testConstructorDestructor()
{
    QVERIFY( mLogsViewManager );
    QVERIFY( mLogsViewManager->mComponentsRepository );
    QVERIFY( mLogsViewManager->mMainWindow.views().count() == 1 );
    QVERIFY( mLogsViewManager->mMainWindow.currentView() != 0 );
    QVERIFY( static_cast<LogsBaseView*>( mLogsViewManager->mMainWindow.currentView() )->viewId() == LogsRecentViewId );
    QVERIFY( mLogsViewManager->mViewStack.count() == 1 );
    QVERIFY( mLogsViewManager->mComponentsRepository->model()->mRefreshCalled );
    
    delete mLogsViewManager;
    mLogsViewManager = 0;
    
    // Contructor when activity restoring started the app
    delete mMainWindow;
    mMainWindow = 0;
    mMainWindow = new LogsMainWindow();
    AfStubHelper::setActivityReason(Af::ActivationReasonActivity);
    mLogsViewManager = new LogsViewManager(*mMainWindow, *mService, *mSettings);
    QVERIFY( mLogsViewManager->mComponentsRepository );
    QVERIFY( static_cast<LogsBaseView*>( mLogsViewManager->mMainWindow.currentView() )->viewId() == LogsRecentViewId );
    QVERIFY( mLogsViewManager->mViewStack.count() == 1 );
    QVERIFY( mLogsViewManager->mComponentsRepository->model()->mRefreshCalled );
    HbStubHelper::reset();
    
    // Constructor when service request started the app
    delete mMainWindow;
    mMainWindow = 0;
    mMainWindow = new LogsMainWindow();
    AfStubHelper::setActivityReason(Af::ActivationReasonNormal);
    QtHighwayStubHelper::setIsService(true);
    LogsServiceHandler* handler2 = new LogsServiceHandler();
    mLogsViewManager->mComponentsRepository->model()->mRefreshCalled = false;
    mLogsViewManager = new LogsViewManager(*mMainWindow, *handler2, *mSettings);
    QVERIFY( mLogsViewManager->mComponentsRepository );
    QVERIFY( static_cast<LogsBaseView*>( mLogsViewManager->mMainWindow.currentView() )->viewId() == LogsRecentViewId );
    QVERIFY( mLogsViewManager->mViewStack.count() == 0 ); // Waiting for signal
    QVERIFY( mLogsViewManager->mComponentsRepository->model()->mRefreshCalled );
    QVERIFY( !mLogsViewManager->mBackgroundStartupWatcher );
    HbStubHelper::reset();
    QtHighwayStubHelper::reset();
    delete handler2;
    
    // Preloading startup
    mSettings->mFeaturePreloadedEnabled = true;
    delete mMainWindow;
    mMainWindow = 0;
    mMainWindow = new LogsMainWindow();
    mLogsViewManager->mComponentsRepository->model()->mRefreshCalled = false;
    mLogsViewManager = new LogsViewManager(*mMainWindow, *mService, *mSettings);
    QVERIFY( mLogsViewManager->mComponentsRepository );
    QVERIFY( static_cast<LogsBaseView*>( mLogsViewManager->mMainWindow.currentView() )->viewId() == LogsRecentViewId );
    QVERIFY( mLogsViewManager->mViewStack.count() == 0 ); // Waiting for coming to foreground
    QVERIFY( !mLogsViewManager->mComponentsRepository->model()->mRefreshCalled );
    QVERIFY( HbStubHelper::isTsTaskVisibilitySet() );
    QVERIFY( !HbStubHelper::tsTaskVisibility() );
    QVERIFY( mLogsViewManager->mBackgroundStartupWatcher );
    HbStubHelper::reset();
    QtHighwayStubHelper::reset();
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
    QVERIFY( mLogsViewManager->activatePreviousView(mLogsViewManager->mComponentsRepository->dialpad()->isOpen(), mLogsViewManager->mComponentsRepository->dialpad()->editor().text()) );
    QVERIFY( mLogsViewManager->mMainWindow.views().count() == 2 );
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->recentCallsView() );
    QVERIFY( mLogsViewManager->mViewStack.at(0) == 
             mLogsViewManager->mComponentsRepository->recentCallsView() );
}

void UT_LogsViewManager::testchangeMatchesView()
{
    QtHighwayStubHelper::reset();
    //Open Matches view, dialpad visible with predefined number
    mLogsViewManager->changeMatchesViewViaService(QString("+123456"));
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->matchesView() );
    QVERIFY( HbStubHelper::isWidgetRaised() );
    
    // Contact search disabled, go to recent calls view instead
    QtHighwayStubHelper::reset();
    mLogsViewManager->mComponentsRepository->mModel->mPredectiveSearchStatus = 0;
    QString dialString("+123456777");
    mLogsViewManager->changeMatchesViewViaService(dialString);
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->recentCallsView() );
    QVERIFY( mLogsViewManager->mComponentsRepository->mDialpad->mLineEdit->text() == dialString );
    QVERIFY( HbStubHelper::isWidgetRaised() );
        
    //Open Matches view, view stack not empty, embedded service canceled
    QtHighwayStubHelper::reset();
    QVERIFY( mLogsViewManager->mViewStack.count() );
    mLogsViewManager->mViewStack.at(0)->mContact = new LogsContact();
    mLogsViewManager->mViewStack.at(0)->mMessage = new LogsMessage();
    mLogsViewManager->changeMatchesViewViaService(QString("+123456"));
    QVERIFY( LogsContact::mServiceRequestCanceled );
    QVERIFY( LogsMessage::mServiceRequestCanceled );
    QVERIFY( HbStubHelper::isWidgetRaised() );
    
    //Open Matches view, view stack is empty, embedded service not canceled
    QtHighwayStubHelper::reset();
    LogsContact::reset();
    mLogsViewManager->mViewStack.clear();
    mLogsViewManager->changeMatchesViewViaService(QString("+123456"));
    QVERIFY( !LogsContact::mServiceRequestCanceled );
    QVERIFY( HbStubHelper::isWidgetRaised() );
}

void UT_LogsViewManager::testchangeRecentView()
{
    QtHighwayStubHelper::reset();
    QString dialString("+123456777");
    mLogsViewManager->changeRecentViewViaService(XQService::LogsViewCalled, false, dialString);
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
            mLogsViewManager->mComponentsRepository->matchesView() );        
    QVERIFY( mLogsViewManager->mComponentsRepository->mDialpad->mLineEdit->text() == dialString );
    QVERIFY( HbStubHelper::isWidgetRaised() );
    
    // Empty string clears dialpad input, check also that dialpad gets closed
    QtHighwayStubHelper::reset();
    mLogsViewManager->mComponentsRepository->mDialpad->mIsOpen = true;
    mLogsViewManager->changeRecentViewViaService(XQService::LogsViewCalled, false, "");
    QVERIFY( mLogsViewManager->mMainWindow.currentView() == 
             mLogsViewManager->mComponentsRepository->recentCallsView() );
    QVERIFY( mLogsViewManager->mComponentsRepository->mDialpad->mLineEdit->text().isEmpty() );
    QVERIFY( HbStubHelper::isWidgetRaised() );
    QVERIFY( !mLogsViewManager->mComponentsRepository->mDialpad->mIsOpen );
    
    //Open recent view, view stack not empty, embedded service canceled    
    QtHighwayStubHelper::reset();
    QVERIFY( mLogsViewManager->mViewStack.count() );
    mLogsViewManager->mViewStack.at(0)->mContact = new LogsContact();
    mLogsViewManager->changeRecentViewViaService(XQService::LogsViewCalled, false, "");
    QVERIFY( LogsContact::mServiceRequestCanceled );
    QVERIFY( HbStubHelper::isWidgetRaised() );
    
    //Open recentt view, view stack is empty, embedded service not canceled
    QtHighwayStubHelper::reset();
    LogsContact::reset();
    mLogsViewManager->mViewStack.clear();
    mLogsViewManager->changeRecentViewViaService(XQService::LogsViewCalled, false, "");
    QVERIFY( !LogsContact::mServiceRequestCanceled );
    QVERIFY( HbStubHelper::isWidgetRaised() );
}

void UT_LogsViewManager::testExitApplication()
{
    // Exit immediately possible, app sent to bg and data is compressed
    HbStubHelper::reset();
    mLogsViewManager->mComponentsRepository->model()->mCompressCalled = false;
    QtHighwayStubHelper::reset();
    mLogsViewManager->exitApplication();
    QVERIFY( HbStubHelper::quitCalled() );
    QVERIFY( QtHighwayStubHelper::utilToBackground() );
    
    // Exit not yet possible, app sent only to bg
    mLogsViewManager->mComponentsRepository->model()->mCompressCalled = false;
    QtHighwayStubHelper::reset();
    mLogsViewManager->mComponentsRepository->recentCallsView()->mMarkingMissedAsSeen = true;
    HbStubHelper::reset();
    mLogsViewManager->exitApplication();
    QVERIFY( !HbStubHelper::quitCalled() );
    QVERIFY( QtHighwayStubHelper::utilToBackground() );
    
    // Simulate view allowing exit after denying it first
    mLogsViewManager->mComponentsRepository->recentCallsView()->mMarkingMissedAsSeen = false;
    mLogsViewManager->proceedExit();
    QVERIFY( HbStubHelper::quitCalled() );
    QVERIFY( !mLogsViewManager->mComponentsRepository->model()->mCompressCalled );
    
    // Fake exit enabled, data is compressed, app hidden and activity saved
    mSettings->mFeatureFakeExitEnabledVal = 1;
    HbStubHelper::reset();
    mLogsViewManager->mComponentsRepository->model()->mCompressCalled = false;
    QtHighwayStubHelper::reset();
    AfStubHelper::reset();
    QCOMPARE( mLogsViewManager->mActivityManager->allActivities().count(), 0 );
    mLogsViewManager->exitApplication();
    QVERIFY( !HbStubHelper::quitCalled() );
    QVERIFY( QtHighwayStubHelper::utilToBackground() );
    QVERIFY( mLogsViewManager->mComponentsRepository->model()->mCompressCalled );
    QVERIFY( HbStubHelper::isTsTaskVisibilitySet() );
    QVERIFY( !HbStubHelper::tsTaskVisibility() );
    QCOMPARE( mLogsViewManager->mActivityManager->allActivities().count(), 1 );
    
}

void UT_LogsViewManager::testStartingWithService()
{     
    // If app is started with highway service, view is not activated
    // before service method call comes.
    LogsMainWindow window;
    window.setCurrentView(0); // clear stub static data
    LogsServiceHandler service;
    service.mIsAppStartedUsingService = true;
    LogsViewManager vm(window, service, *mSettings);
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
    mLogsViewManager->completeViewActivation(); // Simulate paint completion (viewReady signal)
}

void UT_LogsViewManager::testSaveActivity()
{
    mLogsViewManager->activateView(LogsRecentViewId, false, QVariant());
    AfStubHelper::reset();
    AfStubHelper::setActivityReason(Af::ActivationReasonActivity);
    AfActivityStorage* afManager = mLogsViewManager->mActivityManager;
    QCOMPARE( afManager->allActivities().count(), 0 );
    mLogsViewManager->saveActivity();
    QCOMPARE( afManager->allActivities().count(), 1 );
    QVERIFY( !afManager->activityMetaData(QString()).value(logsActivityParamShowDialpad).toBool() );
    QVERIFY( afManager->activityMetaData(QString()).value(logsActivityParamDialpadText).toString().isEmpty() );
    QCOMPARE( afManager->activityMetaData(QString()).value(logsActivityParamInternalViewId).toInt(), (int)LogsRecentViewId );
    
    // Make sure that only one activity is reported
    mLogsViewManager->mComponentsRepository->dialpad()->mIsOpen = true;
    mLogsViewManager->mComponentsRepository->dialpad()->mLineEdit->setText("12345");
    mLogsViewManager->saveActivity();
    QCOMPARE( afManager->allActivities().count(), 1 );
    QVERIFY( afManager->activityMetaData(QString()).value(logsActivityParamShowDialpad).toBool() );
    QCOMPARE( afManager->activityMetaData(QString()).value(logsActivityParamDialpadText).toString(), QString("12345") );
    QCOMPARE( afManager->activityMetaData(QString()).value(logsActivityParamInternalViewId).toInt(), (int)LogsMatchesViewId );
    
    // No views, nothing to save
    mLogsViewManager->mViewStack.clear();
    mLogsViewManager->saveActivity();
    QCOMPARE( afManager->allActivities().count(), 1 );
}

void UT_LogsViewManager::testLoadActivity()
{
    AfStubHelper::reset();
    AfActivityStorage* afManager = mLogsViewManager->mActivityManager;
    mLogsViewManager->mComponentsRepository->mModel->mPredectiveSearchStatus = 1;
    AfStubHelper::setActivityReason(Af::ActivationReasonActivity);
    
    // View activity loaded, no dialpad shown, nor text in it
    AfStubHelper::setActivityId(logsActivityIdViewRecent);
    mLogsViewManager->mComponentsRepository->dialpad()->mIsOpen = false;
    mLogsViewManager->mComponentsRepository->dialpad()->mLineEdit->setText("");
    QVariantHash params;
    params.insert(logsActivityParamShowDialpad, false);
    params.insert(logsActivityParamDialpadText, QString(""));
    params.insert(logsActivityParamInternalViewId, LogsRecentViewId);
    afManager->saveActivity(QString(), QVariant(), params);
    QVERIFY( mLogsViewManager->loadActivity() );
    QVERIFY( static_cast<LogsBaseView*>( mLogsViewManager->mMainWindow.currentView() )->viewId() == LogsRecentViewId );
    QVERIFY( !mLogsViewManager->mComponentsRepository->dialpad()->mIsOpen );
    QCOMPARE( mLogsViewManager->mComponentsRepository->dialpad()->mLineEdit->text(), QString("") );
  
    // Other activity loaded, show dialpad with text
    AfStubHelper::setActivityId(logsActivityIdViewMatches);
    afManager->removeActivity(QString());
    QVariantHash params2;
    params2.insert(logsActivityParamShowDialpad, true);
    params2.insert(logsActivityParamDialpadText, QString("33333"));
    params.insert(logsActivityParamInternalViewId, LogsMatchesViewId);
    afManager->saveActivity(QString(), QVariant(), params2);
    QVERIFY( mLogsViewManager->loadActivity() );
    QVERIFY( static_cast<LogsBaseView*>( mLogsViewManager->mMainWindow.currentView() )->viewId() == LogsMatchesViewId );
    QVERIFY( mLogsViewManager->mComponentsRepository->dialpad()->mIsOpen );
    QCOMPARE( mLogsViewManager->mComponentsRepository->dialpad()->mLineEdit->text(), QString("33333") );
    
    // View activity not loaded
    AfStubHelper::setActivityId("unknownActivity");
    QVERIFY( !mLogsViewManager->loadActivity() );
}

void UT_LogsViewManager::testAppGainedForeground()
{
    // After foreground is gained, proceed exit does not cause exit
    HbStubHelper::reset();
    mLogsViewManager->activateView(LogsRecentViewId, false, QVariant());
    connect( mLogsViewManager->mViewStack.at(0), SIGNAL(exitAllowed()), mLogsViewManager, SLOT(proceedExit()) );
    emit mLogsViewManager->mViewStack.at(0)->exitAllowed();
    QVERIFY( HbStubHelper::quitCalled() );
    
    HbStubHelper::reset();
    mLogsViewManager->mComponentsRepository->model()->mRefreshCalled = false;
    mLogsViewManager->appGainedForeground();
    emit mLogsViewManager->mViewStack.at(0)->exitAllowed();
    QVERIFY( !HbStubHelper::quitCalled() );
    QVERIFY( !mLogsViewManager->mComponentsRepository->model()->mRefreshCalled );
    
    // Gaining foreground causes data refresh if fake exit or preloading feature is enabled
    // If no view activated yet, default view is activated
    HbStubHelper::reset();
    mMainWindow->setCurrentView(0);
    mLogsViewManager->mViewStack.clear();
    mSettings->mFeatureFakeExitEnabledVal = 1;
    mLogsViewManager->appGainedForeground();
    QVERIFY( mLogsViewManager->mComponentsRepository->model()->mRefreshCalled );
    QVERIFY( HbStubHelper::isTsTaskVisibilitySet() );
    QVERIFY( HbStubHelper::tsTaskVisibility() );
    QVERIFY( mMainWindow->currentView() != 0 );
    QCOMPARE( mLogsViewManager->mViewStack.count(), 1 );
    
    HbStubHelper::reset();
    mLogsViewManager->mComponentsRepository->model()->mRefreshCalled = false;
    mSettings->mFeatureFakeExitEnabledVal = 0;
    mSettings->mFeaturePreloadedEnabled = true;
    mLogsViewManager->appGainedForeground();
    QVERIFY( mLogsViewManager->mComponentsRepository->model()->mRefreshCalled );
    QVERIFY( HbStubHelper::isTsTaskVisibilitySet() );
    QVERIFY( HbStubHelper::tsTaskVisibility() );
}

void UT_LogsViewManager::testActivityRequested()
{
    HbStubHelper::reset();
    AfStubHelper::reset();
    AfStubHelper::setActivityId("unknownActivity");
    mLogsViewManager->activityRequested(
            Af::ActivationReasonActivity, "unknownActivity", QVariantHash());
    QVERIFY( !HbStubHelper::isWidgetRaised() );
    
    AfStubHelper::setActivityId(logsActivityIdViewRecent);
    mLogsViewManager->mComponentsRepository->dialpad()->mIsOpen = false;
    mLogsViewManager->mComponentsRepository->dialpad()->mLineEdit->setText("");
    QVariantHash params;
    params.insert(logsActivityParamShowDialpad, false);
    params.insert(logsActivityParamDialpadText, QString(""));
    params.insert(logsActivityParamInternalViewId, LogsRecentViewId);
    mLogsViewManager->mActivityManager->saveActivity(QString(), QVariant(), params);
    mLogsViewManager->activityRequested(
            Af::ActivationReasonActivity, logsActivityIdViewRecent, QVariantHash());
    QVERIFY( static_cast<LogsBaseView*>( mLogsViewManager->mMainWindow.currentView() )->viewId() == LogsRecentViewId );
    QVERIFY( !mLogsViewManager->mComponentsRepository->dialpad()->mIsOpen );
    QCOMPARE( mLogsViewManager->mComponentsRepository->dialpad()->mLineEdit->text(), QString("") );
    QVERIFY( HbStubHelper::isWidgetRaised() );
}

void UT_LogsViewManager::testBgStartupForegroundGained()
{
    if ( !mLogsViewManager->mBackgroundStartupWatcher ){
        mLogsViewManager->mBackgroundStartupWatcher = new LogsForegroundWatcher();
    }
    HbStubHelper::reset();
    mLogsViewManager->mComponentsRepository->model()->mRefreshCalled = false;
    mSettings->mFeaturePreloadedEnabled = true;
    mLogsViewManager->bgStartupForegroundGained();
    QVERIFY( mLogsViewManager->mComponentsRepository->model()->mRefreshCalled );
    QVERIFY( HbStubHelper::isTsTaskVisibilitySet() );
    QVERIFY( HbStubHelper::tsTaskVisibility() );
    QVERIFY( HbStubHelper::isWidgetRaised() );
}
