#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QMediaPlayer>
#include "RangeSlider.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QMediaPlayer;
class PushButton;
class QSlider;
class QAudioProbe;
QT_END_NAMESPACE

class Player: public QWidget {
    Q_OBJECT

public:
    explicit Player(QWidget * parent = nullptr);
    ~Player();

    bool isPlayerAvailable() const;

signals:

private slots:
    void open();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void metaDataChanged();

    void seek(int seconds);
    void updateRange(int low, int high);
    //void jump(const QModelIndex& index);

    void statusChanged(QMediaPlayer::MediaStatus status);
    void stateChanged(QMediaPlayer::PlaybackState state);
    void bufferingProgress(int progress);
    void displayErrorMessage();

private:
    void setTrackInfo(const QString& info);
    void setStatusInfo(const QString& info);
    void handleCursor(QMediaPlayer::MediaStatus status);
    void updateDurationInfo(qint64 currentInfo);

    QMediaPlayer* mPlayer = nullptr;
    QLabel* mCoverLabel = nullptr;
    QLabel* mBreakLabel = nullptr;
    QSlider* mSlider = nullptr;
    RangeSlider* mBreakSlider = nullptr;
    QLabel* mLabelDuration = nullptr;
    QLabel* mStatusLabel = nullptr;
    QStatusBar* mStatusBar = nullptr;


    QAudioProbe* mAudioProbe = nullptr;
    QString mTrackInfo;
    QString mStatusInfo;
    qint64 mDuration;
    QAudioOutput* mAudio;
    QAudioDevice mAudioDevice;

    QList<QUrl> mTracks;
};

#endif // PLAYER_H
