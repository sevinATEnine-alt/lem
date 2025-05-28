make clean
make -f OSXMakefile clean

rm -rf output/*

echo "[*] Building Linux distros"

docker buildx build \
  --platform linux/386,linux/amd64,linux/arm64 \
  -f Dockerfile \
  -o type=local,dest=output \
  .

if [ `uname -s` == "Darwin" ] && [`uname -m` == "arm64"]; then
    echo "[*] Building MacOS distros"

    mkdir output/macos
    make -f OSXMakefile
else
    echo "[X] Can only make MacOS distribution on a Darwin (mac) machnine."
fi