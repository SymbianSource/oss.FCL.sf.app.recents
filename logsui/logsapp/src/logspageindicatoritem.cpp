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

#include <QGraphicsColorizeEffect>
#include <QPropertyAnimation>

#include "logspageindicatoritem.h"


const char logsNormalIconName[]    = "qtg_graf_hspage_normal";//"qtg_mono_tab_passive";//
const char logsHighlightIconName[] = "qtg_graf_hspage_highlight";//"qtg_mono_tab_active";//
  
const int logsEffectDurationInMs = 1000;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsPageIndicatorItem::LogsPageIndicatorItem(bool active, QGraphicsItem *parent)
  : HbIconItem(parent),
    mIsActive(active)
{
    if (active) {
        setIcon(HbIcon(logsHighlightIconName));
    } else {
        setIcon(HbIcon(logsNormalIconName));
    }

    QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect;
    effect->setColor(Qt::white);
    effect->setStrength(0);
    effect->setEnabled(false);
    setGraphicsEffect(effect);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LogsPageIndicatorItem::~LogsPageIndicatorItem()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsPageIndicatorItem::setActive(bool active)
{
    if (mIsActive != active) {
        mIsActive = active;
        if (mIsActive) {
            setIcon(HbIcon(logsHighlightIconName));
            startAnimation();
        } else {
            setIcon(HbIcon(logsNormalIconName));
        }
    }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
bool LogsPageIndicatorItem::isActive() const
{
    return mIsActive;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsPageIndicatorItem::startAnimation()
{
    graphicsEffect()->setEnabled(true);
    setTransformOriginPoint(rect().center());
    QPropertyAnimation *animation = 
        new QPropertyAnimation(graphicsEffect(), "strength");
    animation->setDuration(logsEffectDurationInMs);
    animation->setKeyValueAt(0.2, 1);
    animation->setEndValue(0);
    connect(animation, SIGNAL(finished()), SLOT(animationFinished()));
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsPageIndicatorItem::animationFinished()
{
    graphicsEffect()->setEnabled(false);
}
