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

#include <QGraphicsLinearLayout>
#include <QScopedPointer>
#include <QDebug>

#include <hbmessagebox.h>
#include <hblabel.h>
#include <hbpushbutton.h>
#include <hblineedit.h>
#include <hbcombobox.h>
#include <hbcheckbox.h>
#include <xqappmgr.h>
#include <xqaiwrequest.h>
#include <logsservices.h>

#include "logsservicetesterwidget.h"
#include "logsservicetesterappcloser.h"
#include "logsdefs.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsServiceTesterWidget::LogsServiceTesterWidget(QGraphicsItem *parent ) :
    HbWidget(parent), 
    mViewIndex(0), 
    mShowDialpad(false), 
    mDialpadText(QString())
{
    qDebug() << "[LOGS_TESTER]-> LogsServiceTesterWidget::LogsServiceTesterWidget()";
    mAppCloser = new LogsServiceTesterAppCloser(this);
    connect(mAppCloser, SIGNAL(closed()), this, SLOT(dialerClosed()), 
            Qt::QueuedConnection);
    connect(mAppCloser, SIGNAL(closeError(int)), this, SLOT(dialerCloseError(int)),
            Qt::QueuedConnection);
    
    HbLabel* comboLabel = new HbLabel("Select logs view:");
    
    mComboBox = new HbComboBox();
    mComboBox->addItem( "Recent view", QVariant(LogsServices::ViewAll));
    mComboBox->addItem( "Received view", QVariant(LogsServices::ViewReceived));
    mComboBox->addItem( "Called view", QVariant(LogsServices::ViewCalled));
    mComboBox->addItem( "Missed view", QVariant(LogsServices::ViewMissed));

    mCheckBox = new HbCheckBox("Show dialpad");
    
    HbLabel* editorLabel = new HbLabel("Launch dialpad with number:");
    mEditor = new HbLineEdit();
    mEditor->setInputMethodHints(Qt::ImhPreferNumbers);
    
    HbPushButton* newService = new HbPushButton("Start Dialer via new service name");
    newService->setPreferredHeight(60);
    connect(newService, SIGNAL(clicked()), this, SLOT(startNewService()));

    HbPushButton* oldService = new HbPushButton("Start Dialer via old service name");
    oldService->setPreferredHeight(60);
    connect(oldService, SIGNAL(clicked()), this, SLOT(startOldService()));

    HbPushButton* killDialer = new HbPushButton("Kill Dialer");
    killDialer->setPreferredHeight(60);
    connect(killDialer, SIGNAL(clicked()), mAppCloser, SLOT(closeDialerApp()));
    
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->addItem(comboLabel);
    layout->addItem(mComboBox);
    layout->addItem(mCheckBox);
    layout->addItem(editorLabel);
    layout->addItem(mEditor);
    layout->addItem(newService);
    layout->addItem(oldService);
    layout->addItem(killDialer);
    setLayout(layout);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsServiceTesterWidget:: ~LogsServiceTesterWidget()
{
    qDebug() << "[LOGS_TESTER] <-> LogsServiceTesterWidget::~LogsServiceTesterWidget()";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsServiceTesterWidget::fetchData()
{
    qDebug() << "[LOGS_TESTER] LogsServiceTesterWidget::fetchData()";
    int index = mComboBox->currentIndex()>0 ? mComboBox->currentIndex() : 0;
    mViewIndex = mComboBox->itemData(index).toInt();
    qDebug() << "[LOGS_TESTER] viewIndex: " << mViewIndex;
    
    mShowDialpad = (mCheckBox->checkState() == Qt::Checked) ? true : false;
    qDebug() << "[LOGS_TESTER] showDialpad: " << mShowDialpad;
    
    mDialpadText = mEditor->text();
    qDebug() << "[LOGS_TESTER] dialpadText: " << mDialpadText;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsServiceTesterWidget::startNewService()
{
    qDebug() << "[LOGS_TESTER] -> LogsServiceTesterWidget::startNewService()";
    fetchData();

    XQApplicationManager appMgr;
    QScopedPointer<XQAiwRequest> request(appMgr.create( QLatin1String(logsServiceName),
                    QLatin1String(logsInterfaceName),
                    QLatin1String(logsOperationName),
                    false));
    if (!request.isNull()) {
        int retValue = -1; 
        QList<QVariant> arglist;
        QVariantMap map;
        map.insert(logsViewIndexParam, QVariant(mViewIndex));
        map.insert(logsShowDialpadParam, QVariant(mShowDialpad));
        map.insert(logsDialpadTextParam, QVariant(mDialpadText));
        
        arglist.append(QVariant(map));
        request->setArguments(arglist);
        QVariant ret(retValue);
        
        request->send(ret);
    }
    qDebug() << "[LOGS_TESTER] <- LogsServiceTesterWidget::startNewService()";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsServiceTesterWidget::startOldService()
{
    qDebug() << "[LOGS_TESTER] -> LogsServiceTesterWidget::startOldService()";
    fetchData();
    LogsServices::start( (LogsServices::LogsView)mViewIndex, mShowDialpad, mDialpadText );
    qDebug() << "[LOGS_TESTER] <- LogsServiceTesterWidget::startOldService()";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsServiceTesterWidget::dialerClosed()
{
    HbMessageBox::information("Dialer closed");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsServiceTesterWidget::dialerCloseError(int error)
{
    QString errorMsg;
    if (error == LogsServiceTesterAppCloser::ErrorAppNotRunning) {
        errorMsg = QString("Dialer not running");
    } else if (error == LogsServiceTesterAppCloser::ErrorClosingOngoing) {
        errorMsg = QString("Dialer closing is already ongoing");
    } else  if (error == LogsServiceTesterAppCloser::ErrorClosingTimeout) {
        errorMsg = QString("Dialer closing timeout");
    } else {
        errorMsg = QString("Dialer closing error: %1").arg(error);
    } 
    HbMessageBox::warning(errorMsg);
}
