/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <afactivation.h>
#include <afactivitystorage.h>
#include "af_stub_helper.h"

QHash <QString, QVariantHash> testActivities;
Af::ActivationReason testActivationReason = Af::ActivationReasonNormal;
QString testActivityId = "LogsViewMatches";

void AfStubHelper::reset()
{
    testActivationReason = Af::ActivationReasonNormal;
    testActivityId = "LogsViewMatches";
    testActivities.clear();
}

void AfStubHelper::setActivityReason(Af::ActivationReason reason)
{
    testActivationReason = reason;
}

void AfStubHelper::setActivityId(QString activityId)
{
    testActivityId = activityId;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
AfActivation::AfActivation(QObject *parent) : QObject(parent)
{

}

QVariantHash AfActivation::parameters() const
{
    return QVariantHash();
}
Af::ActivationReason AfActivation::reason() const
{
    return testActivationReason;
}

QString AfActivation::name() const
{
    return testActivityId;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
AfActivityStorage::AfActivityStorage(QObject *parent) : QObject(parent)
{

}

AfActivityStorage::~AfActivityStorage()
{

}

bool AfActivityStorage::saveActivity(
        const QString &activityId, const QVariant &activityData, const QVariantHash &metadata)
{
    Q_UNUSED( activityData );
    testActivities.insert(activityId, metadata);
    return true;
}

bool AfActivityStorage::removeActivity(const QString &activityId)
{
    if ( !testActivities.isEmpty() ){
        testActivities.remove(activityId);
    }
    return true;
}

QStringList AfActivityStorage::allActivities() const
{
    QStringList activities;
    QHash<QString, QVariantHash>::const_iterator it;
    for (it = testActivities.begin(); it != testActivities.end(); ++it){
        activities.append( it.key() );
    }
    return activities;
}

QVariant AfActivityStorage::activityData(const QString &activityId) const
{
    Q_UNUSED( activityId );
    return QVariant();
}

QVariantHash AfActivityStorage::activityMetaData(const QString &activityId) const
{
    Q_UNUSED( activityId );
    if ( !testActivities.isEmpty() ){
        return testActivities.begin().value();
    }
    return QVariantHash();
}

