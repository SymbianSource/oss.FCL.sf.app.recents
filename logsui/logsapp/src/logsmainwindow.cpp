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

#include "logsmainwindow.h"
#include "logslogger.h"
#include <QKeyEvent>
#include <QApplication>
#include <xqserviceutil.h>

// -----------------------------------------------------------------------------
// LogsMainWindow::LogsMainWindow
// -----------------------------------------------------------------------------
//
LogsMainWindow::LogsMainWindow() 
    : HbMainWindow(), mForeground(false), mLocaleChanged(false)
{
    qApp->installEventFilter(this);
}

// -----------------------------------------------------------------------------
// LogsMainWindow::~LogsMainWindow
// -----------------------------------------------------------------------------
//
LogsMainWindow::~LogsMainWindow()
{
}

// -----------------------------------------------------------------------------
// LogsMainWindow::sendAppToBackground
// -----------------------------------------------------------------------------
//
void LogsMainWindow::sendAppToBackground()
{
    LOGS_QDEBUG( "logs [UI] -> LogsMainWindow::sendAppToBackground" );
    XQServiceUtil::toBackground(true);
    LOGS_QDEBUG( "logs [UI] <- LogsMainWindow::sendAppToBackground" );
}

// -----------------------------------------------------------------------------
// LogsMainWindow::bringAppToForeground
// -----------------------------------------------------------------------------
//
void LogsMainWindow::bringAppToForeground()
{
    LOGS_QDEBUG( "logs [UI] -> LogsMainWindow::bringAppToForeground" );
    show();
    raise();
    LOGS_QDEBUG( "logs [UI] <- LogsMainWindow::bringAppToForeground" );
}

// -----------------------------------------------------------------------------
// LogsMainWindow::isForeground
// -----------------------------------------------------------------------------
//
bool LogsMainWindow::isForeground() const
{
    return mForeground;
}

// -----------------------------------------------------------------------------
// LogsMainWindow::keyPressEvent
// -----------------------------------------------------------------------------
//
void LogsMainWindow::keyPressEvent( QKeyEvent *event )
{
    LOGS_QDEBUG_2( "LogsMainWindow::keyPressEvent, key", event->key() );
    if ( event->key() == Qt::Key_Call || event->key() == Qt::Key_Yes ) {
        // Handling at window level seems to be only way to avoid other
        // applications to handle call key as well.
        emit callKeyPressed();
        event->accept();
        return;
    }
    HbMainWindow::keyPressEvent(event);
}

// -----------------------------------------------------------------------------
// LogsMainWindow::eventFilter
// -----------------------------------------------------------------------------
//
bool LogsMainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::ApplicationActivate) {
        LOGS_QDEBUG( "logs [UI] -> eventFilter(), QEvent::ApplicationActivate" );
        mForeground = true;
        if (mLocaleChanged) {
            LOGS_QDEBUG( "logs [UI] -> locale changed when we were on BG" );
            emit localeChanged();
            mLocaleChanged = false;
        }
        emit appGainedForeground();
    } else if (event->type() == QEvent::ApplicationDeactivate) {
        LOGS_QDEBUG( "logs [UI] -> eventFilter(), QEvent::ApplicationDeactivate" );
        mForeground = false;
    } else if (event->type() == QEvent::LocaleChange) {
        if (mForeground) {
            emit localeChanged();
        } else {
            mLocaleChanged = true;
        }
    }
    
    return HbMainWindow::eventFilter(obj,event);
}
