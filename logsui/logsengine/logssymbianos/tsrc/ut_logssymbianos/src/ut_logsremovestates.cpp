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
#include "ut_logsremovestates.h"
#include "logsremovestates.h"
#include "logclient_stubs_helper.h"
#include "logsmodel.h"
#include <logcli.h>
#include <logview.h>

#include <QtTest/QtTest>

void UT_LogsRemoveStates::initTestCase()
{
    int rfsDummy = 0;
    mLogClient = CLogClient::NewL( *((RFs*)&rfsDummy) );
}

void UT_LogsRemoveStates::cleanupTestCase()
{
    delete mLogClient;
}


void UT_LogsRemoveStates::init()
{
    reset();
    
    mCurrentState = 0;
    mIndex = 0;
    mLogView = 0;
    mLogViewEvent = 0;
    mIsRecentView = true;

    mLogView = CLogViewRecent::NewL(*mLogClient);
    mLogViewEvent = 0;
    mDuplicatesView = CLogViewDuplicate::NewL(*mLogClient);
}

void UT_LogsRemoveStates::cleanup()
{
    delete mLogView;
    mLogView = 0;
    delete mLogViewEvent;
    mLogViewEvent = 0;
    delete mDuplicatesView;
    mDuplicatesView = 0;  
}

void UT_LogsRemoveStates::reset()
{
    mRemoveCompleted = false;
    mClearType = -1;
    mRemovedEvents.clear();
}

void UT_LogsRemoveStates::testClearAll()
{
    LogsRemoveStateClearAll state(*this, *this);
    mClearType = LogsModel::TypeLogsClearAll;
    QVERIFY( state.enterL() );
    QCOMPARE( LogClientStubsHelper::lastCalledFunc(), QString("ClearLog") );
    
    mClearType = LogsModel::TypeLogsClearMissed;
    QVERIFY( state.enterL() );
    QCOMPARE( LogClientStubsHelper::lastCalledFunc(), QString("ClearLogRecentList") );
    
    QVERIFY( !state.continueL() );
}

void UT_LogsRemoveStates::testInit()
{
    LogsRemoveStateInit state(*this, *this);
    
    // Nothing happens when no recent view exists
    mIsRecentView = false;
    QVERIFY( !state.enterL() );
    
    // Recent view exists and is getting initialized
    mIndex = 4;
    mIsRecentView = true;
    QVERIFY( state.enterL() );
    QCOMPARE( mIndex, 0 );
    
    QVERIFY( !state.continueL() );
}

void UT_LogsRemoveStates::testDelete()
{
    LogsRemoveStateDelete state(*this, *this);
    
    // Nothing to delete
    QVERIFY( !state.enterL() );
    
    // Delete several
    mRemovedEvents.append(1);
    mRemovedEvents.append(3);
    mRemovedEvents.append(4);
    
    QVERIFY( state.enterL() );
    QCOMPARE( state.mRemoveIndex, 1 );
    QVERIFY( state.continueL() );
    QCOMPARE( state.mRemoveIndex, 2 );
    QVERIFY( state.continueL() );
    QCOMPARE( state.mRemoveIndex, 3 );
    QVERIFY( !state.continueL() );
}

void UT_LogsRemoveStates::testDeleteDuplicates()
{
    LogsRemoveStateDeleteDuplicates state(*this, *this);
    
    // More than one id, duplicate deletion not supported
    mRemovedEvents.append(1);
    mRemovedEvents.append(3);
    QVERIFY( !state.enterL() );
    
    // Dbview iterator is not at correct location
    mRemovedEvents.clear();
    mRemovedEvents.append(99);
    const_cast<CLogEvent&>( mLogView->Event() ).SetId( 100 );
    QVERIFY( !state.enterL() );
    
    // No duplicates
    LogClientStubsHelper::setStubAsyncCallPossible(false);
    LogClientStubsHelper::setViewCount(0); // effective also for duplicates view
    const_cast<CLogEvent&>( mLogView->Event() ).SetId( 99 );
    QVERIFY( !state.enterL() );
    
    // Duplicates
    LogClientStubsHelper::setStubAsyncCallPossible(true);
    LogClientStubsHelper::setViewCount(3); // effective also for duplicates view
    QVERIFY( state.enterL() );
    QVERIFY( !state.mDeleting );
    QCOMPARE( state.mDeleteDuplicateIds.count(), 0 );
    
    QVERIFY( state.continueL() );
    QVERIFY( !state.mDeleting );
    QCOMPARE( state.mDeleteDuplicateIds.count(), 0 );
    QVERIFY( state.continueL() );
    QVERIFY( !state.mDeleting );
    QCOMPARE( state.mDeleteDuplicateIds.count(), 1 );
    QVERIFY( state.continueL() );
    QVERIFY( !state.mDeleting );
    QCOMPARE( state.mDeleteDuplicateIds.count(), 2 );
    QVERIFY( state.continueL() );
    QVERIFY( !state.mDeleting );
    QCOMPARE( state.mDeleteDuplicateIds.count(), 3 );
      
    QVERIFY( state.continueL() );
    QVERIFY( state.mDeleting );
    QCOMPARE( state.mDeleteDuplicateIds.count(), 2 );
    QVERIFY( state.continueL() );
    QVERIFY( state.mDeleting );
    QCOMPARE( state.mDeleteDuplicateIds.count(), 1 );
    QVERIFY( state.continueL() );
    QVERIFY( state.mDeleting );
    QCOMPARE( state.mDeleteDuplicateIds.count(), 0 );
    QVERIFY( !state.continueL() );
    
}

void UT_LogsRemoveStates::testDone()
{
    LogsRemoveStateDone state(*this, *this);
    QVERIFY( !mRemoveCompleted );
    QVERIFY( !state.enterL() );
    QVERIFY( mRemoveCompleted );
}

// ----------------------------------------------------------------------------
// From LogsStateBaseContext
// ----------------------------------------------------------------------------
//
void UT_LogsRemoveStates::setCurrentState(const LogsStateBase& state)
{
    mCurrentState = &state;
}

CLogView& UT_LogsRemoveStates::logView()
{
    if ( mIsRecentView ){
        return *mLogView;
    }
    return *mLogViewEvent;
}

CLogViewDuplicate& UT_LogsRemoveStates::duplicatesView()
{
    return *mDuplicatesView;
}

int& UT_LogsRemoveStates::index()
{
    return mIndex;
}

TRequestStatus& UT_LogsRemoveStates::reqStatus()
{
    return mReqStatus;
}


int UT_LogsRemoveStates::currentEventId()
{
    if ( !mRemovedEvents.isEmpty() ){
        return mRemovedEvents.at(0);
    }
    return mCurrentEventId;
}

CLogClient& UT_LogsRemoveStates::logClient()
{
    return *mLogClient;
}

bool UT_LogsRemoveStates::isRecentView()
{
    return mIsRecentView;
}


// ----------------------------------------------------------------------------
// From LogsRemoveStateContext
// ----------------------------------------------------------------------------
//

LogsRemoveObserver& UT_LogsRemoveStates::observer()
{
    return *this;
}
QList<int>& UT_LogsRemoveStates::removedEvents()
{
    return mRemovedEvents;
}
int UT_LogsRemoveStates::clearType()
{
    return mClearType;
}

// ----------------------------------------------------------------------------
// From LogsRemoveObserver
// ----------------------------------------------------------------------------
//
void UT_LogsRemoveStates::removeCompleted()
{
    mRemoveCompleted = true;
}
void UT_LogsRemoveStates::logsRemoveErrorOccured(int err)
{
    mError = err;
}
