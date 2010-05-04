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
#include "logsdbconnector.h"
#include "logsdbconnector_stub_helper.h"
#include "logsevent.h"

// CONSTANTS

QString logsLastCalledFunction = "";
int logsPredictiveSearchStatus = 0;

void LogsDbConnectorStubHelper::reset()
{
    logsLastCalledFunction = "";
    logsPredictiveSearchStatus = 0;
}

QString LogsDbConnectorStubHelper::lastCalledFunction()
{
    return logsLastCalledFunction;
}

void LogsDbConnectorStubHelper::setPredictiveSearch(int status)
{
    logsPredictiveSearchStatus = status;
}

// ----------------------------------------------------------------------------
// LogsDbConnector::LogsDbConnector
// ----------------------------------------------------------------------------
//
LogsDbConnector::LogsDbConnector( 
    QList<LogsEvent*>& events, bool checkAllEvents, bool resourceControl ) 
: QObject(), mModelEvents( events ), mCheckAllEvents( checkAllEvents ), 
  mResourceControl( resourceControl )
{
}

// ----------------------------------------------------------------------------
// LogsDbConnector::~LogsDbConnector
// ----------------------------------------------------------------------------
//
LogsDbConnector::~LogsDbConnector()
{
    while ( !mEvents.isEmpty() ){
        delete mEvents.takeFirst();
    }
}

// ----------------------------------------------------------------------------
// LogsDbConnector::init
// ----------------------------------------------------------------------------
//
int LogsDbConnector::init()
{
    logsLastCalledFunction = "init";
    return 0;
}

// ----------------------------------------------------------------------------
// LogsDbConnector::start
// ----------------------------------------------------------------------------
//
int LogsDbConnector::start()
{
    logsLastCalledFunction = "start";
    return 0;
}

// ----------------------------------------------------------------------------
// LogsDbConnector::updateDetails
// ----------------------------------------------------------------------------
//
int LogsDbConnector::updateDetails(bool /*clearCached*/)
{
    logsLastCalledFunction = "updateDetails";
    return 0;
}

// ----------------------------------------------------------------------------
// LogsDbConnector::clearList
// ----------------------------------------------------------------------------
//
bool LogsDbConnector::clearList(LogsModel::ClearType /*cleartype*/)
{
    logsLastCalledFunction = "clearList";
    return true;
}

// ----------------------------------------------------------------------------
// LogsDbConnector::clearMissedCallsCounter
// ----------------------------------------------------------------------------
//
int LogsDbConnector::clearMissedCallsCounter()
{
    logsLastCalledFunction = "clearMissedCallsCounter";
    return 0;
}

// ----------------------------------------------------------------------------
// LogsDbConnector::clearEvents
// ----------------------------------------------------------------------------
//
bool LogsDbConnector::clearEvents(const QList<int>& /*ids*/)
{    
    logsLastCalledFunction = "clearEvents";
    return true;
}

// ----------------------------------------------------------------------------
// LogsDbConnector::markEventsSeen
// ----------------------------------------------------------------------------
//
bool LogsDbConnector::markEventsSeen(const QList<int>& ids)
{
    bool started(false);
    logsLastCalledFunction = "markEventsSeen";
    foreach( int currId, ids ){
        if ( !mEventsSeen.contains(currId) ){
            mEventsSeen.append(currId);
            started = true;
        }
    }
    return started;
}

// ----------------------------------------------------------------------------
// LogsDbConnector::readDuplicates
// ----------------------------------------------------------------------------
//
int LogsDbConnector::readDuplicates(int eventId)
{
    Q_UNUSED(eventId);
    logsLastCalledFunction = "readDuplicates";
    return 0;
}
    
// ----------------------------------------------------------------------------
// LogsDbConnector::takeDuplicates
// ----------------------------------------------------------------------------
//
QList<LogsEvent*> LogsDbConnector::takeDuplicates()
{
    QList<LogsEvent*> events;
    events = mDuplicatedEvents;
    mDuplicatedEvents.clear();
    return events;
}

// ----------------------------------------------------------------------------
// LogsDbConnector::refreshData
// ----------------------------------------------------------------------------
//
int LogsDbConnector::refreshData()
{
    logsLastCalledFunction = "refreshData";
    return 0;
}

// ----------------------------------------------------------------------------
// LogsDbConnector::compressData
// ----------------------------------------------------------------------------
//
int LogsDbConnector::compressData()
{
    logsLastCalledFunction = "compressData";
    return 0;
}

// ----------------------------------------------------------------------------
// LogsDbConnector::readCompleted
// ----------------------------------------------------------------------------
//
void LogsDbConnector::readCompleted(int /*numRead*/)
{
}

// ----------------------------------------------------------------------------
// LogsDbConnector::errorOccurred
// ----------------------------------------------------------------------------
//
void LogsDbConnector::errorOccurred(int err)
{
    Q_UNUSED(err)
}

// ----------------------------------------------------------------------------
// LogsDbConnector::readCompleted
// ----------------------------------------------------------------------------
//
void LogsDbConnector::removeCompleted()
{
}

// ----------------------------------------------------------------------------
// LogsDbConnector::errorOccurred
// ----------------------------------------------------------------------------
//
void LogsDbConnector::logsRemoveErrorOccured(int err)
{
    Q_UNUSED(err)
}

// ----------------------------------------------------------------------------
// LogsDbConnector::temporaryErrorOccurred
// ----------------------------------------------------------------------------
//
void LogsDbConnector::temporaryErrorOccurred(int err)
{
    Q_UNUSED(err)
}

// ----------------------------------------------------------------------------
// LogsDbConnector::eventModifyingCompleted
// ----------------------------------------------------------------------------
//
void LogsDbConnector::eventModifyingCompleted()
{

}

// ----------------------------------------------------------------------------
// LogsDbConnector::duplicatesReadingCompleted
// ----------------------------------------------------------------------------
//
void LogsDbConnector::duplicatesReadingCompleted(QList<LogsEvent*> duplicates)
{
    Q_UNUSED(duplicates);
}

// ----------------------------------------------------------------------------
// LogsDbConnector::doMarkEventSeen
// ----------------------------------------------------------------------------
//
int LogsDbConnector::doMarkEventSeen()
{
    return 0;
}

// ----------------------------------------------------------------------------
// LogsDbConnector::predictiveSearchStatus
// ----------------------------------------------------------------------------
//
int LogsDbConnector::predictiveSearchStatus()
{
    logsLastCalledFunction = "predictiveSearchStatus";
    return logsPredictiveSearchStatus;
}

// ----------------------------------------------------------------------------
// LogsDbConnector::setPredictiveSearch
// ----------------------------------------------------------------------------
//
int LogsDbConnector::setPredictiveSearch(bool enabled)
{
    logsLastCalledFunction = "setPredictiveSearch";
    if (logsPredictiveSearchStatus != 0) {
        logsPredictiveSearchStatus = enabled ? 1 : 2;
        return 0;
    } else {
        return -1;
    }
}
