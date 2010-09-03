/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QObject>
#include <QString>
#include "logsapplication.h"


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
LogsApplication::LogsApplication(int &argc, char *argv[])
    : HbApplication(argc, argv),mViewReady(false),mReadCompleted(false), 
      mFeaturePreloadedEnabled(false), mFeatureFakeExitEnabled(false)
{
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
LogsApplication::~LogsApplication()
{
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
bool LogsApplication::logsFeaturePreloadingEnabled()
{
    return mFeaturePreloadedEnabled;
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
bool LogsApplication::logsFeatureFakeExitEnabled()
{
    return mFeatureFakeExitEnabled;
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void LogsApplication::testLogsAppEngineReady(){
    if (mViewReady && !mReadCompleted){
        emit applicationReady();
    }
    mReadCompleted = true;
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void LogsApplication::testLogsHandleAppViewReady(){
    if (mReadCompleted && !mViewReady){
        emit applicationReady();
    }
    mViewReady = true;
}

// end of file
