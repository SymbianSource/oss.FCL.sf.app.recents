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

#include <QContactManager.h>
#include <QContactDetailFilter.h>
#include <QContactPhoneNumber.h>
#include <QSharedDataPointer>
#include <QContactName.h>
#include <QContactDetail.h>

#include <QSharedData>
#include <QContactAvatar.h>

#include "qtcontacts_stubs.h"

ContactQueryResults* ContactQueryResults::mInstance = 0;

// -----------------------------------------------------------------------------
// ContactsDB::instance()
// -----------------------------------------------------------------------------
//
ContactQueryResults* ContactQueryResults::instance()
{
    if ( !mInstance ) {
        mInstance = new ContactQueryResults();
    }
    return mInstance;
}

// -----------------------------------------------------------------------------
// ContactsDB::deleteInstance()
// -----------------------------------------------------------------------------
//
void ContactQueryResults::deleteInstance()
{
    delete mInstance;
    mInstance = 0;
}

// -----------------------------------------------------------------------------
// ContactsDB::~ContactQueryResults()
// -----------------------------------------------------------------------------
//
ContactQueryResults::~ContactQueryResults()
{
    reset();
}

// -----------------------------------------------------------------------------
// ContactsDB::set()
// -----------------------------------------------------------------------------
//
void ContactQueryResults::set( int count, QString fn, QString ln )
{
    reset();
    for( int i=0;i<count;i++ ) {
        mDB.append( fn + QString::number( i + 1 ) );
        mDB.append( ln + QString::number( i + 1 ) );
    }
}

// -----------------------------------------------------------------------------
// ContactsDB::reset()
// -----------------------------------------------------------------------------
//
void ContactQueryResults::reset()
{
    mDB.clear();
    mQuery.clear();
}


// -----------------------------------------------------------------------------
// ContactsDB::firstNameAt()
// -----------------------------------------------------------------------------
//
const QString& ContactQueryResults::firstNameAt( int index ) const
{
    return mDB[ index ];
}

// -----------------------------------------------------------------------------
// ContactsDB::lastNameAt()
// -----------------------------------------------------------------------------
//
const QString& ContactQueryResults::lastNameAt( int index ) const
{
    return mDB[ index + 1 ];
}

// -----------------------------------------------------------------------------
// ContactsDB::contacts()
// -----------------------------------------------------------------------------
//
int ContactQueryResults::contacts() const
{
    return (mDB.length() ? mDB.length() / 2 : 0);
}


QTM_USE_NAMESPACE

class QtMobility::QContactData : public QSharedData
{
public:
    QContactData(): QSharedData()
    {
    }

    ~QContactData() {}
    
    QContactId m_id;
    
};

class QtMobility::QContactDetailPrivate : public QSharedData
{
public:
    QContactDetailPrivate(): QSharedData()
    {
    }

    ~QContactDetailPrivate() {}
};

// ----------------------------------------------------------------------------
// QContactManager
// ----------------------------------------------------------------------------
//
QContactManager::QContactManager(
        const QString& /*managerName*/, const QMap<QString, QString>& /*parameters*/, 
        QObject* /*parent*/)
{

}

QContactManager::~QContactManager()
{

}


QList<QContactLocalId> QContactManager::contactIds(
        const QContactFilter& filter, 
        const QList<QContactSortOrder>& /*sortOrders*/) const
{
    const QContactDetailFilter& df = 
        static_cast<const QContactDetailFilter&>( filter );
        
    QList<QContactLocalId> list;
    ContactQueryResults* resultSet = ContactQueryResults::instance();
    
    for( int i=0;i<resultSet->contacts();i++) {
        list.append( i + 1 );
    }
    resultSet->allResultsAdded();
    
    return list;
}

QContact QContactManager::contact( 
    const QContactLocalId& contactId, 
    const QStringList& definitionRestrictions ) const
{
    QContact contact;
    QContactId id;
    id.setLocalId(contactId  );

    contact.setId( id );
    return contact;
}

// ----------------------------------------------------------------------------
// QContactDetailFilter
// ----------------------------------------------------------------------------
//
QContactDetailFilter::QContactDetailFilter()
{

}
  
void QContactDetailFilter::setDetailDefinitionName(
        const QString& /*definition*/, const QString& /*fieldName*/)
{

}


void QContactDetailFilter::setMatchFlags(QContactFilter::MatchFlags /*flags*/)
{

}
void QContactDetailFilter::setValue(const QVariant& value )
{
    ContactQueryResults* resultSet = ContactQueryResults::instance();
    resultSet->filterValueSet( value.toString() );
    
}

QVariant QContactDetailFilter::value() const
{
    return QVariant();
}


// ----------------------------------------------------------------------------
// QContact
// ----------------------------------------------------------------------------
//
QContact::QContact() : d(new QContactData)
{

}

QContact::~QContact()
{

}


QContact& QContact::operator=(const QContact& other)
{

}

QContactDetail QContact::detail(const QString& definitionId) const
{
    ContactQueryResults* results = ContactQueryResults::instance();
    
    if ( definitionId == QContactName::DefinitionName ){
        QContactName name;
        QContactLocalId id = localId();
        name.setValue(QContactName::FieldFirst, results->firstNameAt( (int) id-1 ) );
        name.setValue(QContactName::FieldLast, results->lastNameAt( (int) id-1 ) );
        return name;
    }
    if ( definitionId == QContactPhoneNumber::DefinitionName ){
        QContactPhoneNumber number;
        QString n( "555789987" );
        number.setValue(QContactPhoneNumber::FieldNumber, n );
        return number;
    }
    if ( definitionId == QContactAvatar::DefinitionName){
        QContactAvatar avatar;
        avatar.setSubType(QContactAvatar::SubTypeImage);
        avatar.setAvatar("c:\\data\\images\\logstest1.jpg");
        QString a( "Avatar" );
        avatar.setValue( QContactAvatar::FieldAvatar, a );
        return avatar;
    }
    QContactDetail detail;
    return detail;
}


// ----------------------------------------------------------------------------
// QContactDetail
// ----------------------------------------------------------------------------
//
QContactDetail::QContactDetail()
{

}
QContactDetail::QContactDetail(const QString& definitionName) : d(new QContactDetailPrivate)
{

}
QContactDetail::~QContactDetail()
{

}


QContactDetail& QContactDetail::operator=(const QContactDetail& other)
{

}

QString QContactDetail::definitionName() const
{
    return QString("");
}

bool QContactDetail::isEmpty() const
{
    return false;
}


QVariant QContactDetail::variantValue(const QString& key) const
{
    QString val = value(key);
    return val;
}



