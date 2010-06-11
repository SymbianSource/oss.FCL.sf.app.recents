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
#include "logsservicehandlerold.h"
#include "logslogger.h"
#include "logsmainwindow.h"
#include <xqserviceutil.h>

//SYSTEM

// -----------------------------------------------------------------------------
// LogsServiceHandlerOld::LogsServiceHandlerOld
// deprecated Use "logs.com.nokia.symbian.ILogsView" interface and 
//            "show(int,bool,QString)" operation instead
// -----------------------------------------------------------------------------
//
LogsServiceHandlerOld::LogsServiceHandlerOld(LogsMainWindow& mainWindow)
    : XQServiceProvider(QLatin1String("com.nokia.services.logsservices.starter"),0), 
      mMainWindow(mainWindow)
{
    LOGS_QDEBUG( "logs [UI] -> LogsServiceHandlerOld::LogsServiceHandlerOld()" )
    
    publishAll();
    mIsAppStartedUsingService = XQServiceUtil::isService();
    
    LOGS_QDEBUG_2( 
        "logs [UI] <- LogsServiceHandlerOld::LogsServiceHandlerOld(), is service",
        mIsAppStartedUsingService )
}

// -----------------------------------------------------------------------------
// LogsServiceHandlerOld::~LogsServiceHandlerOld
// -----------------------------------------------------------------------------
//
LogsServiceHandlerOld::~LogsServiceHandlerOld()
{
    LOGS_QDEBUG( "logs [UI] <-> LogsServiceHandlerOld::~LogsServiceHandlerOld()" )
}

// -----------------------------------------------------------------------------
// LogsServiceHandlerOld::start
// -----------------------------------------------------------------------------
//
int LogsServiceHandlerOld::start(int activatedView, bool showDialpad)
{
    LOGS_QDEBUG_2( "logs [UI] -> LogsServiceHandlerOld::start(), view:", activatedView )
    
    if ( activatedView < LogsServices::ViewAll || 
         activatedView > LogsServices::ViewMissed ){
        LOGS_QDEBUG( "logs [UI] <- LogsServiceHandlerOld::start(), incorrect view" )
        return -1;
    }
    mIsAppStartedUsingService = true;
    
    LOGS_QDEBUG( "logs [UI]     Bring app to foreground" )
    mMainWindow.bringAppToForeground();
    
    emit activateView((LogsServices::LogsView)activatedView, showDialpad);

    LOGS_QDEBUG( "logs [UI] <- LogsServiceHandlerOld::start()" )
    return 0;
}

// -----------------------------------------------------------------------------
// LogsServiceHandlerOld::startWithNum
// -----------------------------------------------------------------------------
//
int LogsServiceHandlerOld::startWithNum(int activatedView, bool showDialpad, QString dialpadText)
{
    LOGS_QDEBUG_2( "logs [UI] -> LogsServiceHandlerOld::startWithNum(), view:", activatedView )
    
    Q_UNUSED(showDialpad);
    
    if ( activatedView < LogsServices::ViewAll || 
         activatedView > LogsServices::ViewMissed ){
        LOGS_QDEBUG( "logs [UI] <- LogsServiceHandlerOld::startWithNum(), incorrect view" )
        return -1;
    }
    mIsAppStartedUsingService = true;
    
    LOGS_QDEBUG( "logs [UI]     Bring app to foreground" )
    mMainWindow.bringAppToForeground();
    
    emit activateView(dialpadText);

    LOGS_QDEBUG( "logs [UI] <- LogsServiceHandlerOld::startWithNum()" )
    return 0;
}

// -----------------------------------------------------------------------------
// LogsServiceHandlerOld::isStartedUsingService
// -----------------------------------------------------------------------------
//
bool LogsServiceHandlerOld::isStartedUsingService() const
{
    return mIsAppStartedUsingService;
}

