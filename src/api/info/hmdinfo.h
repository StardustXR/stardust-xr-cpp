#ifndef STARDUSTAPI_INFO_HMDINFO_H
#define STARDUSTAPI_INFO_HMDINFO_H

#include <QObject>
#include <QOpenXRApplication>

namespace StardustAPI {
namespace Info {

class HmdInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVector3D gazePosition READ gazePosition NOTIFY gazePositionChanged)
    Q_PROPERTY(QVector3D gazeDirection READ gazeDirection NOTIFY gazeDirectionChanged)
public:
    explicit HmdInfo(QOpenXRApplication *oxr);

    QVector3D gazePosition();
    QVector3D gazeDirection();

signals:
    void gazePositionChanged();
    void gazeDirectionChanged();

private:
    QOpenXRApplication *oxr;

    void oxrGazePositionChanged();
    void oxrGazeDirectionChanged();
};

} // namespace Info
} // namespace StardustAPI

#endif // STARDUSTAPI_INFO_HMDINFO_H
