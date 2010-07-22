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

#include <QDebug>
#include "logsservicetesterappcloser.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsServiceTesterAppCloser::LogsServiceTesterAppCloser(QObject* parent) : 
    QObject(parent)
{
    mAppCloseWatch = new CAppCloseWatcher(*this);    
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(closeAppTimeout()));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsServiceTesterAppCloser::~LogsServiceTesterAppCloser()
{
    delete mAppCloseWatch;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsServiceTesterAppCloser::AppClosed(TInt aError)
{
    mTimer.stop();
    if (aError == KErrNone) {
        emit closed();
    } else {
        emit closeError(aError);
    }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsServiceTesterAppCloser::closeAppTimeout()
{
    mAppCloseWatch->Cancel();
    emit closeError(ErrorClosingTimeout);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsServiceTesterAppCloser::closeApp(TApaTask& aTask)
{
    if ( !mAppCloseWatch->IsActive() ) {
        mTimer.start(3000); //3 sec.
        mTimer.setSingleShot(true);
        mAppCloseWatch->Start(aTask);
        aTask.EndTask();
    } else {
        emit closeError(ErrorClosingOngoing);
    }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsServiceTesterAppCloser::closeDialerApp()
{
    qDebug() << "[LOGS_TESTER] -> LogsServiceTesterAppCloser::closeDialerApp()";
    const TUid KUidDialer    = { 0x101F4CD5 };
    RWsSession ws;
    TInt err = ws.Connect();
    if (err == KErrNone) {
        TApaTaskList tl( ws);
        TApaTask dialerTask = (TApaTask)tl.FindApp( KUidDialer );        
        if (dialerTask.Exists()) {
            qDebug() << "[LOGS_TESTER] dialer is running, killing";
            closeApp(dialerTask);
        } else {
            qDebug() << "[LOGS_TESTER] dialer is not running";
            emit closeError(ErrorAppNotRunning);
        }
        ws.Close();
    } else {
        emit closeError(err);
    }
    qDebug() << "[LOGS_TESTER] <- LogsServiceTesterAppCloser::closeDialerApp()";    
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CAppCloseWatcher::CAppCloseWatcher(MCloseOberver& aObserver) :  
    CActive(CActive::EPriorityStandard), iObserver(aObserver)
    {
    CActiveScheduler::Add(this);    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CAppCloseWatcher::~CAppCloseWatcher()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CAppCloseWatcher::Start(const TApaTask& aTask)
    {
    qDebug() << "[LOGS_TESTER] -> CAppCloseWatcher::Start()!";
    if (!IsActive())
        {
        TInt err = iThread.Open(aTask.ThreadId());
        if (err == KErrNone)
            {
            qDebug() << "[LOGS_TESTER] calling  iThread.Logon(iStatus)";    
            iOriginalProcessPriority = iThread.ProcessPriority();
            iThread.SetProcessPriority(EPriorityForeground);
            iThread.Logon(iStatus);
            SetActive();
            }
        else
            {
            qDebug() << "[LOGS_TESTER] calling  iThread.Open() failed: "<< err;
            TRequestStatus* s = &iStatus;
            User::RequestComplete(s, err);
            SetActive();
            }
        } 
    qDebug() << "[LOGS_TESTER] <- CAppCloseWatcher::Start()";    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CAppCloseWatcher::DoCancel()
    {
    qDebug() << "[LOGS_TESTER] -> CAppCloseWatcher::DoCancel()";
    iThread.LogonCancel(iStatus);
    iThread.SetProcessPriority(iOriginalProcessPriority);
    iThread.Close();
    qDebug() << "[LOGS_TESTER] -> CAppCloseWatcher::DoCancel()";
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CAppCloseWatcher::RunL()
    {
    qDebug() << "[LOGS_TESTER] -> CAppCloseWatcher::RunL()";
    if (iThread.Handle())
        iThread.SetProcessPriority(iOriginalProcessPriority);
    iThread.Close();
    iObserver.AppClosed(iStatus.Int());
    qDebug() << "[LOGS_TESTER] <- CAppCloseWatcher::RunL()";
    }
