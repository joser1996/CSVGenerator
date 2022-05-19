#ifndef TIMESEGMENT_H
#define TIMESEGMENT_H
#include <QString>
#include <QDebug>

struct TimeSegment {
    TimeSegment(QString low, QString upper, QString title)
        : mLow(low), mUpper(upper), mTitle(title) {}
    TimeSegment() = delete;
    QString mLow;
    QString mUpper;
    QString mTitle;

    operator QString() const {
        QString msg = "%1:\t (%2, %3)";
        msg = msg.arg(mTitle, mLow, mUpper);
        return msg;
    }

//    QDebug operator<<(QDebug debug, const TimeSegment& t) {
//        QDebugStateSaver saver(debug);
//        debug.nospace() << t.mtitle << ": " << tr("(%1, %2)").arg(mLow).arg(mUpper);

//        return debug;
//    }
};


#endif // TIMESEGMENT_H
