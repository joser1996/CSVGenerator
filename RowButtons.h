#ifndef ROWBUTTONS_H
#define ROWBUTTONS_H
#include <QWidget>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
class QAbstractButton;
QT_END_NAMESPACE

class RowButtons: public QWidget {
    Q_OBJECT

public:
    explicit RowButtons(QWidget* parent = nullptr);
    QMediaPlayer::PlaybackState state() const;

public slots:
    void setState(QMediaPlayer::PlaybackState state);

signals:
    void play();
    void pause();
    void deleteRow();

private slots:
    void playClicked();
    void deleteClicked();

private:
    QMediaPlayer::PlaybackState mPlayerState = QMediaPlayer::StoppedState;
    QAbstractButton* mPlayButton = nullptr;
    QAbstractButton* mDeleteButton = nullptr;
};

#endif // ROWBUTTONS_H
