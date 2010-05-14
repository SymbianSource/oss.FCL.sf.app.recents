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
#include "logsmainwindow.h"

//SYSTEM
#include <hbmainwindow.h>
#include <hbview.h>
#include <logsservices.h>
#include <QApplication>
#include <hblineedit.h>
#include <dialpad.h>

// -----------------------------------------------------------------------------
// LogsViewManager::LogsViewManager
// -----------------------------------------------------------------------------
//
LogsViewManager::LogsViewManager( 
        LogsMainWindow& mainWindow, LogsServiceHandler& service ) : 
    QObject( 0 ), mMainWindow( mainWindow ), mService( service ), 
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
    
    connect( &mMainWindow, SIGNAL(appFocusGained()), this, SLOT(appFocusGained()) );
    connect( &mMainWindow, SIGNAL(appFocusLost()), this, SLOT(appFocusLost()) );
    
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
    
    // Disable view switching as otherwise flick gestures change
    // views which is not desired.
    //Deprecated:
    //mMainWindow.setViewSwitchingEnabled(false);
    
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
    Dialpad* dialpad = mComponentsRepository->dialpad();
    dialpad->editor().setText(dialpadText);
    LogsModel* model = mComponentsRepository->model();
    if ( model && model->predictiveSearchStatus() == logsContactSearchEnabled ){
        LOGS_QDEBUG( "logs [UI]     contact search enabled, go to macthes view" );
        doActivateView(LogsMatchesViewId, true, QVariant());
    } else {
        LOGS_QDEBUG( "logs [UI]     contact search disabled, go to recent view" );
        doActivateView(LogsRecentViewId, true, QVariant());
    }
    LOGS_QDEBUG( "logs [UI] <- LogsViewManager::changeMatchesView()" );
}

// -----------------------------------------------------------------------------
// LogsViewManager::appFocusGained
// Activate recent view if gaining focus first time and app was not started
// via highway service. If app was started by using service request, 
// wait the request to appear before activating view.
// -----------------------------------------------------------------------------
//
void LogsViewManager::appFocusGained()
{
    LOGS_QDEBUG( "logs [UI] -> LogsViewManager::appFocusGained()" );
    
    foreach ( LogsBaseView* view, mViewStack ){
        disconnect( view, SIGNAL(exitAllowed()), this, SLOT(proceedExit()) );
    }
    
    if ( mFirstActivation && !mService.isStartedUsingService() ){
        changeRecentView( mService.currentlyActivatedView(), false );
        mMainWindow.bringAppToForeground();
    }
    
    mComponentsRepository->model()->refreshData();

    LOGS_QDEBUG( "logs [UI] <- LogsViewManager::appFocusGained()" );
}

// -----------------------------------------------------------------------------
// LogsViewManager::appFocusLost
// -----------------------------------------------------------------------------
//
void LogsViewManager::appFocusLost()
{
    LOGS_QDEBUG( "logs [UI] -> LogsViewManager::appFocusLost()" );
    
    LOGS_QDEBUG( "logs [UI] <- LogsViewManager::appFocusLost()" );
}

// -----------------------------------------------------------------------------
// LogsViewManager::proceedExit
// -----------------------------------------------------------------------------
//
void LogsViewManager::proceedExit()
{
    LOGS_QDEBUG( "logs [UI] -> LogsViewManager::proceedExit()" );
    
    mComponentsRepository->model()->compressData();
    
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
    
    // Set application to default state (recent calls and no dialpad)
    activateView( LogsRecentViewId, false, QVariant() );
    mComponentsRepository->recentCallsView()->resetView();

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
        LogsAppViewId viewId, bool showDialpad, QVariant args)
{
    LOGS_QDEBUG( "logs [UI] -> LogsViewManager::doActivateView()" );
    
    bool activated(false);
    LogsBaseView* newView = 0; 
    LogsBaseView* oldView = mViewStack.count() > 0 ? mViewStack.at(0) : 0;
    
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
