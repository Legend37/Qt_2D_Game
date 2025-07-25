cmake_minimum_required(VERSION 3.28)
project(qt_2025_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(qt_2025_game src/main.cpp
        src/MyGame.cpp
        src/MyGame.h
        src/Items/Item.cpp
        src/Items/Item.h
        src/mainwindow.ui
        src/Items/Characters/Character.cpp
        src/Items/Characters/Character.h
        src/Items/Characters/Link.cpp
        src/Items/Characters/Link.h
        src/Items/Characters/Hero.cpp
        src/Items/Characters/Hero.h
        src/Items/Armors/Armor.cpp
        src/Items/Armors/Armor.h
        src/Items/HeadEquipments/HeadEquipment.cpp
        src/Items/HeadEquipments/HeadEquipment.h
        src/Items/LegEquipments/LegEquipment.cpp
        src/Items/LegEquipments/LegEquipment.h
        src/Items/HeadEquipments/CapOfTheHero.cpp
        src/Items/HeadEquipments/CapOfTheHero.h
        src/Items/HeadEquipments/CapOfTheCharacter.cpp
        src/Items/HeadEquipments/CapOfTheCharacter.h
        src/Items/Armors/OldShirt.cpp
        src/Items/Armors/OldShirt.h
        src/Items/LegEquipments/WellWornTrousers.cpp
        src/Items/LegEquipments/WellWornTrousers.h
        src/Items/LegEquipments/CharacterTrousers.cpp
        src/Items/LegEquipments/CharacterTrousers.h
        src/Scenes/Scene.cpp
        src/Scenes/Scene.h
        src/Scenes/BattleScene.cpp
        src/Scenes/BattleScene.h
        assets/assets.qrc
        src/Items/Maps/Map.cpp
        src/Items/Maps/Map.h
        src/Items/Maps/Battlefield.cpp
        src/Items/Maps/Battlefield.h
        src/Items/Mountable.cpp
        src/Items/Mountable.h
        src/Items/Armors/FlamebreakerArmor.cpp
        src/Items/Armors/FlamebreakerArmor.h
        src/Items/Armors/BodyArmor.cpp
        src/Items/Armors/BodyArmor.h
        src/Items/Weapons/Weapon.cpp
        src/Items/Weapons/Weapon.h
        src/Items/Weapons/Pistol.cpp
        src/Items/Weapons/Pistol.h
        src/Items/Weapons/Shotgun.cpp
        src/Items/Weapons/Shotgun.h
        src/Items/Weapons/Submachine.cpp
        src/Items/Weapons/Submachine.h
        src/Items/Weapons/Knife.cpp
        src/Items/Weapons/Knife.h
        src/Items/Weapons/Ball.cpp
        src/Items/Weapons/Ball.h
        src/Items/Weapons/Bullet.cpp
        src/Items/Weapons/Bullet.h
        src/Items/Medicines/Medicine.cpp
        src/Items/Medicines/Medicine.h
        src/Items/Medicines/Bandage.cpp
        src/Items/Medicines/Bandage.h
        src/Items/Medicines/Medkit.cpp
        src/Items/Medicines/Medkit.h
        src/Items/Medicines/Adrenaline.cpp
        src/Items/Medicines/Adrenaline.h
        src/Items/Platform.cpp
        src/Items/Platform.h)
target_link_libraries(qt_2025_game
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

