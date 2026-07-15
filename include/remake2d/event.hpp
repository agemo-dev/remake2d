#ifndef REMAKE2D_EVENT_
#define REMAKE2D_EVENT_

#include <remake2d/time.hpp>
#include <remake2d/error.hpp>
#include <remake2d/signal.hpp>
#include <remake2d/vector.hpp>
#include <remake2d/numeric.hpp>

#include <SDL2/SDL.h>

#include <string>
#include <unordered_map>

namespace rmk {

class EventManager {
private:
    SDL_Event   									m_event;
    bool        									m_has_event{false};
    bool        									m_text_input{false};
    std::unordered_map<i32, SDL_GameController*>	m_controllers;

private:
    EventManager(void);
    EventManager(const EventManager&)            = delete;
    EventManager& operator=(const EventManager&) = delete;

    void _process(SDL_Event&);

public:
    static EventManager& getInstance(void);

    void poll(void);
    void wait(void);
    void wait(time::Second);

public:
	void textInput(bool);
	bool textInput(void);
	
public:
	friend SDL_GameController* _getOpenController(i32) noexcept;

public:

    _EventSignal<> onPressA, onPressB, onPressC, onPressD, onPressE;
    _EventSignal<> onPressF, onPressG, onPressH, onPressI, onPressJ;
    _EventSignal<> onPressK, onPressL, onPressM, onPressN, onPressO;
    _EventSignal<> onPressP, onPressQ, onPressR, onPressS, onPressT;
    _EventSignal<> onPressU, onPressV, onPressW, onPressX, onPressY;
    _EventSignal<> onPressZ;


    _EventSignal<> onPress0, onPress1, onPress2, onPress3, onPress4;
    _EventSignal<> onPress5, onPress6, onPress7, onPress8, onPress9;


    _EventSignal<> onPressSpace,    onPressEnter,    onPressEscape;
    _EventSignal<> onPressBackspace, onPressTab,     onPressCapsLock;
    _EventSignal<> onPressDelete,   onPressInsert,   onPressHome;
    _EventSignal<> onPressEnd,      onPressPageUp,   onPressPageDown;
    _EventSignal<> onPressLeft,     onPressRight,    onPressUp,    onPressDown;
    _EventSignal<> onPressLShift,   onPressRShift;
    _EventSignal<> onPressLCtrl,    onPressRCtrl;
    _EventSignal<> onPressLAlt,     onPressRAlt;
    _EventSignal<> onPressAny;


    _EventSignal<> onPressF1,  onPressF2,  onPressF3,  onPressF4;
    _EventSignal<> onPressF5,  onPressF6,  onPressF7,  onPressF8;
    _EventSignal<> onPressF9,  onPressF10, onPressF11, onPressF12;


    _EventSignal<> onReleaseA, onReleaseB, onReleaseC, onReleaseD, onReleaseE;
    _EventSignal<> onReleaseF, onReleaseG, onReleaseH, onReleaseI, onReleaseJ;
    _EventSignal<> onReleaseK, onReleaseL, onReleaseM, onReleaseN, onReleaseO;
    _EventSignal<> onReleaseP, onReleaseQ, onReleaseR, onReleaseS, onReleaseT;
    _EventSignal<> onReleaseU, onReleaseV, onReleaseW, onReleaseX, onReleaseY;
    _EventSignal<> onReleaseZ;


    _EventSignal<> onRelease0, onRelease1, onRelease2, onRelease3, onRelease4;
    _EventSignal<> onRelease5, onRelease6, onRelease7, onRelease8, onRelease9;


    _EventSignal<> onReleaseSpace,    onReleaseEnter,    onReleaseEscape;
    _EventSignal<> onReleaseBackspace, onReleaseTab,     onReleaseCapsLock;
    _EventSignal<> onReleaseDelete,   onReleaseInsert,   onReleaseHome;
    _EventSignal<> onReleaseEnd,      onReleasePageUp,   onReleasePageDown;
    _EventSignal<> onReleaseLeft,     onReleaseRight,    onReleaseUp,  onReleaseDown;
    _EventSignal<> onReleaseLShift,   onReleaseRShift;
    _EventSignal<> onReleaseLCtrl,    onReleaseRCtrl;
    _EventSignal<> onReleaseLAlt,     onReleaseRAlt;
    _EventSignal<> onReleaseAny;


    _EventSignal<> onReleaseF1,  onReleaseF2,  onReleaseF3,  onReleaseF4;
    _EventSignal<> onReleaseF5,  onReleaseF6,  onReleaseF7,  onReleaseF8;
    _EventSignal<> onReleaseF9,  onReleaseF10, onReleaseF11, onReleaseF12;


    _EventSignal<> onPressScanA, onPressScanB, onPressScanC, onPressScanD, onPressScanE;
    _EventSignal<> onPressScanF, onPressScanG, onPressScanH, onPressScanI, onPressScanJ;
    _EventSignal<> onPressScanK, onPressScanL, onPressScanM, onPressScanN, onPressScanO;
    _EventSignal<> onPressScanP, onPressScanQ, onPressScanR, onPressScanS, onPressScanT;
    _EventSignal<> onPressScanU, onPressScanV, onPressScanW, onPressScanX, onPressScanY;
    _EventSignal<> onPressScanZ;


    _EventSignal<> onPressScan0, onPressScan1, onPressScan2, onPressScan3, onPressScan4;
    _EventSignal<> onPressScan5, onPressScan6, onPressScan7, onPressScan8, onPressScan9;


    _EventSignal<> onPressScanSpace, onPressScanEnter,  onPressScanEscape;
    _EventSignal<> onPressScanLeft,  onPressScanRight,  onPressScanUp, onPressScanDown;


    _EventSignal<> onReleaseScanA, onReleaseScanB, onReleaseScanC, onReleaseScanD, onReleaseScanE;
    _EventSignal<> onReleaseScanF, onReleaseScanG, onReleaseScanH, onReleaseScanI, onReleaseScanJ;
    _EventSignal<> onReleaseScanK, onReleaseScanL, onReleaseScanM, onReleaseScanN, onReleaseScanO;
    _EventSignal<> onReleaseScanP, onReleaseScanQ, onReleaseScanR, onReleaseScanS, onReleaseScanT;
    _EventSignal<> onReleaseScanU, onReleaseScanV, onReleaseScanW, onReleaseScanX, onReleaseScanY;
    _EventSignal<> onReleaseScanZ;


    _EventSignal<> onReleaseScan0, onReleaseScan1, onReleaseScan2, onReleaseScan3, onReleaseScan4;
    _EventSignal<> onReleaseScan5, onReleaseScan6, onReleaseScan7, onReleaseScan8, onReleaseScan9;


    _EventSignal<> onReleaseScanSpace, onReleaseScanEnter,  onReleaseScanEscape;
    _EventSignal<> onReleaseScanLeft,  onReleaseScanRight,  onReleaseScanUp, onReleaseScanDown;




    _EventSignal<std::string> onTextInput; 
    _EventSignal<std::string> onTextEdit;  




    _EventSignal<Vec2d> onMouseMove;    
    _EventSignal<Vec2d> onMouseRawMove; 
    _EventSignal<Vec2d> onLeftDown;
    _EventSignal<Vec2d> onLeftUp;
    _EventSignal<Vec2d> onRightDown;
    _EventSignal<Vec2d> onRightUp;
    _EventSignal<Vec2d> onMiddleDown;
    _EventSignal<Vec2d> onMiddleUp;
    _EventSignal<Vec2d> onDoubleClick;  
    _EventSignal<Vec2d> onWheel;






    _EventSignal<i32> onPressACtrl,    onPressBCtrl;
    _EventSignal<i32> onPressXCtrl,    onPressYCtrl;
    _EventSignal<i32> onPressStart,     onPressSelect;
    _EventSignal<i32> onPressLShoulder, onPressRShoulder;
    _EventSignal<i32> onPressDpadLeft,  onPressDpadRight;
    _EventSignal<i32> onPressDpadUp,    onPressDpadDown;
    _EventSignal<i32> onPressLStick,    onPressRStick;


    _EventSignal<i32> onReleaseACtrl,    onReleaseBCtrl;
    _EventSignal<i32> onReleaseXCtrl,    onReleaseYCtrl;
    _EventSignal<i32> onReleaseStart,     onReleaseSelect;
    _EventSignal<i32> onReleaseLShoulder, onReleaseRShoulder;
    _EventSignal<i32> onReleaseDpadLeft,  onReleaseDpadRight;
    _EventSignal<i32> onReleaseDpadUp,    onReleaseDpadDown;
    _EventSignal<i32> onReleaseLStick,    onReleaseRStick;


    _EventSignal<i32, i16> onAxisLeftX,    onAxisLeftY;
    _EventSignal<i32, i16> onAxisRightX,   onAxisRightY;
    _EventSignal<i32, i16> onAxisLTrigger, onAxisRTrigger;


    _EventSignal<i32> onControllerAdded;
    _EventSignal<i32> onControllerRemoved;




    _EventSignal<Vec2d> onFingerDown;   
    _EventSignal<Vec2d> onFingerUp;
    _EventSignal<Vec2d> onFingerMove;
    _EventSignal<Vec2d> onMultiGesture; 




    _EventSignal<u32>        onWindowClose;
    _EventSignal<u32>        onWindowMinimized;
    _EventSignal<u32>        onWindowMaximized;
    _EventSignal<u32>        onWindowRestored;  
    _EventSignal<u32>        onWindowShown;  
    _EventSignal<u32>        onWindowHidden;
    _EventSignal<u32>        onWindowExposed;
    _EventSignal<u32>        onWindowFocusGained;
    _EventSignal<u32>        onWindowFocusLost;
    _EventSignal<u32>        onWindowMouseEnter;   
    _EventSignal<u32>        onWindowMouseLeave;   
    _EventSignal<u32, Dim2d> onWindowResized;
    _EventSignal<u32, Vec2d> onWindowMoved;




    _EventSignal<>            onQuit;            
    _EventSignal<std::string> onDropFile;
    _EventSignal<>            onClipboardUpdate;
    _EventSignal<>            onAppForeground;
    _EventSignal<>            onAppBackground;
    _EventSignal<>            onAppLowMemory;
    _EventSignal<>            onLocaleChanged;   
    _EventSignal<>            onDisplayChanged;  
    _EventSignal<>            onRenderReset;     




    _EventSignal<> onEventNone; 
};

inline EventManager& event = EventManager::getInstance();

} //namespace rmk

#endif