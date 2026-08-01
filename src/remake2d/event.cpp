#include <remake2d/event.hpp>

#include <string>
#include <filesystem>
#include <algorithm>

namespace rmk {

EventManager::EventManager(void) {

    onPressScanA._setScancode(SDL_SCANCODE_A);
    onPressScanB._setScancode(SDL_SCANCODE_B);
    onPressScanC._setScancode(SDL_SCANCODE_C);
    onPressScanD._setScancode(SDL_SCANCODE_D);
    onPressScanE._setScancode(SDL_SCANCODE_E);
    onPressScanF._setScancode(SDL_SCANCODE_F);
    onPressScanG._setScancode(SDL_SCANCODE_G);
    onPressScanH._setScancode(SDL_SCANCODE_H);
    onPressScanI._setScancode(SDL_SCANCODE_I);
    onPressScanJ._setScancode(SDL_SCANCODE_J);
    onPressScanK._setScancode(SDL_SCANCODE_K);
    onPressScanL._setScancode(SDL_SCANCODE_L);
    onPressScanM._setScancode(SDL_SCANCODE_M);
    onPressScanN._setScancode(SDL_SCANCODE_N);
    onPressScanO._setScancode(SDL_SCANCODE_O);
    onPressScanP._setScancode(SDL_SCANCODE_P);
    onPressScanQ._setScancode(SDL_SCANCODE_Q);
    onPressScanR._setScancode(SDL_SCANCODE_R);
    onPressScanS._setScancode(SDL_SCANCODE_S);
    onPressScanT._setScancode(SDL_SCANCODE_T);
    onPressScanU._setScancode(SDL_SCANCODE_U);
    onPressScanV._setScancode(SDL_SCANCODE_V);
    onPressScanW._setScancode(SDL_SCANCODE_W);
    onPressScanX._setScancode(SDL_SCANCODE_X);
    onPressScanY._setScancode(SDL_SCANCODE_Y);
    onPressScanZ._setScancode(SDL_SCANCODE_Z);

    
    onPressScan0._setScancode(SDL_SCANCODE_0);
    onPressScan1._setScancode(SDL_SCANCODE_1);
    onPressScan2._setScancode(SDL_SCANCODE_2);
    onPressScan3._setScancode(SDL_SCANCODE_3);
    onPressScan4._setScancode(SDL_SCANCODE_4);
    onPressScan5._setScancode(SDL_SCANCODE_5);
    onPressScan6._setScancode(SDL_SCANCODE_6);
    onPressScan7._setScancode(SDL_SCANCODE_7);
    onPressScan8._setScancode(SDL_SCANCODE_8);
    onPressScan9._setScancode(SDL_SCANCODE_9);

    
    onPressSpace._setScancode(SDL_SCANCODE_SPACE);
    onPressEnter._setScancode(SDL_SCANCODE_RETURN);
    onPressEscape._setScancode(SDL_SCANCODE_ESCAPE);
    onPressBackspace._setScancode(SDL_SCANCODE_BACKSPACE);
    onPressTab._setScancode(SDL_SCANCODE_TAB);
    onPressCapsLock._setScancode(SDL_SCANCODE_CAPSLOCK);
    onPressDelete._setScancode(SDL_SCANCODE_DELETE);
    onPressInsert._setScancode(SDL_SCANCODE_INSERT);
    onPressHome._setScancode(SDL_SCANCODE_HOME);
    onPressEnd._setScancode(SDL_SCANCODE_END);
    onPressPageUp._setScancode(SDL_SCANCODE_PAGEUP);
    onPressPageDown._setScancode(SDL_SCANCODE_PAGEDOWN);
    onPressLeft._setScancode(SDL_SCANCODE_LEFT);
    onPressRight._setScancode(SDL_SCANCODE_RIGHT);
    onPressUp._setScancode(SDL_SCANCODE_UP);
    onPressDown._setScancode(SDL_SCANCODE_DOWN);
    onPressLShift._setScancode(SDL_SCANCODE_LSHIFT);
    onPressRShift._setScancode(SDL_SCANCODE_RSHIFT);
    onPressLCtrl._setScancode(SDL_SCANCODE_LCTRL);
    onPressRCtrl._setScancode(SDL_SCANCODE_RCTRL);
    onPressLAlt._setScancode(SDL_SCANCODE_LALT);
    onPressRAlt._setScancode(SDL_SCANCODE_RALT);

    
    onPressF1._setScancode(SDL_SCANCODE_F1);
    onPressF2._setScancode(SDL_SCANCODE_F2);
    onPressF3._setScancode(SDL_SCANCODE_F3);
    onPressF4._setScancode(SDL_SCANCODE_F4);
    onPressF5._setScancode(SDL_SCANCODE_F5);
    onPressF6._setScancode(SDL_SCANCODE_F6);
    onPressF7._setScancode(SDL_SCANCODE_F7);
    onPressF8._setScancode(SDL_SCANCODE_F8);
    onPressF9._setScancode(SDL_SCANCODE_F9);
    onPressF10._setScancode(SDL_SCANCODE_F10);
    onPressF11._setScancode(SDL_SCANCODE_F11);
    onPressF12._setScancode(SDL_SCANCODE_F12);

    
    onPressScanSpace._setScancode(SDL_SCANCODE_SPACE);
    onPressScanEnter._setScancode(SDL_SCANCODE_RETURN);
    onPressScanEscape._setScancode(SDL_SCANCODE_ESCAPE);
    onPressScanLeft._setScancode(SDL_SCANCODE_LEFT);
    onPressScanRight._setScancode(SDL_SCANCODE_RIGHT);
    onPressScanUp._setScancode(SDL_SCANCODE_UP);
    onPressScanDown._setScancode(SDL_SCANCODE_DOWN);

    
    onPressACtrl._setButton(SDL_CONTROLLER_BUTTON_A);
    onPressBCtrl._setButton(SDL_CONTROLLER_BUTTON_B);
    onPressXCtrl._setButton(SDL_CONTROLLER_BUTTON_X);
    onPressYCtrl._setButton(SDL_CONTROLLER_BUTTON_Y);
    onPressStart._setButton(SDL_CONTROLLER_BUTTON_START);
    onPressSelect._setButton(SDL_CONTROLLER_BUTTON_BACK);
    onPressLShoulder._setButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
    onPressRShoulder._setButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
    onPressDpadLeft._setButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    onPressDpadRight._setButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
    onPressDpadUp._setButton(SDL_CONTROLLER_BUTTON_DPAD_UP);
    onPressDpadDown._setButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN);
    onPressLStick._setButton(SDL_CONTROLLER_BUTTON_LEFTSTICK);
    onPressRStick._setButton(SDL_CONTROLLER_BUTTON_RIGHTSTICK);
}

EventManager& EventManager::getInstance(void) {
    static EventManager instance;
    return instance;
}




void EventManager::_process(SDL_Event& e) {
    switch (e.type) {

        case SDL_KEYDOWN: {
            if (e.key.repeat != 0) break;
            onPressAny._evaluate();
            
            switch (e.key.keysym.sym) {
                case SDLK_a: onPressA._evaluate(); break;
                case SDLK_b: onPressB._evaluate(); break;
                case SDLK_c: onPressC._evaluate(); break;
                case SDLK_d: onPressD._evaluate(); break;
                case SDLK_e: onPressE._evaluate(); break;
                case SDLK_f: onPressF._evaluate(); break;
                case SDLK_g: onPressG._evaluate(); break;
                case SDLK_h: onPressH._evaluate(); break;
                case SDLK_i: onPressI._evaluate(); break;
                case SDLK_j: onPressJ._evaluate(); break;
                case SDLK_k: onPressK._evaluate(); break;
                case SDLK_l: onPressL._evaluate(); break;
                case SDLK_m: onPressM._evaluate(); break;
                case SDLK_n: onPressN._evaluate(); break;
                case SDLK_o: onPressO._evaluate(); break;
                case SDLK_p: onPressP._evaluate(); break;
                case SDLK_q: onPressQ._evaluate(); break;
                case SDLK_r: onPressR._evaluate(); break;
                case SDLK_s: onPressS._evaluate(); break;
                case SDLK_t: onPressT._evaluate(); break;
                case SDLK_u: onPressU._evaluate(); break;
                case SDLK_v: onPressV._evaluate(); break;
                case SDLK_w: onPressW._evaluate(); break;
                case SDLK_x: onPressX._evaluate(); break;
                case SDLK_y: onPressY._evaluate(); break;
                case SDLK_z: onPressZ._evaluate(); break;
                case SDLK_0: onPress0._evaluate(); break;
                case SDLK_1: onPress1._evaluate(); break;
                case SDLK_2: onPress2._evaluate(); break;
                case SDLK_3: onPress3._evaluate(); break;
                case SDLK_4: onPress4._evaluate(); break;
                case SDLK_5: onPress5._evaluate(); break;
                case SDLK_6: onPress6._evaluate(); break;
                case SDLK_7: onPress7._evaluate(); break;
                case SDLK_8: onPress8._evaluate(); break;
                case SDLK_9: onPress9._evaluate(); break;
                case SDLK_SPACE:     onPressSpace._evaluate();     break;
                case SDLK_RETURN:    onPressEnter._evaluate();     break;
                case SDLK_ESCAPE:    onPressEscape._evaluate();    break;
                case SDLK_BACKSPACE: onPressBackspace._evaluate(); break;
                case SDLK_TAB:       onPressTab._evaluate();       break;
                case SDLK_CAPSLOCK:  onPressCapsLock._evaluate();  break;
                case SDLK_DELETE:    onPressDelete._evaluate();    break;
                case SDLK_INSERT:    onPressInsert._evaluate();    break;
                case SDLK_HOME:      onPressHome._evaluate();      break;
                case SDLK_END:       onPressEnd._evaluate();       break;
                case SDLK_PAGEUP:    onPressPageUp._evaluate();    break;
                case SDLK_PAGEDOWN:  onPressPageDown._evaluate();  break;
                case SDLK_LEFT:      onPressLeft._evaluate();      break;
                case SDLK_RIGHT:     onPressRight._evaluate();     break;
                case SDLK_UP:        onPressUp._evaluate();        break;
                case SDLK_DOWN:      onPressDown._evaluate();      break;
                case SDLK_LSHIFT:    onPressLShift._evaluate();    break;
                case SDLK_RSHIFT:    onPressRShift._evaluate();    break;
                case SDLK_LCTRL:     onPressLCtrl._evaluate();     break;
                case SDLK_RCTRL:     onPressRCtrl._evaluate();     break;
                case SDLK_LALT:      onPressLAlt._evaluate();      break;
                case SDLK_RALT:      onPressRAlt._evaluate();      break;
                case SDLK_F1:        onPressF1._evaluate();        break;
                case SDLK_F2:        onPressF2._evaluate();        break;
                case SDLK_F3:        onPressF3._evaluate();        break;
                case SDLK_F4:        onPressF4._evaluate();        break;
                case SDLK_F5:        onPressF5._evaluate();        break;
                case SDLK_F6:        onPressF6._evaluate();        break;
                case SDLK_F7:        onPressF7._evaluate();        break;
                case SDLK_F8:        onPressF8._evaluate();        break;
                case SDLK_F9:        onPressF9._evaluate();        break;
                case SDLK_F10:       onPressF10._evaluate();       break;
                case SDLK_F11:       onPressF11._evaluate();       break;
                case SDLK_F12:       onPressF12._evaluate();       break;
                default: break;
            }

            
            switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_A: onPressScanA._evaluate(); break;
                case SDL_SCANCODE_B: onPressScanB._evaluate(); break;
                case SDL_SCANCODE_C: onPressScanC._evaluate(); break;
                case SDL_SCANCODE_D: onPressScanD._evaluate(); break;
                case SDL_SCANCODE_E: onPressScanE._evaluate(); break;
                case SDL_SCANCODE_F: onPressScanF._evaluate(); break;
                case SDL_SCANCODE_G: onPressScanG._evaluate(); break;
                case SDL_SCANCODE_H: onPressScanH._evaluate(); break;
                case SDL_SCANCODE_I: onPressScanI._evaluate(); break;
                case SDL_SCANCODE_J: onPressScanJ._evaluate(); break;
                case SDL_SCANCODE_K: onPressScanK._evaluate(); break;
                case SDL_SCANCODE_L: onPressScanL._evaluate(); break;
                case SDL_SCANCODE_M: onPressScanM._evaluate(); break;
                case SDL_SCANCODE_N: onPressScanN._evaluate(); break;
                case SDL_SCANCODE_O: onPressScanO._evaluate(); break;
                case SDL_SCANCODE_P: onPressScanP._evaluate(); break;
                case SDL_SCANCODE_Q: onPressScanQ._evaluate(); break;
                case SDL_SCANCODE_R: onPressScanR._evaluate(); break;
                case SDL_SCANCODE_S: onPressScanS._evaluate(); break;
                case SDL_SCANCODE_T: onPressScanT._evaluate(); break;
                case SDL_SCANCODE_U: onPressScanU._evaluate(); break;
                case SDL_SCANCODE_V: onPressScanV._evaluate(); break;
                case SDL_SCANCODE_W: onPressScanW._evaluate(); break;
                case SDL_SCANCODE_X: onPressScanX._evaluate(); break;
                case SDL_SCANCODE_Y: onPressScanY._evaluate(); break;
                case SDL_SCANCODE_Z: onPressScanZ._evaluate(); break;
                case SDL_SCANCODE_0: onPressScan0._evaluate(); break;
                case SDL_SCANCODE_1: onPressScan1._evaluate(); break;
                case SDL_SCANCODE_2: onPressScan2._evaluate(); break;
                case SDL_SCANCODE_3: onPressScan3._evaluate(); break;
                case SDL_SCANCODE_4: onPressScan4._evaluate(); break;
                case SDL_SCANCODE_5: onPressScan5._evaluate(); break;
                case SDL_SCANCODE_6: onPressScan6._evaluate(); break;
                case SDL_SCANCODE_7: onPressScan7._evaluate(); break;
                case SDL_SCANCODE_8: onPressScan8._evaluate(); break;
                case SDL_SCANCODE_9: onPressScan9._evaluate(); break;
                case SDL_SCANCODE_SPACE:  onPressScanSpace._evaluate();  break;
                case SDL_SCANCODE_RETURN: onPressScanEnter._evaluate();  break;
                case SDL_SCANCODE_ESCAPE: onPressScanEscape._evaluate(); break;
                case SDL_SCANCODE_LEFT:   onPressScanLeft._evaluate();   break;
                case SDL_SCANCODE_RIGHT:  onPressScanRight._evaluate();  break;
                case SDL_SCANCODE_UP:     onPressScanUp._evaluate();     break;
                case SDL_SCANCODE_DOWN:   onPressScanDown._evaluate();   break;
                default: break;
            }
            break;
        }

        
        case SDL_KEYUP: {
            onReleaseAny._evaluate();

            switch (e.key.keysym.sym) {
                case SDLK_a: onReleaseA._evaluate(); break;
                case SDLK_b: onReleaseB._evaluate(); break;
                case SDLK_c: onReleaseC._evaluate(); break;
                case SDLK_d: onReleaseD._evaluate(); break;
                case SDLK_e: onReleaseE._evaluate(); break;
                case SDLK_f: onReleaseF._evaluate(); break;
                case SDLK_g: onReleaseG._evaluate(); break;
                case SDLK_h: onReleaseH._evaluate(); break;
                case SDLK_i: onReleaseI._evaluate(); break;
                case SDLK_j: onReleaseJ._evaluate(); break;
                case SDLK_k: onReleaseK._evaluate(); break;
                case SDLK_l: onReleaseL._evaluate(); break;
                case SDLK_m: onReleaseM._evaluate(); break;
                case SDLK_n: onReleaseN._evaluate(); break;
                case SDLK_o: onReleaseO._evaluate(); break;
                case SDLK_p: onReleaseP._evaluate(); break;
                case SDLK_q: onReleaseQ._evaluate(); break;
                case SDLK_r: onReleaseR._evaluate(); break;
                case SDLK_s: onReleaseS._evaluate(); break;
                case SDLK_t: onReleaseT._evaluate(); break;
                case SDLK_u: onReleaseU._evaluate(); break;
                case SDLK_v: onReleaseV._evaluate(); break;
                case SDLK_w: onReleaseW._evaluate(); break;
                case SDLK_x: onReleaseX._evaluate(); break;
                case SDLK_y: onReleaseY._evaluate(); break;
                case SDLK_z: onReleaseZ._evaluate(); break;
                case SDLK_0: onRelease0._evaluate(); break;
                case SDLK_1: onRelease1._evaluate(); break;
                case SDLK_2: onRelease2._evaluate(); break;
                case SDLK_3: onRelease3._evaluate(); break;
                case SDLK_4: onRelease4._evaluate(); break;
                case SDLK_5: onRelease5._evaluate(); break;
                case SDLK_6: onRelease6._evaluate(); break;
                case SDLK_7: onRelease7._evaluate(); break;
                case SDLK_8: onRelease8._evaluate(); break;
                case SDLK_9: onRelease9._evaluate(); break;
                case SDLK_SPACE:     onReleaseSpace._evaluate();     break;
                case SDLK_RETURN:    onReleaseEnter._evaluate();     break;
                case SDLK_ESCAPE:    onReleaseEscape._evaluate();    break;
                case SDLK_BACKSPACE: onReleaseBackspace._evaluate(); break;
                case SDLK_TAB:       onReleaseTab._evaluate();       break;
                case SDLK_CAPSLOCK:  onReleaseCapsLock._evaluate();  break;
                case SDLK_DELETE:    onReleaseDelete._evaluate();    break;
                case SDLK_INSERT:    onReleaseInsert._evaluate();    break;
                case SDLK_HOME:      onReleaseHome._evaluate();      break;
                case SDLK_END:       onReleaseEnd._evaluate();       break;
                case SDLK_PAGEUP:    onReleasePageUp._evaluate();    break;
                case SDLK_PAGEDOWN:  onReleasePageDown._evaluate();  break;
                case SDLK_LEFT:      onReleaseLeft._evaluate();      break;
                case SDLK_RIGHT:     onReleaseRight._evaluate();     break;
                case SDLK_UP:        onReleaseUp._evaluate();        break;
                case SDLK_DOWN:      onReleaseDown._evaluate();      break;
                case SDLK_LSHIFT:    onReleaseLShift._evaluate();    break;
                case SDLK_RSHIFT:    onReleaseRShift._evaluate();    break;
                case SDLK_LCTRL:     onReleaseLCtrl._evaluate();     break;
                case SDLK_RCTRL:     onReleaseRCtrl._evaluate();     break;
                case SDLK_LALT:      onReleaseLAlt._evaluate();      break;
                case SDLK_RALT:      onReleaseRAlt._evaluate();      break;
                case SDLK_F1:        onReleaseF1._evaluate();        break;
                case SDLK_F2:        onReleaseF2._evaluate();        break;
                case SDLK_F3:        onReleaseF3._evaluate();        break;
                case SDLK_F4:        onReleaseF4._evaluate();        break;
                case SDLK_F5:        onReleaseF5._evaluate();        break;
                case SDLK_F6:        onReleaseF6._evaluate();        break;
                case SDLK_F7:        onReleaseF7._evaluate();        break;
                case SDLK_F8:        onReleaseF8._evaluate();        break;
                case SDLK_F9:        onReleaseF9._evaluate();        break;
                case SDLK_F10:       onReleaseF10._evaluate();       break;
                case SDLK_F11:       onReleaseF11._evaluate();       break;
                case SDLK_F12:       onReleaseF12._evaluate();       break;
                default: break;
            }

            switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_A: onReleaseScanA._evaluate(); break;
                case SDL_SCANCODE_B: onReleaseScanB._evaluate(); break;
                case SDL_SCANCODE_C: onReleaseScanC._evaluate(); break;
                case SDL_SCANCODE_D: onReleaseScanD._evaluate(); break;
                case SDL_SCANCODE_E: onReleaseScanE._evaluate(); break;
                case SDL_SCANCODE_F: onReleaseScanF._evaluate(); break;
                case SDL_SCANCODE_G: onReleaseScanG._evaluate(); break;
                case SDL_SCANCODE_H: onReleaseScanH._evaluate(); break;
                case SDL_SCANCODE_I: onReleaseScanI._evaluate(); break;
                case SDL_SCANCODE_J: onReleaseScanJ._evaluate(); break;
                case SDL_SCANCODE_K: onReleaseScanK._evaluate(); break;
                case SDL_SCANCODE_L: onReleaseScanL._evaluate(); break;
                case SDL_SCANCODE_M: onReleaseScanM._evaluate(); break;
                case SDL_SCANCODE_N: onReleaseScanN._evaluate(); break;
                case SDL_SCANCODE_O: onReleaseScanO._evaluate(); break;
                case SDL_SCANCODE_P: onReleaseScanP._evaluate(); break;
                case SDL_SCANCODE_Q: onReleaseScanQ._evaluate(); break;
                case SDL_SCANCODE_R: onReleaseScanR._evaluate(); break;
                case SDL_SCANCODE_S: onReleaseScanS._evaluate(); break;
                case SDL_SCANCODE_T: onReleaseScanT._evaluate(); break;
                case SDL_SCANCODE_U: onReleaseScanU._evaluate(); break;
                case SDL_SCANCODE_V: onReleaseScanV._evaluate(); break;
                case SDL_SCANCODE_W: onReleaseScanW._evaluate(); break;
                case SDL_SCANCODE_X: onReleaseScanX._evaluate(); break;
                case SDL_SCANCODE_Y: onReleaseScanY._evaluate(); break;
                case SDL_SCANCODE_Z: onReleaseScanZ._evaluate(); break;
                case SDL_SCANCODE_0: onReleaseScan0._evaluate(); break;
                case SDL_SCANCODE_1: onReleaseScan1._evaluate(); break;
                case SDL_SCANCODE_2: onReleaseScan2._evaluate(); break;
                case SDL_SCANCODE_3: onReleaseScan3._evaluate(); break;
                case SDL_SCANCODE_4: onReleaseScan4._evaluate(); break;
                case SDL_SCANCODE_5: onReleaseScan5._evaluate(); break;
                case SDL_SCANCODE_6: onReleaseScan6._evaluate(); break;
                case SDL_SCANCODE_7: onReleaseScan7._evaluate(); break;
                case SDL_SCANCODE_8: onReleaseScan8._evaluate(); break;
                case SDL_SCANCODE_9: onReleaseScan9._evaluate(); break;
                case SDL_SCANCODE_SPACE:  onReleaseScanSpace._evaluate();  break;
                case SDL_SCANCODE_RETURN: onReleaseScanEnter._evaluate();  break;
                case SDL_SCANCODE_ESCAPE: onReleaseScanEscape._evaluate(); break;
                case SDL_SCANCODE_LEFT:   onReleaseScanLeft._evaluate();   break;
                case SDL_SCANCODE_RIGHT:  onReleaseScanRight._evaluate();  break;
                case SDL_SCANCODE_UP:     onReleaseScanUp._evaluate();     break;
                case SDL_SCANCODE_DOWN:   onReleaseScanDown._evaluate();   break;
                default: break;
            }
            break;
        }

        case SDL_TEXTINPUT:
            onTextInput._evaluate(std::string(e.text.text));
            break;

        case SDL_TEXTEDITING:
            onTextEdit._evaluate(std::string(e.edit.text));
            break;

        case SDL_MOUSEMOTION: {
            onMouseMove._evaluate({(f32)e.motion.x, (f32)e.motion.y});
            if (e.motion.xrel != 0 || e.motion.yrel != 0)
                onMouseRawMove._evaluate({(f32)e.motion.xrel, (f32)e.motion.yrel});
            break;
        }

        case SDL_MOUSEBUTTONDOWN: {
            Vec2d pos = {(f32)e.button.x, (f32)e.button.y};
            switch (e.button.button) {
                case SDL_BUTTON_LEFT:
                    onLeftDown._evaluate(pos);
                    if (e.button.clicks == 2) onDoubleClick._evaluate(pos);
                    break;
                case SDL_BUTTON_RIGHT:  onRightDown._evaluate(pos);  break;
                case SDL_BUTTON_MIDDLE: onMiddleDown._evaluate(pos); break;
            }
            break;
        }

        case SDL_MOUSEBUTTONUP: {
            Vec2d pos = {(f32)e.button.x, (f32)e.button.y};
            switch (e.button.button) {
                case SDL_BUTTON_LEFT:   onLeftUp._evaluate(pos);   break;
                case SDL_BUTTON_RIGHT:  onRightUp._evaluate(pos);  break;
                case SDL_BUTTON_MIDDLE: onMiddleUp._evaluate(pos); break;
            }
            break;
        }

        case SDL_MOUSEWHEEL:
            onWheel._evaluate({(f32)e.wheel.x, (f32)e.wheel.y});
            break;

        
        case SDL_CONTROLLERBUTTONDOWN: {
            i32 id = e.cbutton.which;
            switch (e.cbutton.button) {
                case SDL_CONTROLLER_BUTTON_A:             onPressACtrl._evaluate(id);     break;
                case SDL_CONTROLLER_BUTTON_B:             onPressBCtrl._evaluate(id);     break;
                case SDL_CONTROLLER_BUTTON_X:             onPressXCtrl._evaluate(id);     break;
                case SDL_CONTROLLER_BUTTON_Y:             onPressYCtrl._evaluate(id);     break;
                case SDL_CONTROLLER_BUTTON_START:         onPressStart._evaluate(id);     break;
                case SDL_CONTROLLER_BUTTON_BACK:          onPressSelect._evaluate(id);    break;
                case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:  onPressLShoulder._evaluate(id); break;
                case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: onPressRShoulder._evaluate(id); break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:     onPressDpadLeft._evaluate(id);  break;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:    onPressDpadRight._evaluate(id); break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP:       onPressDpadUp._evaluate(id);    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:     onPressDpadDown._evaluate(id);  break;
                case SDL_CONTROLLER_BUTTON_LEFTSTICK:     onPressLStick._evaluate(id);    break;
                case SDL_CONTROLLER_BUTTON_RIGHTSTICK:    onPressRStick._evaluate(id);    break;
                default: break;
            }
            break;
        }

        case SDL_CONTROLLERBUTTONUP: {
            i32 id = e.cbutton.which;
            switch (e.cbutton.button) {
                case SDL_CONTROLLER_BUTTON_A:             onReleaseACtrl._evaluate(id);    break;
                case SDL_CONTROLLER_BUTTON_B:             onReleaseBCtrl._evaluate(id);    break;
                case SDL_CONTROLLER_BUTTON_X:             onReleaseXCtrl._evaluate(id);    break;
                case SDL_CONTROLLER_BUTTON_Y:             onReleaseYCtrl._evaluate(id);    break;
                case SDL_CONTROLLER_BUTTON_START:         onReleaseStart._evaluate(id);     break;
                case SDL_CONTROLLER_BUTTON_BACK:          onReleaseSelect._evaluate(id);    break;
                case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:  onReleaseLShoulder._evaluate(id); break;
                case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: onReleaseRShoulder._evaluate(id); break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:     onReleaseDpadLeft._evaluate(id);  break;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:    onReleaseDpadRight._evaluate(id); break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP:       onReleaseDpadUp._evaluate(id);    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:     onReleaseDpadDown._evaluate(id);  break;
                case SDL_CONTROLLER_BUTTON_LEFTSTICK:     onReleaseLStick._evaluate(id);    break;
                case SDL_CONTROLLER_BUTTON_RIGHTSTICK:    onReleaseRStick._evaluate(id);    break;
                default: break;
            }
            break;
        }

        
        case SDL_CONTROLLERAXISMOTION: {
            i32 id  = e.caxis.which;
            i16 val = e.caxis.value;
            switch (e.caxis.axis) {
                case SDL_CONTROLLER_AXIS_LEFTX:        onAxisLeftX._evaluate(id, val);    break;
                case SDL_CONTROLLER_AXIS_LEFTY:        onAxisLeftY._evaluate(id, val);    break;
                case SDL_CONTROLLER_AXIS_RIGHTX:       onAxisRightX._evaluate(id, val);   break;
                case SDL_CONTROLLER_AXIS_RIGHTY:       onAxisRightY._evaluate(id, val);   break;
                case SDL_CONTROLLER_AXIS_TRIGGERLEFT:  onAxisLTrigger._evaluate(id, val); break;
                case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: onAxisRTrigger._evaluate(id, val); break;
                default: break;
            }
            break;
        }

        
		case SDL_CONTROLLERDEVICEADDED: {
		    SDL_GameController* ctrl = SDL_GameControllerOpen(e.cdevice.which);
		    if (ctrl) {
		        SDL_Joystick* joy = SDL_GameControllerGetJoystick(ctrl);
		        SDL_JoystickID instanceId = SDL_JoystickInstanceID(joy);
		        event.m_controllers[instanceId] = ctrl;
		        onControllerAdded._evaluate(instanceId);
		    }
		    break;
		}
		
		case SDL_CONTROLLERDEVICEREMOVED: {
		    auto it = event.m_controllers.find(e.cdevice.which);
		    if (it != event.m_controllers.end()) {
		        SDL_GameControllerClose(it->second);
		        event.m_controllers.erase(it);
		    }
		    onControllerRemoved._evaluate(e.cdevice.which);
		    break;
		}

        
        case SDL_FINGERDOWN:
            onFingerDown._evaluate({(f32)e.tfinger.x, (f32)e.tfinger.y});
            break;

        case SDL_FINGERUP:
            onFingerUp._evaluate({(f32)e.tfinger.x, (f32)e.tfinger.y});
            break;

        case SDL_FINGERMOTION:
            onFingerMove._evaluate({(f32)e.tfinger.x, (f32)e.tfinger.y});
            break;

        case SDL_MULTIGESTURE:
            onMultiGesture._evaluate({(f32)e.mgesture.x, (f32)e.mgesture.y});
            break;

        
        case SDL_WINDOWEVENT: {
            switch (e.window.event) {
                case SDL_WINDOWEVENT_CLOSE:             onWindowClose._evaluate(e.window.windowID);                                        break;
                case SDL_WINDOWEVENT_MINIMIZED:         onWindowMinimized._evaluate(e.window.windowID);                                    break;
                case SDL_WINDOWEVENT_MAXIMIZED:         onWindowMaximized._evaluate(e.window.windowID);                                    break;
                case SDL_WINDOWEVENT_RESTORED:          onWindowRestored._evaluate(e.window.windowID);                                     break;
                case SDL_WINDOWEVENT_SHOWN:             onWindowShown._evaluate(e.window.windowID);                                        break;
                case SDL_WINDOWEVENT_HIDDEN:            onWindowHidden._evaluate(e.window.windowID);                                       break;
                case SDL_WINDOWEVENT_EXPOSED:           onWindowExposed._evaluate(e.window.windowID);                                      break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:      onWindowFocusGained._evaluate(e.window.windowID);                                  break;
                case SDL_WINDOWEVENT_FOCUS_LOST:        onWindowFocusLost._evaluate(e.window.windowID);                                    break;
                case SDL_WINDOWEVENT_ENTER:             onWindowMouseEnter._evaluate(e.window.windowID);                                   break;
                case SDL_WINDOWEVENT_LEAVE:             onWindowMouseLeave._evaluate(e.window.windowID);                                   break;
                case SDL_WINDOWEVENT_SIZE_CHANGED:      onWindowResized._evaluate(e.window.windowID, {(f32)e.window.data1, (f32)e.window.data2}); break;
                case SDL_WINDOWEVENT_MOVED:             onWindowMoved._evaluate(e.window.windowID, {(f32)e.window.data1, (f32)e.window.data2});   break;
                default: break;
            }
            break;
        }

        
        case SDL_QUIT:
            onQuit._evaluate();
            break;

        case SDL_DROPFILE: {
            std::string path(e.drop.file);
            SDL_free(e.drop.file);
            onDropFile._evaluate(path);
            break;
        }

        case SDL_CLIPBOARDUPDATE:
            onClipboardUpdate._evaluate();
            break;

        case SDL_APP_WILLENTERFOREGROUND:
            onAppForeground._evaluate();
            break;

        case SDL_APP_WILLENTERBACKGROUND:
            onAppBackground._evaluate();
            break;

        case SDL_APP_LOWMEMORY:
            onAppLowMemory._evaluate();
            break;

        case SDL_LOCALECHANGED:
            onLocaleChanged._evaluate();
            break;

        case SDL_DISPLAYEVENT:
            onDisplayChanged._evaluate();
            break;

        case SDL_RENDER_DEVICE_RESET:
            onRenderReset._evaluate();
            break;
        
        default: break;
    }
}


SDL_GameController* _getOpenController(i32 instanceId) noexcept {
    auto it = event.m_controllers.find(instanceId);
    return (it != event.m_controllers.end()) ? it->second : nullptr;
}


void EventManager::poll(void) {
    while (SDL_PollEvent(&m_event)) {
        m_has_event = true;
        _process(m_event);
    }
    if (!m_has_event) onEventNone._evaluate();
}

void EventManager::wait(void) {
    if (SDL_WaitEvent(&m_event)) _process(m_event);
}

void EventManager::wait(time::Second timeout) {
    if (SDL_WaitEventTimeout(&m_event, (int)timeout.count())) _process(m_event);
    else onEventNone._evaluate();
}

void EventManager::textInput(bool stat) {
	m_text_input = stat;
	if(stat) SDL_StartTextInput();
	else     SDL_StopTextInput();
}

bool EventManager::textInput(void) {
	return m_text_input;
}

}//namespace rmk