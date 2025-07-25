//
// Created by gerw on 8/21/24.
//

#include "Mountable.h"

void Mountable::unmount() {
    mounted = false;
    // 当物品被丢弃时，启用重力让它掉落
    setGravityEnabled(true);
}

bool Mountable::isMounted() const {
    return mounted;
}

void Mountable::mountToParent() {
    mounted = true;
    // 当物品被拾取时，禁用重力
    setGravityEnabled(false);
}
