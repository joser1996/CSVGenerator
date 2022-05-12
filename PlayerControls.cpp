#include "PlayerControls.h"
#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QAudio>
#include <QComboBox>

PlayerControls::PlayerControls(QWidget* parent): QWidget(parent) {
    mPlayButton = new QToolButton(this);
    mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(mPlayButton, &QAbstractButton::clicked, this, &PlayerControls::playClicked);

    mStopButton = new QToolButton(this);
    mStopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    mStopButton->setEnabled(false);
    connect(mStopButton, &QAbstractButton::clicked, this, &PlayerControls::stop);

    //no next
    //no previous
    mMuteButton = new QToolButton(this);
    mMuteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    connect(mMuteButton, &QAbstractButton::clicked, this, &PlayerControls::muteClicked);

    mVolumeSlider = new QSlider(Qt::Horizontal, this);
    mVolumeSlider->setRange(0, 100);
    connect(mVolumeSlider, &QSlider::valueChanged, this, &PlayerControls::onVolumeSliderValueChanged);

    mRateBox = new QComboBox(this);
    mRateBox->addItem("0.5x", QVariant(0.5));
    mRateBox->addItem("1.0x", QVariant(1.0));
    mRateBox->addItem("2.0x", QVariant(2.0));
    mRateBox->setCurrentIndex(1);
    connect(mRateBox, QOverload<int>::of(&QComboBox::activated), this, &PlayerControls::updateRate);

    QBoxLayout* layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mStopButton);
    layout->addWidget(mPlayButton);
    layout->addWidget(mMuteButton);
    layout->addWidget(mVolumeSlider);
    layout->addWidget(mRateBox);
    setLayout(layout);
}

QMediaPlayer::PlaybackState PlayerControls::state() const {
    return mPlayerState;
}

void PlayerControls::setState(QMediaPlayer::PlaybackState state) {
    if (state != mPlayerState) {
        mPlayerState = state;

        switch(state) {
        case QMediaPlayer::StoppedState:
            mStopButton->setEnabled(false);
            mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case QMediaPlayer::PlayingState:
            mStopButton->setEnabled(true);
            mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case QMediaPlayer::PausedState:
            mStopButton->setEnabled(true);
            mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
}

int PlayerControls::volume() const {
    qreal linearVolume = QAudio::convertVolume(mVolumeSlider->value() / qreal(100),
                                               QAudio::LogarithmicVolumeScale,
                                               QAudio::LinearVolumeScale);
    return qRound(linearVolume * 100);
}

void PlayerControls::setVolume(int volume) {
    qreal logarithmicVolume = QAudio::convertVolume(volume / qreal(100), QAudio::LinearVolumeScale,
                                                    QAudio::LogarithmicVolumeScale);
    mVolumeSlider->setValue(qRound(logarithmicVolume * 100));
}

bool PlayerControls::isMuted() const {
    return mPlayerMuted;
}

void PlayerControls::setMuted(bool muted) {
    if (muted != mPlayerMuted) {
        mPlayerMuted = muted;
        mMuteButton->setIcon(style()->standardIcon(muted
               ? QStyle::SP_MediaVolumeMuted
               : QStyle::SP_MediaVolume));
    }
}

void PlayerControls::playClicked() {
    switch(mPlayerState) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        emit play();
        break;
    case QMediaPlayer::PlayingState:
        emit pause();
        break;
    }
}

void PlayerControls::muteClicked() {
    emit changeMuting(!mPlayerMuted);
}

qreal PlayerControls::playbackRate() const {
    return mRateBox->itemData(mRateBox->currentIndex()).toDouble();
}

void PlayerControls::setPlaybackRate(float rate) {
    for (int i = 0; i < mRateBox->count(); i++) {
        if (qFuzzyCompare(rate, float(mRateBox->itemData(i).toDouble()))) {
            mRateBox->setCurrentIndex(i);
            return;
        }
    }

    mRateBox->addItem(QString("%1x").arg(rate), QVariant(rate));
    mRateBox->setCurrentIndex(mRateBox->count() - 1);
}

void PlayerControls::updateRate() {
    emit changeRate(playbackRate());
}

void PlayerControls::onVolumeSliderValueChanged() {
    emit changeVolume(volume());
}

















