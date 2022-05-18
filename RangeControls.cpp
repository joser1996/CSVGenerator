#include "RangeControls.h"
#include <QToolButton>
#include <QStyle>
#include <QHBoxLayout>

RangeControls::RangeControls(QWidget* parent): QWidget(parent) {
    mPlayButton = new QToolButton(this);
    mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(mPlayButton, &QAbstractButton::clicked, this, &RangeControls::playClicked);

    QBoxLayout* layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mPlayButton);
    setLayout(layout);
}

QMediaPlayer::PlaybackState RangeControls::state() const {
    return mPlayerState;
}

void RangeControls::setState(QMediaPlayer::PlaybackState state) {
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

void RangeControls::playClicked() {

    //aside from emmiting play need to update time on player

    switch(mPlayerState) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        emit updateTime();
        emit play();
        break;
    case QMediaPlayer::PlayingState:
        emit pause();
        break;
    }
}
