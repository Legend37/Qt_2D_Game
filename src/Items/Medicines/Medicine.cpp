#include "Medicine.h"

Medicine::Medicine(QGraphicsItem *parent, const QString &pixmapPath, const QString &medicineName)
    : Mountable(parent, pixmapPath), medicineName(medicineName) {
    // �������ţ���ҩƷ��������С����
    setScale(1.5);
}
