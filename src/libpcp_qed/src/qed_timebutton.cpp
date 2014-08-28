/*
 * Copyright (c) 2014, Red Hat.
 * Copyright (c) 2007, Aconex.  All Rights Reserved.
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */
#include "qed_timebutton.h"
#include "qed_console.h"

QedTimeButton::QedTimeButton(QWidget *parent) : QToolButton(parent)
{
    my.state = Timeless;
    setIconSize(QSize(52, 52));
    setFocusPolicy(Qt::NoFocus);
    my.forwardLiveIcon = QIcon(":/play_live.png");
    my.stoppedLiveIcon = QIcon(":/stop_live.png");
    my.forwardRecordIcon = QIcon(":/play_record.png");
    my.stoppedRecordIcon = QIcon(":/stop_record.png");
    my.forwardArchiveIcon = QIcon(":/play_archive.png");
    my.stoppedArchiveIcon = QIcon(":/stop_archive.png");
    my.backwardArchiveIcon = QIcon(":/back_archive.png");
    my.stepForwardArchiveIcon = QIcon(":/stepfwd_archive.png");
    my.stepBackwardArchiveIcon = QIcon(":/stepback_archive.png");
    my.fastForwardArchiveIcon = QIcon(":/fastfwd_archive.png");
    my.fastBackwardArchiveIcon = QIcon(":/fastback_archive.png");
    console->post(QedApp::DebugUi, "Time button resources loaded");
}

void QedTimeButton::setButtonState(QedTimeButton::State state)
{
    if (my.state == state)
	return;
    switch (state) {
    case QedTimeButton::ForwardLive:
	setIcon(my.forwardLiveIcon);
	break;
    case QedTimeButton::StoppedLive:
	setIcon(my.stoppedLiveIcon);
	break;
    case QedTimeButton::ForwardRecord:
	setIcon(my.forwardRecordIcon);
	break;
    case QedTimeButton::StoppedRecord:
	setIcon(my.stoppedRecordIcon);
	break;
    case QedTimeButton::ForwardArchive:
	setIcon(my.forwardArchiveIcon);
	break;
    case QedTimeButton::StoppedArchive:
	setIcon(my.stoppedArchiveIcon);
	break;
    case QedTimeButton::BackwardArchive:
	setIcon(my.backwardArchiveIcon);
	break;
    case QedTimeButton::StepForwardArchive:
	setIcon(my.stepForwardArchiveIcon);
	break;
    case QedTimeButton::StepBackwardArchive:
	setIcon(my.stepBackwardArchiveIcon);
	break;
    case QedTimeButton::FastForwardArchive:
	setIcon(my.fastForwardArchiveIcon);
	break;
    case QedTimeButton::FastBackwardArchive:
	setIcon(my.fastBackwardArchiveIcon);
	break;
    default:
	abort();
    }
    my.state = state;
}