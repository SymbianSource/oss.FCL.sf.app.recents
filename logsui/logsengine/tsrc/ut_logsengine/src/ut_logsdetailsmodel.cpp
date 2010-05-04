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
#include "ut_logsdetailsmodel.h"
#include "logsdetailsmodel.h"
#include "logsevent.h"
#include "logsdbconnector.h"
#include "logscall.h"
#include "logscontact.h"
#include "logsmessage.h"
#include "logseventdata.h"
#include "qtcontacts_stubs_helper.h"
#include "logsdbconnector_stub_helper.h"
#include <hblineedit.h>
#include <hbglobal.h>
#include <QtTest/QtTest>

Q_DECLARE_METATYPE(LogsCall *)
Q_DECLARE_METATYPE(LogsMessage *)
Q_DECLARE_METATYPE(LogsContact *)

const QString testEmpty = "";
const QString testDetailsRemoteParty = "Remoteman";
const QString testDetailsRemoteNum = "12345566";
const QString testDetailsRemoteInfo = "Remoteman";
QDateTime testDetailsDateAndTime;
const LogsEvent::LogsDirection testDetailsDirection = LogsEvent::DirIn;
const LogsEvent::LogsEventType testDetailsEventType = LogsEvent::TypeVoiceCall;
const int testDetailsDuration = 2000;
const QString testDetailsNumber = "+4152800";

void UT_LogsDetailsModel::initTestCase()
{
    
    mDbConnector = new LogsDbConnector(mEvents);
}

void UT_LogsDetailsModel::cleanupTestCase()
{
    delete mDbConnector;
}


void UT_LogsDetailsModel::init()
{
    testDetailsDateAndTime.setTime_t( 3000 );
    
    LogsEvent event;
    event.setRemoteParty( testDetailsRemoteParty );
    event.setNumber( testDetailsRemoteNum );
    event.setTime( testDetailsDateAndTime );
    event.setDirection( testDetailsDirection );
    event.setEventType( testDetailsEventType );
    event.setDuration( testDetailsDuration );
    mModel = new LogsDetailsModel(*mDbConnector, event);
}

void UT_LogsDetailsModel::cleanup()
{
    delete mModel;
    mModel = 0;
}

void UT_LogsDetailsModel::testConstructor()
{
    QVERIFY( mModel );
}

void UT_LogsDetailsModel::testRowCount()
{
    QVERIFY( mModel->rowCount(QModelIndex()) == 5 );
}

void UT_LogsDetailsModel::testgetNumberToClipboard()
{
    HbLineEdit *cliptmp2 = new HbLineEdit(" ");
    
    cliptmp2->setText(" ");
    cliptmp2->selectAll();
    cliptmp2->copy();
     
    mModel->getNumberToClipboard();
    
    cliptmp2->setText(" ");
    cliptmp2->paste();
    
    QVERIFY( cliptmp2->text() == testDetailsRemoteNum  );
    delete cliptmp2;
}

void UT_LogsDetailsModel::testData()
{
    QVERIFY( mModel->data(QModelIndex(), Qt::DisplayRole).isNull() );
    
    // Test remote party info
    QStringList display = mModel->data(mModel->index(0), Qt::DisplayRole).toStringList();
    QVERIFY( display.count() == 2 );
    QVERIFY( display.at(1) == testDetailsRemoteNum );  
    QList<QVariant> decoration = mModel->data(mModel->index(0), Qt::DecorationRole).toList();
    QVERIFY( decoration.count() == 1 );
    
    // Test date and time
    display = mModel->data(mModel->index(1), Qt::DisplayRole).toStringList();
    QVERIFY( display.count() == 2 );
    QVERIFY( display.at(1) == testDetailsDateAndTime.toTimeSpec(Qt::LocalTime).toString() ); 
    decoration = mModel->data(mModel->index(1), Qt::DecorationRole).toList();
    QVERIFY( decoration.count() == 1 );
    
    // Test call direction
    display = mModel->data(mModel->index(2), Qt::DisplayRole).toStringList();
    QVERIFY( display.count() == 2 );
    QVERIFY( display.at(1) == mModel->mEvent->directionAsString() );
    decoration = mModel->data(mModel->index(2), Qt::DecorationRole).toList();
    QVERIFY( decoration.count() == 1 );
    
    // Test call type
    display = mModel->data(mModel->index(3), Qt::DisplayRole).toStringList();
    QVERIFY( display.count() == 2 );
    QVERIFY( display.at(1) == mModel->mEvent->typeAsString() );
    decoration = mModel->data(mModel->index(3), Qt::DecorationRole).toList();
    QVERIFY( decoration.count() == 1 );
    
    // Test call duration
    display = mModel->data(mModel->index(4), Qt::DisplayRole).toStringList();
    QVERIFY( display.count() == 2 );
    QTime n(0,0,0);
    QTime t = n.addSecs(testDetailsDuration);
    QVERIFY( display.at(1) == t.toString("hh:mm:ss") );
    decoration = mModel->data(mModel->index(4), Qt::DecorationRole).toList();
    QVERIFY( decoration.count() == 1 );
    
    // Test call, event type supports call
    QVariant callData = mModel->data(mModel->index(0), LogsDetailsModel::RoleCall);
    LogsCall *call = qVariantValue<LogsCall *>( callData );
    QVERIFY( call );
    delete call;
    
    // Test call, event type does not support call
    mModel->mEvent->setEventType(LogsEvent::TypeUndefined);
    QVariant callData2 = mModel->data(mModel->index(0), LogsDetailsModel::RoleCall);
    LogsCall *call2 = qVariantValue<LogsCall *>( callData2 );
    QVERIFY( !call2 );
    
    // Test message
    QVariant messageData = mModel->data(mModel->index(0), LogsDetailsModel::RoleMessage);
    LogsMessage* message = qVariantValue<LogsMessage *>( messageData );
    QVERIFY( message != 0 );
    delete message;
    LogsEventData* eventData = new LogsEventData;
    eventData->mRemoteUrl = "test@1.2.3.4";
    mModel->mEvent->setLogsEventData( eventData );
    QVariant messageData2 = mModel->data(mModel->index(0), LogsDetailsModel::RoleMessage);
    LogsMessage* message2 = qVariantValue<LogsMessage *>( messageData2 );
    QVERIFY( message2 == 0 );
    
    // Contact supported
    QVERIFY( mModel->mEvent->getNumberForCalling().length() > 0 );
    QVariant contactData = mModel->data(mModel->index(0), LogsDetailsModel::RoleContact);
    LogsContact *contact = qVariantValue<LogsContact *>( contactData );
    QVERIFY( contact );
    delete contact;
    
    // Contact not supported
    mModel->mEvent->setNumber("");
    mModel->mEvent->setLogsEventData( 0 );
    contactData = mModel->data(mModel->index(0), LogsModel::RoleContact);
    contact = qVariantValue<LogsContact *>( contactData );
    QVERIFY( !contact );
}

void UT_LogsDetailsModel::testHeaderData()
{
    QCOMPARE(mModel->headerData(0, Qt::Vertical).toString(),
             testDetailsRemoteInfo);
    QVERIFY(mModel->headerData(0, Qt::Vertical, Qt::DecorationRole).isNull());
}

void UT_LogsDetailsModel::testgetRemoteUri()
{
    LogsEvent event;
    QVERIFY( mModel->getRemoteUri(event) == QString("") );
    LogsEventData* eventData = new LogsEventData;
    eventData->mRemoteUrl = "test@1.2.3.4";
    mModel->mEvent->setLogsEventData( eventData );
    QVERIFY( mModel->getRemoteUri(*mModel->mEvent) == QString("test@1.2.3.4") );
}

void UT_LogsDetailsModel::testInitTextsAndIcons()
{
    //No VoIP call,no remote url or local url and contactname
    testDetailsDateAndTime.setTime_t( 3000 );
    LogsEvent event;
    event.setRemoteParty( testDetailsRemoteParty );
    event.setNumber( testDetailsRemoteNum );
    event.setTime( testDetailsDateAndTime );
    event.setDirection( testDetailsDirection );
    event.setEventType( testDetailsEventType );
    event.setDuration( testDetailsDuration );
    LogsDetailsModel* model = new LogsDetailsModel(*mDbConnector, event);
    QVERIFY(model->mDetailIcons.count() == 5);
    QVERIFY(model->mDetailTexts.count() == 5);
    delete model;
    model = 0;
    
    //No VoIP call,no remote url or local url and no contactname
    testDetailsDateAndTime.setTime_t( 3000 );
    event.setRemoteParty( testEmpty );
    event.setNumber( testDetailsRemoteNum );
    event.setTime( testDetailsDateAndTime );
    event.setDirection( testDetailsDirection );
    event.setEventType( testDetailsEventType );
    event.setDuration( testDetailsDuration );
    model = new LogsDetailsModel(*mDbConnector, event);
    QVERIFY(model->mDetailIcons.count() == 4);
    QVERIFY(model->mDetailTexts.count() == 4);
    delete model;
    model = 0;
    
    //VoIP call,only remote url,no contact name
    testDetailsDateAndTime.setTime_t( 3000 );
    event.setRemoteParty( testEmpty );
    event.setNumber( testEmpty );
    event.setTime( testDetailsDateAndTime );
    event.setDirection( testDetailsDirection );
    event.setEventType( testDetailsEventType );
    event.setDuration( testDetailsDuration );
    LogsEventData* eventData = new LogsEventData;
    eventData->mRemoteUrl = "tester@100.200.300.400";
    event.setLogsEventData( eventData );
    model = new LogsDetailsModel(*mDbConnector, event);
    QVERIFY(model->mDetailIcons.count() == 4);
    QVERIFY(model->mDetailTexts.count() == 4);
    delete model;
    model = 0;
    
    //VoIP call,only remote url, contact name
    testDetailsDateAndTime.setTime_t( 3000 );
    event.setRemoteParty( testDetailsRemoteParty );
    event.setNumber( testEmpty );
    event.setTime( testDetailsDateAndTime );
    event.setDirection( testDetailsDirection );
    event.setEventType( testDetailsEventType );
    event.setDuration( testDetailsDuration );
    eventData = new LogsEventData;
    eventData->mRemoteUrl = "tester@100.200.300.400";
    event.setLogsEventData( eventData );
    model = new LogsDetailsModel(*mDbConnector, event);
    QVERIFY(model->mDetailIcons.count() == 5);
    QVERIFY(model->mDetailTexts.count() == 5);
    delete model;
    model = 0;

    //VoIP call,remote uri and local uri, contact name
    testDetailsDateAndTime.setTime_t( 3000 );
    event.setRemoteParty( testDetailsRemoteParty );
    event.setNumber( testEmpty );
    event.setTime( testDetailsDateAndTime );
    event.setDirection( testDetailsDirection );
    event.setEventType( testDetailsEventType );
    event.setDuration( testDetailsDuration );
    eventData = new LogsEventData;
    eventData->mRemoteUrl = "tester@100.200.300.400";
    eventData->mLocalUrl = "caller@100.200.300.400";
    event.setLogsEventData( eventData );
    model = new LogsDetailsModel(*mDbConnector, event);
    QVERIFY(model->mDetailIcons.count() == 5);
    QVERIFY(model->mDetailTexts.count() == 5);
    delete model;
    model = 0;
    
    // Missed call, duration is not shown
    // No VoIP call,no remote url or local url and contactname
    event.setRemoteParty( testDetailsRemoteParty );
    event.setNumber( testDetailsRemoteNum );
    event.setTime( testDetailsDateAndTime );
    event.setEventType( testDetailsEventType );
    event.setDirection( LogsEvent::DirMissed );
    model = new LogsDetailsModel(*mDbConnector, event);
    QVERIFY(model->mDetailIcons.count() == 4);
    QVERIFY(model->mDetailTexts.count() == 4);
    delete model;
    model = 0;
}

void UT_LogsDetailsModel::testInitUnseenMissed()
{
    LogsDbConnectorStubHelper::reset();
    testDetailsDateAndTime.setTime_t( 3000 );
    QString dateAndTimeRowHeading;
    
    // No duplicates
    LogsEvent event;
    event.setRemoteParty( testDetailsRemoteParty );
    event.setNumber( testDetailsRemoteNum );
    event.setTime( testDetailsDateAndTime );
    event.setDirection( LogsEvent::DirMissed );
    event.setEventType( testDetailsEventType );
    event.setDuration( testDetailsDuration );
    LogsDetailsModel* model = new LogsDetailsModel(*mDbConnector, event);
    QVERIFY(model->mDetailIcons.count() == 4);
    QVERIFY(model->mDetailTexts.count() == 4);
    QVERIFY(LogsDbConnectorStubHelper::lastCalledFunction().isEmpty());
    dateAndTimeRowHeading = model->mDetailTexts.at(1).at(0);
    delete model;
    model = 0;
    
    // Already read
    event.setIsRead(true);
    model = new LogsDetailsModel(*mDbConnector, event);
    QVERIFY(model->mDetailIcons.count() == 4);
    QVERIFY(model->mDetailTexts.count() == 4);
    QVERIFY(LogsDbConnectorStubHelper::lastCalledFunction().isEmpty());
    delete model;
    model = 0;
    
    // Not read and duplicates exist
    event.setIsRead(false);
    event.setDuplicates(3);
    model = new LogsDetailsModel(*mDbConnector, event);
    QVERIFY(model->mDetailIcons.count() == 4);
    QVERIFY(model->mDetailTexts.count() == 4);
    QVERIFY(LogsDbConnectorStubHelper::lastCalledFunction() == "readDuplicates" );
    
    // Reading duplicates completes
    QSignalSpy spy( model, SIGNAL(modelReset()) );
    LogsEvent* dup1 = new LogsEvent;
    model->mDbConnector->mDuplicatedEvents.append(dup1);
    LogsEvent* dup2 = new LogsEvent;
    model->mDbConnector->mDuplicatedEvents.append(dup2);
    model->duplicatesRead();
    QVERIFY(model->mDetailIcons.count() == 6);
    QVERIFY(model->mDetailTexts.count() == 6);
    // When having multiple date and time items, first item has different heading than others
    QVERIFY( model->mDetailTexts.at(1).at(0) != dateAndTimeRowHeading );
    QVERIFY( model->mDetailTexts.at(5).at(0) == dateAndTimeRowHeading );
    delete model;
    model = 0;
    
}

void UT_LogsDetailsModel::testGetHeaderData()
{
    // No name or number
    LogsEvent event;
    QVERIFY( mModel->getHeaderData(event) == hbTrId("txt_dial_dblist_call_id_val_unknown_number") );

    // No name
    QString num("+12345555");
    event.setNumber(num);
    QVERIFY( mModel->getHeaderData(event) == num );
    
    // No number
    QString remote("Souuu");
    event.setRemoteParty(remote);
    event.setNumber("");
    QVERIFY( mModel->getHeaderData(event) == remote );
    
    // Both
    event.setNumber(num);
    QVERIFY( mModel->getHeaderData(event) == remote );
    
    // Only remote url
    event.setNumber("");
    event.setRemoteParty("");
    LogsEventData* eventData = new LogsEventData;
    eventData->mRemoteUrl = "test@1.2.3.4";
    event.setLogsEventData( eventData );
    QVERIFY( mModel->getHeaderData(event) == eventData->mRemoteUrl );
}


void UT_LogsDetailsModel::testGetCallerId()
{
    // No name or number
    LogsEvent event;
    QVERIFY( mModel->getCallerId(event) == QString("") );

    // No name
    QString num("+12345555");
    event.setNumber(num);
    QVERIFY( mModel->getCallerId(event) == QString("") );
    
    // No number
    QString remote("Souuu");
    event.setRemoteParty(remote);
    event.setNumber("");
    QVERIFY( mModel->getCallerId(event) == QString("") );
    
    // Both
    event.setNumber(num);
    QVERIFY( mModel->getCallerId(event) == num );
    
    //Only number
    event.setRemoteParty("");
    QVERIFY( mModel->getCallerId(event) == QString("") );
    
    // Only remote url
    event.setNumber("");
    event.setRemoteParty("contactname");
    LogsEventData* eventData = new LogsEventData;
    eventData->mRemoteUrl = "test@1.2.3.4";
    event.setLogsEventData( eventData );
    QVERIFY( mModel->getCallerId(event) == eventData->mRemoteUrl );
}

void UT_LogsDetailsModel::testCreateContact()
{
    LogsEvent event;
    LogsModelItemContainer item(&event);
    LogsContact* contact = qVariantValue<LogsContact*>(mModel->createContact(item));
    QVERIFY( !contact );
    
    LogsEvent event2;
    event2.setNumber("2145");
    LogsModelItemContainer item2(&event2);
    contact = qVariantValue<LogsContact*>(mModel->createContact(item2));
    QVERIFY( contact );
    delete contact;
}

void UT_LogsDetailsModel::testContactActionCompleted()
{
    mModel->mDetailTexts.clear();
    mModel->contactActionCompleted(false);
    QVERIFY( mModel->mDetailTexts.count() == 0 );
    
    // No match
    mModel->mEvent->setRemoteParty( "" );
    mModel->mEvent->setNumber( "123445" );
    mModel->contactActionCompleted(true);
    QVERIFY( mModel->mDetailTexts.count() == 0 );
    QVERIFY( mModel->mEvent->remoteParty().length() == 0 );
    
    // Match
    QtContactsStubsHelper::setContactNames("first", "last");
    mModel->mEvent->setRemoteParty( "" );
    mModel->mEvent->setNumber( "11112222" );
    mModel->contactActionCompleted(true);
    QVERIFY( mModel->mDetailTexts.count() > 0 );
    QVERIFY( mModel->mEvent->remoteParty().length() > 0 );
}
