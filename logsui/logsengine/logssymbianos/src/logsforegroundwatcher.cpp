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

#include "logsforegroundwatcher.h"
#include "logslogger.h"

// -----------------------------------------------------------------------------
// Currently no other way to monitor app foreground/background switches.
// HbForegroundWatcher might become public at some point which could be then
// used instead.
// -----------------------------------------------------------------------------
//
LogsForegroundWatcher::LogsForegroundWatcher(QObject* parent) : QObject(parent)
{
    CCoeEnv* env = CCoeEnv::Static();
    if (env) {
        TRAP_IGNORE(env->AddForegroundObserverL(*this));
    }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsForegroundWatcher::HandleGainingForeground()
{
    LOGS_QDEBUG( "logs [UI] -> LogsForegroundWatcher::HandleGainingForeground" );
    emit gainingForeground();
    LOGS_QDEBUG( "logs [UI] <- LogsForegroundWatcher::HandleGainingForeground" );
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsForegroundWatcher::HandleLosingForeground()
{
    LOGS_QDEBUG( "logs [UI] -> LogsForegroundWatcher::HandleLosingForeground" );
    emit losingForeground();
    LOGS_QDEBUG( "logs [UI] <- LogsForegroundWatcher::HandleLosingForeground" );
}

