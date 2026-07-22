#include <remake2d/config/config.hpp>
#include <remake2d/script.hpp>
#include <remake2d/event.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaEvent(sol::table& rmk) noexcept {

    script._registerEngineType<EventManager>("EventManager", [&](SolState::Type& ut) {
        ut["poll"]      = &EventManager::poll;
        ut["wait"]      = sol::overload(
                            [](EventManager& self) { self.wait(); },
                            [](EventManager& self, time::Millisecond t) { self.wait(t); }
        );
        ut["textInput"] = sol::overload(
                            [](EventManager& self) { return self.textInput(); },
                            [](EventManager& self, bool s) { self.textInput(s); }
        );
        
        ut["onPressA"]       = &event.onPressA;
        ut["onPressB"]       = &event.onPressB;
        ut["onPressC"]       = &event.onPressC;
        ut["onPressD"]       = &event.onPressD;
        ut["onPressE"]       = &event.onPressE;
        ut["onPressF"]       = &event.onPressF;
        ut["onPressG"]       = &event.onPressG;
        ut["onPressH"]       = &event.onPressH;
        ut["onPressI"]       = &event.onPressI;
        ut["onPressJ"]       = &event.onPressJ;
        ut["onPressK"]       = &event.onPressK;
        ut["onPressL"]       = &event.onPressL;
        ut["onPressM"]       = &event.onPressM;
        ut["onPressN"]       = &event.onPressN;
        ut["onPressO"]       = &event.onPressO;
        ut["onPressP"]       = &event.onPressP;
        ut["onPressQ"]       = &event.onPressQ;
        ut["onPressR"]       = &event.onPressR;
        ut["onPressS"]       = &event.onPressS;
        ut["onPressT"]       = &event.onPressT;
        ut["onPressU"]       = &event.onPressU;
        ut["onPressV"]       = &event.onPressV;
        ut["onPressW"]       = &event.onPressW;
        ut["onPressX"]       = &event.onPressX;
        ut["onPressY"]       = &event.onPressY;
        ut["onPressZ"]       = &event.onPressZ;
    
        ut["onPress0"]       = &event.onPress0;
        ut["onPress1"]       = &event.onPress1;
        ut["onPress2"]       = &event.onPress2;
        ut["onPress3"]       = &event.onPress3;
        ut["onPress4"]       = &event.onPress4;
        ut["onPress5"]       = &event.onPress5;
        ut["onPress6"]       = &event.onPress6;
        ut["onPress7"]       = &event.onPress7;
        ut["onPress8"]       = &event.onPress8;
        ut["onPress9"]       = &event.onPress9;
    
        ut["onPressSpace"]       = &event.onPressSpace;
        ut["onPressEnter"]       = &event.onPressEnter;
        ut["onPressEscape"]      = &event.onPressEscape;
        ut["onPressBackspace"]   = &event.onPressBackspace;
        ut["onPressTab"]         = &event.onPressTab;
        ut["onPressCapsLock"]    = &event.onPressCapsLock;
        ut["onPressDelete"]      = &event.onPressDelete;
        ut["onPressInsert"]      = &event.onPressInsert;
        ut["onPressHome"]        = &event.onPressHome;
        ut["onPressEnd"]         = &event.onPressEnd;
        ut["onPressPageUp"]      = &event.onPressPageUp;
        ut["onPressPageDown"]    = &event.onPressPageDown;
        ut["onPressLeft"]        = &event.onPressLeft;
        ut["onPressRight"]       = &event.onPressRight;
        ut["onPressUp"]          = &event.onPressUp;
        ut["onPressDown"]        = &event.onPressDown;
        ut["onPressLShift"]      = &event.onPressLShift;
        ut["onPressRShift"]      = &event.onPressRShift;
        ut["onPressLCtrl"]       = &event.onPressLCtrl;
        ut["onPressRCtrl"]       = &event.onPressRCtrl;
        ut["onPressLAlt"]        = &event.onPressLAlt;
        ut["onPressRAlt"]        = &event.onPressRAlt;
        ut["onPressAny"]         = &event.onPressAny;
    
        ut["onPressF1"]        = &event.onPressF1;
        ut["onPressF2"]        = &event.onPressF2;
        ut["onPressF3"]        = &event.onPressF3;
        ut["onPressF4"]        = &event.onPressF4;
        ut["onPressF5"]        = &event.onPressF5;
        ut["onPressF6"]        = &event.onPressF6;
        ut["onPressF7"]        = &event.onPressF7;
        ut["onPressF8"]        = &event.onPressF8;
        ut["onPressF9"]        = &event.onPressF9;
        ut["onPressF10"]       = &event.onPressF10;
        ut["onPressF11"]       = &event.onPressF11;
        ut["onPressF12"]       = &event.onPressF12;
    
        ut["onReleaseA"]       = &event.onReleaseA;
        ut["onReleaseB"]       = &event.onReleaseB;
        ut["onReleaseC"]       = &event.onReleaseC;
        ut["onReleaseD"]       = &event.onReleaseD;
        ut["onReleaseE"]       = &event.onReleaseE;
        ut["onReleaseF"]       = &event.onReleaseF;
        ut["onReleaseG"]       = &event.onReleaseG;
        ut["onReleaseH"]       = &event.onReleaseH;
        ut["onReleaseI"]       = &event.onReleaseI;
        ut["onReleaseJ"]       = &event.onReleaseJ;
        ut["onReleaseK"]       = &event.onReleaseK;
        ut["onReleaseL"]       = &event.onReleaseL;
        ut["onReleaseM"]       = &event.onReleaseM;
        ut["onReleaseN"]       = &event.onReleaseN;
        ut["onReleaseO"]       = &event.onReleaseO;
        ut["onReleaseP"]       = &event.onReleaseP;
        ut["onReleaseQ"]       = &event.onReleaseQ;
        ut["onReleaseR"]       = &event.onReleaseR;
        ut["onReleaseS"]       = &event.onReleaseS;
        ut["onReleaseT"]       = &event.onReleaseT;
        ut["onReleaseU"]       = &event.onReleaseU;
        ut["onReleaseV"]       = &event.onReleaseV;
        ut["onReleaseW"]       = &event.onReleaseW;
        ut["onReleaseX"]       = &event.onReleaseX;
        ut["onReleaseY"]       = &event.onReleaseY;
        ut["onReleaseZ"]       = &event.onReleaseZ;
    
        ut["onRelease0"]       = &event.onRelease0;
        ut["onRelease1"]       = &event.onRelease1;
        ut["onRelease2"]       = &event.onRelease2;
        ut["onRelease3"]       = &event.onRelease3;
        ut["onRelease4"]       = &event.onRelease4;
        ut["onRelease5"]       = &event.onRelease5;
        ut["onRelease6"]       = &event.onRelease6;
        ut["onRelease7"]       = &event.onRelease7;
        ut["onRelease8"]       = &event.onRelease8;
        ut["onRelease9"]       = &event.onRelease9;
    
        ut["onReleaseSpace"]       = &event.onReleaseSpace;
        ut["onReleaseEnter"]       = &event.onReleaseEnter;
        ut["onReleaseEscape"]      = &event.onReleaseEscape;
        ut["onReleaseBackspace"]   = &event.onReleaseBackspace;
        ut["onReleaseTab"]         = &event.onReleaseTab;
        ut["onReleaseCapsLock"]    = &event.onReleaseCapsLock;
        ut["onReleaseDelete"]      = &event.onReleaseDelete;
        ut["onReleaseInsert"]      = &event.onReleaseInsert;
        ut["onReleaseHome"]        = &event.onReleaseHome;
        ut["onReleaseEnd"]         = &event.onReleaseEnd;
        ut["onReleasePageUp"]      = &event.onReleasePageUp;
        ut["onReleasePageDown"]    = &event.onReleasePageDown;
        ut["onReleaseLeft"]        = &event.onReleaseLeft;
        ut["onReleaseRight"]       = &event.onReleaseRight;
        ut["onReleaseUp"]          = &event.onReleaseUp;
        ut["onReleaseDown"]        = &event.onReleaseDown;
        ut["onReleaseLShift"]      = &event.onReleaseLShift;
        ut["onReleaseRShift"]      = &event.onReleaseRShift;
        ut["onReleaseLCtrl"]       = &event.onReleaseLCtrl;
        ut["onReleaseRCtrl"]       = &event.onReleaseRCtrl;
        ut["onReleaseLAlt"]        = &event.onReleaseLAlt;
        ut["onReleaseRAlt"]        = &event.onReleaseRAlt;
        ut["onReleaseAny"]         = &event.onReleaseAny;
    
        ut["onReleaseF1"]        = &event.onReleaseF1;
        ut["onReleaseF2"]        = &event.onReleaseF2;
        ut["onReleaseF3"]        = &event.onReleaseF3;
        ut["onReleaseF4"]        = &event.onReleaseF4;
        ut["onReleaseF5"]        = &event.onReleaseF5;
        ut["onReleaseF6"]        = &event.onReleaseF6;
        ut["onReleaseF7"]        = &event.onReleaseF7;
        ut["onReleaseF8"]        = &event.onReleaseF8;
        ut["onReleaseF9"]        = &event.onReleaseF9;
        ut["onReleaseF10"]       = &event.onReleaseF10;
        ut["onReleaseF11"]       = &event.onReleaseF11;
        ut["onReleaseF12"]       = &event.onReleaseF12;
    
        ut["onPressScanA"]       = &event.onPressScanA;
        ut["onPressScanB"]       = &event.onPressScanB;
        ut["onPressScanC"]       = &event.onPressScanC;
        ut["onPressScanD"]       = &event.onPressScanD;
        ut["onPressScanE"]       = &event.onPressScanE;
        ut["onPressScanF"]       = &event.onPressScanF;
        ut["onPressScanG"]       = &event.onPressScanG;
        ut["onPressScanH"]       = &event.onPressScanH;
        ut["onPressScanI"]       = &event.onPressScanI;
        ut["onPressScanJ"]       = &event.onPressScanJ;
        ut["onPressScanK"]       = &event.onPressScanK;
        ut["onPressScanL"]       = &event.onPressScanL;
        ut["onPressScanM"]       = &event.onPressScanM;
        ut["onPressScanN"]       = &event.onPressScanN;
        ut["onPressScanO"]       = &event.onPressScanO;
        ut["onPressScanP"]       = &event.onPressScanP;
        ut["onPressScanQ"]       = &event.onPressScanQ;
        ut["onPressScanR"]       = &event.onPressScanR;
        ut["onPressScanS"]       = &event.onPressScanS;
        ut["onPressScanT"]       = &event.onPressScanT;
        ut["onPressScanU"]       = &event.onPressScanU;
        ut["onPressScanV"]       = &event.onPressScanV;
        ut["onPressScanW"]       = &event.onPressScanW;
        ut["onPressScanX"]       = &event.onPressScanX;
        ut["onPressScanY"]       = &event.onPressScanY;
        ut["onPressScanZ"]       = &event.onPressScanZ;
    
        ut["onPressScan0"]       = &event.onPressScan0;
        ut["onPressScan1"]       = &event.onPressScan1;
        ut["onPressScan2"]       = &event.onPressScan2;
        ut["onPressScan3"]       = &event.onPressScan3;
        ut["onPressScan4"]       = &event.onPressScan4;
        ut["onPressScan5"]       = &event.onPressScan5;
        ut["onPressScan6"]       = &event.onPressScan6;
        ut["onPressScan7"]       = &event.onPressScan7;
        ut["onPressScan8"]       = &event.onPressScan8;
        ut["onPressScan9"]       = &event.onPressScan9;
    
        ut["onPressScanSpace"]   = &event.onPressScanSpace;
        ut["onPressScanEnter"]   = &event.onPressScanEnter;
        ut["onPressScanEscape"]  = &event.onPressScanEscape;
        ut["onPressScanLeft"]    = &event.onPressScanLeft;
        ut["onPressScanRight"]   = &event.onPressScanRight;
        ut["onPressScanUp"]      = &event.onPressScanUp;
        ut["onPressScanDown"]    = &event.onPressScanDown;
    
        ut["onReleaseScanA"]     = &event.onReleaseScanA;
        ut["onReleaseScanB"]     = &event.onReleaseScanB;
        ut["onReleaseScanC"]     = &event.onReleaseScanC;
        ut["onReleaseScanD"]     = &event.onReleaseScanD;
        ut["onReleaseScanE"]     = &event.onReleaseScanE;
        ut["onReleaseScanF"]     = &event.onReleaseScanF;
        ut["onReleaseScanG"]     = &event.onReleaseScanG;
        ut["onReleaseScanH"]     = &event.onReleaseScanH;
        ut["onReleaseScanI"]     = &event.onReleaseScanI;
        ut["onReleaseScanJ"]     = &event.onReleaseScanJ;
        ut["onReleaseScanK"]     = &event.onReleaseScanK;
        ut["onReleaseScanL"]     = &event.onReleaseScanL;
        ut["onReleaseScanM"]     = &event.onReleaseScanM;
        ut["onReleaseScanN"]     = &event.onReleaseScanN;
        ut["onReleaseScanO"]     = &event.onReleaseScanO;
        ut["onReleaseScanP"]     = &event.onReleaseScanP;
        ut["onReleaseScanQ"]     = &event.onReleaseScanQ;
        ut["onReleaseScanR"]     = &event.onReleaseScanR;
        ut["onReleaseScanS"]     = &event.onReleaseScanS;
        ut["onReleaseScanT"]     = &event.onReleaseScanT;
        ut["onReleaseScanU"]     = &event.onReleaseScanU;
        ut["onReleaseScanV"]     = &event.onReleaseScanV;
        ut["onReleaseScanW"]     = &event.onReleaseScanW;
        ut["onReleaseScanX"]     = &event.onReleaseScanX;
        ut["onReleaseScanY"]     = &event.onReleaseScanY;
        ut["onReleaseScanZ"]     = &event.onReleaseScanZ;
    
        ut["onReleaseScan0"]     = &event.onReleaseScan0;
        ut["onReleaseScan1"]     = &event.onReleaseScan1;
        ut["onReleaseScan2"]     = &event.onReleaseScan2;
        ut["onReleaseScan3"]     = &event.onReleaseScan3;
        ut["onReleaseScan4"]     = &event.onReleaseScan4;
        ut["onReleaseScan5"]     = &event.onReleaseScan5;
        ut["onReleaseScan6"]     = &event.onReleaseScan6;
        ut["onReleaseScan7"]     = &event.onReleaseScan7;
        ut["onReleaseScan8"]     = &event.onReleaseScan8;
        ut["onReleaseScan9"]     = &event.onReleaseScan9;
    
        ut["onReleaseScanSpace"] = &event.onReleaseScanSpace;
        ut["onReleaseScanEnter"] = &event.onReleaseScanEnter;
        ut["onReleaseScanEscape"]= &event.onReleaseScanEscape;
        ut["onReleaseScanLeft"]  = &event.onReleaseScanLeft;
        ut["onReleaseScanRight"] = &event.onReleaseScanRight;
        ut["onReleaseScanUp"]    = &event.onReleaseScanUp;
        ut["onReleaseScanDown"]  = &event.onReleaseScanDown;
    
        ut["onTextInput"] = &event.onTextInput;
        ut["onTextEdit"]  = &event.onTextEdit;
    
        ut["onMouseMove"]     = &event.onMouseMove;
        ut["onMouseRawMove"]  = &event.onMouseRawMove;
        ut["onLeftDown"]      = &event.onLeftDown;
        ut["onLeftUp"]        = &event.onLeftUp;
        ut["onRightDown"]     = &event.onRightDown;
        ut["onRightUp"]       = &event.onRightUp;
        ut["onMiddleDown"]    = &event.onMiddleDown;
        ut["onMiddleUp"]      = &event.onMiddleUp;
        ut["onDoubleClick"]   = &event.onDoubleClick;
        ut["onWheel"]         = &event.onWheel;
    
        ut["onPressACtrl"]      = &event.onPressACtrl;
        ut["onPressBCtrl"]      = &event.onPressBCtrl;
        ut["onPressXCtrl"]      = &event.onPressXCtrl;
        ut["onPressYCtrl"]      = &event.onPressYCtrl;
        ut["onPressStart"]      = &event.onPressStart;
        ut["onPressSelect"]     = &event.onPressSelect;
        ut["onPressLShoulder"]  = &event.onPressLShoulder;
        ut["onPressRShoulder"]  = &event.onPressRShoulder;
        ut["onPressDpadLeft"]   = &event.onPressDpadLeft;
        ut["onPressDpadRight"]  = &event.onPressDpadRight;
        ut["onPressDpadUp"]     = &event.onPressDpadUp;
        ut["onPressDpadDown"]   = &event.onPressDpadDown;
        ut["onPressLStick"]     = &event.onPressLStick;
        ut["onPressRStick"]     = &event.onPressRStick;
    
        ut["onReleaseACtrl"]    = &event.onReleaseACtrl;
        ut["onReleaseBCtrl"]    = &event.onReleaseBCtrl;
        ut["onReleaseXCtrl"]    = &event.onReleaseXCtrl;
        ut["onReleaseYCtrl"]    = &event.onReleaseYCtrl;
        ut["onReleaseStart"]    = &event.onReleaseStart;
        ut["onReleaseSelect"]   = &event.onReleaseSelect;
        ut["onReleaseLShoulder"]= &event.onReleaseLShoulder;
        ut["onReleaseRShoulder"]= &event.onReleaseRShoulder;
        ut["onReleaseDpadLeft"] = &event.onReleaseDpadLeft;
        ut["onReleaseDpadRight"]= &event.onReleaseDpadRight;
        ut["onReleaseDpadUp"]   = &event.onReleaseDpadUp;
        ut["onReleaseDpadDown"] = &event.onReleaseDpadDown;
        ut["onReleaseLStick"]   = &event.onReleaseLStick;
        ut["onReleaseRStick"]   = &event.onReleaseRStick;
    
        ut["onAxisLeftX"]    = &event.onAxisLeftX;
        ut["onAxisLeftY"]    = &event.onAxisLeftY;
        ut["onAxisRightX"]   = &event.onAxisRightX;
        ut["onAxisRightY"]   = &event.onAxisRightY;
        ut["onAxisLTrigger"] = &event.onAxisLTrigger;
        ut["onAxisRTrigger"] = &event.onAxisRTrigger;
    
        ut["onControllerAdded"]   = &event.onControllerAdded;
        ut["onControllerRemoved"] = &event.onControllerRemoved;
    
        ut["onFingerDown"]   = &event.onFingerDown;
        ut["onFingerUp"]     = &event.onFingerUp;
        ut["onFingerMove"]   = &event.onFingerMove;
        ut["onMultiGesture"] = &event.onMultiGesture;
    
        ut["onWindowClose"]        = &event.onWindowClose;
        ut["onWindowMinimized"]    = &event.onWindowMinimized;
        ut["onWindowMaximized"]    = &event.onWindowMaximized;
        ut["onWindowRestored"]     = &event.onWindowRestored;
        ut["onWindowShown"]        = &event.onWindowShown;
        ut["onWindowHidden"]       = &event.onWindowHidden;
        ut["onWindowExposed"]      = &event.onWindowExposed;
        ut["onWindowFocusGained"]  = &event.onWindowFocusGained;
        ut["onWindowFocusLost"]    = &event.onWindowFocusLost;
        ut["onWindowMouseEnter"]   = &event.onWindowMouseEnter;
        ut["onWindowMouseLeave"]   = &event.onWindowMouseLeave;
        ut["onWindowResized"]      = &event.onWindowResized;
        ut["onWindowMoved"]        = &event.onWindowMoved;
    
        ut["onQuit"]              = &event.onQuit;
        ut["onDropFile"]          = &event.onDropFile;
        ut["onClipboardUpdate"]   = &event.onClipboardUpdate;
        ut["onAppForeground"]     = &event.onAppForeground;
        ut["onAppBackground"]     = &event.onAppBackground;
        ut["onAppLowMemory"]      = &event.onAppLowMemory;
        ut["onLocaleChanged"]     = &event.onLocaleChanged;
        ut["onDisplayChanged"]    = &event.onDisplayChanged;
        ut["onRenderReset"]       = &event.onRenderReset;
    
        ut["onEventNone"] = &event.onEventNone;
    });
    
    rmk["event"] = &event;
}

} // namespace solstat
} // namespace config
} // namespace rmk