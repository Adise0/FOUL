rm -rf build/release
rm -rf out
cmake --preset release
cmake --build --preset release
cmake --install build/release --prefix ./out
