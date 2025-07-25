# Qt 2D Game

һ������Qt6��2D��Ϸ��Ŀ��������ɫ��������װ����ҩƷ����ϷԪ�ء�

## ����Ҫ��

- CMake 3.28 ����߰汾
- Qt6 (Core, Gui, Widgets)
- C++17 ֧�ֵı�����

## ���ع���

### Windows (ʹ�� Visual Studio)
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### Linux/macOS
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

## GitHub Actions �Զ�����

����Ŀ���������� GitHub Actions ��������

### 1. �������� (`build.yml`)
- **��������**: ���͵� `master`/`main`/`develop` ��֧���򴴽� Pull Request
- **֧��ƽ̨**: Windows��Linux��macOS
- **����**: 
  - ��ƽ̨����
  - �ϴ���������
  - �Զ����� (�����ͱ�ǩʱ)

### 2. ���ٹ��� (`quick-build.yml`)
- **��������**: ���͵� `master`/`main` ��֧���򴴽� Pull Request
- **֧��ƽ̨**: �� Windows
- **����**: 
  - ������֤����
  - ���� Qt ��װ�Լ��ٹ���
  - �ϴ���ִ���ļ�

## �ֶ���������

�������� GitHub �ֿ�� "Actions" ��ǩҳ���ֶ�����������

1. ���� Actions ҳ��
2. ѡ����Ӧ�Ĺ�����
3. ��� "Run workflow" ��ť

## ���ع�������

������ɺ��������� Actions ҳ�����Ӧ���������ع������

- `qt-2d-game-windows` - Windows ��ִ���ļ�
- `qt-2d-game-linux` - Linux ��ִ���ļ�
- `qt-2d-game-macos` - macOS ��ִ���ļ�

## ��Ŀ�ṹ

```
������ src/                    # Դ����
��   ������ main.cpp           # �������
��   ������ MyGame.cpp/h       # ��Ϸ����
��   ������ Items/             # ��Ϸ��Ʒ
��   ��   ������ Characters/    # ��ɫ��
��   ��   ������ Weapons/       # ������
��   ��   ������ Armors/        # ������
��   ��   ������ Medicines/     # ҩƷ��
��   ��   ������ Maps/          # ��ͼ��
��   ������ Scenes/            # ��Ϸ����
������ assets/                # ��Ϸ��Դ
������ .github/workflows/     # GitHub Actions ����
```

## ��Ϸ����

- ��ɫ���ƺ��ƶ�
- ��������ϵͳ
- ����ϵͳ����ǹ������ǹ�����ǹ��ذ�ס�Ǧ��
- ҩƷϵͳ��������ҽ�ư����������أ�
- װ��ϵͳ�����ס�ͷ�������ӣ�
- ƽ̨��Ծ
- ��ײ���
- ����ֵϵͳ

## ����˵��

- ʹ�� Qt6 ͼ�ο��
- CMake ����ϵͳ
- C++17 ��׼
- ����������ģʽ
- ������ܹ�
