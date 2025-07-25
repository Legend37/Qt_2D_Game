//
// Created by gerw on 8/21/24.
//

#include "Mountable.h"

void Mountable::unmount() {
    mounted = false;
    // ����Ʒ������ʱ������������������
    setGravityEnabled(true);
}

bool Mountable::isMounted() const {
    return mounted;
}

void Mountable::mountToParent() {
    mounted = true;
    // ����Ʒ��ʰȡʱ����������
    setGravityEnabled(false);
}
