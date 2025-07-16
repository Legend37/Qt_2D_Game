#include "Item.h"

Item::Item(QGraphicsItem *parent, const QString &pixmapPath, bool mirrorPixmap) : QGraphicsItem(parent) {
    if (pixmapPath != "") {
        QPixmap pix(pixmapPath);
        if (mirrorPixmap) {
            pix = pix.transformed(QTransform().scale(-1, 1));
        }
        pixmapItem = new QGraphicsPixmapItem(pix, this);
    }
}
