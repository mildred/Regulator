namespace {
  template<typename T>
  T mini(T a, T b) {
    return a > b ? b : a;
  }

  template<typename T>
  T maxi(T a, T b) {
    return a < b ? b : a;
  }
}
