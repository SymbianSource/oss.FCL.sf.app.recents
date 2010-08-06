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

#include "logsabstractmodel.h"
#include "logsdbconnector.h"
#include "logsevent.h"
#include "logslogger.h"
#include "logscall.h"
#include "logscontact.h"
#include "logsengdefs.h"
#include "logsmessage.h"
#include <hbicon.h>
#include <QStringBuilder>
#include <hbextendedlocale.h>
#include "logssystemtimeobserver.h"


Q_DECLARE_METATYPE(LogsEvent *)
Q_DECLARE_METATYPE(LogsCall *)
Q_DECLARE_METATYPE(LogsMessage *)
Q_DECLARE_METATYPE(LogsContact *)

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsAbstractModel::LogsAbstractModel() : QAbstractListModel(), mDbConnector(0)
{
    LOGS_QDEBUG( "logs [ENG] -> LogsAbstractModel::LogsAbstractModel()" )
    mSystemTimeObserver = new LogsSystemTimeObserver(this);
    connect(mSystemTimeObserver, SIGNAL(timeFormatChanged()),
            this, SLOT(updateModel()));
    LOGS_QDEBUG( "logs [ENG] <- LogsAbstractModel::LogsAbstractModel()" )
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsAbstractModel::~LogsAbstractModel()
{
    LOGS_QDEBUG( "logs [ENG] -> LogsAbstractModel::~LogsAbstractModel()" )

    qDeleteAll( mIcons );
    
    LOGS_QDEBUG( "logs [ENG] <- LogsAbstractModel::~LogsAbstractModel()" )
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsContact* LogsAbstractModel::createContact(const QString& number)
{
    LogsContact* contact = new LogsContact(number, *mDbConnector);
    connect( contact, SIGNAL(saveCompleted(bool)), this, SLOT(contactSavingCompleted(bool)) );
    return contact;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
int LogsAbstractModel::predictiveSearchStatus()
{
    if ( !mDbConnector ){
        return -1;
    }
    return mDbConnector->predictiveSearchStatus();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
int LogsAbstractModel::setPredictiveSearch(bool enabled)
{
    return doSetPredictiveSearch(enabled);
}


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
bool LogsAbstractModel::isCommunicationPossible(const LogsEvent& event) const
{
    return ( !event.isRemotePartyPrivate() && !event.isRemotePartyUnknown() );
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsAbstractModel::contactSavingCompleted(bool modified)
{
    Q_UNUSED(modified);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsAbstractModel::updateModel()
{
    LOGS_QDEBUG( "logs [ENG] -> LogsAbstractModel::updateModel(), SYSTEM TIME CHANGED!" )
    //reset();
    if (rowCount()) {
        emit dataChanged(createIndex(0,0), createIndex(rowCount(),0));
    }
    LOGS_QDEBUG( "logs [ENG] <- LogsAbstractModel::updateModel()" )
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
QVariant LogsAbstractModel::doGetData(int role, const LogsModelItemContainer& item) const
{
    if ( role == RoleFullEvent ) {
        QVariant var = qVariantFromValue(item.event());
        return var;
    } else if ( role == RoleCall ) { 
        return createCall(item);
    } else if ( role == RoleMessage ) {
        return createMessage(item);   
    } else if ( role == RoleContact ) {
        return createContact(item);
    }
    return QVariant();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
QVariant LogsAbstractModel::createCall(const LogsModelItemContainer& item) const
{
    LOGS_QDEBUG( "logs [ENG] -> LogsAbstractModel::createCall()" )
    LogsEvent* event = item.event();
    if ( !event ){
        return QVariant();
    }
    LogsCall* logscall = new LogsCall(*event);
    if (!logscall->isAllowedCallType() || !isCommunicationPossible(*event)) {
        delete logscall;
        logscall = 0;
    }
    QVariant var = qVariantFromValue(logscall);
    return var;         
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
QVariant LogsAbstractModel::createMessage(const LogsModelItemContainer& item) const
{
    LOGS_QDEBUG( "logs [ENG] -> LogsAbstractModel::createMessage()" )
    LogsEvent* event = item.event();
    if ( !event ){
        return QVariant();
    }
    LogsMessage* logsMessage = new LogsMessage(*event);
    if (!logsMessage->isMessagingAllowed() || !isCommunicationPossible(*event)) {
        delete logsMessage;
        logsMessage = 0;
    }
    QVariant var = qVariantFromValue(logsMessage);
    return var;      
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
QVariant LogsAbstractModel::createContact(const LogsModelItemContainer& item) const
{
    LOGS_QDEBUG( "logs [ENG] -> LogsAbstractModel::createContact()" )
    LogsEvent* event = item.event();
    if ( !event ){
        return QVariant();
    }
    Q_ASSERT(mDbConnector);
    LogsContact* logsContact = new LogsContact(*event, *mDbConnector);
    if ( !logsContact->isContactRequestAllowed() || !isCommunicationPossible(*event) ) {
        delete logsContact;
        logsContact = 0;
    }
    QVariant var = qVariantFromValue(logsContact);
    return var;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
int LogsAbstractModel::doSetPredictiveSearch(bool enabled){
    if ( !mDbConnector ){
        return -1;
    }
    return mDbConnector->setPredictiveSearch(enabled);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
QString LogsAbstractModel::directionIconName(const LogsEvent& event)
{
    QString direction;

    if ( event.direction() == LogsEvent::DirIn ) {
        if ( event.eventType() == LogsEvent::TypeVoiceCall ) {
            direction = logsReceivedVoiceCallIconId;
        } else if ( event.eventType() == LogsEvent::TypeVideoCall ) {
            direction = logsReceivedVideoCallIconId;
        } else if ( event.eventType() == LogsEvent::TypeVoIPCall ) {
            direction = logsReceivedVoipCallIconId;
        }
    } else if ( event.direction() == LogsEvent::DirOut ) {
        if ( event.eventType() == LogsEvent::TypeVoiceCall ) {
            direction = logsDialledVoiceCallIconId;
        } else if ( event.eventType() == LogsEvent::TypeVideoCall ) {
            direction = logsDialledVideoCallIconId;
        } else if ( event.eventType() == LogsEvent::TypeVoIPCall ) {
            direction = logsDialledVoipCallIconId;
        }
    } else  if ( event.direction() == LogsEvent::DirMissed ) {
        if ( event.eventType() == LogsEvent::TypeVoiceCall ) {
        	if (!event.isSeenLocally() ){
        		direction = logsMissedVoiceCallUnseenIconId;
        	}
        	else {
        		direction = logsMissedVoiceCallIconId;
        	}
            
        } else if ( event.eventType() == LogsEvent::TypeVideoCall ) {
        	if (!event.isSeenLocally() ){
        		direction = logsMissedVideoCallUnseenIconId;
        	}
        	else {
        		direction = logsMissedVideoCallIconId;
        	}
        } else if ( event.eventType() == LogsEvent::TypeVoIPCall ) {
        	if (!event.isSeenLocally() ){
        		direction = logsMissedVoipCallUnseenIconId;
        	}
        	else {
        		direction = logsMissedVoipCallIconId;
        	}
        }
    }    
    
    return direction;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
QString LogsAbstractModel::typeIconName(const LogsEvent& event)
{
    QString brand;
    if ( event.eventType() == LogsEvent::TypeVoiceCall ) {
        brand = QString(logsVoiceCallIconId);
    } else if ( event.eventType() == LogsEvent::TypeVideoCall ) {
        brand = QString(logsVideoCallIconId);
    } else if ( event.eventType() == LogsEvent::TypeVoIPCall ) {
        brand = QString(logsVoipCallIconId);
    }
    return brand;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsAbstractModel::getDecorationData(const LogsEvent& event, 
        QList<QVariant>& iconList) const
{
    HbIcon emptyIcon;
    iconList.append( *mIcons.value(directionIconName(event), &emptyIcon) );
    
    // TODO: alternative service icon
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsDbConnector* LogsAbstractModel::dbConnector()
{
    return mDbConnector;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
QString LogsAbstractModel::dateAndTimeString(const QDateTime& dateTime) const
{
    HbExtendedLocale locale = HbExtendedLocale::system();
    QString dateTimeString =
                locale.format(dateTime.date(), r_qtn_date_usual_with_zero)
                % QChar(' ')
                % locale.format(dateTime.time(), r_qtn_time_usual_with_zero);
    return dateTimeString;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
QString LogsAbstractModel::durationString(const QTime& time) const
{
    return HbExtendedLocale::system().format(time, r_qtn_time_durat_long_with_zero);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsModelItemContainer::LogsModelItemContainer(LogsEvent* event) : mEvent(event)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsModelItemContainer::~LogsModelItemContainer()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsEvent* LogsModelItemContainer::event() const
{
    return mEvent;
}

