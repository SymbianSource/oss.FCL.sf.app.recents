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
#include "ut_logsmessage.h"
#include "logsmessage.h"
#include "logsevent.h"
#include "logseventdata.h"
#include "qthighway_stub_helper.h"

#include <xqaiwdecl.h>
#include <QtTest/QtTest>

void UT_LogsMessage::initTestCase()
{
}

void UT_LogsMessage::cleanupTestCase()
{
}


void UT_LogsMessage::init()
{
    mLogsEvent = new LogsEvent();
    mLogsEvent->setNumber(QString::number(12345));
    mLogsEvent->setEventType(LogsEvent::TypeVoiceCall);
    mLogsMessage = new LogsMessage(*mLogsEvent);
}

void UT_LogsMessage::cleanup()
{
    delete mLogsMessage;
    mLogsMessage = 0;
    delete mLogsEvent;
    mLogsEvent = 0;
}

void UT_LogsMessage::testConstructor()
{
    QVERIFY( mLogsMessage );
    
    LogsMessage messageWithoutEvent( 2, "1234", "firstname" );
    QVERIFY( messageWithoutEvent.mContactId == 2 );
    QVERIFY( messageWithoutEvent.mNumber == "1234" );
    QVERIFY( messageWithoutEvent.mIsAllowed );
    
    LogsMessage messageWithoutEvent2( 2, "","" );
    QVERIFY( messageWithoutEvent2.mContactId == 0 );
    QVERIFY( messageWithoutEvent2.mNumber == "" );
    QVERIFY( !messageWithoutEvent2.mIsAllowed );
}

void UT_LogsMessage::testIsMessagingAllowed()
{
    QVERIFY( mLogsMessage->isMessagingAllowed() );
    
    // Not allowed as event not CS compatible
    LogsEvent event;
    LogsEventData* eventData = new LogsEventData;
    eventData->mRemoteUrl = "test@1.2.3.4";
    event.setLogsEventData( eventData );
    LogsMessage message(event);
    QVERIFY( !message.isMessagingAllowed() );
}

void UT_LogsMessage::testSendMessage()
{
    QtHighwayStubHelper::reset();
    QVERIFY( mLogsMessage->sendMessage() );
    QVERIFY( QtHighwayStubHelper::interface() == XQI_MESSAGE_SEND );
    QVERIFY( QtHighwayStubHelper::operation() == XQOP_MESSAGE_SEND_WITH_ID );
    QVERIFY( mLogsMessage->mAiwRequest );
        
    //Test handleError() and handleRequestComplete()
    mLogsMessage->handleError(0, QString());
    QVERIFY( !mLogsMessage->mAiwRequest );
    mLogsMessage->handleRequestCompleted(QVariant());
    QVERIFY( !mLogsMessage->mAiwRequest );
    
    // Sending message fails (aiwrequest creation fails)
    QtHighwayStubHelper::reset();
    QtHighwayStubHelper::setFailCreateAiwRequest(true);
    QVERIFY( !mLogsMessage->sendMessage() );
    QVERIFY( QtHighwayStubHelper::interface().isEmpty() );
    QVERIFY( QtHighwayStubHelper::operation().isEmpty() );
    QVERIFY( !mLogsMessage->mAiwRequest );
}

void UT_LogsMessage::testSendMessageToNumber()
{
    QtHighwayStubHelper::reset();
    LogsMessage* message(0);
    QVERIFY( message = mLogsMessage->sendMessageToNumber( "1234567" ) );
    QVERIFY( QtHighwayStubHelper::interface() == XQI_MESSAGE_SEND );
    QVERIFY( QtHighwayStubHelper::operation() == XQOP_MESSAGE_SEND_WITH_ID );
    delete message;

    QtHighwayStubHelper::reset();
    QVERIFY( message = mLogsMessage->sendMessageToNumber( "1234567", "name" ) );
    QVERIFY( QtHighwayStubHelper::interface() == XQI_MESSAGE_SEND );
    QVERIFY( QtHighwayStubHelper::operation() == XQOP_MESSAGE_SEND_WITH_ID );
    delete message;

    QtHighwayStubHelper::reset();
    QVERIFY( message = mLogsMessage->sendMessageToNumber( "4234567", "namef", 3 ) );
    QVERIFY( QtHighwayStubHelper::interface() == XQI_MESSAGE_SEND );
    QVERIFY( QtHighwayStubHelper::operation() == XQOP_MESSAGE_SEND_WITH_ID );
    delete message;    
}
