# 🎬 Sprite Map Viewer — DirectDraw Sprite/Map Viewer & Combo Tester

> Windows용 **DirectDraw 7 기반 스프라이트·맵 뷰어 & 콤보 테스터**.
> `dnb-sprite-tool`로 만든 `.Spr` 시트와 `dnb-map-editor`로 만든 `.Map` 파일을 **실시간 렌더링 + 충돌 박스 시각화 + 콤보 테스트**할 수 있는 도구.
> Win32 + DirectDraw 7 직접 API 사용 (MFC 미사용), 풀스크린 + 노멀 모드 지원.
> 비영리 개인 프로젝트로, 본인이 직접 작성한 C++ 소스를 공개합니다.

![platform](https://img.shields.io/badge/platform-Windows-0078d4?style=flat-square) ![lang](https://img.shields.io/badge/C%2B%2B-VC%2B%2B%206.0-00599c?style=flat-square) ![graphics](https://img.shields.io/badge/graphics-DirectDraw7-red?style=flat-square) ![input](https://img.shields.io/badge/input-Keyboard%20%2B%20%24-green?style=flat-square) ![license](https://img.shields.io/badge/license-MIT-blue?style=flat-square)

---

## 📸 관련 자료

> 🎬 **본 뷰어는 `dnb-sprite-tool` + `dnb-map-editor`의 산물인 `.Spr`/`.Map` 파일을 실시간으로 재생 + 충돌 박스를 시각화하는 도구입니다.** 폴더명 `스프라이트_맵뷰어` 자체가 그 역할을 시사합니다.

### 같은 콘솔 환경 게임 시리즈 (참고)

본 뷰어는 별도 시리즈 약어(`CS`/`BS`/`GH`/`MT`/`PG`/`ST` 등)에 포함되지 않지만, 동일 개발자가 만든 **도구 모음집**의 일부입니다.

| 약어 | 풀네임 (추정) | 시기 | 원본 위치 | 비고 |
|---|---|---|---|---|
| **CS** | Console Shooting | 2002.10 | `manual/Etc/CS.htm` | 콘솔 게임 |
| **BS** | (Three Cucumber 프로젝트) | 2003. 2~5월 | `manual/Etc/BS.htm` | FlameSword 선행 |
| **DB** | Dungeon / Debug Build | (추정) | `manual/Etc/DB.htm` | |
| **GH** | Gate of Heavens (시제품) | 2002. 4~6월 | `manual/Etc/GH.htm` | GateHeavens 선행 |
| **MT** | Map Tool (DnB 맵 에디터) | 2003. 1~2월 | `manual/Etc/MT.htm` | `dnb-map-editor` |
| **PG** | PleasureGate (시제품) | 2002. 11~12월 | `manual/Etc/PG.htm` | PleasureGate 선행 |
| **ST** | Sprite Tool (DnB 스프라이트 에디터) | 2003. 2.10~19 | `manual/Etc/ST.htm` | `dnb-sprite-tool` |
| **SMV** | **Sprite/Map Viewer (본 저장소)** | 2003~ | `Win/ToolSet/스프라이트_맵뷰어/` | **`.Spr` + `.Map` 실시간 뷰어 / 콤보 테스터** |
| **JR** | JR (어드벤처?) + Flash 데모 | (추정) | `manual/Etc/JR.htm` + `JR.swf` | |
| **BlueMB** | 부루마블대왕 (콘솔 시제품) | 2003.05 | `manual/Etc/BlueMB.htm` | |
| **DnB** | (저장소 `dnb-action-rpg`) | 2003~ | `Win/DnB/` | 2D 액션 RPG |
| **GateHeavens** | Gate of Heavens | 2003~ | `Win/GateHeavens/` | 7종 보스 |
| **PleasureGate** | (저장소 `pleasuregate-shmup`) | 2003~ | `Win/PleasureGate/` | 6종 보스 |
| **FlameSword** | (저장소 `flamesword-action`) | 2003~ | `Win/FlameSword/` | 횡스크롤 액션 |

> **MT(2003.1~2월) → ST(2003.2.10~19) → SMV(2003~)** 순서로 도구가 만들어졌습니다. SMV는 ST로 만든 `.Spr`와 MT로 만든 `.Map`를 재생/테스트하는 뷰어/플레이어 도구입니다.

---

## 📜 목차

- [개요](#-개요)
- [🎬 뷰어 컨셉](#-뷰어-컨셉)
- [🖼️ 뷰어 화면 미리보기](#️-뷰어-화면-미리보기)
- [✨ 주요 특징](#-주요-특징)
- [🖱️ 조작법](#️-조작법)
- [📂 폴더 구성](#-폴더-구성)
- [🧱 코드 모듈](#-코드-모듈)
- [🧬 시스템 아키텍처](#-시스템-아키텍처)
- [💾 데이터 모델 (.Spr / .Map)](#-데이터-모델-spr--map)
- [🎨 DirectDraw 렌더링](#-directdraw-렌더링)
- [🗺️ 맵 & 스프라이트 통합](#-맵--스프라이트-통합)
- [🔄 도구 모음집 연관 관계](#-도구-모음집-연관-관계)
- [⚙️ 빌드 환경](#-빌드-환경)
- [🔨 빌드 방법](#-빌드-방법)
- [🐛 알려진 한계](#-알려진-한계)
- [📜 라이선스](#-라이선스)
- [📁 원본 출처](#-원본-출처)

---

## 개요

**Sprite Map Viewer**는 Windows용 **DirectDraw 7 기반 스프라이트·맵 뷰어**입니다.
Win32 + DirectDraw 7 API를 직접 사용하며, MFC를 쓰지 않습니다. 풀스크린 또는 노멀 모드, ColorKey 트랜스페어런트, 4종 충돌 박스 시각화, 60 FPS 자동 애니메이션을 지원합니다.

원본 폴더에는 다음의 흥미로운 자료가 들어 있습니다.

- **Win32 + DirectDraw 7 직접 사용** (MFC 미사용)
- **풀스크린 / 노멀 모드** (`InItDDraw` / `InItDDrawNormal`)
- **3종 핵심 데이터 구조체** (`MapH`, `TileH`, `ActionH`, `FrameH`)
- **4종 충돌 박스 시각화** (`DefaultRect` / `DefenseRect` / `AttackRect` / `EtcRect`)
- **`MapChkView` / `MapChk_Rt`** — 액션 단위 충돌 박스
- **자동/수동 프레임 모드** (`AutoFrame` 매개변수)
- **Timer + delta time** — 50 FPS 기본
- **테스트 데이터** (`TestSpr.spr` + `_TestSpr.spr` + `MapTest.Map`)
- **이름: "HelloDX"** — 초기 학습 프로젝트의 흔적

---

## 🎬 뷰어 컨셉

- **용도**: `.Spr` 스프라이트 시트 + `.Map` 맵 데이터 실시간 재생 + 충돌 박스 시각화
- **플랫폼**: Windows (Win32)
- **UI**: 순수 Win32 (WinMain + WndProc, MFC 미사용)
- **언어**: C++ (VC++ 6 + DirectX 7 SDK)
- **그래픽**: DirectDraw 7 (IDirectDraw7 + IDirectDrawSurface7)
- **해상도**: 800×600 (풀스크린 또는 노멀 창)
- **테스트 데이터**: 2개 `.spr` 파일 + 1개 `.Map` 파일
- **이름**: `HelloDX` (개발자의 첫 DirectDraw 학습 프로젝트 흔적)

---

## 🖼️ 뷰어 화면 미리보기

```text
┌─────────────────────────────────────────────────────────────────────┐
│ File  Edit  View  Insert  Format  Help                              │ ← Win32 메뉴
│ ┌─┬─┬─┬─┬─┐                                                          │
│ │📁│💾│◀│▶│▶│                                                          │ ← 툴바
│ └─┴─┴─┴─┴─┘                                                          │
├─────────────────────────────────────────────────────────────────────┤
│ ┌─────────────[Map Display 800×600]──────────────────────────────┐  │
│ │ 🟦🟦🟦🟦⬛⬛⬛🟦🟦🟦🟦🟦⬛⬛⬛🟦🟦🟦⬛⬛                         │  │
│ │ 🟦🟦🟦🟦⬛⬛⬛🟦🟦🟦🟦🟦⬛⬛⬛🟦🟦🟦⬛⬛  ←  타일 64×64        │  │
│ │ 🟦🟦🟦🟦🟨🟨🟨🟦🟦🟦🟦🟦🟦🟦🟦🟦🟦🟦🟦                          │  │
│ │ 🟦🟦🟦🟦🟨🟨🟨🟦🟦🟦🟦🟦🟦🟦🟦🟦🟦🟦🟦  ←  타일 어트리뷰트 0x36  │  │
│ │ 🟦🟦🟦🟦🟨🟨🟨🟦🟦🟦🟦🟦🟦🟦🟦🟦🟦🟦🟦                          │  │
│ │ 🟦🟦🟦🟦⬛⬛⬛🟦🟦🟦🟦🟦⬛⬛⬛🟦🟦🟦⬛⬛                          │  │
│ │                                                                   │  │
│ │              ⬛⬛⬛                                                │  │
│ │              ⬛🦸⬛  ← 스프라이트 (m_Ation[0].pFrame[0])         │  │
│ │              ⬛⬛⬛                                                │  │
│ │              ┌──────┐                                              │  │
│ │              │ 🟧🟧 │  ← AttackRect (m_RectView = 4)              │  │
│ │              └──────┘                                              │  │
│ │              ┌──────┐                                              │  │
│ │              │ 🟦🟦 │  ← DefaultRect (m_RectView = 2)             │  │
│ │              └──────┘                                              │  │
│ │              ┌──────┐                                              │  │
│ │              │ 🟩🟩 │  ← DefenseRect (m_RectView = 3)            │  │
│ │              └──────┘                                              │  │
│ │              ┌──────┐                                              │  │
│ │              │ ⬜⬜ │  ← EtcRect (m_RectView = 5)                 │  │
│ │              └──────┘                                              │  │
│ │              ┌──────┐                                              │  │
│ │              │ ⬛🟥 │  ← MapChk_Rt (액션 단위 충돌 박스)         │  │
│ │              └──────┘                                              │  │
│ │                                                                   │  │
│ │         🌳🌳🌳🌳🌳🌳🌳🌳🌳🌳🌳🌳🌳🌳🌳🌳🌳🌳🌳🌳🌳  ← 타일/배경      │  │
│ └───────────────────────────────────────────────────────────────────┘  │
│  Sprite: walk  Action: 0  Frame: 5/12  FPS: 50  X,Y: 100,200          │ ← 상태바
└─────────────────────────────────────────────────────────────────────┘
```

> ⚠️ 위 다이어그램은 README용 ASCII 일러스트이며 실제 뷰어는 DirectDraw 7 + Win32로 구현되어 있습니다.

---

## ✨ 주요 특징

| 영역 | 설명 |
|---|---|
| 🖼️ **DirectDraw 7** | IDirectDraw7 + IDirectDrawSurface7 풀 래퍼 |
| 🪟 **Win32 (MFC 미사용)** | WinMain + WndProc + 표준 윈도우 |
| 🖥️ **풀스크린 모드** | `DDSCL_EXCLUSIVE` + `SetDisplayMode` |
| 🪟 **노멀 모드** | `DDSCL_NORMAL` + 클리퍼 |
| 🎬 **실시간 애니메이션** | 50 FPS 자동 프레임 + 수동 프레임 모드 |
| 🎨 **ColorKey** | 액션별 RGB ColorKey + `DD_OffScreen` flag=1 |
| 🧱 **4종 충돌 박스** | DefaultRect / DefenseRect / AttackRect / EtcRect |
| 🟦 **MapChkView (액션 충돌 박스)** | `MapChk_Rt` 시각화 |
| 🗺️ **3중 레이어 맵** | `m_Tile[MaxArr]` (배경/오브젝트/이벤트) |
| 🧱 **맵 어트리뷰트** | `Attr[100][100]` 8비트 값 |
| 💾 **DIB 헬퍼** | 8/16/24/32비트 BMP 변환 + 555/565 RGB 변환 |
| 🖱️ **마우스 입력** | `OnLButtonDown` / `OnMouseMove` / `OnLButtonUp` / `OnRButtonUp` |
| ⌨️ **키보드 입력** | `KeyDown` / `KeyUp` 매크로 + `GetAsyncKeyState` |
| 💾 **테스트 데이터** | `TestSpr.spr` + `_TestSpr.spr` + `MapTest.Map` |
| 🖱️ **파일 다이얼로그** | `commdlg.h` `GetOpenFileName` |

---

## 🖱️ 조작법

| 입력 | 동작 |
|---|---|
| **마우스 좌클릭** | 스프라이트 위치 선택 / RECT 그리기 |
| **마우스 좌클릭 + 드래그** | RECT 연속 그리기 |
| **마우스 우클릭** | RECT 선택 / 메뉴 |
| **마우스 이동** | 좌표 표시 / 호버링 미리보기 |
| `←` `→` `↑` `↓` | 스프라이트 이동 (KeyDown) |
| `Z` `X` `C` `V` | 액션 변경 (확인 필요) |
| `Esc` | 종료 / 메뉴 |
| `Enter` | 확인 |
| **메뉴** | File / Edit / View / Insert / Format / Help |

**키 매크로** (`DirectDraw.h`):

```cpp
#define KeyDown(vk_code) ((GetAsyncKeyState(vk_code)) &0x8000 ? 1 : 0)
#define KeyUp(vk_code)   ((GetAsyncKeyState(vk_code)) &0x8000 ? 0 : 1)
```

**m_RectView 모드** (충돌 박스 시각화 모드):

| 값 | 표시 |
|---|---|
| 0 | 없음 |
| 1 | MapChkView (액션 단위 충돌 박스) |
| 2 | DefaultRectView |
| 3 | DefenseRectView |
| 4 | AttackRectView |
| 5 | EtcRectView |

---

## 📂 폴더 구성

```text
스프라이트_맵뷰어/ (sprite-map-viewer)
├── 📄 DirectDraw.cpp / .h                            ← WinMain + WndProc + DX 초기화
├── 📄 DirectDraw.dsp / .dsw                          ← VC++ 6 프로젝트
├── 📄 DirectDraw.rc                                  ← Win32 리소스
├── 📄 DirectDraw.exe                                 ← (제외) 빌드된 실행 파일
│
├── 📁 엔진 헬퍼
│   ├── SetDDraw.cpp / .h                             ← DirectDraw 7 풀 래퍼 (SETDDRAW 클래스)
│   ├── DIB.cpp / .h                                  ← DIB (BMP 변환 + 555/565 RGB)
│   ├── Timer.cpp / .h                                ← FPS + delta time Timer
│   ├── Map.cpp / .h                                  ← CMAP (맵 로드/충돌)
│   ├── Sprite.cpp / .h                               ← CSprite (.spr 시트)
│   └── CObject.cpp / .h                              ← CObject (스프라이트 인스턴스 + 충돌)
│
├── 📁 테스트 데이터
│   ├── TestSpr.spr                                   ← 스프라이트 시트 #1
│   ├── _TestSpr.spr                                  ← 스프라이트 시트 #2
│   └── MapTest.Map                                   ← 64×64 타일, 20×12 셀 맵
│
├── 📁 빌드 / 환경
│   ├── resource.h                                    ← 리소스 ID
│   ├── Script1.rc                                    ← 추가 리소스 스크립트
│   ├── Debug/                                        ← (제외) 빌드 산출물
│   ├── DirectDraw.aps / .suo                         ← (제외) 리소스 / 사용자 환경
│   ├── DirectDraw.ncb / .opt / .plg / .positions     ← (제외) VC++ 6 환경 파일
│   ├── ALZ9210.TMP                                   ← (제외) 알집 임시 파일
│   └── ~VC*.tmp                                      ← (제외) VS 임시 파일
│
└── 📁 docs/
    └── SMV-screenshot-2003.jpg                       ← (없음, 스크린샷 없음)
```

> 참고: 본 저장소에는 `docs/` 폴더만 만들었고 스크린샷은 포함하지 않았습니다 (원본에 스크린샷이 없음). 시리즈 부록 스크린샷은 README에서 링크로만 참조합니다.

---

## 🧱 코드 모듈

### 응용 프로그램 진입점 (`DirectDraw.cpp`)

```cpp
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SetDlgProc(HWND, UINT, WPARAM, LPARAM);
void DD_Rotat(int x, int y, int r);

HWND g_hWnd;
BOOL GAME = 1;

LPDIRECTDRAWSURFACE7 lpOff_sb;

int edit_x = 0, edit_y = 0;
RECT g_rectWindow;

void DDTextOut();
void DD_DibDraw();
void DDBit(int width, int height, CDIB *dib, LPDIRECTDRAWSURFACE7 lpOffscreen);
void DrawPlayer();
void DrawTester();

void OpenSprite(char* path);
void OpenMap(char* path);

BYTE g_TActionFlag;
BOOL g_Map_Attr_View;
WORD g_Sel_Action;
WORD g_Sel_FPS = 50;
HINSTANCE g_hInst;

SETDDRAW *SetDDraw = new SETDDRAW;
CMAP     *g_Map = new CMAP;
CSprite  *g_player_Spr = new CSprite;
CObject  *g_player = new CObject;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    static char szAppName[] = "HelloDX";
    // ...
}
```

**전형적인 DirectDraw 학습 프로젝트 구조**: WinMain + WndProc + 글로벌 변수 + 헬퍼 클래스 인스턴스. "HelloDX"라는 이름은 이 프로젝트가 **개발자의 첫 DirectDraw 학습/테스트**였음을 시사합니다.

### SETDDRAW (DirectDraw 래퍼) — `SetDDraw.h`

```cpp
class SETDDRAW {
protected:
    static int m_nInit;            // 단일 인스턴스 카운터
    HWND     m_hwnd;
    HRESULT  m_hResult;
    DDBLTFX  m_ddbf;
    LPDIRECTDRAW7           m_lpdd7;
    LPDIRECTDRAWSURFACE7    m_lpPrimary;
    LPDIRECTDRAWSURFACE7    m_lpBack;
    LPDIRECTDRAWSURFACE7    m_lpBackOffScreen;
    LPDIRECTDRAWCLIPPER     m_lpClipper;
    LPDIRECTDRAWCLIPPER     m_lpClipper2;

    BOOL m_Normal;
    int  m_Screen_Width, m_Screen_Heigh, m_BitColor;
    int  m_Display_Width, m_Display_Heigh, m_Display_BitColor;

public:
    SETDDRAW();
    ~SETDDRAW();

    BOOL InItDDraw(HWND, int Width, int Heigh, int BitColor);            // 풀스크린
    BOOL InItDDrawNormal(HWND, int Width, int Heigh, int BitColor);     // 노멀 창
    void ConvertWin(HWND, int Width, int Heigh, int BitColor);

    void DD_Restore();         // 서피스가 손실되면 복구
    void DD_BackColor(int color);
    void DD_OffScreen(DWORD, DWORD, LPDIRECTDRAWSURFACE7*, CDIB* = NULL, BOOL flag = 0, WORD colorKey = 0);
    // flag=1 → DDCOLORKEY 사용
};
```

**핵심 기능**:
- **풀스크린 모드** (`InItDDraw`) — `DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN` + `SetDisplayMode`
- **노멀 창 모드** (`InItDDrawNormal`) — `DDSCL_NORMAL` + 클리퍼
- **오프스크린 서피스** + **ColorKey** 트랜스페어런트
- **서피스 복구** — Alt+Tab 후 DirectDraw 서피스가 손실되면 `DD_Restore`로 복구
- **백버퍼 플립** — Primary + Back Buffer

### DIB (Device Independent Bitmap) — `DIB.h`

```cpp
#define Convert555(R, G, B) ((R >> 3) << 10 | (G >> 3) << 5 | (B >> 3))   // 16비트 RGB555
#define Convert565(R, G, B) ((R >> 3) << 11 | (G >> 2) << 5 | (B >> 3))   // 16비트 RGB565
#define offmax 100

class CDIB {
public:
    void Convert32To16(BOOL flag);
    void Convert24To16(BOOL flag);
    void Convert8To16(BOOL flag);
    void Convert16(BOOL flag);
    DWORD GetSizeImage();
    BYTE* GetImage();
    BOOL IsSuccessed();
    LONG GetHeight(), GetWidth();
    void Draw(HDC hdc, POINT point);
    void ConvertDDB(HDC hdc);    // DIB → DirectDraw Blt
    void RemoveAll();
    // ... 파일 버전도 동일
};
```

**BMP 색상 포맷 변환** — 8/16/24/32비트 BMP를 16비트 RGB555/565로 변환. GP32 게임과 DirectDraw 7에서 사용.

### Timer — `Timer.h`

```cpp
class Timer {
public:
    Timer();
    void SetFPS(DWORD fps);     // FPS 설정
    void Update();              // 현재 시간으로 BeforeTime 갱신
    bool GetDrawChance();       // 프레임 그릴 타이밍이면 true

private:
    DWORD FPS;                  // Frame Per Second 값
    DWORD BeforeTime;           // 이전 프레임 시간
    DWORD CurrentTime;          // 현재 프레임 시간
};
```

**delta time 기반** 프레임 타이밍. `GetDrawChance()`는 `FPS`와 시간 차이를 비교해 그릴 타이밍 결정.

### CMAP (맵) — `Map.h`

```cpp
#define MapArrMax_X 100
#define MapArrMax_Y 100
#define MaxArr      3

#define LEFT  1
#define TOP   2
#define RIGHT 3
#define BOTTOM 4

struct MapH {
    int   MaxTile;
    int   TileSizeXCount;
    int   TileSizeX, TileSizeY;
    int   MapSizeX, MapSizeY;
    BYTE  Attr[MapArrMax_X][MapArrMax_Y];  // 100×100 어트리뷰트
};

struct TileH {
    BYTE  MapArr[MapArrMax_X][MapArrMax_Y];
    BYTE  Tile[MapArrMax_X][MapArrMax_Y];
};

class CMAP : public CDIB {
public:
    CMAP();
    ~CMAP();
    BYTE Mapchk(CObject* Obj, WORD val, WORD Action, WORD Frame = 0);
    void MapLoad(char* path);
    void DrawMap();
    void SetDrawBase(SETDDRAW* DDrawBase);

public:
    MapH  m_Map;
    TileH m_Tile[MaxArr];      // 3중 레이어
    LPDIRECTDRAWSURFACE7* lpOff_map;
    int   m_EditX, m_EditY;
};
```

`dnb-map-editor`(1000×1000)와 달리 **100×100** 타일 한정 (뷰어/테스트 도구이므로).

### CSprite (스프라이트) — `Sprite.h`

```cpp
struct FrameH {
    WORD   FileID, ActionID;
    POINT  Center;          // 중심점
    WORD   Delay;
    RECT   Img_Rt;          // 이미지 영역

    WORD   MaxDefault, MaxDefense, MaxAttack, MaxEtc;  // 4종 충돌 박스 개수
    RECT*  pDefault_Rt;     // 기본 충돌
    RECT*  pDefense_Rt;     // 방어 충돌
    RECT*  pAttack_Rt;      // 공격 히트박스
    RECT*  pEtc_Rt;         // 기타
};

struct ActionH {
    WORD    MaxFrame;
    BYTE    R, G, B;        // ColorKey (RGB)
    RECT    MapChk_Rt;      // 액션 단위 충돌 박스
    FrameH* pFrame;         // 프레임 배열
};

class CSprite : public CDIB {
public:
    CSprite();
    ~CSprite();
    void LoadSprite(char* path);
    void SetDrawBase(SETDDRAW* DDrawBase);

public:
    SETDDRAW* m_SetDDraw;
    ActionH*  m_Ation;
    WORD      m_MaxImage, m_MaxAction;
    LPDIRECTDRAWSURFACE7* m_lpOff_sprite;
};
```

**`dnb-sprite-tool`의 데이터 구조와 거의 동일** — `AniFrame`/`AinAction` ↔ `FrameH`/`ActionH`. `pDefault_Rt` 등 4종 충돌 박스 동일.

### CObject (스프라이트 인스턴스) — `CObject.h`

```cpp
class CObject {
protected:
    void MapChkView(WORD Action);
    void DefaultRtView(WORD Action, WORD Frame);
    void DefenseRtView(WORD Action, WORD Frame);
    void AttackRtView(WORD Action, WORD Frame);
    void EtcRtView(WORD Action, WORD Frame);
    Timer* m_Timer;

public:
    CObject();
    ~CObject();

    void SetSpriteBase(CSprite* SpriteBase) { m_Sprite = SpriteBase; }

    BOOL DrawSprite(int x, int y, WORD Action, WORD Frame = 0,
                    BOOL AutoFrame = 0, WORD SetFps = 50,
                    DDBLTFX* ddbfx = 0,
                    int flag = DDBLT_WAIT | DDBLT_KEYSRC);

    RECT GetDefaultRect(WORD Action, WORD Frame);
    RECT GetDefenseRect(WORD Action, WORD Frame);
    RECT GetAttackRect(WORD Action, WORD Frame);
    RECT GetEtcRect(WORD Action, WORD Frame);

    inline int Get_CX(WORD Action, WORD Frame = 0)
    { return m_X - m_Sprite->m_Ation[Action].pFrame[m_FrameCount].Center.x; }
    inline int Get_CY(WORD Action, WORD Frame = 0)
    { return m_Y - m_Sprite->m_Ation[Action].pFrame[m_FrameCount].Center.y; }

    BOOL Collision_Chk_Rect(CObject* pObj1, WORD Action1, WORD Frame1,
                            CObject* pObj2, WORD Action2, WORD Frame2, BYTE flag);

    int  m_X, m_Y;             // 좌표
    int  m_FrameCount;          // 현재 프레임
    int  m_Move;                // 이동량
    BOOL m_jump;                // 점프 중
    int  m_jumpPower, m_gravity;// 점프·중력
    CSprite* m_Sprite;          // 참조 스프라이트
    BYTE m_RectView;            // 충돌 박스 시각화 모드 (1=MapChk, 2=Default, 3=Defense, 4=Attack, 5=Etc)
};
```

**핵심**:
- **4종 충돌 박스 getter** — `GetDefaultRect` / `GetDefenseRect` / `GetAttackRect` / `GetEtcRect`
- **MapChkView** — `m_Sprite->m_Ation[Action].MapChk_Rt`를 시각화
- **점프 + 중력 시스템** — `m_jump`, `m_jumpPower`, `m_gravity` (콤보 테스터에서 사용 추정)
- **자동/수동 프레임** — `AutoFrame` 매개변수
- **충돌 검사** — `Collision_Chk_Rect` (RECT vs RECT)

### `DrawSprite` 핵심 로직 (`CObject.cpp`)

```cpp
BOOL CObject::DrawSprite(int x, int y, WORD Action, WORD Frame,
                        BOOL AutoFrame, WORD SetFps, DDBLTFX* ddbfx, int flag) {
    m_Timer->SetFPS(SetFps);
    m_Timer->Update();

    if (AutoFrame == 1) {
        // 자동 프레임 (애니메이션)
        m_Sprite->m_SetDDraw->DDBit(
            x - m_Sprite->m_Ation[Action].pFrame[m_FrameCount].Center.x,
            y - m_Sprite->m_Ation[Action].pFrame[m_FrameCount].Center.y,
            &m_Sprite->m_Ation[Action].pFrame[m_FrameCount].Img_Rt,
            m_Sprite->m_lpOff_sprite[m_Sprite->m_Ation[Action].pFrame[m_FrameCount].FileID],
            flag, ddbfx);

        if (m_RectView == 1) MapChkView(Action);
        if (m_RectView == 2) DefaultRtView(Action, m_FrameCount);
        if (m_RectView == 3) DefenseRtView(Action, m_FrameCount);
        if (m_RectView == 4) AttackRtView(Action, m_FrameCount);
        if (m_RectView == 5) EtcRtView(Action, m_FrameCount);
    } else {
        // 수동 프레임 (정지 이미지)
        // ... (동일 구조)
    }

    if (m_FrameCount >= m_Sprite->m_Ation[Action].MaxFrame - 1) {
        m_FrameCount = 0;
        return 1;  // 애니메이션 끝
    }

    if ((m_Timer->GetDrawChance() == 1) && (AutoFrame == 1)) m_FrameCount++;

    return 0;
}
```

**핵심 기능**:
- **자동/수동 프레임** 두 모드
- **FPS 기반 타이밍** — `m_Timer->GetDrawChance()`로 프레임 진행
- **5종 시각화 모드** — MapChk/Default/Defense/Attack/Etc
- **센터 좌표 보정** — `m_Sprite->m_Ation[Action].pFrame[Frame].Center.x/y`로 스프라이트 중심 보정

---

## 🧬 시스템 아키텍처

### Win32 + DirectDraw 7 구조

```text
  ┌──────────────────────────────────────────┐
  │  WinMain (HelloDX)                       │  ← 응용 프로그램 진입점
  │  ├─ WNDCLASSEX 등록                      │
  │  ├─ CreateWindow (szAppName="HelloDX")   │
  │  ├─ SetDDraw::InItDDraw / InItDDrawNormal
  │  └─ 메시지 루프 (GetMessage/Dispatch)    │
  └──────────────────────────────────────────┘
                │
                ▼
  ┌──────────────────────────────────────────┐
  │  WndProc (HWND, UINT, WPARAM, LPARAM)    │  ← 윈도우 프로시저
  │  ├─ WM_CREATE → SETDDRAW 초기화          │
  │  ├─ WM_PAINT → DrawPlayer / DrawTester   │
  │  ├─ WM_KEYDOWN / WM_KEYUP               │
  │  ├─ WM_LBUTTONDOWN / WM_MOUSEMOVE        │
  │  └─ WM_DESTROY → Release                 │
  └──────────────────────────────────────────┘
                │
                ▼
  ┌──────────────────────────────────────────┐
  │  SETDDRAW (DirectDraw 7 래퍼)            │
  │  ├─ IDirectDraw7                         │
  │  ├─ m_lpPrimary + m_lpBack (더블 버퍼) │
  │  ├─ m_lpClipper (노멀 모드)              │
  │  └─ DD_OffScreen (오프스크린)            │
  └──────────────────────────────────────────┘
                │
                ├─→ CMAP (맵)
                ├─→ CSprite (스프라이트 시트)
                └─→ CObject (스프라이트 인스턴스)
```

### 데이터 흐름 (한 프레임)

```text
[키보드/마우스 입력]
    │
    ▼
WndProc → SetDDraw → 화면 갱신
    │
    ├─ OpenSprite / OpenMap (파일 로드)
    │
    ▼
CObject::DrawSprite(x, y, Action, Frame, AutoFrame=1, FPS=50)
    │
    ├─ m_Timer->SetFPS(50) + Update()
    │
    ├─ m_Sprite->m_SetDDraw->DDBit()  ← DirectDraw 비트 전송
    │   ├─ 백 버퍼에 스프라이트 그림
    │   └─ ColorKey로 트랜스페어런트
    │
    ├─ if m_RectView == 2: DefaultRtView() ← 4종 충돌 박스 시각화
    ├─ if m_RectView == 3: DefenseRtView()
    ├─ if m_RectView == 4: AttackRtView()
    └─ if m_RectView == 5: EtcRtView()
    │
    ▼
SETDDRAW::Flip() 또는 BitBlt
    │
    ▼
[화면 출력]
```

### DirectDraw 초기화 (풀스크린)

```cpp
BOOL SETDDRAW::InItDDraw(HWND hWnd, int Width, int Heigh, int BitColor) {
    m_Screen_Width = Width;
    m_Screen_Heigh = Heigh;
    m_BitColor = BitColor;

    if (m_nInit == 0) {
        m_hResult = DirectDrawCreateEx(0, (LPVOID*)&m_lpdd7, IID_IDirectDraw7, 0);
        if (m_hResult != DD_OK) { ErrorMessage("DirectDraw 생성 실패"); return 0; }
        m_nInit++;
    }

    m_hResult = m_lpdd7->SetCooperativeLevel(hWnd,
        DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    if (m_hResult != DD_OK) { ErrorMessage("협력레벨 설정 실패"); return 0; }

    m_hResult = m_lpdd7->SetDisplayMode(Width, Heigh, BitColor, 0, 0);

    DDSURFACEDESC2 ddsd2;
    ZeroMemory(&ddsd2, sizeof(ddsd2));
    ddsd2.dwSize = sizeof(ddsd2);
    ddsd2.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd2.dwBackBufferCount = 1;
    ddsd2.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
    m_hResult = m_lpdd7->CreateSurface(&ddsd2, &m_lpPrimary, 0);

    DDSCAPS2 ddscaps2;
    ZeroMemory(&ddscaps2, sizeof(ddscaps2));
    ddscaps2.dwCaps = DDSCAPS_BACKBUFFER;
    m_hResult = m_lpPrimary->GetAttachedSurface(&ddscaps2, &m_lpBack);

    // Clipper for normal mode
    m_lpdd7->CreateClipper(0, &m_lpClipper, 0);
    RGNDATA* pRgn = ...;
    m_lpClipper->SetClipList(pRgn, 0);
    m_lpBack->SetClipper(m_lpClipper);

    m_Normal = 0;
    return 1;
}
```

---

## 💾 데이터 모델 (.Spr / .Map)

### `.Spr` 파일 포맷 (CSprite / CObject 모델)

```text
┌─ DC Header ─────────────┐
│  BMP DC + 파일명         │
└──────────────────────────┘
              │
              ▼
┌─ Action[MaxAction] ─────┐
│  MaxFrame                │  ← 이 액션의 프레임 수
│  R, G, B                 │  ← ColorKey
│  MapChk_Rt               │  ← 액션 단위 충돌 박스
│  pFrame[MaxFrame]        │  ← 프레임 배열
└──────────────────────────┘
              │
              ▼
┌─ Frame[MaxFrame] ────────┐
│  FileID                  │  ← 참조할 BMP 인덱스
│  ActionID                │
│  Center (POINT)          │  ← 스프라이트 중심점
│  Delay                   │  ← 프레임 지연 (ms)
│  Img_Rt (RECT)           │  ← 소스 스프라이트 시트 내 영역
│                           │
│  MaxDefault              │  ← 기본 충돌 박스 개수
│  MaxDefense              │  ← 방어 충돌 박스 개수
│  MaxAttack               │  ← 공격 히트박스 개수
│  MaxEtc                  │  ← 기타 충돌 박스 개수
│                           │
│  pDefault_Rt[MaxDefault] │  ← 가변 RECT 배열
│  pDefense_Rt[MaxDefense]
│  pAttack_Rt[MaxAttack]
│  pEtc_Rt[MaxEtc]
└──────────────────────────┘
```

### `.Map` 파일 포맷 (`MapTest.Map` 실제 헤더)

```text
┌─ MapH ─────────────────┐
│  MaxTile         (DWORD) │  = 4 (0x04000000)
│  TileSizeXCount  (DWORD) │  = 8 (0x08000000)
│  TileSizeX       (DWORD) │  = 64 (0x40000000)
│  TileSizeY       (DWORD) │  = 64 (0x40000000)
│  MapSizeX        (DWORD) │  = 20 (0x14000000)
│  MapSizeY        (DWORD) │  = 12 (0x0c000000)
│  Attr[100][100]  (BYTE) │  ← 어트리뷰트 매트릭스
│                           │  예: Attr(0,0)=0x36, Attr(0,1)=0x28
└──────────────────────────┘
              │
              ▼
┌─ TileH[3] ───────────────┐
│  MapArr[100][100] (BYTE) │  ← 메인 타일/오브젝트 인덱스
│  Tile[100][100]   (BYTE) │  ← 보조 데이터
└──────────────────────────┘
```

**`MapTest.Map` 헤더 16진 덤프**:

```text
00000000: 0400 0000 0800 0000 4000 0000 4000 0000
00000010: 1400 0000 0c00 0000 3636 2800 0000 0000
00000020: 0002 0000 0001 0000 0100 1800 0000 0000
```

해석:
- `0x00000004` (little-endian) = MaxTile **4**
- `0x00000008` = TileSizeXCount **8**
- `0x00000040` = TileSizeX **64** (픽셀)
- `0x00000040` = TileSizeY **64** (픽셀)
- `0x00000014` = MapSizeX **20** (셀)
- `0x0000000c` = MapSizeY **12** (셀)
- 이후 Attr 매트릭스: `0x36 0x36 0x28 ...` (셀 (0,0)=0x36, (0,1)=0x36, (0,2)=0x28, ...)

### 4종 충돌 박스 데이터 흐름

```text
[Sprite Tool .Spr 출력]
   │
   ▼
[CSprite::LoadSprite]
   │
   ├─ FrameH 읽기
   │  ├─ pDefault_Rt[MaxDefault]
   │  ├─ pDefense_Rt[MaxDefense]
   │  ├─ pAttack_Rt[MaxAttack]
   │  └─ pEtc_Rt[MaxEtc]
   │
   ▼
[CObject::DrawSprite]
   │
   ├─ MapChkView(Action)        ← ActionH::MapChk_Rt
   ├─ DefaultRtView(Action, Frame)  ← FrameH::pDefault_Rt
   ├─ DefenseRtView(Action, Frame)  ← FrameH::pDefense_Rt
   ├─ AttackRtView(Action, Frame)   ← FrameH::pAttack_Rt
   └─ EtcRtView(Action, Frame)     ← FrameH::pEtc_Rt
```

### 충돌 검사

```cpp
BOOL CObject::Collision_Chk_Rect(
    CObject* pObject1, WORD Action1, WORD Frame1,
    CObject* pObject2, WORD Action2, WORD Frame2,
    BYTE flag) {
    // flag: 1=일반충돌, 2=방어충돌, 3=공격충돌, 4=기타
    RECT rt1, rt2;
    switch(flag) {
        case 1: rt1 = pObject1->GetDefaultRect(Action1, Frame1); break;
        case 2: rt1 = pObject1->GetDefenseRect(Action1, Frame1); break;
        case 3: rt1 = pObject1->GetAttackRect(Action1, Frame1); break;
        case 4: rt1 = pObject1->GetEtcRect(Action1, Frame1); break;
    }
    // ... RECT vs RECT 충돌 검사
}
```

---

## 🎨 DirectDraw 렌더링

### 오프스크린 서피스 + ColorKey

```cpp
void SETDDRAW::DD_OffScreen(DWORD dwWidth, DWORD dwHeight,
                              LPDIRECTDRAWSURFACE7* lpOffScreen,
                              CDIB* Dib = NULL,
                              BOOL flag = 0,
                              WORD colorKey = 0) {
    // 오프스크린 서피스 생성
    DDSURFACEDESC2 ddsd2;
    ZeroMemory(&ddsd2, sizeof(ddsd2));
    ddsd2.dwSize = sizeof(ddsd2);
    ddsd2.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    ddsd2.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    ddsd2.dwWidth = dwWidth;
    ddsd2.dwHeight = dwHeight;
    m_lpdd7->CreateSurface(&ddsd2, lpOffScreen, 0);

    if (flag == 1) {
        // ColorKey 사용
        DDCOLORKEY ddck;
        ddck.dwColorSpaceLow = colorKey;
        ddck.dwColorSpaceHigh = colorKey;
        (*lpOffScreen)->SetColorKey(DDCKEY_SRCBLT, &ddck);
    }
}
```

### DIB → DirectDraw Blt

```cpp
void CDIB::ConvertDDB(HDC hdc) {
    // DIB를 DirectDraw 서피스로 전송
    StretchDIBits(hdc, 0, 0, GetWidth(), GetHeight(),
                  0, 0, GetWidth(), GetHeight(),
                  GetImage(), GetBitmapInfo(), DIB_RGB_COLORS, SRCCOPY);
}
```

### RGB 555/565 변환

```cpp
#define Convert555(R, G, B) ((R >> 3) << 10 | (G >> 3) << 5 | (B >> 3))
#define Convert565(R, G, B) ((R >> 3) << 11 | (G >> 2) << 5 | (B >> 3))
```

**8/24/32비트 BMP**를 **16비트 RGB555/565**로 변환. GP32 게임의 16비트 컬러 모드와 호환.

---

## 🗺️ 맵 & 스프라이트 통합

본 뷰어의 동작 흐름:

```text
[파일 열기]
    │
    ├─ OpenSprite("TestSpr.spr")
    │   └─ CSprite::LoadSprite → m_Ation / m_lpOff_sprite 설정
    │
    └─ OpenMap("MapTest.Map")
        └─ CMAP::MapLoad → m_Map / m_Tile[3] 설정

[렌더링]
    │
    ├─ CMAP::DrawMap()
    │   └─ SETDDRAW::DDBit() × (MapSizeX × MapSizeY × 3 layers)
    │
    └─ CObject::DrawSprite(x, y, Action, Frame)
        ├─ 스프라이트 시트에서 (Action, Frame)의 (Img_Rt, Center) 추출
        ├─ Center 좌표로 보정하여 화면에 그림
        └─ if m_RectView: 4종 충돌 박스 시각화

[충돌 검사]
    │
    └─ CObject::Collision_Chk_Rect(pObj1, pObj2, flag)
        └─ RECT vs RECT AABB 충돌 검사
```

---

## 🔄 도구 모음집 연관 관계

본 뷰어는 다음 도구/게임과 직접 연관됩니다.

### 도구 모음집 (`Win/ToolSet/`)

| 폴더 | 역할 | 저장소 |
|---|---|---|
| `맵툴/` | MFC SDI 타일 맵 에디터 | `dnb-map-editor` |
| `스프라이트툴/` | MFC SDI 스프라이트 에디터 | `dnb-sprite-tool` |
| `스프라이트_맵뷰어/` | **DirectDraw 뷰어/콤보 테스터 (본 저장소)** | `sprite-map-viewer` |

### 게임 프로젝트 (스프라이트/맵 사용처)

| 게임 | 스프라이트 형식 | 맵 형식 | 도구 |
|---|---|---|---|
| `dnb-action-rpg` | `.Spr` (4종 충돌 박스) | `.Map` (24개 맵) | ST/MT/SMV |
| `flamesword-action` | `.Spr` | `.Map` | ST/MT/SMV |
| `pleasuregate-shmup` | `.Spr` | (다중 레이어) | ST/MT/SMV |
| `gateheavens-shmup` | `.Spr` (다중 시트) | (인체 부위 시스템) | ST/SMV |

### 데이터 포맷 호환성

| 항목 | `dnb-sprite-tool` | 본 뷰어 (SMV) | `dnb-action-rpg` 게임 |
|---|---|---|---|
| `.Spr` 프레임 구조 | `AniFrame` | `FrameH` | (게임 로더) |
| 4종 충돌 박스 | `rt_Default` 등 4개 | `pDefault_Rt` 등 4개 | 충돌 판정 |
| ColorKey (RGB) | `R, G, B` | `R, G, B` | 트랜스페어런트 |
| `.Map` 헤더 | `MapH + TileH[3]` | `MapH + TileH[3]` (MaxArr=3) | `Data/Map/StageXX-YY_Map.Map` |
| 최대 타일 수 | 1000×1000 | 100×100 | (맵마다 다름) |
| Frame Center | (스프라이트툴엔 없음) | `Center` (POINT) | 위치 보정 |

> 본 뷰어의 `FrameH.Center` 필드는 **게임 런타임의 스프라이트 위치 보정**에 사용됩니다. `dnb-sprite-tool`에는 없는 필드로, **실제 게임 런타임(DirectX)에서 필요해서 뷰어/플레이어 측에 추가된 것으로 보입니다.**

---

## ⚙️ 빌드 환경

| 항목 | 값 |
|---|---|
| 플랫폼 | Windows (Win32) |
| 언어 | C++ (VC++ 6 스타일) |
| 그래픽 | DirectDraw 7 SDK |
| UI | **순수 Win32 (MFC 미사용)** |
| 컴파일러 | Microsoft Visual C++ 6.0 |
| 프로젝트 파일 | `DirectDraw.dsp`, `DirectDraw.dsw` |
| 해상도 | 800×600 |
| 문자 집합 | 멀티바이트 (한글 주석) |
| 플랫폼 | x86 (32비트) |

---

## 🔨 빌드 방법

### Visual C++ 6.0

```cmd
cd sprite-map-viewer
devenv DirectDraw.dsw /Build Debug
```

빌드 결과: `Debug/DirectDraw.exe` (Win32 + DirectDraw 앱).

### Visual Studio 2005 이상

`DirectDraw.dsp`/`.dsw`는 VC6 포맷이므로 직접 열리지 않습니다.

1. VC6에서 프로젝트를 열어 `.vcproj`/`.sln`으로 변환
2. 또는 새 Win32 WinMain 프로젝트를 만들고 `*.cpp`/`*.h`를 모두 추가
3. **라이브러리 의존성**: `ddraw.lib`, `dxguid.lib`

### 빌드 옵션

- **문자 집합**: 멀티바이트
- **링커 추가 의존성**: `ddraw.lib`, `dxguid.lib`, `winmm.lib`
- **서브시스템**: Windows (/SUBSYSTEM:WINDOWS)
- **진입점**: `WinMain`

---

## 🐛 알려진 한계

이 코드는 2003년경의 VC++ 6 + DirectDraw 7 + Win32 API 직접 사용 사례로 작성되어, 다음 한계가 있습니다.

1. **VC++ 6 빌드 시스템** — `.dsp`/`.dsw`는 최신 Visual Studio에서 직접 열리지 않음
2. **DirectDraw 7** — Windows 10/11에서 호환성 모드 필요 (Win32 API 기반)
3. **MFC 미사용** — Win32 메시지 처리를 직접 구현, 코드 길이 길어짐
4. **글로벌 변수 다수** — `g_Map`, `g_player`, `g_player_Spr`, `g_Sel_FPS`, `g_TActionFlag` 등
5. **앱 이름 "HelloDX"** — 학습 프로젝트의 흔적이 그대로 남아 있음
6. **알집 임시 파일** — `ALZ9210.TMP` 잔재 (공개 시 제외)
7. **VC++ 6 환경 파일** — `DirectDraw.opt/plg/ncb/positions/aps/suo` 잔재 (공개 시 제외)
8. **빌드된 실행 파일** — `DirectDraw.exe` 잔재 (공개 시 제외)
9. **기본 50 FPS 하드코딩** — `WORD g_Sel_FPS = 50;`
10. **`DrawTester` / `DrawPlayer`** — 함수 선언만 있고 구현은 미확인 (코드 발췌에 없음, 헤더만)
11. **점프/중력 시스템** — `m_jump`, `m_jumpPower`, `m_gravity` 정의는 있으나 `OnLButtonDown` / `WM_KEYDOWN` 등과의 연결 코드 발췌에 없음
12. **MapChkView 5종 모드** — `m_RectView = 1~5`에 따라 다른 충돌 박스를 그리지만, 모드 전환 UI(메뉴/툴바) 미확인
13. **파일 다이얼로그** — `commdlg.h` 인클루드만 있고 `GetOpenFileName` 호출은 발췌에 없음
14. **`DD_Rotat`** — 함수 선언만 있고 구현은 미확인 (회전 효과 추정)
15. **`SetDlgProc`** — 함수 선언만 있고 사용처 미확인

---

## 📜 라이선스

- 본 저장소의 **소스 코드와 테스트 데이터**는 MIT 라이선스로 공개합니다. (`LICENSE` 참고)
- 다음은 본 저장소에 포함되어 있지 않으며, 각 소유자의 라이선스가 별도로 적용됩니다:
  - **Visual C++ 6.0** — Microsoft 소유
  - **DirectDraw 7 SDK** — Microsoft 소유
  - **`TestSpr.spr`, `_TestSpr.spr`, `MapTest.Map`** — 본인이 제작한 테스트 데이터로 MIT 라이선스 적용
- 본 프로젝트는 비영리 개인 학습·아카이브 목적입니다.

---

## 📁 원본 출처

이 저장소는 다음 경로의 사본입니다.

```text
~/work/backup/01-public/Win/ToolSet/스프라이트_맵뷰어/
```

원본 빌드 산출물(`DirectDraw.exe`), Visual C++ 사용자 환경 파일(`DirectDraw.opt`, `DirectDraw.plg`, `DirectDraw.ncb`, `DirectDraw.positions`, `DirectDraw.aps`, `DirectDraw.suo`), 그리고 알집 임시 파일(`ALZ9210.TMP`)은 공개 저장소에서 제외했습니다.

---

> 🎬 **Tip**: 뷰어 실행에는 DirectX 7~9 SDK 런타임이 필요합니다. Windows 10 이상에서는 `DirectDraw.exe` 속성 → 호환성 → "Windows XP 서비스 팩 2" 모드로 실행해 보세요. 풀스크린 모드에서는 Alt+Tab으로 창 모드 전환 후 `DD_Restore()`로 서피스를 복구할 수 있습니다.