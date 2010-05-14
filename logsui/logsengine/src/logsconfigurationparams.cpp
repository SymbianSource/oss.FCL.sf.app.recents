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
#include "logsconfigurationparams.h"
#include "logslogger.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsConfigurationParams::LogsConfigurationParams( QObject* parent ) : 
    QObject(parent), mListItemTextWidth(0)
{
    
    LOGS_QDEBUG( "logs [ENG] <-> LogsConfigurationParams::LogsConfigurationParams()" )
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsConfigurationParams& LogsConfigurationParams::operator=( 
    const LogsConfigurationParams& params )
{
    
    LOGS_QDEBUG( "logs [ENG] <-> LogsConfigurationParams::operator=()" )
    mListItemTextWidth = params.mListItemTextWidth;
    return *this;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsConfigurationParams::~LogsConfigurationParams()
{
    LOGS_QDEBUG( "logs [ENG] <-> LogsConfigurationParams::~LogsConfigurationParams()" )
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsConfigurationParams::setListItemTextWidth(int width)
{
    mListItemTextWidth = width;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
int LogsConfigurationParams::listItemTextWidth() const
{
    return mListItemTextWidth;
}

// End of file

