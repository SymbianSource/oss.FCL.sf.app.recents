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
#include "logsviewmanager.h"
#include "logscomponentrepository.h"
#include "logsrecentcallsview.h"
#include "logsmatchesview.h"
#include "logsdetailsview.h"
#include "logsfilter.h"
#include "logsdefs.h"
#include "logslogger.h"
#include "logsservicehandler.h"
#include "logsservicehandlerold.h"
#include "logsmainwindow.h"

//SYSTEM
#include <hbmainwindow.h>
#include <hbview.h>
#include <logsservices.h>
#include <QApplication>
#include <hblineedit.h>
#include <dialpad.h>
#include <hbactivitymanager.h>
#include <hbapplication.h>

// -----------------------------------------------------------------------------
// LogsViewManager::LogsViewManager
// -----------------------------------------------------------------------------
//
LogsViewManager::LogsViewManager( 
        LogsMainWindow& mainWindow, LogsServiceHandler& service,
        LogsServiceHandlerOld& serviceOld ) : 
    QObject( 0 ), mMainWindow( mainWindow ), 
    mService( service ), mServiceOld( serviceOld ),
    mFirstActivation(true), mViewActivationShowDialpad(false)
{
    LOGS_QDEBUG( "logs [UI] -> LogsViewManager::LogsViewManager()" );

    //It is important that we always handle orientation change first, before
    //dialpad widget. If connection is moved to a view, then it's not guarantied.
    connect( &mainWindow, SIGNAL(orientationChanged(Qt::Orientation)),
            this, SLOT(handleOrientationChanged()) );

    mComponentsRepository = new LogsComponentRepository(*this);
    initViews();
    
    connect( &mService, SIGNAL( activateView(LogsServices::LogsView, bool) ), 
             this, SLOT( changeRecentView(LogsServices::LogsView, bool) ) );

    connect( &mService, SIGNAL( activateView(QString) ), 
             this, SLOT( changeMatchesView(QString) ));

    connect( &mServiceOld, SIGNAL( activateView(LogsServices::LogsView, bool) ), 
             this, SLOT( changeRecentView(LogsServices::LogsView, bool) ) );

    connect( &mServiceOld, SIGNAL( activateView(QString) ), 
             this, SLOT( changeMatchesView(QString) ));
    
    QObject::connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(saveActivity()));

    handleFirstActivation();
    
    LOGS_QDEBUG( "logs [UI] <- LogsViewManager::LogsViewManager()" );
}


// -----------------------------------------------------------------------------
// LogsViewManager::~LogsViewManager
// -----------------------------------------------------------------------------
//
LogsViewManager::~LogsViewManager()
{
    LOGS_QDEBUG( "logs [UI] -> LogsViewManager::~LogsViewManager()" );
     
    delete mComponentsRepository;
    
    LOGS_QDEBUG( "logs [UI] <- LogsViewManager::~LogsViewManager()" );
}

// -----------------------------------------------------------------------------
// LogsViewManager::initViews
// -----------------------------------------------------------------------------
//
void LogsViewManager::initViews()
{
    LOGS_QDEBUG( "logs [UI] -> LogsViewManager::initViews()" );
    
    // Don't activate any view, app focus gaining or service request
    // will cause view activation
    //
    LogsRecentCallsView* view = mComponentsRepository->recentCallsView();    
    Q_ASSERT_X(view != 0, "logs [UI] ", "recentCallsView not found!");
    LOGS_QDEBUG( "logs [UI] adding view" );
    mViewStack.append(view);
    mMainWindow.addView(view);
        
    LogsDetailsView* detailsView = mComponentsRepository->detailsView();
    mViewStack.append(detailsView);
    mMainWindow.addView(detailsView);
    
    LogsMatchesView* matchesView = mComponentsRepository->matchesView();
    mViewStack.append(matchesView);
    mMainWindow.addView(matchesView);

    LOGS_QDEBUG( "logs [UI] <- LogsViewManager::initViews()" );
}

// -----------------------------------------------------------------------------
// LogsViewManager::changeRecentView
// -----------------------------------------------------------------------------
//
void LogsViewManager::changeRecentView(LogsServices::LogsView view, bool showDialpad)
{
    QVariant args(view);
    doActivateView(LogsRecentViewId, showDialpad, args);
}

// -----------------------------------------------------------------------------
// LogsViewManager::changeMatchesView
// -----------------------------------------------------------------------------
//
void LogsViewManager::changeMatchesView(QString dialpadText)
{
    LOGS_QDEBUG( "logs [UI] -> LogsViewManager::changeMatchesView()" );
    doActivateView(LogsMatchesViewId, true, QVariant(), dialpadText);
    LOGS_QDEBUG( "logs [UI] <- LogsViewManager::changeMatchesView()" );
}

// -----------------------------------------------------------------------------
// LogsViewManager::proceedExit
// -----------------------------------------------------------------------------
//
void LogsViewManager::proceedExit()
{
    LOGS_QDEBUG( "logs [UI] -> LogsViewManager::proceedExit()" );
    
    // Instead of compressing data, exit the application for real because
    // of bug in wk16 platform which prevents bg/fg switching of the app.
    //
    
    //mComponentsRepository->model()->compressData();
    qApp->quit();
    
    LOGS_QDEBUG( "logs [UI] <- LogsViewManager::proceedExit()" );
}


// -----------------------------------------------------------------------------
// LogsViewManager::activateView
// -----------------------------------------------------------------------------
//
bool LogsViewManager::activateView(LogsAppViewId viewId)
{
    return doActivateView(viewId, false, QVariant());
}

// -----------------------------------------------------------------------------
// LogsViewManager::activateView
// -----------------------------------------------------------------------------
//
bool LogsViewManager::activateView(
        LogsAppViewId viewId, bool showDialpad, QVariant args)
{
    return doActivateView(viewId, showDialpad, args);
}

// -----------------------------------------------------------------------------
// LogsViewManager::mainWindow
// -----------------------------------------------------------------------------
//
HbMainWindow& LogsViewManager::mainWindow()
{
    return mMainWindow;
}

// -----------------------------------------------------------------------------
// LogsViewManager::exitApplication
// -----------------------------------------------------------------------------
//
void LogsViewManager::exitApplication()
{
    LOGS_QDEBUG( "logs [UI] -> LogsViewManager::exitApplication()" );
    
    LOGS_QDEBUG( "logs [UI] Exit delayed" );
    
    // Fake exit by sending app to background
    mMainWindow.sendAppToBackground();

    bool exitAllowed( true );
    foreach ( LogsBaseView* view, mViewStack ){
        if ( !view->isExitAllowed() ){
            exitAllowed = false;
            connect( view, SIGNAL(exitAllowed()), this, SLOT(proceedExit()) );
        }
    }
    if ( exitAllowed ){
        LOGS_QDEBUG( "logs [UI] Handle exit immediataly" );
        proceedExit();
    } else {
        // Just wait for signal from view(s) that exiting can proceed
        LOGS_QDEBUG( "logs [UI] Delayed exit handling" );
    }
    
    LOGS_QDEBUG( "logs [UI] <- LogsViewManager::exitApplication()" );
}

// -----------------------------------------------------------------------------
// LogsViewManager::activatePreviousView
// Previously activated view is at slot 1 of view stack
// -----------------------------------------------------------------------------
//
bool LogsViewManager::activatePreviousView()
{
    if ( mViewStack.count() < 2 ){
        return false;
    }
    return doActivateView(mViewStack.at(1)->viewId(), false, QVariant());
}

// -----------------------------------------------------------------------------
// LogsViewManager::doActivateView
// Currently activated view is always at beginning of view stack. Deactivates
// previously activated view and activates new view and place it at top of
// view stack.
// -----------------------------------------------------------------------------
//
bool LogsViewManager::doActivateView(
        LogsAppViewId viewId, bool showDialpad, QVariant args, const QString& dialpadText)
{
    LOGS_QDEBUG( "logs [UI] -> LogsViewManager::doActivateView()" );
    
    bool activated(false);
    LogsBaseView* newView = 0; 
    LogsBaseView* oldView = mViewStack.count() > 0 ? mViewStack.at(0) : 0;
    
    viewId = checkMatchesViewTransition(viewId, dialpadText);
    
    for ( int i = 0; i < mViewStack.count(); ++i ){
        if ( mViewStack.at(i)->viewId() == viewId ){
            newView = mViewStack.takeAt(i);
        }
    }
    
    if ( oldView && newView && oldView != newView ){   
        oldView->deactivated();
        disconnect( &mMainWindow, SIGNAL( callKeyPressed() ), oldView, 0 );
    }
    
    if ( newView ){
        mViewStack.insert(0, newView);
        mMainWindow.setCurrentView(newView);
        mViewActivationArgs = args;
        mViewActivationShowDialpad = showDialpad;
        completeViewActivation();
        activated = true;
    }

    LOGS_QDEBUG( "logs [UI] <- LogsViewManager::doActivateView()" );
    
    return activated;
}

// -----------------------------------------------------------------------------
// LogsViewManager::completeViewActivation
// -----------------------------------------------------------------------------
//
void LogsViewManager::completeViewActivation()
{
    LOGS_QDEBUG_2( "logs [UI] -> LogsViewManager::completeViewActivation(), first", 
                   mFirstActivation );
    if ( mFirstActivation ){
        mFirstActivation = false;
        connect( &mMainWindow, SIGNAL(viewReady()), this, SLOT(completeViewActivation()) );
    } else {
        disconnect( &mMainWindow, SIGNAL(viewReady()), this, SLOT(completeViewActivation()) );
        LogsBaseView* newView = mViewStack.at(0);
        newView->activated(mViewActivationShowDialpad, mViewActivationArgs);
        connect( &mMainWindow, SIGNAL( callKeyPressed() ), newView, SLOT( callKeyPressed() ) );
        
        // First activation completed, clear counter
        mComponentsRepository->model()->clearMissedCallsCounter();
    }
    LOGS_QDEBUG( "logs [UI] <- LogsViewManager::completeViewActivation()" );
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsViewManager::handleOrientationChanged()
{
    LOGS_QDEBUG( "logs [UI] -> LogsViewManager::handleOrientationChanged()" );
    QMetaObject::invokeMethod(mMainWindow.currentView(), "handleOrientationChanged");
    LOGS_QDEBUG( "logs [UI] <- LogsViewManager::handleOrientationChanged()" );
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsViewManager::saveActivity()
{
    LOGS_QDEBUG( "logs [UI] -> LogsViewManager::saveActivity()" );
    
    if ( mViewStack.count() == 0 ){
        return;
    }
    
    HbActivityManager* activityManager = static_cast<HbApplication*>(qApp)->activityManager();
    foreach ( LogsBaseView* view, mViewStack ){
        view->clearActivity(*activityManager);
    }
    
    QVariantHash metaData;
    metaData.insert("screenshot", QPixmap::grabWidget(&mMainWindow, mMainWindow.rect()));
    
    QByteArray serializedActivity;
    QDataStream stream(&serializedActivity, QIODevice::WriteOnly | QIODevice::Append);
    
    metaData.insert( 
        logsActivityParamShowDialpad, mComponentsRepository->dialpad()->isOpen() );
    metaData.insert( 
        logsActivityParamDialpadText, mComponentsRepository->dialpad()->editor().text() );
    QString activityId = mViewStack.at(0)->saveActivity(stream, metaData);
    
    // add the activity to the activity manager
    bool ok = activityManager->addActivity(activityId, serializedActivity, metaData);
    if ( !ok ){
        LOGS_QDEBUG( "logs [UI] activity adding failed" );
    }
    LOGS_QDEBUG( "logs [UI] <- LogsViewManager::saveActivity()" );
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
bool LogsViewManager::loadActivity()
{
    LOGS_QDEBUG( "logs [UI] -> LogsViewManager::loadActivity()" );
    bool loaded = false;
    HbApplication* hbApp = static_cast<HbApplication*>(qApp);
    QString activityId = hbApp->activateId();
    LOGS_QDEBUG_2( "logs [UI] activity id:", activityId );
    bool ok = hbApp->activityManager()->waitActivity();
    if ( !ok ){
        LOGS_QDEBUG( "logs [UI] Activity reschedule failed" );
    }
    
    LogsBaseView* matchingView = 0;
    for ( int i = 0; i < mViewStack.count() && !matchingView; i++ ){
        if ( mViewStack.at(i)->matchWithActivityId(activityId) ){
            matchingView =  mViewStack.at(i);
        }
    }
    
    if ( matchingView ){
        // Should have only one param hash in the list, use first always
        QList<QVariantHash> allParams = hbApp->activityManager()->activities();  
        QVariantHash params = allParams.isEmpty() ? QVariantHash() : allParams.at(0);
        LOGS_QDEBUG_2( "logs [UI] Activity params", params );
        bool showDialpad = params.value(logsActivityParamShowDialpad).toBool();
        QString dialpadText = params.value(logsActivityParamDialpadText).toString();
        
        QByteArray serializedActivity = hbApp->activateData().toByteArray();
        QDataStream stream(&serializedActivity, QIODevice::ReadOnly);
        
        QVariant args = matchingView->loadActivity(activityId, stream, params);
        loaded = doActivateView( matchingView->viewId(), showDialpad, args, dialpadText );
    }
    LOGS_QDEBUG_2( "logs [UI] <- LogsViewManager::loadActivity() loaded:", loaded );
    return loaded;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsAppViewId LogsViewManager::checkMatchesViewTransition(
    LogsAppViewId viewId, const QString& dialpadText)
{
    if ( !dialpadText.isEmpty() ){
        Dialpad* dialpad = mComponentsRepository->dialpad();
        dialpad->editor().setText(dialpadText);
    }
    
    if ( viewId == LogsMatchesViewId ){
        LogsModel* model = mComponentsRepository->model();
        if ( model && model->predictiveSearchStatus() != logsContactSearchEnabled ){
            LOGS_QDEBUG( "logs [UI]     contact search disabled, go to recent view" );
            viewId = LogsRecentViewId;
        }
    }
    return viewId;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsViewManager::handleFirstActivation()
{      
    LOGS_QDEBUG( "logs [UI] -> LogsViewManager::handleFirstActivation()" );
    bool useSavedActivity( static_cast<HbApplication*>(qApp)->activateReason() == 
            Hb::ActivationReasonActivity );
    
    if ( useSavedActivity && loadActivity() ){
        LOGS_QDEBUG( "logs [UI] loaded saved activity" );    
        mMainWindow.bringAppToForeground();
    } else if ( mFirstActivation && !mService.isStartedUsingService() && 
                !mServiceOld.isStartedUsingService()) {
        changeRecentView( LogsServices::ViewAll, false );
        mMainWindow.bringAppToForeground();
    }

    // Clear previously stored activations

    HbActivityManager* activityManager = static_cast<HbApplication*>(qApp)->activityManager();
    foreach ( LogsBaseView* view, mViewStack ){
        view->clearActivity(*activityManager);
    }
    LOGS_QDEBUG( "logs [UI] <- LogsViewManager::handleFirstActivation()" );
}
