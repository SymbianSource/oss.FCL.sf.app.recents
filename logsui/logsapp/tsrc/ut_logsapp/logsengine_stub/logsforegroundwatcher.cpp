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

// -----------------------------------------------------------------------------
// Currently no other way to monitor app foreground/background switches.
// HbForegroundWatcher might become public at some point which could be then
// used instead.
// -----------------------------------------------------------------------------
//
LogsForegroundWatcher::LogsForegroundWatcher(QObject* parent) : QObject(parent)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsForegroundWatcher::HandleGainingForeground()
{
    emit gainingForeground();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsForegroundWatcher::HandleLosingForeground()
{
    emit losingForeground();
}

