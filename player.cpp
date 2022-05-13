#include "player.h"
#include "PlayerControls.h"

#include <QtWidgets>
#include <QMediaPlayer>

Player::Player(QWidget* parent): QWidget(parent) {
    mPlayer = new QMediaPlayer(this);
    mAudio = new QAudioOutput;
    mAudioDevice = QAudioDevice();
    mAudio->setDevice(mAudioDevice);
    mPlayer->setAudioOutput(mAudio);

    //set audio role?
    connect(mPlayer, &QMediaPlayer::durationChanged, this, &Player::durationChanged);
    connect(mPlayer, &QMediaPlayer::positionChanged, this, &Player::positionChanged);
    connect(mPlayer, QOverload<>::of(&QMediaPlayer::metaDataChanged), this, &Player::metaDataChanged);
    connect(mPlayer, &QMediaPlayer::mediaStatusChanged, this, &Player::statusChanged);
    connect(mPlayer, &QMediaPlayer::playbackStateChanged, this, &Player::stateChanged);
    connect(mPlayer, &QMediaPlayer::bufferProgressChanged, this, &Player::bufferingProgress);
    connect(mPlayer, &QMediaPlayer::errorChanged, this, &Player::displayErrorMessage);
    //open
    //jump

    mSlider = new QSlider(Qt::Horizontal, this);
    mSlider->setRange(0, mPlayer->duration() / 1000);

    mLabelDuration = new QLabel(this);
    connect(mSlider, &QSlider::sliderMoved, this, &Player::seek);

    //dont' need audio probe for this project
    QPushButton* openButton = new QPushButton(tr("Open"), this);
    connect(openButton, &QPushButton::clicked, this, &Player::open);

    PlayerControls* controls = new PlayerControls(this);
    controls->setState(mPlayer->playbackState());
    controls->setVolume(mAudio->volume()); //using audio output class?
    controls->setMuted(controls->isMuted());

    connect(controls, &PlayerControls::play, mPlayer, &QMediaPlayer::play);
    connect(controls, &PlayerControls::pause, mPlayer, &QMediaPlayer::pause);
    connect(controls, &PlayerControls::stop, mPlayer, &QMediaPlayer::stop);
    //connect(controls, &PlayerControls::next, mPlayer, &QMediaPlayer::stop);
    connect(controls, &PlayerControls::changeVolume, mAudio, &QAudioOutput::setVolume);
    connect(controls, &PlayerControls::changeMuting, mAudio, &QAudioOutput::setMuted);
    connect(controls, &PlayerControls::changeRate, mPlayer, &QMediaPlayer::setPlaybackRate);

    connect(mPlayer, &QMediaPlayer::playbackStateChanged, controls, &PlayerControls::setState);
    connect(mAudio, &QAudioOutput::volumeChanged, controls, &PlayerControls::setVolume);
    connect(mAudio, &QAudioOutput::mutedChanged, controls, &PlayerControls::setMuted);

    QBoxLayout* controlsLayout = new QHBoxLayout;
    controlsLayout->setContentsMargins(0, 0, 0, 0);
    controlsLayout->addWidget(openButton);
    controlsLayout->addStretch(1);
    controlsLayout->addWidget(controls);
    controlsLayout->addStretch(1);

    QBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(mSlider);
    hLayout->addWidget(mLabelDuration);
    layout->addLayout(hLayout);
    layout->addLayout(controlsLayout);
    //didn't include display layout

#if defined(Q_OS_QNX)
    mStatusLabel = new QLabel;
    mStatusBar = new QStatusBar;
    mStatusBar->addPermanentWidget(mStatusLabel);
    mStatusBar->setSizeGripEnabled(false);
    layout->addWidget(mStatusBar);
#endif


    setLayout(layout);

    if (!isPlayerAvailable()) {
        QMessageBox::warning(this, tr("Service not available"),
                             tr("The QMediaPlayer object does not have a valid service.\n"\
                                "Please check the media services plugins are installed."));
        controls->setEnabled(false);
        openButton->setEnabled(false);
    }

    metaDataChanged();
}

Player::~Player() {}

bool Player::isPlayerAvailable() const {
    return mPlayer->isAvailable();
}

void Player::open() {
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Files"));
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
        mTracks = fileDialog.selectedUrls(); //Save Track as URL Name
}

void Player::durationChanged(qint64 duration) {
    mDuration = duration / 1000;
    mSlider->setMaximum(mDuration);
}

void Player::positionChanged(qint64 progress) {
    if (!mSlider->isSliderDown())
        mSlider->setValue(progress / 1000);
    updateDurationInfo(progress / 1000);
}

void Player::metaDataChanged() {
    QMediaMetaData metaData = mPlayer->metaData();
    if (!metaData.isEmpty()) {
        setTrackInfo(QString("%1 - %2")
                     .arg(metaData.value(QMediaMetaData::AlbumArtist).toString())
                     .arg(metaData.value(QMediaMetaData::Title).toString()));
        if (mCoverLabel) {
            QUrl url = metaData.value(QMediaMetaData::CoverArtImage).value<QUrl>();
            mCoverLabel->setPixmap(!url.isEmpty() ? QPixmap(url.toString()) : QPixmap());
        }
    }
}

// Previous Clicked method

//jump to track in playlist
//void Player::jump(const QModelIndex& index)
//playlist position changed

void Player::seek(int seconds) {
    mPlayer->setPosition(seconds * 1000);
}

void Player::statusChanged(QMediaPlayer::MediaStatus status) {
    handleCursor(status);

    switch (status) {
    case QMediaPlayer::NoMedia:
    case QMediaPlayer::LoadedMedia:
        setStatusInfo(QString());
        break;
    case QMediaPlayer::LoadingMedia:
        setStatusInfo(tr("Loading..."));
        break;
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
        setStatusInfo(tr("Buffering %1%").arg(mPlayer->bufferProgress() * 100));
        break;
    case QMediaPlayer::StalledMedia:
        setStatusInfo(tr("Stalled %1%").arg(mPlayer->bufferProgress() * 100));
        break;
    case QMediaPlayer::EndOfMedia:
        QApplication::alert(this);
        break;
    case QMediaPlayer::InvalidMedia:
        displayErrorMessage();
        break;
    }
}

void Player::stateChanged(QMediaPlayer::PlaybackState state) {
    if (state == QMediaPlayer::StoppedState)
        qDebug() << "Nothing";
}

void Player::handleCursor(QMediaPlayer::MediaStatus status) {
#ifndef QT_NO_CURSOR
    if (status == QMediaPlayer::LoadingMedia ||
        status == QMediaPlayer::BufferingMedia ||
        status == QMediaPlayer::StalledMedia)
        setCursor(QCursor(Qt::BusyCursor));
    else
        unsetCursor();
#endif
}

void Player::bufferingProgress(int progress) {
    if (mPlayer->mediaStatus() == QMediaPlayer::StalledMedia)
        setStatusInfo(tr("Stalled %1%").arg(progress));
    else
        setStatusInfo(tr("Buffering %1%").arg(progress));
}

void Player::setTrackInfo(const QString &info) {
    mTrackInfo = info;
    if (mStatusBar) {
        mStatusBar->showMessage(mTrackInfo);
        mStatusLabel->setText(mStatusInfo);
    } else {
        if (!mStatusInfo.isEmpty())
            setWindowTitle(QString("%1 | %2").arg(mTrackInfo).arg(mStatusInfo));
        else
            setWindowTitle(mTrackInfo);
    }
}

void Player::setStatusInfo(const QString &info) {
    mStatusInfo = info;
    if (mStatusBar) {
        mStatusBar->showMessage(mTrackInfo);
        mStatusLabel->setText(mStatusInfo);
    } else {
        if (!mStatusInfo.isEmpty())
            setWindowTitle(QString("%1 | %2").arg(mTrackInfo).arg(mStatusInfo));
        else
            setWindowTitle(mTrackInfo);
    }
}

void Player::displayErrorMessage() {
    setStatusInfo(mPlayer->errorString());
}

void Player::updateDurationInfo(qint64 currentInfo) {
    QString tStr;
    if (currentInfo || mDuration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
                          currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((mDuration / 3600) % 60, (mDuration / 60) % 60,
                        mDuration % 60, (mDuration * 1000) % 1000);
        QString format = "mm:ss";
        if (mDuration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    mLabelDuration->setText(tStr);
}

//no showColorDialog
//no clearHistogram










