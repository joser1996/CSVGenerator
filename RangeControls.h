#ifndef RANGECONTROLS_H
#define RANGECONTROLS_H
#include <QWidget>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
class QAbstractButton;
QT_END_NAMESPACE

class RangeControls: public QWidget {
    Q_OBJECT

public:
    explicit RangeControls(QWidget* parent = nullptr);
    QMediaPlayer::PlaybackState state() const;
public slots:
    void setState(QMediaPlayer::PlaybackState state);

signals:
    void play();
    void pause();
    void updateTime();

private slots:
    void playClicked();

private:
    QMediaPlayer::PlaybackState mPlayerState = QMediaPlayer::StoppedState;
    QAbstractButton* mPlayButton = nullptr;
};

#endif // RANGECONTROLS_H
