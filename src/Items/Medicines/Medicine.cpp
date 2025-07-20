#include "Medicine.h"

Medicine::Medicine(QGraphicsItem *parent, const QString &pixmapPath, const QString &medicineName)
    : Mountable(parent, pixmapPath), medicineName(medicineName) {
    // 设置缩放，让药品和武器大小类似
    setScale(1.5);
}
