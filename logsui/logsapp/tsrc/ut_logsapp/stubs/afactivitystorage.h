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
#ifndef AFACTIVITYSTORAGE_H
#define AFACTIVITYSTORAGE_H

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QScopedPointer>
#include <afactivities_global.h>

class AfActivityStorage : public QObject
{
    Q_OBJECT

public:
    AfActivityStorage(QObject *parent = 0);
    virtual ~AfActivityStorage();

public slots:
    bool saveActivity(const QString &activityId, const QVariant &activityData, const QVariantHash &metadata);
    bool removeActivity(const QString &activityId);

    QStringList allActivities() const;

    QVariant activityData(const QString &activityId) const;
    QVariantHash activityMetaData(const QString &activityId) const;

};

#endif // AFACTIVITYSTORAGE_H
