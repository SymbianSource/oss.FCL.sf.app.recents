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
#include "logsservicehandler.h"
#include "logslogger.h"
#include "logsmainwindow.h"
#include <xqserviceutil.h>

//SYSTEM

// -----------------------------------------------------------------------------
// LogsService::LogsService
// -----------------------------------------------------------------------------
//
LogsServiceHandler::LogsServiceHandler(LogsMainWindow& mainWindow)
    : XQServiceProvider(QLatin1String("com.nokia.services.logsservices.starter"),0), 
      mActivatedView(0),
      mMainWindow(mainWindow)
{
    LOGS_QDEBUG( "logs [UI] -> LogsServiceHandler::LogsServiceHandler()" )
    
    publishAll();
    mIsAppStartedUsingService = XQServiceUtil::isService();
    
    LOGS_QDEBUG_2( 
        "logs [UI] <- LogsServiceHandler::LogsServiceHandler(), is service",
        mIsAppStartedUsingService )
}

// -----------------------------------------------------------------------------
// LogsServiceHandler::~LogsServiceHandler
// -----------------------------------------------------------------------------
//
LogsServiceHandler::~LogsServiceHandler()
{
    LOGS_QDEBUG( "logs [UI] <-> LogsServiceHandler::~LogsServiceHandler()" )
}

// -----------------------------------------------------------------------------
// LogsServiceHandler::start
// -----------------------------------------------------------------------------
//
int LogsServiceHandler::start(int activatedView, bool showDialpad)
{
    LOGS_QDEBUG_2( "logs [UI] -> LogsServiceHandler::start(), view:", activatedView )
    
    if ( activatedView < LogsServices::ViewAll || 
         activatedView > LogsServices::ViewMissed ){
        LOGS_QDEBUG( "logs [UI] <- LogsServiceHandler::start(), incorrect view" )
        return -1;
    }
    mIsAppStartedUsingService = true;
    mActivatedView = activatedView;
    
    LOGS_QDEBUG( "logs [UI]     Bring app to foreground" )
    mMainWindow.bringAppToForeground();
    
    emit activateView((LogsServices::LogsView)mActivatedView, showDialpad);

    LOGS_QDEBUG( "logs [UI] <- LogsServiceHandler::start()" )
    return 0;
}

// -----------------------------------------------------------------------------
// LogsServiceHandler::startWithNum
// -----------------------------------------------------------------------------
//
int LogsServiceHandler::startWithNum(int activatedView, bool showDialpad, QString dialpadText)
{
    LOGS_QDEBUG_2( "logs [UI] -> LogsServiceHandler::startWithNum(), view:", activatedView )
    
    Q_UNUSED(showDialpad);
    
    if ( activatedView < LogsServices::ViewAll || 
         activatedView > LogsServices::ViewMissed ){
        LOGS_QDEBUG( "logs [UI] <- LogsServiceHandler::startWithNum(), incorrect view" )
        return -1;
    }
    mIsAppStartedUsingService = true;
    mActivatedView = activatedView;
    
    LOGS_QDEBUG( "logs [UI]     Bring app to foreground" )
    mMainWindow.bringAppToForeground();
    
    emit activateView(dialpadText);

    LOGS_QDEBUG( "logs [UI] <- LogsServiceHandler::startWithNum()" )
    return 0;
}

// -----------------------------------------------------------------------------
// LogsServiceHandler::currentlyActivatedView
// -----------------------------------------------------------------------------
//
LogsServices::LogsView LogsServiceHandler::currentlyActivatedView()
{
    return (LogsServices::LogsView)mActivatedView;
}

// -----------------------------------------------------------------------------
// LogsServiceHandler::isStartedUsingService
// -----------------------------------------------------------------------------
//
bool LogsServiceHandler::isStartedUsingService() const
{
    return mIsAppStartedUsingService;
}
