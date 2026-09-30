#ifndef REMAKE2D_SCRIPT_OTRACKER_
#define REMAKE2D_SCRIPT_OTRACKER_


namespace skip {

template<IsTrackable T> bool operator==(const T& a, const T& b) {

    if constexpr ( requires(const T& a) { { a.operator==(b) } -> std::convertible_to<bool>; } ) {
        return a.operator==(b);
    } else {
        return a.tracker() == b.tracker();
    }

}

} // namespace skip
#endif