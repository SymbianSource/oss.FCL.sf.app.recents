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
#include "logsremove.h"
#include "logsmodel.h"
#include "logslogger.h"
#include "logsremoveobserver.h"
#include "logsremovestates.h"
#include <logcli.h>
#include <f32file.h>
#include <logview.h>
//SYSTEM

// CONSTANTS

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsRemove::LogsRemove( LogsRemoveObserver& observer, bool readingAllEvents )
: LogsWorker(readingAllEvents),
  mObserver(observer),
  mFsSession(0),
  mClearType(-1)
{
}

// -----------------------------------------------------------------------------
// LogsRemove::~LogsRemove
// -----------------------------------------------------------------------------
//
LogsRemove::~LogsRemove()
{
    LOGS_QDEBUG( "logs [ENG] -> LogsRemove::~LogsRemove()" )
    Cancel();
    delete mLogViewRecent;
    mLogViewRecent = 0;
    delete mDuplicatesView;
    mDuplicatesView = 0;
    delete mLogClient;
    mLogClient = 0;
    if ( mFsSession ){
        mFsSession->Close();
    }
    delete mFsSession;
    qDeleteAll( mRemoveStates );
    qDeleteAll( mClearAllStates );
    LOGS_QDEBUG( "logs [ENG] <- LogsRemove::~LogsRemove()" )
}

// ----------------------------------------------------------------------------
// LogsRemove::RunError
// ----------------------------------------------------------------------------
//
TInt LogsRemove::RunError(TInt aError)
{
	mObserver.logsRemoveErrorOccured(aError);
	return KErrNone;	
}


// ----------------------------------------------------------------------------
// LogsRemove::init
// ----------------------------------------------------------------------------
//
int LogsRemove::init()
{
    LOGS_QDEBUG( "logs [ENG] -> LogsRemove::init()" )    
    TRAPD( err, initL() );
    LOGS_QDEBUG_2( "logs [ENG] <- LogsRemove::init(), err:", err )    
    return err;
}


// ----------------------------------------------------------------------------
// LogsRemove::clearList
// ----------------------------------------------------------------------------
//
bool LogsRemove::clearList(LogsModel::ClearType cleartype)
{
    LOGS_QDEBUG( "logs [ENG] -> LogsRemove::clearList" )
  
    bool clearingStarted( false );
    TRAP_IGNORE( clearingStarted = clearListL(cleartype) )
    
    LOGS_QDEBUG_2( "logs [ENG] <- LogsRemove::clearList, started", clearingStarted )
    return clearingStarted;
}

// ----------------------------------------------------------------------------
// LogsRemove::clearListL
// ----------------------------------------------------------------------------
//
bool LogsRemove::clearListL(LogsModel::ClearType cleartype)
{
    LOGS_QDEBUG( "logs [ENG] -> LogsRemove::clearListL" )
  
    initializeClearAllL();
    
    mClearType = cleartype;
    
    bool clearingStarted = startClearingL();
    
    LOGS_QDEBUG_2( "logs [ENG] <- LogsRemove::clearListL, started", clearingStarted )
    return clearingStarted;
}

// ----------------------------------------------------------------------------
// LogsRemove::initL
// ----------------------------------------------------------------------------
//
void LogsRemove::initL()
{
    LOGS_QDEBUG( "logs [ENG] -> LogsRemove::initL" )
        
    __ASSERT_ALWAYS( !IsActive(), User::Leave( KErrInUse ) );
       
    if ( !mFsSession ){
        mFsSession = new (ELeave) RFs();
        User::LeaveIfError( mFsSession->Connect() );
    }
    
    if ( !mLogClient ){
        mLogClient = CLogClient::NewL( *mFsSession );
    }  
       
    LOGS_QDEBUG( "logs [ENG] <- LogsRemove::initL" )
}


// ----------------------------------------------------------------------------
// LogsRemove::clearEvents
// ----------------------------------------------------------------------------
//
int LogsRemove::clearEvents(const QList<int>& eventIds, bool& async)
{
    TRAPD( err, clearEventsL(eventIds, async) );
    return err;
}
    
// ----------------------------------------------------------------------------
// LogsRemove::clearEventsL
// ----------------------------------------------------------------------------
//
void LogsRemove::clearEventsL(const QList<int>& eventIds, bool& async)
{
    LOGS_QDEBUG( "logs [ENG] -> LogsRemove::clearEventsL()")
    
    async = false;
    initializeIdBasedRemovalL();
    
    mRemovedEvents = eventIds;
    mCurrentEventId = eventIds.isEmpty() ? -1 : eventIds.at(0);
    if ( !mRemovedEvents.isEmpty() ){
        async = startClearingL();
    }
  
    LOGS_QDEBUG_2( "logs [ENG] <- LogsRemove::clearEventsL(): async", async ) 
}

// ----------------------------------------------------------------------------
// LogsRemove::initializeClearAllL
// ----------------------------------------------------------------------------
//
void LogsRemove::initializeClearAllL()
{
    initL();
    
    if ( mClearAllStates.count() == 0 ){
        LogsRemoveStateClearAll* clearAll = new LogsRemoveStateClearAll(*this, *this);
        LogsRemoveStateDone* done = new LogsRemoveStateDone(*this, *this);
        clearAll->setNextState(*done);
        mClearAllStates.append(clearAll);
        mClearAllStates.append(done);
    }
    mCurrentStateMachine = &mClearAllStates;
    setCurrentState(*mClearAllStates.at(0));
}

// ----------------------------------------------------------------------------
// LogsRemove::initializeIdBasedRemovalL
// ----------------------------------------------------------------------------
//
void LogsRemove::initializeIdBasedRemovalL()
{
    initL();
    
    if ( !mLogViewRecent ){
        mLogViewRecent = CLogViewRecent::NewL( *mLogClient );
    }
    if ( !mDuplicatesView ) {
        mDuplicatesView = CLogViewDuplicate::NewL( *mLogClient ); 
    }
    
    if ( mRemoveStates.count() == 0 ){
        LogsRemoveStateInit* init = new LogsRemoveStateInit(*this, *this);
        LogsStateSearchingEvent* searchEvent = new LogsStateSearchingEvent(*this);
        LogsRemoveStateDeleteDuplicates* deleteDuplicates = 
            new LogsRemoveStateDeleteDuplicates(*this, *this);
        LogsRemoveStateDelete* deleteFromMain = new LogsRemoveStateDelete(*this, *this);
        LogsRemoveStateDone* done = new LogsRemoveStateDone(*this, *this);
        init->setNextState(*searchEvent);
        searchEvent->setNextState(*deleteDuplicates);
        deleteDuplicates->setNextState(*deleteFromMain);
        deleteFromMain->setNextState(*done);
        mRemoveStates.append(init);
        mRemoveStates.append(searchEvent);
        mRemoveStates.append(deleteDuplicates);
        mRemoveStates.append(deleteFromMain);
        mRemoveStates.append(done);
    }
    mCurrentStateMachine = &mRemoveStates;
    setCurrentState(*mRemoveStates.at(0));
}

// ----------------------------------------------------------------------------
// LogsRemove::startClearingL
// ----------------------------------------------------------------------------
//
bool LogsRemove::startClearingL()
{
    __ASSERT_ALWAYS( !IsActive(), User::Leave( KErrInUse ) );

    if ( currentState().enterL() ){
        SetActive();
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------
// LogsRemove::observer
// ----------------------------------------------------------------------------
//
LogsRemoveObserver& LogsRemove::observer()
{
    return mObserver;
}

// ----------------------------------------------------------------------------
// LogsRemove::removedEvents
// ----------------------------------------------------------------------------
//
QList<int>& LogsRemove::removedEvents()
{
    return mRemovedEvents;
}

// ----------------------------------------------------------------------------
// LogsRemove::clearType
// ----------------------------------------------------------------------------
//
int LogsRemove::clearType()
{
    return mClearType;
}


// End of file

