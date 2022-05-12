#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <QMediaPlayer>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QAbstractSlider;
class QComboBox;
QT_END_NAMESPACE

class PlayerControls: public QWidget {
    Q_OBJECT

public:
    explicit PlayerControls(QWidget* parent = nullptr);
    QMediaPlayer::PlaybackState state() const;
    int volume() const;
    bool isMuted() const;
    qreal playbackRate() const;

public slots:
    void setState(QMediaPlayer::PlaybackState state);
    void setVolume(int volume);
    void setMuted(bool muted);
    void setPlaybackRate(float rate);

signals:
    void play();
    void pause();
    void stop();
    void changeVolume(int volume);
    void changeMuting(bool muting);
    void changeRate(qreal rate);

private slots:
    void playClicked();
    void muteClicked();
    void updateRate();
    void onVolumeSliderValueChanged();

private:
    QMediaPlayer::PlaybackState mPlayerState = QMediaPlayer::StoppedState;
    bool mPlayerMuted = false;
    QAbstractButton* mPlayButton = nullptr;
    QAbstractButton* mStopButton = nullptr;
    QAbstractButton* mMuteButton = nullptr;
    QAbstractSlider* mVolumeSlider = nullptr;
    QComboBox* mRateBox = nullptr;
};

#endif // PLAYERCONTROLS_H
