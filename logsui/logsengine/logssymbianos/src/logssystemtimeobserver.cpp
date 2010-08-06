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

// INCLUDE FILES
#include <bacntf.h>                   // CEnvironmentChangeNotifier
#include "logssystemtimeobserver.h"
#include "logslogger.h"

// CONSTANTS

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
LogsSystemTimeObserver::LogsSystemTimeObserver(QObject* parent) : QObject(parent)
{
    LOGS_QDEBUG( "logs [ENG] -> LogsSystemTimeObserver::LogsSystemTimeObserver()" )
    TCallBack callback(EnvironmentChangeCallback, this);

    QT_TRAP_THROWING(mEnvChangeNotifier = 
            CEnvironmentChangeNotifier::NewL( CActive::EPriorityStandard, callback ));

    mEnvChangeNotifier->Start();
    LOGS_QDEBUG( "logs [ENG] <- LogsSystemTimeObserver::LogsSystemTimeObserver()" )
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
LogsSystemTimeObserver::~LogsSystemTimeObserver()
{
    LOGS_QDEBUG( "logs [ENG] -> LogsSystemTimeObserver::~LogsSystemTimeObserver()" )
    
    delete mEnvChangeNotifier;
    
    LOGS_QDEBUG( "logs [ENG] <- LogsSystemTimeObserver::~LogsSystemTimeObserver()" )
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
TInt LogsSystemTimeObserver::EnvironmentChangeCallback(TAny* aThis)
{
    LOGS_QDEBUG( "logs [ENG] -> LogsSystemTimeObserver::EnvironmentChangeCallback()" )
    LogsSystemTimeObserver* self = static_cast<LogsSystemTimeObserver*>(aThis);
    if (self->mEnvChangeNotifier->Change() & EChangesLocale) {
        LOGS_QDEBUG( "logs [ENG] system time or Locale CHANGED!!!!" )
        emit self->timeFormatChanged();
    }
    return 0;
}
