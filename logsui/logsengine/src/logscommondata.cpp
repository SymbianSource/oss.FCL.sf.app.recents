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
#include "logscommondata.h"
#include "logslogger.h"
#include "logsconfigurationparams.h"
#include "logsengdefs.h"

//SYSTEM
#include <xqsettingsmanager.h>
#include <qcontactmanager.h>
#include <LogsDomainCRKeys.h>
#include <QCoreApplication>
#include <hbinstance.h>
#include <hbcolorscheme.h>

static LogsCommonData* mLogsCommonInstance = 0;

// CONSTANTS

// Telephony Configuration API
// Keys under this category are used in defining telephony configuration.
const TUid logsTelConfigurationCRUid = {0x102828B8};

// Amount of digits to be used in contact matching.
// This allows a customer to variate the amount of digits to be matched.
const TUint32 logsTelMatchDigits = 0x00000001;

const int logsNotInitialized = -1;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsCommonData::LogsCommonData() : QObject(),
    mContactManager(0), mMaxReadSize(-1), 
    mMaxReadSizeDir(LogsEvent::DirUndefined), 
    mSettingsManager(new XQSettingsManager()),
    mMatchLen(logsDefaultMatchLength),
    mPredictiveSearchStatus(logsNotInitialized),
    mCompressed(false),
    mPendingThemeChange(false)
{
    LOGS_QDEBUG( "logs [ENG] -> LogsCommonData::LogsCommonData()" )
    // If client has created only coreapp, don't treat as GUI client
    // since many things will not work properly
    QApplication* app = qobject_cast<QApplication*>( qApp );
    mIsInGuiProcess = ( app != 0 );
    if ( mIsInGuiProcess ){
        LOGS_QDEBUG( "logs [ENG]    Full GUI" )
        updateHighlightColor();
        connect( hbInstance->theme(), SIGNAL ( changeFinished() ),
                 this, SLOT ( handleThemeChange()));
    }
    LOGS_QDEBUG( "logs [ENG] <- LogsCommonData::LogsCommonData()" )
}
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsCommonData::~LogsCommonData()
{
    LOGS_QDEBUG( "logs [ENG] -> LogsCommonData::~LogsCommonData()" )
    delete mContactManager;
    delete mSettingsManager;
    LOGS_QDEBUG( "logs [ENG] <- LogsCommonData::~LogsCommonData()" )
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsCommonData& LogsCommonData::getInstance() 
{ 
    if ( !mLogsCommonInstance ){
        mLogsCommonInstance = new LogsCommonData;
    }
    return *mLogsCommonInstance; 
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsCommonData::freeCommonData()
{
    delete mLogsCommonInstance;
    mLogsCommonInstance = 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
QContactManager& LogsCommonData::contactManager()
{
    if (!mContactManager){
        LOGS_QDEBUG( "logs [ENG] -> LogsCommonData::contactManager(), create mgr" )
        mContactManager = new QContactManager("symbian");
        LOGS_QDEBUG( "logs [ENG] <- LogsCommonData::contactManager()" )
    }
    return *mContactManager;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsCommonData::configureReadSize(int maxSize, LogsEvent::LogsDirection dir)
{
    mMaxReadSize = maxSize;
    mMaxReadSizeDir = dir;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
int LogsCommonData::maxReadSize() const
{
    return mMaxReadSize;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsEvent::LogsDirection LogsCommonData::maxReadSizeDirection() const
{
    return mMaxReadSizeDir;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
int LogsCommonData::updateConfiguration(const LogsConfigurationParams& params)
{
    mConfiguration = params;
    return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsConfigurationParams& LogsCommonData::currentConfiguration()
{
    return mConfiguration;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
int LogsCommonData::telNumMatchLen() const
{
    return mMatchLen;
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
int LogsCommonData::predictiveSearchStatus()
{
    if (mPredictiveSearchStatus == logsNotInitialized) {
        mPredictiveSearchStatus = getPredictiveSearchStatus();
    }
    return mPredictiveSearchStatus;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
int LogsCommonData::getPredictiveSearchStatus()
{
    int status(logsNotInitialized);
    LOGS_QDEBUG( "logs [ENG] -> LogsCommonData::getPredictiveSearchStatus()" )
    XQSettingsKey key(XQSettingsKey::TargetCentralRepository, 
                      KCRUidLogs.iUid, 
                      KLogsPredictiveSearch);
    QVariant value = mSettingsManager->readItemValue(key, 
                                                     XQSettingsManager::TypeInt);
    if (!value.isNull()) {
        status = value.toInt();
    }
    LOGS_QDEBUG_2( "logs [ENG] <- LogsCommonData::getPredictiveSearchStatus(), status: ", status )
    return status;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
int LogsCommonData::setPredictiveSearch(bool enabled)
{
    int err(-1);
    LOGS_QDEBUG_2( "logs [ENG] -> LogsCommonData::setPredictiveSearch(), enabled: ", enabled )
    if (predictiveSearchStatus() != 0) {
        XQSettingsKey key(XQSettingsKey::TargetCentralRepository, 
                          KCRUidLogs.iUid, 
                          KLogsPredictiveSearch);
        int value = enabled ? 1 : 2;
        if (mSettingsManager->writeItemValue(key, QVariant(value))) {
            err = 0;
            mPredictiveSearchStatus = value;
        }
    }
    LOGS_QDEBUG_2( "logs [ENG] <- LogsCommonData::setPredictiveSearch(), err: ", err )
    return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
int LogsCommonData::clearMissedCallsCounter()
{
    int err(-1);
    XQSettingsKey key(XQSettingsKey::TargetCentralRepository, 
                      KCRUidLogs.iUid, 
                      KLogsNewMissedCalls);
    QVariant value = mSettingsManager->readItemValue(
                            key, XQSettingsManager::TypeInt);
    if (!value.isNull()) {
        err = 0;
        if (value.toInt() > 0) {
            err = mSettingsManager->writeItemValue(key, 0) ? 0 : -1;
        }
    }
    return err;    
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
bool LogsCommonData::getTelNumMatchLen(int& matchLen)
{
    bool ok(false);
    //use local manager, since normally this function is called only once, 
    //after that we can clean related cenrep handler resources
    XQSettingsManager manager;
    XQSettingsKey key(XQSettingsKey::TargetCentralRepository, 
                      logsTelConfigurationCRUid.iUid, 
                      logsTelMatchDigits);
    QVariant value = manager.readItemValue(
                             key, XQSettingsManager::TypeInt);
    if (!value.isNull()) {
        matchLen = value.toInt();
        mMatchLen = matchLen;
        ok = true;
    }
    return ok;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
bool LogsCommonData::isGui() const
{
    return mIsInGuiProcess;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const QString& LogsCommonData::highlightStart() const
{
    return mHighlightColorStart;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const QString& LogsCommonData::highlightEnd() const
{
    return mHighlightColorEnd;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsCommonData::refreshData()
{
    LOGS_QDEBUG( "logs [ENG] -> LogsCommonData::refreshData()" )
    bool prevCompressed = mCompressed;
    mCompressed = false;
    if ( isGui() && prevCompressed ){
        updateHighlightColor();
        if ( mPendingThemeChange ){
            LOGS_QDEBUG( "logs [ENG]    Theme changed while compressed" )
            emit commonDataChanged();
        }
    }
    mPendingThemeChange = false;
    LOGS_QDEBUG( "logs [ENG] <- LogsCommonData::refreshData()" )
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsCommonData::compressData()
{
    mCompressed = true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsCommonData::handleThemeChange()
{
    LOGS_QDEBUG_2( "logs [ENG] -> LogsCommonData::handleThemeChange(), compr", mCompressed )
    if ( mCompressed ){
        mPendingThemeChange = true;
    } else {
        updateHighlightColor();
        emit commonDataChanged();
    }
    LOGS_QDEBUG( "logs [ENG] <- LogsCommonData::handleThemeChange()" )
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsCommonData::updateHighlightColor()
{
    LOGS_QDEBUG( "logs [ENG] -> LogsCommonData::updateHighlightColor()" )
    QColor highlight = HbColorScheme::color("qtc_lineedit_marker_normal");
    QColor color = HbColorScheme::color("qtc_lineedit_selected");
                    
    mHighlightColorStart = QString("<span style=\"background-color: %1; color: %2\">")
            .arg(highlight.name().toUpper())
            .arg(color.name().toUpper());
    mHighlightColorEnd = "</span>";
    
    LOGS_QDEBUG_2( "logs [ENG] <- LogsCommonData::updateHighlightColor():", 
                   mHighlightColorStart )
}

   
// End of file


