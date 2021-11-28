#pragma once

namespace Hedit::Control {
    template<class, class>
    ref class FuncDelegate;

    template<class _EH, class _R, class... _Args>
    [Windows::Foundation::Metadata::WebHostHidden]
    ref class FuncDelegate<_EH, _R(_Args...)> sealed {
    internal:
        explicit FuncDelegate(std::function<_R(_Args...)> func) {
            _func = func;
            _eh = ref new _EH(this, &FuncDelegate<_EH, _R(_Args...)>::Inovke);
        }
        _R Inovke(_Args... args) { return _func(args...); }
        property _EH^ Handler {
            _EH^ get() {
                return _eh;
            }
        }

    private:
        std::function<_R(_Args...)> _func;
        _EH^ _eh;
    };
}
