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
#include "logsmessage.h"
#include "logslogger.h"
#include "logseventdata.h"
#include <xqaiwdecl.h>
#include <xqappmgr.h>

//SYSTEM

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsMessage::LogsMessage(LogsEvent& event)
    :QObject(), mIsAllowed(false), mContactId(0), mAiwRequest(0)
{
    if ( event.logsEventData() && !event.logsEventData()->isCsCompatible() ){
        LOGS_QDEBUG( "logs [ENG]    LogsMessage::LogsMessage, not CS compatible" )
        mIsAllowed = false;
    } else {
        mIsAllowed = true;
        mNumber = event.getNumberForCalling();
        mContactId = event.contactLocalId();
        mDisplayName = event.remoteParty();
    }
    
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsMessage::LogsMessage(unsigned int contactId, const QString& number,
	 const QString& displayName)
    :QObject(), mIsAllowed(false), mContactId(0), mAiwRequest(0)
{
    if ( number.length() == 0 ){
        LOGS_QDEBUG( "logs [ENG]    LogsMessage::LogsMessage, not CS compatible" )
        mIsAllowed = false;
    } else {
        mIsAllowed = true;
        mNumber = number;
        mContactId = contactId;
        mDisplayName = displayName;
    }
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
LogsMessage::~LogsMessage()
{
    LOGS_QDEBUG( "logs [ENG] <-> LogsMessage::~LogsMessage()" )
    delete mAiwRequest;
}
    
// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
//
bool LogsMessage::isMessagingAllowed()
{
    return mIsAllowed;
}    

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
bool LogsMessage::sendMessage()
{
    LOGS_QDEBUG( "logs [ENG] -> LogsMessage::sendMessage()" )
    cancelServiceRequest();
    bool sending(false);
    XQApplicationManager appMng;
    //TODO: change request to be embedded once messaging team has implemented
    //handling of clientDisconnected() signal
    mAiwRequest = appMng.create(XQI_MESSAGE_SEND, XQOP_MESSAGE_SEND_WITH_ID, false); // embedded 
    if (mAiwRequest) {
        connect(mAiwRequest, SIGNAL(requestOk(const QVariant&)), 
                this, SLOT(handleRequestCompleted(const QVariant&)));
        connect(mAiwRequest, SIGNAL(requestError(int,const QString&)), 
                this, SLOT(handleError(int,const QString&)));

        QList<QVariant> arguments;
        arguments.append(QVariant(mNumber));
        arguments.append(QVariant(mContactId));
        arguments.append(QVariant(mDisplayName));
        mAiwRequest->setArguments(arguments);
        mAiwRequest->setSynchronous(false);
        sending = mAiwRequest->send();
    }
    return sending;
}


// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void LogsMessage::cancelServiceRequest()
{
    LOGS_QDEBUG( "logs [ENG] -> LogsMessage::cancelServiceRequest()" )
    delete mAiwRequest;
    mAiwRequest = 0;
    LOGS_QDEBUG( "logs [ENG] <- LogsMessage::cancelServiceRequest()" )
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
LogsMessage* LogsMessage::sendMessageToNumber(
        const QString& number, const QString& displayName, unsigned int contactId)
{
    LOGS_QDEBUG( "logs [ENG] -> LogsMessage::sendMessageToNumber()" )
    LogsMessage* message = new LogsMessage(contactId, number, displayName);
    message->sendMessage();
    return message;
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void LogsMessage::handleRequestCompleted(const QVariant& result)
{
    LOGS_QDEBUG( "logs [ENG] <-> LogsMessage::handleRequestCompleted()" )
    Q_UNUSED(result)
    cancelServiceRequest();
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void LogsMessage::handleError(int errorCode, const QString& errorMessage)
{
    LOGS_QDEBUG_4( "logs [ENG] <-> LogsMessage::handleError(): ", errorCode,
            " ,msg: ", errorMessage)
    Q_UNUSED(errorCode)
    Q_UNUSED(errorMessage)
    cancelServiceRequest();
}

// End of file

