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
#include <QDebug>
#include <centralrepository.h>
#include <hbradiobuttonlist.h>
#include <hbpushbutton.h>
#include <hbaction.h>
#include <hblabel.h>
#include <hbapplication.h>
#include <LogsDomainCRKeys.h>
#include "logscenrepeditorview.h"

const QStringList KPredictiveSearchList = (QStringList() 
        << "Permanently Off" 
        << "On" 
        << "Temporarily Off"
        << "Not defined");

LogsCenrepEditorView::LogsCenrepEditorView() 
    : HbView(0),
      mList(0),
      mRepository(0)
{
    qDebug() << "[LOGS_CENREP]-> LogsCenrepEditorView::LogsCenrepEditorView()";
    this->setTitle("Logs cenrep editor");

    QT_TRAP_THROWING( mRepository = CRepository::NewL( KCRUidLogs ) );

    TInt value(-1);
    TInt err = mRepository->Get( KLogsPredictiveSearch, value );
    qDebug() << "[LOGS_CENREP]-> mRepository->Get(KLogsPredictiveSearch) value: " << value
            << ", err: " << err;
    
    mList = new HbRadioButtonList(this);    
    mList->setItems(KPredictiveSearchList);
    int listCurrentIndex = (err == 0) ? value : KPredictiveSearchList.count()-1;
    mList->setSelected(listCurrentIndex);

    HbPushButton* buttonSave = new HbPushButton("Save and Exit", this);
    buttonSave->setMinimumHeight(60);
    connect(buttonSave, SIGNAL(clicked()), this, SLOT(saveSettings()));
    connect(buttonSave, SIGNAL(clicked()), qApp, SLOT(quit()));

    HbPushButton* buttonExit = new HbPushButton("Exit without saving", this);
    buttonExit->setMinimumHeight(60);
    connect(buttonExit, SIGNAL(clicked()), qApp, SLOT(quit()));

    HbLabel* label = new HbLabel("Predictive search feature", this); 
    
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->addItem(label);
    layout->addItem(mList);
    layout->addItem(buttonSave);
    layout->addItem(buttonExit);
    setLayout(layout);
}


LogsCenrepEditorView::~LogsCenrepEditorView()
{
    qDebug() << "[LOGS_CENREP]-> LogsCenrepEditorView::~LogsCenrepEditorView()";
    delete mRepository;
    qDebug() << "[LOGS_CENREP]<- LogsCenrepEditorView::~LogsCenrepEditorView()";
}

void LogsCenrepEditorView::saveSettings()
{
    qDebug() << "[LOGS_CENREP]-> LogsCenrepEditorView::saveSettings()";
    if (mList->selected() < KPredictiveSearchList.count()) {
        int err = mRepository->Set( KLogsPredictiveSearch, mList->selected() );
        qDebug() << "[LOGS_CENREP]-> mRepository->Set(KLogsPredictiveSearch), value:"
                << mList->selected() << ", err: " << err;
    } else {
        qDebug() << "[LOGS_CENREP]-> not saving!!";
    }

    qDebug() << "[LOGS_CENREP]<- LogsCenrepEditorView::saveSettings()";
}
