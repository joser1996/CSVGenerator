#include "RowButtons.h"
#include <QToolButton>
#include <QStyle>
#include <QHBoxLayout>
#include <QDebug>

RowButtons::RowButtons(QWidget* parent): QWidget(parent) {
    mPlayButton = new QToolButton(this);
    mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(mPlayButton, &QAbstractButton::clicked, this, &RowButtons::playClicked);

    mDeleteButton = new QToolButton(this);
    mDeleteButton->setIcon(style()->standardIcon(QStyle::SP_DialogDiscardButton));
    connect(mDeleteButton, &QAbstractButton::clicked, this, &RowButtons::deleteClicked);

    QBoxLayout* layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mPlayButton);
    layout->addWidget(mDeleteButton);
    setLayout(layout);
}

QMediaPlayer::PlaybackState RowButtons::state() const {
    return mPlayerState;
}

void RowButtons::setState(QMediaPlayer::PlaybackState state) {
    if (state != mPlayerState) {
        mPlayerState = state;
        switch(state) {
        case QMediaPlayer::StoppedState:
        case QMediaPlayer::PausedState:
            mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case QMediaPlayer::PlayingState:
            mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        }
    }
}

void RowButtons::playClicked() {
    switch(mPlayerState) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        //emit updateTime();
        emit play();
        break;
    case QMediaPlayer::PlayingState:
        emit pause();
        break;
    }
}

void RowButtons::deleteClicked() {
    qDebug() << "Delete button was pressed";
}

